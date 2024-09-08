#include "BdfReaderGzip.hpp"

namespace Bdf {
	BdfReaderGzip::BdfReaderGzip(const char* database, size_t size) {
		static_assert(BdfReaderGzip::isBdfReaderGzipEnabled());
		std::string decompressedData = Gzip::decompress(database, size);
	}
	
	constexpr static bool isBdfReaderGzipEnabled() {
		#ifdef BDF_ENABLE_GZIP
		return true;
		#else
		return false;
		#endif
	}
}
		