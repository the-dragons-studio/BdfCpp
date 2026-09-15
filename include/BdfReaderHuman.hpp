
#ifndef BDFREADERHUMAN_HPP_
#define BDFREADERHUMAN_HPP_

#include <filesystem>
#include <string>

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
		 * @throw BdfError if data could not be parsed.
         * @warning Implicit conversion of BdfReaderHuman objects using this constructor is deprecated.
      	 *          This constructor will be marked explicit from 2.0.0 onwards.
		 * @since 1.0
		 */
		BdfReaderHuman(const std::string &data);
		
		/**
		 * Initialises a BdfReaderHuman object by parsing data as human-readable BDF data.
		 * @param data wide-encoded text representing human-readable BDF data.
		 * @throw BdfError if data could not be parsed.
   		 * @warning Implicit conversion of BdfReaderHuman objects using this constructor is deprecated.
         *          This constructor will be marked explicit from 2.0.0 onwards.
		 * @since 1.0
		 */
		BdfReaderHuman(const std::wstring &data);
		
		/**
		 * Initialises a BdfReaderHuman object by parsing the file located at data as human-readable BDF data.
		 * @param data wide-encoded text representing human-readable BDF data.
		 * @throw BdfError if data could not be parsed.
		 * @since 1.5.0
		 */
		explicit BdfReaderHuman(const std::filesystem::path &location);

	protected:
		/**
		 * Helper method for preparing a buffer in a way that can be delegated to the BdfReaderHuman(const std::wstring&)
		 * constructor.
		 */
		std::wstring prepareBufferFromFilesystemPath(const std::filesystem::path &location) const;
	};
}

#endif
