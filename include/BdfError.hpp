
#ifndef BDFERROR_HPP_
#define BDFERROR_HPP_

#include "BdfStringReader.hpp"

#include <cstdint>
#include <string>
#include <stacktrace>

namespace Bdf
{	
	/**
	 * Class thrown when BdfCpp encounters fatal parse errors. It contains methods for getting the exact
	 * type of error, an explanatory string, line and character information, and context to help debug the
	 * error.
	 * Note that some of these methods may not be able to provide any information if BdfCpp was trying to
	 * unserialise binary BDF data.
	 *
	 * @since 1.0
	 */
	class BdfError : public std::exception
	{
	private:
		std::string error_short;
		std::string message;
		std::stacktrace trace;
		int type = -1;
		int line = -1;
		int at = -1;
		std::string context = "";
	
	public:
		/**
		 * Enumeration type representing the type of error.
   		 * @since 1.5.0
		 */
		enum ErrorType: uint8_t {
			/**
			 * Indicates that a syntax error was detected.
		 	 * @note This error can only occur when parsing a human-readable file.
	    	 */
			SYNTAX,
			
			/**
			 * Indicates that a premature end of file was found. For example, the library
			 * was trying to read an integer but did not find the datatype character before the end of file.
             * @note: Versions before 2.0.0 could not actually throw this error. That's
             *        because this "error" was used to indicate that parsing was finished,
			 *        and a finished BdfReaderHuman object could be presented. This was rewritten
             *        in 2.0.0, and the error was instead repurposed to indicate *premature*
			 *        end of file.
			 * @note This error can only occur when parsing a human-readable file.
			 */
			END_OF_FILE, 
			
			/**
			 * Indicates that a multiline comment was not escaped before the end of file was found.
		 	 * @note This error can only occur when parsing a human-readable file.
	         */
			UNESCAPED_COMMENT_BEFORE_EOF,

			/**
			 * Indicates that a multiline comment was not escaped before the end of file was found.
		 	 * @note This error can only occur when parsing a human-readable file.
	         */
			UNESCAPED_STRING_BEFORE_EOF,

			/**
   			 * Indicates that an illegal backslash escape code was encountered.
       	     * @note This error can only occur when parsing a human-readable file.
       		 */
			ILLEGAL_STRING_BACKSLASH_ESCAPE,

			/**
			 * Indicates that an attempt to unserialise data that is out of range of the requested datatype was made.
		 	 * For example, "bad": 128B.
		 	 * @note This error can only occur when parsing a human-readable file.
	     	 */
			NUMERICAL_OUT_OF_RANGE,

			/**
			 * Indicates that the size tag found at the start of a binary file does not match the memory actually required to parse it.
		 	 * @note This error can only occur when parsing a binary file.
			 */
			BINARY_SIZE_TAG_MISMATCH,
		};
		
		/**
  		 * @deprecated Use BdfError::ErrorType::SYNTAX instead.
     	 */
		static const int ERROR_SYNTAX = 0;

		/**
  		 * @deprecated Use BdfError::ErrorType::END_OF_FILE instead.
     	 */
		static const int ERROR_END_OF_FILE = 1;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::UNESCAPED_COMMENT_BEFORE_EOF instead.
     	 */
		static const int ERROR_UNESCAPED_COMMENT = 2;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::UNESCAPED_STRING_BEFORE_EOF instead.
     	 */
		static const int ERROR_UNESCAPED_STRING = 3;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::NUMERICAL_OUT_OF_RANGE instead.
     	 */
		static const int ERROR_OUT_OF_RANGE = 4;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::BINARY_SIZE_TAG_MISMATCH instead.
     	 */
		static const int ERROR_SIZE_TAG_MISMATCH = 5;
	    
		/**
		 * Creates a BdfError consisting of the error code code.
		 * No other explanatory information will be available (all will be set to default).
		 * @since 1.5.0
	     */
		explicit BdfError(const BdfError::ErrorType& type, std::stacktrace trace = std::stacktrace::current());

		/**
  		 * @deprecated Use BdfError::BdfError(const BdfError::ErrorType& type) instead.
		 * @since 1.4.0
     	 */
		explicit BdfError(const int code);

		/**
		 * Creates a BdfError consisting of the error code at code, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @since 1.0
		 * @internal
		 */
		BdfError(const int code, BdfStringReader reader, int length, std::stacktrace trace = std::stacktrace::current());
		
		/**
		 * Creates a BdfError consisting of the error code at code, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @since 1.0
		 * @internal
		 */
		BdfError(const int code, BdfStringReader reader, std::stacktrace trace = std::stacktrace::current());
		
		/**
		 * Gets a pre-formatted error message.
		 * @return a string consisting of: type of error line:at.
		 * @since 1.0
		 */
		std::string getErrorShort() const noexcept;
		
		/**
		 * Gets a pre-formatted error message consting of the same contents as getErrorShort(),
		 * but also including context information.
		 * @note Callers of this method should be capable of handling multiline strings.
		 * @return a string consisting of: type of error line:at. \n context.
		 * @since 1.0
		 */
		std::string getError() const noexcept;
		
		/**
		 * Gets the type of error. The returned value will correspond to a constant within BdfError.
		 * indicating what error occured (e.g. ERROR_OUT_OF_RANGE).
		 * An English-language string can be obtained by: BdfError::ERRORS[this->getType()].
		 * @return an integer corresponding to the type of error.
		 * @since 1.0
		 */
		int getType() const noexcept;
		
		/**
		 * Gets the line at which the error occured.
		 * @return an integer representing the line number in the file where the error occured, or -1 if unknown.
		 * @since 1.4.0
		 */
		int getLine() const noexcept;
		
		/**
		 * Gets the character on the line at getLine() at which the error occured.
		 * @return an integer representing the character number on the line where the error occured, or -1 if unknown.
		 * @since 1.4.0
		 */
		int getAt() const noexcept;
		
		/**
		 * Gets the context which shows where and how the error occured.
		 * @note Callers of this method should be capable of handling multiline strings.
		 * @return a string representing the exact data which caused the error, or blank if unknown.
		 * @since 1.4.0
		 */
		std::string getContext() const noexcept;
	
		/**
		 * Get the C++ exception error message. Same as getError().c_str().
		 * @note Callers of this method should be capable of handling multiline strings.
		 * @return a const char* string representing the error.
		 * @since 1.0
		 */
		virtual const char* what() const noexcept;
		
		std::stacktrace getTrace() const noexcept;
	};
}

#endif
