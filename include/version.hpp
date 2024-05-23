#pragma once

#include <string>

namespace Bdf {
	/**
	 * The header version.
	 * @todo Convert this to use cmake instead.
	 */
	const std::string VERSION = "1.4.2";
	
	/**
	 * The header codename.
	 */
	const std::string CODENAME = "HAVEN_EDGE";
	
	/**
	 * Get the build date of the library currently in use with BdfCpp.
	 * - If using a static library, returns the build date of the static library that was used to build the executable,
         *   for example libbdf.a. 
         * - If using a shared library, returns the build date of the shared library that was used when the executable was launched,
         *   for example libbdf.dll or libbdf.so.
	 * @return the build date in ISO 8601 UTC format.
	 */
	const std::string getLibraryBuildDate();
	
	/**
	 * Get the version of the library currently in use with BdfCpp.
	 * - If using a static library, returns the build date of the static library that was used to build the executable,
         *   for example libbdf.a. 
         * - If using a shared library, returns the build date of the shared library that was used when the executable was launched,
         *   for example libbdf.dll or libbdf.so.
	 * @return a string containing the library version.
	 */
	const std::string getLibraryVersion();
}	
