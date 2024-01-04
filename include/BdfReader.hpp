
#ifndef BDFREADER_HPP_
#define BDFREADER_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>
#include <filesystem>

namespace Bdf
{
	class BdfReader
	{
	protected:
		BdfObject* bdf;
		BdfLookupTable* lookupTable;
		void initEmpty();
	
	public:
		/**
		 * Default constructor. Initialises an empty BdfReader, which contains an undefined type BdfObject
		 * (which may still be used for setting data).
		 */
		BdfReader();
		
		/**
		 * Initialises a BdfReader by reading data until size as binary BDF data.
		 * @param database a pointer to characters representing binary BDF data.
		 * @param size the size to read from database.
		 */
		BdfReader(const char* data, size_t size);

		/** 
		 * Move constructor. Takes ownership of all of BdfReader's resources as well all BdfObjects it owns.
		 * The BdfReader given in that becomes a default constructed reader.
		 */
		BdfReader(BdfReader &&that) noexcept;
		
		/**
		 * Switches the pointers of this with that for both the lookupTable and object.
		 */
		void swap(BdfReader &&that) noexcept;
		
		/**
		 * Deleted (no copy constructor).
		 * Unfortunately, due to how BdfReaders are unserialised, it's not possible to copy objects purely based
		 * on any given BdfReader.
		 */
		BdfReader(const BdfReader&) = delete;
		
		/**
		 * Move assignment operator. Replaces the contents of this with the BdfReader specified in that.
		 * The BdfReader given in that becomes a default constructed reader.
		 * @warning All pointers to objects created from the old BdfObject will also become invalid.
		 */
		BdfReader& operator=(BdfReader &&that) noexcept;
		
		/**
		 * Deleted (no copy assignment).
		 */
		BdfReader& operator=(const BdfReader&) = delete;
		
		/**
		 * Destroys the BDF reader.
		 */
		virtual ~BdfReader();
		
		/**
		 * Serialises binary-encoded BDF data to data, then sets size to the number of bytes written.
		 * @param data a char buffer that data will be written to.
		 * @param size an integer which will store the size.
		 */
		void serialize(char** data, int* size);

		/**
		 * Gets the object currently associated with this BdfReader.
		 * If default-constructed, this object will contain a BdfReader of undefined type. Otherwise,
		 * the reader will contain a representation of the data used to create the reader.
		 * @return a pointer to a BdfObject.
		 */
		BdfObject* getObject();
		
		/**
		 * Resets the BdfObject currently associated with this reader and returns a new, undefined
		 * object.
		 * All data in the original object will be lost.
		 * @warning All pointers to objects created from the old BdfObject will also become invalid.
		 * @return a pointer to a default BdfObject.
		 */
		BdfObject* resetObject();
		
		/**
		 * Serialises human-readable BDF data and returns it as a string. This overload
		 * allows you to specify how the data will be formated (using indent and spacing).
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data using the indenter provided at indent.
		 */
		std::string serializeHumanReadable(BdfIndent indent);
		
		/**
		 * Serialises human-readable BDF data and returns it as a string.
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data.
		 */
		std::string serializeHumanReadable();
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream.
		 */
		void serializeHumanReadable(std::ostream &stream);
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream, indented using the indenter provided at indent.
		 */
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent);  
	};
}

#endif
