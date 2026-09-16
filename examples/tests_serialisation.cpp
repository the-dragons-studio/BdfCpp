
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
	const Bdf::BdfIndent &indenter = Bdf::BdfIndent({"\t", "\n"}),
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
	
	toSerialise.serializeHumanReadable(serialisationFilename, indenter);
	
	// Should we perform the round trip serialisation test?
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
	
	// Test serialising booleans
	bdf->setBoolean(true);
	test(reader, false);
	
	bdf->setBoolean(false);
	test(reader, false);
	
	// Test that C++ style comments work
	bdf->useCommentCppStyle()->setCommentLines({"Hello", "World!"});
	test(reader, false);
	
	// Test a BdfList containing a string, plus C-style comments
	bdf->useList()->reserve(2);
	bdf->getList()->get(0)->setString("This list has a C style comment!");
	bdf->getList()->get(1)->useCommentCStyle()->setCommentLines({"Hello", "World!"});
	test(reader, false);
	
	// Test a BdfList containing a string, plus C++ style comments
	bdf->useList()->reserve(2);
	bdf->getList()->get(0)->setString("This list has a C++ style comment!");
	bdf->getList()->get(1)->useCommentCppStyle()->setCommentLines({"Hello", "World!"});
	test(reader, false);
	
	// Test a BdfNamedList containing a string, plus C-style comments
	bdf->useNamedList();
	bdf->getNamedList()->use("my_named_element")->setString("This list has a C style comment! The name I have should also appear as usual.");
	bdf->getNamedList()->use("this_name_should_not_appear")->useCommentCStyle()->setCommentLines({
			"Hello",
			"World!",
			"This comment has a name that the BdfNamedList owns it by, but that name will not appear."
		}
	);
	bdf->getNamedList()->get("the_missing_key")->setShort(4813);
	test(reader, false);
	
	// Test a BdfNamedList containing a string, plus C++ style comments
	bdf->useNamedList();
	bdf->getNamedList()->use("my_named_element")->setString("This list has a C++ style comment! The name I have should also appear as usual.");
	bdf->getNamedList()->use("this_name_should_not_appear")->useCommentCppStyle()->setCommentLines({
			"Hello",
			"World!",
			"This comment has a name that the BdfNamedList owns it by, but that name will not appear."
		}
	);
	bdf->getNamedList()->get("the_missing_key")->setShort(4813);
	test(reader, false);
}