
#ifndef BDFREADER_HPP_
#define BDFREADER_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>
#include <filesystem>

namespace Bdf
{
	/**
	 * Class that unserialises BDF data and exposes it through a pointed at BdfObject, allowing callers to interface with it programatically.
     * Data may also be serialised as either binary data or human-readable data.
	 *
     * This class should be your main entry point to interfacing with the BdfReader library. To use it, call the BdfReader(const char* data, size_t size)
	 * constructor, which will unserialise the data.
     *
	 * Make sure that the BdfReader remains alive for the duration that you need to interface with its BdfObject. Once it is destructed (either by going
	 * out of scope or by being the left-hand side value of move assignment, all pointers to its object and any subobjects become invalid; their behaviour
	 * when dereferenced from this point on is undefined. Consider moving the object using the move constructor and assignment operators to prevent
	 * invalidating pointers. In addition, do not attempt to delete any pointers obtained using this object.
	 *
     * @since 1.0
	 * @newable
     * @stable to extend
	 */
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
		 * @todo: This is planned for after BdfObject copy assignment is implemented.
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
		void serialize(char** data, int* size) const;
			
		/**
		* Get a pointer to a BdfReader best suited to unserialising the data given at filename.
		* That BdfReader will be returned having already unserialised the data at the file.
		*
		* This function should only be used by programs that follow the below guideline for file
		* extensions in BDF format:
		* - .bdf <-> BdfReader
		* - .hbdf <-> BdfReaderHuman
		* - .bdfgz <-> BdfReaderGzip
		* - .bdfxz <-> BdfReaderXz
		*
		* @warning This function has undefined behaviour if the file does not contain the type
		* of data expected given its filename (such as human readable data inside a .bdf file).
		*
		* @param filename the filename which contains BDF data that needs to be unserialised.
		* @return a pointer to a BdfReader that was used to unserialise the file.
		* @throw std::out_of_range if the requested extension could not be matched to a BdfReader,
		* and any exceptions that might be thrown during BdfReader construction such as BdfError.
		*/
		static BdfReader getBestBdfReaderForFile(std::filesystem::path filename);
		
		/**
		 * Gets the object currently associated with this BdfReader.
		 * This overload cannot be selected if the BdfReader is declared const.
		 * If default-constructed, this object will contain a BdfReader of undefined type. Otherwise,
		 * the reader will contain a representation of the data used to create the reader.
		 * @return a pointer to a BdfObject.
		 */
		BdfObject* getObject();
		
		/**
		 * Gets a pointer to a const object associated with this BdfReader.
		 * This overload will be selected for a const BdfReader and will not permit changing
		 * If default-constructed, this object will contain a BdfReader of undefined type. Otherwise,
		 * the reader will contain a representation of the data used to create the reader.
		 * @return a pointer to a const BdfObject.
		 */
		const BdfObject * getObject() const;
		
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
		std::string serializeHumanReadable(BdfIndent indent) const;
		
		/**
		 * Serialises human-readable BDF data and returns it as a string.
		 * @return a string representing the contents of the BdfObject contained serialised
		 * to human-readable string data.
		 */
		std::string serializeHumanReadable() const;
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream.
		 */
		void serializeHumanReadable(std::ostream &stream) const;
		
		/**
		 * Streams human-readable BDF data representing the BdfObject contained in the reader
		 * to &stream, indented using the indenter provided at indent.
		 */
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent) const;  
	};
}

#endif
