
#ifndef BDFSTRINGREADER_HPP_
#define BDFSTRINGREADER_HPP_

#include <string>

namespace Bdf
{
	class BdfStringReader
	{
	public:
		const wchar_t* start;
		const wchar_t* end;
		const wchar_t* upto;
	
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
		std::string getQuotedString();
		bool isNext(std::wstring check);
		bool inRange();
	};
}

#endif
