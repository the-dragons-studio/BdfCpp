
#ifndef BDFERROR_HPP_
#define BDFERROR_HPP_

// #include "BdfStringReader.hpp"

#include <cstdint>
#include <optional>
#include <string>

#if __cplusplus >= 202302L
#include <stacktrace>
#endif

namespace Bdf
{	
	class BdfStringReader;
	
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
	public:
		/**
		 * Enumeration type representing the type of error.
   		 * @since 1.5.0
		 */
		enum class ErrorType {
			/**
			 * Indicates that a syntax error was detected.
		 	 * @note This error can only occur when parsing a human-readable file.
	    	 */
			SYNTAX,
			
			/**
			 * Indicates that a premature end of file was found. For example, the library
			 * was trying to read an integer but did not find the datatype character before the end of file.
             * @note Versions before 2.0.0 could not actually throw this error. That's
             *       because this "error" was used to indicate that parsing was finished,
			 *       and a finished BdfReaderHuman object could be presented. This was rewritten
             *       in 2.0.0, and the error was instead repurposed to indicate *premature*
			 *       end of file.
			 * @note This error can only occur when parsing a human-readable file.
			 */
			UNEXPECTED_END_OF_FILE, 
			
			/**
			 * Indicates that a multiline comment was not escaped before the end of file was found.
		 	 * @note This error can only occur when parsing a human-readable file.
	         */
			UNCLOSED_COMMENT_BEFORE_EOF,

			/**
			 * Indicates that a multiline comment was not escaped before the end of file was found.
		 	 * @note This error can only occur when parsing a human-readable file.
	         */
			UNCLOSED_STRING_BEFORE_EOF,

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
			
			/**
			 * Indicates that allocation of sufficient size for the BdfLookupTable has failed. Most likely due to a corrupt binary BDF data.
			 * @note This error can only occur when parsing a binary file.
			 */
			BINARY_LOOKUPTABLE_SIZE_ALLOCATION_FAILED,
			
			/**
			 * Indicates that the caller tried to set data to an object when that object is not compatible with it.
			 */
			DATA_INCOMPATIBLE_WITH_OBJECT
		};
		
	
	private:
		std::string error_short;
		std::string message;
		BdfError::ErrorType type;
		std::optional<size_t> line = std::nullopt;
		std::optional<size_t> at = std::nullopt;
		std::string context;
		
		#if __cplusplus >= 202302L
		std::stacktrace trace;
		#endif
		
		/**
		 * Returns an ErrorType enum given a "classic" error code.
		 * @note Do not mark this function as C++ deprecated. Instead mark the wrapper overloads that call this method.
		 * @internal
		 * @deprecated since 1.5.0, this method is only here as a transitional method to the new 2.0.0 enum system.
		 * @since 1.5.0
		 */
		BdfError::ErrorType getErrorTypeFromClassicCode(int code);
		
		/**
		 * Returns a "classic" error code given an ErrorType enum.
		 * @note Do not mark this function as C++ deprecated. Instead mark the wrapper overloads that call this method.
		 * @internal
		 * @deprecated since 1.5.0, this method is only here as a transitional method to the new 2.0.0 enum system.
		 * @since 1.5.0
		 */
		int getClassicCodeFromErrorType(BdfError::ErrorType type) const noexcept;
	
	public:
		/**
  		 * @deprecated Use BdfError::ErrorType::SYNTAX instead.
     	 */
		#if __cplusplus >= 201402L
		[[deprecated("Use BdfError::ErrorType::SYNTAX instead.")]]
		#endif
		static const int ERROR_SYNTAX = 0;

		/**
  		 * @deprecated Use BdfError::ErrorType::END_OF_FILE instead.
     	 */
		#if __cplusplus >= 201402L
		[[deprecated("Use BdfError::ErrorType::UNEXPECTED_END_OF_FILE instead.")]]
		#endif
		static const int ERROR_END_OF_FILE = 1;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::UNCLOSED_COMMENT_BEFORE_EOF instead.
     	 */
		#if __cplusplus >= 201402L
		[[deprecated("Use BdfError::ErrorType::UNCLOSED_COMMENT_BEFORE_EOF instead.")]]
		#endif
		static const int ERROR_UNESCAPED_COMMENT = 2;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::UNCLOSED_STRING_BEFORE_EOF instead.
     	 */
		#if __cplusplus >= 201402L
		[[deprecated("Use BdfError::ErrorType::UNCLOSED_STRING_BEFORE_EOF instead.")]]
		#endif
		static const int ERROR_UNESCAPED_STRING = 3;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::NUMERICAL_OUT_OF_RANGE instead.
     	 */
		#if __cplusplus >= 201402L
		[[deprecated("Use BdfError::ErrorType::NUMERICAL_OUT_OF_RANGE instead.")]]
		#endif
		static const int ERROR_OUT_OF_RANGE = 4;
		
		/**
  		 * @deprecated Use BdfError::ErrorType::BINARY_SIZE_TAG_MISMATCH instead.
     	 */
		#if __cplusplus >= 201402L
		[[deprecated("Use BdfError::ErrorType::BINARY_SIZE_TAG_MISMATCH instead.")]]
		#endif
		static const int ERROR_SIZE_TAG_MISMATCH = 5;

		/**
		 * Creates a BdfError consisting of the error type at type and a default stacktrace, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @internal
		 */
		BdfError(BdfError::ErrorType type, BdfStringReader reader, size_t length) noexcept;
	
		/**
		 * Creates a BdfError consisting of the error type at type and the stacktrace given at trace, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @since 1.5.0
		 * @internal
		 */
		#if __cplusplus >= 202302L
		BdfError(BdfError::ErrorType type, BdfStringReader reader, size_t length, std::stacktrace trace) noexcept;
		#endif
		
		/**
		 * Creates a BdfError consisting of the error code at code, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @since 1.0
		 * @internal
		 */
		[[deprecated("Use Bdf::BdfError::BdfError(ErrorType code, const BdfStringReader& reader, size_t length) instead.")]]
		BdfError(const int code, BdfStringReader reader, int length) noexcept;
		
		/**
		 * Creates a BdfError consisting of the error type given at type and a default stacktrace, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @since 1.5.0
		 * @internal
		 */
		BdfError(ErrorType type, BdfStringReader reader) noexcept;
		
		/**
		 * Creates a BdfError consisting of the error type given at type and the stacktrace given at trace, and uses the BdfStringReader at
		 * reader to get line and context information. Up to length bytes will be read.
		 * @since 1.5.0
		 * @internal
		 */
		#if __cplusplus >= 202302L
		BdfError(ErrorType type, BdfStringReader reader, std::stacktrace trace) noexcept;
		#endif
		
		/**
		 * Creates a BdfError consisting of the error code at code, and uses the BdfStringReader at
		 * reader to get line and context information.
		 * @since 1.0
		 * @internal
		 */
		[[deprecated("Use Bdf::BdfError::BdfError(ErrorType code, const BdfStringReader& reader) instead.")]]
		BdfError(const int code, BdfStringReader reader) noexcept;
			    
		/**
		 * Creates a BdfError consisting of the error code code and a default stacktrace.
		 * No other explanatory information will be available (all will be set to default).
		 * @since 1.5.0
	     */
		BdfError(BdfError::ErrorType type) noexcept;
		
		/**
		 * Creates a BdfError consisting of the error type given at type and the stacktrace given at trace.
		 * @since 1.5.0
		 */
		#if __cplusplus >= 202302L
		BdfError(BdfError::ErrorType type, std::stacktrace trace) noexcept;
		#endif

		/**
  		 * @deprecated Use BdfError::BdfError(BdfError::ErrorType type) instead.
		 * @since 1.4.0
     	 */
		[[deprecated("Use Bdf::BdfError::BdfError(ErrorType code) instead.")]]
		explicit BdfError(const int code) noexcept;
		
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
		[[deprecated("Use Bdf::BdfError::getErrorType() instead.")]]
		int getType() const noexcept;
		
		/**
		 * Gets a basic English description of the error type.
		 * Callers may find that the stock descriptions might be unsatisfactory or may want to provide their
		 * own languages.
		 * @since 1.5.0
		 */
		std::string getErrorEnglishDescription(std::optional<BdfError::ErrorType> type = std::nullopt);

		BdfError::ErrorType getErrorType() const noexcept;
		
		/**
		 * Gets the line at which the error occured.
		 * @return an integer representing the line number in the file where the error occured, or -1 if unknown.
		 * @since 1.4.0
		 */
		[[deprecated("Use BdfError::getLineOptional().value() or similar instead.")]]
		int getLine() const noexcept; 
		 
		std::optional<size_t> getLineOptional() const noexcept;
		
		/**
		 * Gets the character on the line at getLine() at which the error occured.
		 * @return an integer representing the character number on the line where the error occured, or -1 if unknown.
		 * @since 1.4.0
		 */
		[[deprecated("Use BdfError::getAtOptional().value() or similar instead.")]]
		int getAt() const noexcept;  
		
		
		std::optional<size_t> getAtOptional() const noexcept;
		
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
		
		/**
		 * Get the stacktrace associated with this object.
		 * @return a stacktrace that should point to this object.
		 * @since 1.5.0
		 */
		#if __cplusplus >= 202302L
		std::stacktrace getTrace() const noexcept;
		#endif
	};
}

#endif
