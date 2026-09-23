
#include "../include/Bdf.hpp"
#include "../include/BdfHelpers.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <codecvt>
#include <locale>

using namespace Bdf;
using namespace BdfHelpers;

BdfReaderHuman::BdfReaderHuman(const std::wstring &data)
{
	// Make a BdfStringReader from the given data.
	BdfStringReader sr(data.c_str(), data.size());

	try {
		// Skip ahead to the first non-comment character.
		// If there isn't EOF at that point, create our BdfObject from there.
		if (!sr.ignoreBlanks()) {
			this->bdf = new BdfObject(lookupTable, &sr);
		} else {
			// Just init an empty object
			this->initEmpty();
		}
		
		// Run sr.ignoreBlanks() and hope EOF was actually reached.
		if (!sr.ignoreBlanks()) {
			// Otherwise that means we already attempted to create the file yet haven't hit end of file yet, which
			// probably means something has gone wrong. Throw a BdfError and delete the attempted object.
			throw BdfError(BdfError::ErrorType::SYNTAX, sr);
		}
	// In case we run into an exception, make sure our BDF object is left in a valid but empty state.
	} catch (...) {
		this->initEmpty();
		throw;
	}
}

BdfReaderHuman::BdfReaderHuman(const std::string &data) : BdfReaderHuman(
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(data)) {
}

BdfReaderHuman::BdfReaderHuman(const std::filesystem::path &location):
	BdfReaderHuman(this->prepareBufferFromFilesystemPath(location)) {
}
	
std::wstring BdfReaderHuman::prepareBufferFromFilesystemPath(const std::filesystem::path &location) const {
	std::wifstream ifstr(location);
	std::wstringstream dataStream;
	
    while(!ifstr.eof()) {
        std::wstring line;
        std::getline(ifstr, line);
		dataStream << line << "\n";
    }
	
	// Return our complete stream's string
	return dataStream.str();
}
