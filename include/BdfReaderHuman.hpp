
#ifndef BDFREADERHUMAN_HPP_
#define BDFREADERHUMAN_HPP_

#include <string>
#include <filesystem>

namespace Bdf
{
	/**
	 * Derived class of BdfReader that unserialises human-readable BDF data instead of binary data.
   * This class may not be used for binary data; use BdfReader instead.
	 * All other operations of BdfReader are supported, including getting an BdfObject for viewing
   * and modifying data parsed, as well as serialising the data either as binary or human-readable data.
	 * @newable
   * @stable to extend
	 * @since 1.0
	 */
	class BdfReaderHuman : public BdfReader
	{
	public:
		/**
		 * Initialises a BdfReaderHuman object by parsing data as human-readable BDF data.
		 * @param data narrow-encoded text representing human-readable BDF data.
		 * @warning Do not use this constructor for BDF binary data! Instead, use BdfReader::BdfReader(const char* data, int size).
		 * @throw BdfError if data could not be parsed..
		 * @since 1.0
		 */
		explicit BdfReaderHuman(std::string data);
		
		/**
		 * Initialises a BdfReaderHuman object by parsing data as human-readable BDF data.
		 * @param data wide-encoded text representing human-readable BDF data.
		 * @throw BdfError if data could not be parsed.
		 * @since 1.0
		 */
		explicit BdfReaderHuman(std::wstring data);
		
		/**
		 * Opens the file located at filename as a BDF file.
		 * @param filename the file to read as a BDF file.
		 * @throw BdfError if data could not be parsed.
		 */
		explicit BdfReaderHuman(std::filesystem::path filename);
	};
}

#endif
