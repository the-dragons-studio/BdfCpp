
#include "../include/Bdf.hpp"
#include "../include/version.hpp"
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>

std::string command = "bdfconvert";

void help()
{
	std::cerr << command << " -m [binary/human] --pretty\n";
	std::cerr << "Convert data from stdin to its binary/human representation.\nbdfconvert: based on BdfCpp " << Bdf::getLibraryVersion();
}

int main(int argc, char** argv)
{
	if(argc > 0) {
		command = argv[0];
	}

	if(argc < 1) {
		help();
		return 1;
	}

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
			help();
			return 1;
		}
	}

	std::stringstream ss;

	{
		char buffer[1024];
		int size;

		while((size = std::fread(buffer, 1, 1024, stdin)) > 0) {
			ss.write(buffer, size);
		}
	}

	std::string data_in = ss.str();

	// Try different ways to read the data.
	// It could be human readable or binary.

	Bdf::BdfReader* reader = new Bdf::BdfReader(data_in.c_str(), data_in.size());

	if(reader->getObject()->getType() == Bdf::BdfTypes::UNDEFINED)
	{
		try
		{
			delete reader;

			reader = new Bdf::BdfReaderHuman(data_in);

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
		help();
		return 1;
	}

	delete reader;

	return 0;
}
