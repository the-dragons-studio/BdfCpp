
#ifndef BDFERROR_HPP_
#define BDFERROR_HPP_

#include "BdfStringReader.hpp"
#include <string>

namespace Bdf
{
	class BdfError : public std::exception
	{
	private:
		std::string error_short;
		std::string message;
		int type = -1;
		int line = -1;
		int at = -1;
		std::string context = "";
	
	public:
		/**
		 * Indicates that a syntax error was detected.
		 * @note This error can only occur when parsing a human-readable file.
	     */
		static const int ERROR_SYNTAX = 0;
		
		/**
		 * Indicates that an attempt to read beyond the end of file was made.
		 * @note This error can only occur when parsing a human-readable file.
	     */
		static const int ERROR_END_OF_FILE = 1;
		
		/**
		 * Indicates that a multiline comment was not escaped.
		 * @note This error can only occur when parsing a human-readable file.
	     */
		static const int ERROR_UNESCAPED_COMMENT = 2;
		
		/**
		 * Indicates that a string was not escaped.
		 * @note This error can only occur when parsing a human-readable file.
	     */
		static const int ERROR_UNESCAPED_STRING = 3;
		
		/**
		 * Indicates that an attempt to unserialise data that is out of range of the requested datatype was made.
		 * For example, "bad": 128B.
		 * @note This error can only occur when parsing a human-readable file.
	     */
		static const int ERROR_OUT_OF_RANGE = 4;
		
		/**
		 * Indicates that the size tag found at the start of a binary file does not match the memory actually required to parse it.
		 * @note This error can only occur when parsing a binary file.
		 */
		static const int ERROR_SIZE_TAG_MISMATCH = 5;
	    
		/**
		 * Creates a BdfError consisting of the error code code.
		 * No other explanatory information will be available (all will be set to default).
	     */
		explicit BdfError(const int code);
		
		/**
		 * Creates a BdfError consisting of the error code at code, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @internal
		 */
		BdfError(const int code, BdfStringReader reader, int length);
		
		/**
		 * Creates a BdfError consisting of the error code at code, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 */
		BdfError(const int code, BdfStringReader reader);
		
		/**
		 * Gets a pre-formatted error message consting of: type of error line:at.
		 */
		std::string getErrorShort() const noexcept;
		std::string getError() const noexcept;
		int getType() const noexcept;
		int getLine() const noexcept;
		int getAt() const noexcept;
		std::string getContext() const noexcept;
	
		virtual const char* what() const noexcept;
	};
}

#endif
