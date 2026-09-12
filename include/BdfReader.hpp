
#ifndef BDFREADER_HPP_
#define BDFREADER_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>

namespace Bdf
{
	class BdfReader
	{
	protected:
		BdfObject* bdf;
		BdfLookupTable* lookupTable;
		void initEmpty();
	
	public:
		BdfReader();
		BdfReader(const char* database, int size);
		virtual ~BdfReader();
		void serialize(char** data, int* size);
		BdfObject* getObject();
		BdfObject* resetObject();
		
		/**
		 * Serialises human-readable BDF data and returns it as a string. This overload
		 * allows you to specify how the data will be formated (using indent and spacing).
   		 * @param indent settings used for indenting the human-readable BDF data.
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data using the indenter provided at indent.
   		 * @since 1.0
		 */
		std::string serializeHumanReadable(const BdfIndent &indent);
		
		/**
		 * Serialises human-readable BDF data and returns it as a string.
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data.
   		 * @since 1.0
		 */
		std::string serializeHumanReadable();
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream, indented using the indenter provided at indent.
   	         * @param stream an output stream to which human-readable BDF data will be sent.
	         * @param indent settings used for indenting the human-readable BDF data.
   		 * @since 1.0
		 */
		void serializeHumanReadable(std::ostream &stream, const BdfIndent &indent);  
	};
}

#endif
