
#include "../include/Bdf.hpp"
#include "../include/BdfHelpers.hpp"
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
	BdfObject* bdfNew = nullptr;
	// Skip ahead to the first non-comment character.
	try {
		while(!sr.ignoreBlanks()) {
			// If this is our first time in the loop, bdfNew will be null. Create it using a new BdfObject.
			if (!bdfNew) {
				bdfNew = new BdfObject(lookupTable, &sr);
			// Otherwise that means we already attempted to create the file yet haven't hit end of file yet, which
			// probably means something has gone wrong. Throw a BdfError and delete the attempted object.
			} else {
				throw BdfError(BdfError::ERROR_SYNTAX, sr);
			}
		}
	// In case we run into an exception, make sure bdfNew is deallocated.
	} catch (...) {
		delete bdfNew;
		throw;
	}

	// Make our BdfObject the new one.
	this->bdf = bdfNew;
}

BdfReaderHuman::BdfReaderHuman(const std::string &data) : BdfReaderHuman(
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(data)) {
}
