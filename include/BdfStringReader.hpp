
#ifndef BDFSTRINGREADER_HPP_
#define BDFSTRINGREADER_HPP_

#include "./BdfError.hpp"

#include <string>

namespace Bdf
{
	/**
	 * Class used to help unserialise human-readable BDF files.
	 * @internal
	 */
	class BdfStringReader
	{
	public:
		const wchar_t* start;
		const wchar_t* end;
		const wchar_t* upto;
		wchar_t c;
	
		BdfStringReader(const wchar_t* ptr, int size);
		BdfStringReader getPointer(int amount);
	
		void increment();
		void increment(int amount);
		int length();
		
		/**
		 * Stops at the first character that isn't a comment.
		 * @return true if the end of the file was reached, false otherwise
		 */
		bool ignoreBlanks();
		
		/**
		 * Stops at the first character that isn't a comment.
		 * Disallows encountering EOF; if it is, a BdfError will be thrown.
		 * @param onEof The ErrorType of BdfError that should be thrown if EOF is encountered.
		 * @throw BdfError of type given in ErrorType (default UNEXPECTED_END_OF_FILE) if EOF is encountered,
		 *                 even though callers of this method *do not* expect EOF.
		 * @since 1.5.0
		 */
		void ignoreBlanksDisallowEof(BdfError::ErrorType onEof = BdfError::ErrorType::UNEXPECTED_END_OF_FILE);
		
		/**
		 * Ignores the current C++ style comment.
		 * @return true if the end of the file was reached, false otherwise
		 */
		bool ignoreCommentCppStyle() noexcept;
		
		/**
		 * Ignores the current C-style comment.
		 * @throw BdfError if that comment isn't closed before the end of the file.
		 */
		void ignoreCommentCStyle();
		
		/**
		 * Returns true if the current character should be ginored.
		 */
		bool shouldIgnoreCurrentChar() const noexcept;
		
		std::string getQuotedString();
		bool isNext(const std::wstring &check);
		bool inRange();
	};
}

#endif
