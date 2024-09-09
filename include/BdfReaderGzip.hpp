#pragma once

#ifdef BDF_ENABLE_GZIP
#include <gzip/compress.hpp>
#include <gzip/config.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>
#include <gzip/version.hpp>
#endif


 
namespace Bdf {
	/**
	 * Derived class of BdfReader that allows unserialiation and serialisation from and to
     * compressed BDF files.
 	 * @note This class is only usable if BdfCpp was compiled with Gzip compression enabled,
	 * which will require you to also ensure the zlib library is bundled with your application.
	 * and discoverable during building.
	 */
	struct BdfReaderGzip: public BdfReader {
		/**
		 * Unserialises the file given at bdfCompressedFile.
	     * @param bdfCompressedFile the compressed BDF file that needs to be decompressed.
	     */
		explicit BdfReaderGzip(std::filesystem::path bdfCompressedFile);
	
    	/**
	     * Unserialises the compressed BDF data given at database up to the given size.
	     * @param database the compressed BDF data that needs to be decompressed, size_t the size of the data that needs
	     * to be decompressed.
	     */
		BdfReaderGzip(const char* database, uint64_t size);
	    
		/**
		 * Serialises the BDF data and returns it as an std::string.
		 * @return a string representing compressed BDF data.
		 */
		std::string serializeCompressed();
		
		/**
		 * Serialises the BDF data to the file given at targetFile.
		 * @param targetFile the file to which the BDF compressed data will be saved. It is recommended that targetFile.extension() == "bdfgz".
		 */
		void serializedCompressed(std::filesystem::path targetFile);
		
		private:
	    /**
	     * Returns true if BdfReaderGzip is enabled, false otherwise.
		 * This function is used to guard against use of BdfReaderGzip when the library has not been compiled with it enabled.
	     */
	    constexpr static bool isBdfReaderGzipEnabled();
	};
}
	