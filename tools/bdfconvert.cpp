
#include "../include/Bdf.hpp"
#include "../include/version.hpp"

#include <tclap/CmdLine.h>

#include <filesystem>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stacktrace>

std::string command = "bdfconvert";

std::string inputMode, outputMode;
std::filesystem::path inputFile, outputFile, indent, breaker;
bool keepGoing = false;
bool validate = false;
bool pretty = false;
bool minified = false;
bool debug = false;

void getCliArgsOrShowHelp(int argc, char** argv) {
	try {
		TCLAP::CmdLine cmd("Converts to and from forms of BDF input and output data.", ' ', Bdf::getLibraryVersion());
		
		// -i, --input-mode
		std::vector<std::string> inputModeArgVector{"binary", "human", "auto"};
		TCLAP::ValuesConstraint<std::string> inputModeArgConstraint(inputModeArgVector);
		TCLAP::ValueArg<std::string> inputModeArg("i", "input-mode", "Select the type of BDF data that you would like to input.\nIf set to 'auto', bdfconvert will try different methods to read your input BDF data until it finds one that works.", false, "auto", &inputModeArgConstraint, cmd);
		
		// -o, --output-mode
		std::vector<std::string> outputModeArgVector{"binary", "human", "inverse", "auto"};
		TCLAP::ValuesConstraint<std::string> outputModeArgConstraint(outputModeArgVector);
		TCLAP::ValueArg<std::string> outputModeArg("o", "output-mode", "Select the type of BDF data that you would like to output.\nIf set to 'auto', bdfconvert will output human-readable data if sending to standard output, OR if the input was binary. It will output binary data if the input was human-readable, AND you have specified an output file to write to.\nIf set to 'inverse', bdfconvert will output binary data if the BDF input was human-readable. It will output human-readable data if the input was binary. Unlike 'auto', the use of standard output will not influence the selected output type.", false, "auto", &outputModeArgConstraint, cmd);
		
		// -f, --input-file
		TCLAP::ValueArg<std::string> inputFileArg("f", "input-file", "If you need to read BDF data from a file, specify its path here. Leave this argument unspecified to read from standard input instead.", false, std::string(), "filename", cmd);
		
		// -w, --output-file
		TCLAP::ValueArg<std::string> outputFileArg("w", "output-file", "If you need to write BDF data to a file, specify its path here. Leave this argument unspecified to write to standard output instead.", false, std::string(), "filename", cmd);
		
		// -m, --minified
		TCLAP::SwitchArg minifiedArg("n", "minified", "If bdfconvert outputs human-readable BDF data, optimise it to consume as little characters as possible.", false);
		
		// -p, --pretty
		TCLAP::SwitchArg prettyArg("p", "pretty", "If bdfconvert outputs human-readable BDF data, optimise it to look pretty.", false);
		
		// Group -p and -m
		TCLAP::EitherOf humanReadableOutputType;
        humanReadableOutputType.add(minifiedArg).add(prettyArg);
		cmd.add(humanReadableOutputType);
		
		// -k, --keep-going
		// TCLAP::SwitchArg dryRunOnlyArg("k", "keep-going", "Attempt to recover from errors. May result in empty BDF data being used if no sense of valid data could be determined at all. Useful when used with --dry-run-only to validate and report all errors in BDF data.", cmd, false);
		
		// -v, --validate
		TCLAP::SwitchArg validateArg("V", "validate", "Only read the input and check it for errors, but do not attempt to convert it to any output. If no error message is generated, bdfconvert has successfully validated your BDF data is without error.", cmd, false);
		
		// -g, --debug
		TCLAP::SwitchArg debugArg("g", "debug", "Print out advanced debug information if errors occur.", cmd, false);
		
		// Parse the argv array.
		cmd.parse( argc, argv );
		
		// keepGoing = keepGoingArg.getValue();
		pretty = prettyArg.getValue();
		minified = minifiedArg.getValue();
		validate = validateArg.getValue();
		
		inputMode = inputModeArg.getValue();
		outputMode = outputModeArg.getValue();
		inputFile = inputFileArg.getValue();
		outputFile = outputFileArg.getValue();
		debug = debugArg.getValue();
	} catch (TCLAP::ArgException &e) {
		std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}

bool stringHasNonPrintableChars(const std::string &toCheck) {
	return !std::all_of( toCheck.begin(), toCheck.end(), []( char c ) { return !std::isgraph( c ); } );
}

Bdf::BdfReader *tryBinaryReader(const std::stringstream &inputData) {
	Bdf::BdfReader *reader = nullptr;
	try {
		Bdf::BdfReader *reader = new Bdf::BdfReader(inputData.str().c_str(), inputData.str().size());
		
		// Make sure we have a valid reader.
		if (reader->getObject()->getType() == Bdf::BdfTypes::UNDEFINED) {
			delete reader;
		} else {
			// Set our input mode now that we've determined we have a valid file.
			inputMode = "binary";
		}
	} catch (Bdf::BdfError &e) {

		delete reader;
		
		// Size tag mismatches could theoretically be triggered by human-readable BDF data being interpreted as binary
		// if our characters line up just the right way. No need to rethrow those if we're using auto inputMode.
		if (inputMode != "auto") {
			throw;
		}
	}
	
	return reader;
}

Bdf::BdfReader *tryHumanReader(const std::stringstream &inputData) {
	Bdf::BdfReader *reader = nullptr;
	try {
		reader = new Bdf::BdfReaderHuman(inputData.str());
		
		inputMode = "human";
	} catch (Bdf::BdfError &e) {
		delete reader;
		throw;
	} catch (...) {
		delete reader;
		
		if (inputMode != "auto") {
			throw;
		}
	}
	
	return reader;
}

Bdf::BdfReader* getBdfInputReader(const std::stringstream &inputData) {
	Bdf::BdfReader *reader = nullptr;
	bool triedBinaryReader = false;
	bool triedHumanReader = false;
	bool checkedForNonPrintableCharacters = false;
	
	if (inputMode == "auto") {
		while (reader == nullptr) {
			// We need to try determining if we have a binary or human-readable file.
			
			// 1. If we detect unprintable characters, start with binary.
			// 2. If that doesn't work, or if we don't detect unprintable characters, try human-readable.
			// 3. Otherwise try binary if we haven't already tried that.
			// 4. If none of the above steps are able to get us a valid reader, throw an exception.
			
			if (!checkedForNonPrintableCharacters) {
				checkedForNonPrintableCharacters = true;
				
				if (stringHasNonPrintableChars(inputData.str())) {
					triedBinaryReader = true;
					reader = tryBinaryReader(inputData);
				}
			} else if (!triedHumanReader) {
				triedHumanReader = true;
				reader = tryHumanReader(inputData);
			} else if (!triedBinaryReader) {
				triedBinaryReader = true;
				reader = tryBinaryReader(inputData);
			} else {
				throw std::runtime_error("Could not automatically determine the type of input BDF data. Check that it is valid, and try passing -i human|binary to manually set the input data type.");
			}
		}
	} else if (inputMode == "human") {
		reader = tryHumanReader(inputData);
	} else if (inputMode =="binary") {
		reader = tryBinaryReader(inputData);
	}
	
	return reader;
}

std::stringstream getBdfInputData(std::istream &istream) {
	std::stringstream dataStream;
	// We can't rely on the native BdfReaderHuman filesystem::path constructor as we also need to handle stdin
	// with the same code. So write our own.
    while(!istream.eof()) {
        std::string line;
        std::getline(istream, line);
		dataStream << line;
    }
	
	// Return our complete stream
	return dataStream;
}

Bdf::BdfIndent getIndenter() {
	if (pretty) {
		return Bdf::BdfIndent("\t", "\n");
	} else {
		return Bdf::BdfIndent("", "");
	}
}

int main(int argc, char** argv)
{
	// Get command line arguments, or show help if necessary 
	getCliArgsOrShowHelp(argc, argv);
	
	std::string mode = "";
	std::string indent = "";
	std::string breaker = "";
	Bdf::BdfReader *reader = nullptr;
	std::stringstream inputData;

	try {
		// Get an input stream.
		// Use stdin if a valid path is not given.
		if (inputFile.empty()) {
			inputData = getBdfInputData(std::cin);
		} else {
			if (!std::filesystem::exists(inputFile)) {
				throw std::runtime_error("An input file was specified but it does not exist.");
			}
			std::ifstream ifstr(inputFile);
			inputData = getBdfInputData(ifstr);
		}

		// Prepare our reader.
		reader = getBdfInputReader(inputData);
	} catch (Bdf::BdfError &e) {
		std::cerr << "A parse error occured while parsing the input BDF data." << std::endl;
		std::cerr << "Description: " << e.getErrorShort() << std::endl;
		std::cerr << "Line       : " << e.getLine() << std::endl;
		std::cerr << "At         : " << e.getAt() << std::endl;
		std::cerr << "Context    : " << e.getContext() << std::endl;
		if (debug) {
			std::cerr << "Stack trace: " << std::endl << e.getTrace() << std::endl;
		}
		
		if (!keepGoing) {
			exit(2);
		}
	}
	
	try {
		// Exit immediately if in validate mode.
		if (validate) {
			exit(0);
		}
		
		// We might end up with a nullptr BdfReader if one of the above steps failed,
		// especially if --keep-going was set.
		if (reader == nullptr) {
			reader = new Bdf::BdfReader();
		}
		
		// If outputMode is auto or inverse, determine it automatically.
		if (outputMode == "auto" || outputMode == "inverse") {
			// Are we using stdout or is the input binary?
			if (inputMode == "binary" || (outputMode == "auto" && outputFile.empty())) {
				outputMode = "human";
			} else {
				outputMode = "binary";
			}
		}
		
		// Fallthrough
		if(outputMode == "binary") {
			char* data;
			int data_size;
			reader->serialize(&data, &data_size);

			if (outputFile.empty()) {
				for(int i=0;i<data_size;i+=1024) {
					std::cout.write(data + i, std::min(1024, data_size - i));
				}
			} else {
				std::ofstream ofstr(outputFile);
				for(int i=0;i<data_size;i+=1024) {
					ofstr.write(data + i, std::min(1024, data_size - i));
				}
			}

			delete[] data;
		} else if(outputMode == "human") {
			if (outputFile.empty()) {
				reader->serializeHumanReadable(std::cout, getIndenter());
			} else {
				std::ofstream ofstr(inputFile);
				reader->serializeHumanReadable(ofstr, getIndenter());
			}
		}		
	} catch (std::exception &e) {
		std::cerr << "An error occured while writing the output BDF data." << std::endl;
		std::cerr << e.what();
		
		exit(3);
	}

	delete reader;

	return 0;
}
	