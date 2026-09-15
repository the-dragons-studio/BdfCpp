
#include <filesystem>
#include <iostream>
#include <stacktrace>

#include "../include/Bdf.hpp"
#include "../include/version.hpp"

// Test executable for serialisation.

std::size_t testNumber = 0;

// A test is a boolean expression that results in true when passed.
// If a test passes, show the test number and a basic stacktrace. Execution continues on to the next test.
// If a test fails, show: the test number, and trace to the faulting test call. Execution is aborted.
void test(
	const Bdf::BdfReader &toSerialise,
	bool performRoundTripSerialisationTest = true,
	std::stacktrace trace = std::stacktrace::current(),
	size_t traceIncrToMain = 0
) {
	testNumber++;
	
	// Certain compilers need correction for the number of traces advanced until we get
	// the trace corresponding to main, which is what we want. Correct for it here.
	auto traceEntryIt = trace.begin();
	
	for (size_t i = 0; i < traceIncrToMain && traceEntryIt != trace.end(); ++i) {
		++traceEntryIt;
	}
	
	// Determine the filename we will use.
	std::filesystem::path serialisationFilename(std::filesystem::current_path() / (std::string("test_serialisation_") + std::to_string(testNumber) + ".hbdf"));
	
	toSerialise.serializeHumanReadable(serialisationFilename);
	
	if (performRoundTripSerialisationTest) {
		Bdf::BdfReaderHuman reserialise(serialisationFilename);
		
		// Perform an operator<=> test compared to our old reader.
		if (*(toSerialise.getObject()) == *(reserialise.getObject())) {
			std::cout << "Test " << testNumber << " (" << *traceEntryIt << ") passed\n";
		} else {
			std::cout << "Test " << testNumber << " (" << *traceEntryIt << ") failed\n";
			throw std::runtime_error("breakpoint\n" + std::to_string(trace));
		}
	} else {
		// Just print out the message that the test has been serialised.
		std::cout << "Test " << testNumber << " (" << *traceEntryIt << ") serialised\n";
	}
}

int main() {
	// Print out the version of BdfCpp we are testing.
	std::cout << "Testing BdfCpp version " << Bdf::getLibraryVersion() << std::endl;
	
	// Create our test reader
	Bdf::BdfReader reader;
	Bdf::BdfObject *bdf(reader.getObject());
	
	// Test that C++ style comments work
	bdf->coerceCommentCppStyle()->setCommentLines({"Hello", "World!"});
	test(reader, false);
	
}