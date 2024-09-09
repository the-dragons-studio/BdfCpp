
#ifndef BDF_HPP_
#define BDF_HPP_

/**
 * Main entry point for BdfCpp.
 *
 * It is recommended that projects use this header file to include BdfCpp. It
 * will automatically define all the objects you'll need to begin usage of BdfCpp.
 *
 */
 
namespace Bdf
{
	class BdfList;
	class BdfIndent;
	class BdfLookupTable;
	class BdfNamedList;
	class BdfObject;
	class BdfReader;
	class BdfTypes;
	class BdfError;
	class BdfStringReader;
	class BdfReaderHuman;
	
}

#include "BdfLookupTable.hpp"
#include "BdfList.hpp"
#include "BdfIndent.hpp"
#include "BdfNamedList.hpp"
#include "BdfObject.hpp"
#include "BdfReader.hpp"
#include "BdfTypes.hpp"
#include "BdfError.hpp"
#include "BdfStringReader.hpp"
#include "BdfReaderHuman.hpp"
#include "version.hpp"

#ifdef BDF_ENABLE_GZIP
#include "BdfReaderGzip.hpp"
#endif

#endif
