
#include "../include/Bdf.hpp"
#include "../include/version.hpp"

#include <tclap/CmdLine.h>

#include <filesystem>
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>

std::string command = "bdfconvert";

void getCliArgsOrShowHelp(int argc, char** argv) {
	try {
		TCLAP::CmdLine cmd("Converts to and from forms of BDF input and output data.", ' ', Bdf::getLibraryVersion());
		
		// -i, --input-mode
		std::vector<std::string> inputModeArgVector{"binary", "human", "auto"};
		TCLAP::ValuesConstraint<std::string> inputModeArgConstraint(inputModeArgVector);
		TCLAP::ValueArg<std::string> inputModeArg("i", "input-mode", "Select the type of BDF data that you would like to input.\nIf set to 'auto', bdfconvert will try different methods to read your input BDF data until it finds one that works.", false, "auto", &inputModeArgConstraint, cmd);
		
		// -o, --output-mode
		std::vector<std::string> outputModeArgVector{"binary", "human", "auto"};
		TCLAP::ValuesConstraint<std::string> outputModeArgConstraint(outputModeArgVector);
		TCLAP::ValueArg<std::string> outputModeArg("o", "output-mode", "Select the type of BDF data that you would like to output.\nIf set to 'auto', bdfconvert will output human-readable data if sending to standard output, OR if the input was binary. It will output binary data if the input was human-readable, AND you have specified an output file to write to.", false, "auto", &outputModeArgConstraint, cmd);
		
		// -f, --input-file
		TCLAP::ValueArg<std::filesystem::path> inputFileArg("f", "input-file", "If you need to read BDF data from a file, specify its path here. Leave this argument unspecified to read from standard input instead.", false, std::filesystem::path(), "filename", cmd);
		
		// -w, --output-file
		TCLAP::ValueArg<std::filesystem::path> outputFileArg("w", "output-file", "If you need to write BDF data to a file, specify its path here. Leave this argument unspecified to write to standard output instead.", false, std::filesystem::path(), "filename", cmd);
		
		// -m, --minified
		TCLAP::SwitchArg minifiedArg("n", "minified", "If bdfconvert outputs human-readable BDF data, optimise it to consume as little characters as possible.", false);
		
		// -p, --pretty
		TCLAP::SwitchArg prettyArg("p", "pretty", "If bdfconvert outputs human-readable BDF data, optimise it to look pretty.", false);
		
		// Group -p and -m
		TCLAP::EitherOf humanReadableOutputType;
        humanReadableOutputType.add(minifiedArg).add(prettyArg);
		cmd.add(humanReadableOutputType);
		
		// -v, --validate
		TCLAP::SwitchArg dryRunOnlyArg("d", "dry-run-only", "Only read BDF input data, but do not write to any output, even if you specify one. Useful for validation purposes; if bdfconvert runs silently, the BDF input is without error.", cmd, false);
		
		// Parse the argv array.
		cmd.parse( argc, argv );
	} catch (TCLAP::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}

std::string getBdfInputFromString(std::ifstream &stream) {
	// Prepare the stringstream
	std::stringstream ss;
	
	char buffer[1024];
	int size;
	/*
	// Read data as much as we can.
	while((size = std::fread(buffer, 1, 1024, stream)) > 0) {
		ss.write(buffer, size);
	}
	*/
	
	return ss.str();
}

int main(int argc, char** argv)
{
	// Get command line arguments, or show help if necessary 
	getCliArgsOrShowHelp(argc, argv);
	
	std::string mode = "";
	std::string indent = "";
	std::string breaker = "";

	for(int i=1;i<argc;i++)
	{
		std::string arg = argv[i];

		if((arg == "-m" || arg == "--mode") && i + 1 < argc) {
			mode = argv[i+1];
			i += 1;
		}

		else if(arg == "-p" || arg == "--pretty") {
			breaker = "\n";
			indent = "\t";
			i += 1;
		}

		else {
			// help();
			return 1;
		}
	}

	// Try different ways to read the data.
	// It could be human readable or binary.

	Bdf::BdfReader* reader = new Bdf::BdfReader;

	if(reader->getObject()->getType() == Bdf::BdfTypes::UNDEFINED)
	{
		try
		{
			delete reader;

			// reader = new Bdf::BdfReaderHuman(data_in);

			if(mode == "") {
				mode = "binary";
			}
		}

		catch(Bdf::BdfError &e)
		{
			reader = new Bdf::BdfReader();

			if(mode == "") {
				mode = "human";
			}
		}
	}

	else if(mode == "") {
		mode = "human";
	}

	if(mode == "binary")
	{
		char* data;
		int data_size;
		reader->serialize(&data, &data_size);

		for(int i=0;i<data_size;i+=1024) {
			std::cout.write(data + i, std::min(1024, data_size - i));
		}

		delete[] data;
	}

	else if(mode == "human") {
		reader->serializeHumanReadable(std::cout, Bdf::BdfIndent(indent, breaker));
	}

	else {
		// help();
		return 1;
	}

	delete reader;

	return 0;
}
	