#include "../include/version.hpp"
#include "../include/timestamp.h"

namespace Bdf {
	// Gets the project build date as found in timestamp.h.
	// Note that the exact contents of this file can vary.
	const std::string getLibraryBuildDate() {
		return _TIMEZ_;
	}
	
	// Gets the project version found in version.hpp.
	const std::string getLibraryVersion() {
		return VERSION;
	}
}