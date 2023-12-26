
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

BdfError::BdfError(const int code, BdfStringReader reader, int length)
{
	type = code;
		
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
	
	error_short = ERRORS[code] + " " + std::to_string(this->line) + ":" + std::to_string(this->at);
	
	context = cv.to_bytes(std::wstring(start_of_line, line_size)) + "\n";
	context += spacer;

	for(int i=0;i<length;i++) {
		context += "^";
	}

	message += "\n" + context;
}

BdfError::BdfError(const int code, BdfStringReader reader) : BdfError(code, reader, 1) {
}

std::string BdfError::getErrorShort() {
	return error_short;
}

std::string BdfError::getError() {
	return message;
}

int BdfError::getType() {
	return type;
}

// Get the line number at which the error occured.
int BdfError::getLine() {
	return this->line;
}

// Get the character number at which the error occured.
int BdfError::getAt() {
	return this->at;
}

// Get the context at which the error occured.
std::string BdfError::getContext() {
	return this->context;
}

const char* BdfError::what() const throw() {
	return message.c_str();
}
