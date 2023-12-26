
#ifndef BDFREADERHUMAN_HPP_
#define BDFREADERHUMAN_HPP_

#include <string>
#include <filesystem>

namespace Bdf
{
	/**
	 * Class for reading BDF files that are human-readable.
	 */
	class BdfReaderHuman : public BdfReader
	{
	public:
		/**
		 * Parses data as a human-readable BDF file.
		 * @param data narrow-encoded text representing a human-readable BDF file.
		 * @throw BdfError if data could not be parsed.
		 */
		explicit BdfReaderHuman(std::string data);
		
		/**
		 * Parses data as a human-readable BDF file.
		 * @param data wide-encoded text representing a human-readable BDF file.
		 * @throw BdfError if data could not be parsed.
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
