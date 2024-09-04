
#ifndef BDFREADER_HPP_
#define BDFREADER_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>

namespace Bdf
{
	/**
	 * Class that unserialises binary BDF data and exposes it through a pointed at BdfObject, allowing callers to interface with it programatically.
         * Data may also be serialised as either binary data or human-readable data.
	 *
         * This class or one of its extended classes should be your main entry point to interfacing with the BdfReader library. 
	 * To use it, call the BdfReader(const char* data, size_t size) constructor, which will unserialise the data.
         *
	 * Make sure that the BdfReader remains alive for the duration that you need to interface with its BdfObject. Once it is destructed (either by going
	 * out of scope or by being the left-hand side value of move assignment, all pointers to its object and any subobjects become invalid; their behaviour
	 * when dereferenced from this point on is undefined. Consider moving the object using the move constructor and assignment operators to prevent
	 * invalidating pointers. In addition, do not attempt to delete any pointers obtained using this object.
	 *
	 * @newable
         * @stable to extend
         * @since 1.0
	 */
	class BdfReader
	{
	protected:
		BdfObject* bdf;
		BdfLookupTable* lookupTable;

		/**
                 * Initialises an empty BdfReader.
		 * @since 1.0
   		 */
		void initEmpty();
	
	public:
		/**
		 * Default constructor. Initialises an empty BdfReader, which contains an undefined type BdfObject
		 * (which may still be used for setting data).
   		 * @since 1.0
		 */
		BdfReader();
		
		/**
		 * Initialises a BdfReader by reading data until size as binary BDF data.
		 * @param database a pointer to characters representing binary BDF data.
		 * @param size the size to read from database.
   		 * @since 1.0
		 */
		BdfReader(const char* data, size_t size);

		/** 
		 * Move constructor. Takes ownership of all of BdfReader's resources as well as all BdfObjects it owns.
		 * The BdfReader given in that becomes a default constructed reader.
                 * @param that the object which will be used to construct this reader.
		 * @since 1.4.0
		 */
		BdfReader(BdfReader &&that) noexcept;
		
		/**
		 * Switches the pointers of this with that for both the lookupTable and object.
   		 * @param that the object to switch pointers with.
                 * @since 1.4.0
		 */
		void swap(BdfReader &&that) noexcept;
		
		/**
		 * Deleted (no copy constructor).
		 * Unfortunately, due to how BdfReaders are unserialised, it's not possible to copy objects purely based
		 * on any given BdfReader. This may change in 2.0.
                 * @since 1.4.0
		 */
		BdfReader(const BdfReader&) = delete;
		
		/**
		 * Move assignment operator. Replaces the contents of this with the BdfReader specified in that.
		 * The BdfReader given in that becomes a default constructed reader.
		 * @warning All pointers to objects created from the old BdfObject will also become invalid.
   		 * @param that the object which will be assigned to this.
      	         * @return a new BdfReader using that's resources.
	 	 * @since 1.4.0
		 */
		BdfReader& operator=(BdfReader &&that) noexcept;
		
		/**
		 * Deleted (no copy assignment).
                 * @since 1.4.0
		 */
		BdfReader& operator=(const BdfReader&) = delete;
		
		/**
		 * Destroys the BDF reader.
   		 * @since 1.0
		 */
		virtual ~BdfReader();
		
		/**
		 * Serialises binary-encoded BDF data to data, then sets size to the number of bytes written.
		 * @param data a char buffer that data will be written to.
		 * @param size an integer which will store the size.
   		 * @since 1.0
		 */
		void serialize(char** data, int* size);

		/**
		 * Gets the object currently associated with this BdfReader.
		 * If default-constructed, this object will contain a BdfReader of undefined type. Otherwise,
		 * the reader will contain a representation of the data used to create the reader.
		 * @return a pointer to a BdfObject.
   		 * @since 1.0
		 */
		BdfObject* getObject();
		
		/**
		 * Resets the BdfObject currently associated with this reader and returns a new, undefined
		 * object.
		 * All data in the original object will be lost.
		 * @warning All pointers to objects created from the old BdfObject will also become invalid.
		 * @return a pointer to a default BdfObject.
   		 * @since 1.0
		 */
		BdfObject* resetObject();
		
		/**
		 * Serialises human-readable BDF data and returns it as a string. This overload
		 * allows you to specify how the data will be formated (using indent and spacing).
   		 * @param indenter settings used for indenting the human-readable BDF data.
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data using the indenter provided at indent.
   		 * @since 1.0
		 */
		std::string serializeHumanReadable(BdfIndent indent);
		
		/**
		 * Serialises human-readable BDF data and returns it as a string.
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data.
   		 * @since 1.0
		 */
		std::string serializeHumanReadable();
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream.
   		 * @param stream an output stream to which human-readable BDF data will be sent.
   		 * @since 1.0
		 */
		void serializeHumanReadable(std::ostream &stream);
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream, indented using the indenter provided at indent.
   	         * @param stream an output stream to which human-readable BDF data will be sent.
	         * @param indent settings used for indenting the human-readable BDF data.
   		 * @since 1.0
		 */
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent);  
	};
}

#endif
