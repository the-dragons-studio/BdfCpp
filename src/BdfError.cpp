
#include "../include/BdfStringReader.hpp"
#include "../include/BdfError.hpp"

#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

using namespace Bdf;

const std::string ERRORS[5] = {
	"Syntax error",
	"End of file",
	"Unescaped comment",
	"Unescaped string",
	"Number out of range",
};

BdfError::BdfError(BdfError::ErrorType type, BdfStringReader reader, size_t length, std::stacktrace trace): type(type), trace(trace)
{
	const wchar_t* start_of_line = reader.start;
	int line = 0;
	int at = 0;

	if(reader.upto > reader.end - 2) {
		reader.upto = reader.end - 2;
	}

	for(const wchar_t* i=reader.start;i<reader.upto;i++)
	{
		if(i[0] == '\n') {
			start_of_line = i + 1;
			line += 1;
			at = 0;
			continue;
		}

		at += 1;
	}

	int line_size = 0;
	std::string spacer = "";

	for(const wchar_t* i=start_of_line;i<reader.end;i++)
	{
		if(i[0] == '\n') {
			break;
		}

		line_size += 1;

		if(i == reader.end - 1) {
			break;
		}

		if(i < reader.upto)
		{
			if(i[0] == '\t') {
				spacer += "\t";
				continue;
			}

			spacer += " ";
		}
	}

	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> cv;

	this->line = line + 1;
	this->at = at + 1;
	
	error_short = this->getErrorEnglishDescription() + " " + std::to_string(*this->line) + ":" + std::to_string(*this->at);
	
	context = cv.to_bytes(std::wstring(start_of_line, line_size)) + "\n";
	context += spacer;

	for(int i=0;i<length;i++) {
		context += "^";
	}

	message += "\n" + context;
}

std::string BdfError::getErrorEnglishDescription(std::optional<BdfError::ErrorType> type) {
	switch (type.value_or(this->type)) {
        case BdfError::ErrorType::SYNTAX: return "Syntax error";
        case BdfError::ErrorType::UNEXPECTED_END_OF_FILE: return "Unexpected end of file";
        case BdfError::ErrorType::UNCLOSED_COMMENT_BEFORE_EOF: return "Multiline comment was not closed before the end of the file";
		case BdfError::ErrorType::UNCLOSED_STRING_BEFORE_EOF: return "String was not closed before the end of the file";
		case BdfError::ErrorType::ILLEGAL_STRING_BACKSLASH_ESCAPE: return "Illegal string backslash escape code";
		case BdfError::ErrorType::NUMERICAL_OUT_OF_RANGE: return "Number out of range of the given datatype";
		case BdfError::ErrorType::BINARY_SIZE_TAG_MISMATCH: return "Size tag found in binary data does not match actual data size";
    }
	
	return "Unknown error";
}

BdfError::BdfError(const int code, BdfStringReader reader, int length): BdfError(this->getErrorTypeFromClassicCode(code), reader, length) {
}

BdfError::BdfError(BdfError::ErrorType type, BdfStringReader reader): BdfError(type, reader, 1) {
}

BdfError::BdfError(const int code, BdfStringReader reader) : BdfError(this->getErrorTypeFromClassicCode(code), reader, 1) {
}

BdfError::BdfError(BdfError::ErrorType type, std::stacktrace trace): type(type), trace(trace) {}

BdfError::BdfError(const int code): BdfError(this->getErrorTypeFromClassicCode(code)) {}

BdfError::ErrorType BdfError::getErrorTypeFromClassicCode(int code) {
	// Define an array with the enums in the exact order that the classic error codes went in.
	static std::array<BdfError::ErrorType, 5> classicCodeOrderedEnums({
			BdfError::ErrorType::SYNTAX,
			BdfError::ErrorType::UNEXPECTED_END_OF_FILE,
			BdfError::ErrorType::UNCLOSED_COMMENT_BEFORE_EOF,
			BdfError::ErrorType::UNCLOSED_STRING_BEFORE_EOF,
			BdfError::ErrorType::NUMERICAL_OUT_OF_RANGE
	});
	
	if (code >= 0 && code < classicCodeOrderedEnums.size()) {
		return classicCodeOrderedEnums[code];
	} else {
		return BdfError::ErrorType::SYNTAX;
	}
}

std::string BdfError::getErrorShort() const noexcept {
	return error_short;
}

std::string BdfError::getError() const noexcept {
	return message;
}

BdfError::ErrorType BdfError::getErrorType() const noexcept {
	return this->type;
}

// Get the line number at which the error occured.
size_t BdfError::getLine() const noexcept {
	return *this->line;
}


std::optional<size_t> BdfError::getLineOptional() const noexcept {
	return this->line;
}

// Get the character number at which the error occured.
size_t BdfError::getAt() const noexcept {
	return *this->at;
}

std::optional<size_t> BdfError::getAtOptional() const noexcept {
	return this->at;
}

// Get the context at which the error occured.
std::string BdfError::getContext() const noexcept {
	return this->context;
}

std::stacktrace BdfError::getTrace() const noexcept {
	return this->trace;
}

const char* BdfError::what() const noexcept {
	return message.c_str();
}