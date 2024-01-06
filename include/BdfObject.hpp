
#ifndef BDFOBJECT_HPP_
#define BDFOBJECT_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>

namespace Bdf
{
	class BdfObject
	{
	private:
	
		BdfLookupTable* lookupTable;
		int last_seek;
		void *object;
		char type;
		char *data;
		int s;

		/**
		 * Free all resources associated with this object.
		 * After calling, this->getType() == BdfTypes::UNDEFINED.
		 *
		 * @internal
		 */
		void freeAll();
	
	public:
	
		/**
		 * Initialises an empty BdfObject.
		 * @internal
		 */
		BdfObject(BdfLookupTable* lookupTable);
		
		/**
		 * Initialises a BdfObject from the data given at data up to the given size.
		 * @internal
		 */
		BdfObject(BdfLookupTable* lookupTable, const char *data, int size);
		
		/**
		 * Initialises a BdfObject using the given StringReader at sr.
		 * @internal
		 */
		BdfObject(BdfLookupTable* lookupTable, BdfStringReader* sr);

		/**
		 * Initialises a BdfObject using the given StringReader at sr.
		 * @internal
		 */
		BdfObject(BdfLookupTable* lookupTable, BdfStringReader* sr);
		
		/**
		 * Deleted (no copy constructor).
		 */
		BdfObject(const BdfObject &that);
		
		/**
		 * Deleted (no move constructor).
		 * If we did make one available, you would easily be able to mess with BdfReaders by doing BdfObject evil = new BdfObject(std::move(*victim.getObject()),
		 * which would cause victim to suddenly hold a pointer to an empty BdfObject!
		 * Instead, use BdfReader's move constructor.
		 * @see BdfReader::BdfReader(BdfObject &&that)
		 */
		BdfObject(BdfObject&&) = delete;

		/**
		 * Destroys a BdfObject.
		 * @internal
		 */
		virtual ~BdfObject();
	
		/**
		 * Gets the type of object this currently is.
		 * The returned value can be compared against a type listed in BdfTypes.
		 * @return a char representing the type of BdfObject.
		 */
		char getType() const noexcept;

		/**
		 * Returns true if the BdfObject has valid data, i.e. any type other than BdfTypes::UNDEFINED.
		 * Equivalent to the comparison (example->getType() != BdfTypes::UNDEFINED).
		 * @return true if this->type() is anything other than BdfTypes::UNDEFINED, false if this->
		 * getType() == BdfTypes::UNDEFINED.
		 */
		explicit operator bool() const noexcept;

		void getLocationUses(int* locations);
		int serializeSeeker(int* locations);
		int serialize(char* data, int* locations, unsigned char flags);
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent, int upto);
	
		static void getFlagData(const char* data, char* type, char* size_bytes, char* parent_flags);
		static char getSizeBytes(char size_bytes);
		static int getSize(const char* data);
		
		int getKeyLocation(std::string key);
		std::string getKeyName(int key);
	
		/**
		 * Attempts to cast the object to a std::string.
		 * @return an std::optional that contains a copy of the string if casting succeeded.
		 * @warning Calling getString() in order to force this object to become a string is deprecated. From 2.0.0, this method will return
                 *          std::nullopt if the object is not already a string.
		 */
		std::string getString() const noexcept;
		
		/**
		 * Attempts to cast the object to a BdfList. If the object is not already a list, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a pointer to a BdfList; the current one associated with this object if already a list, or a brand new one if converted.
		 * @warning Calling getList() in order to force this object to become a list is deprecated. From 2.0.0, this method will return
                 *          nullptr if the object is not already a list.
		 */
		BdfList* getList() const noexcept;
		
		/**
		 * Attempts to cast the object to a BdfNamedList. If the object is not already a named list, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a pointer to a BdfNamedList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @warning Calling getNamedList() in order to force this object to become a named list is deprecated. From 2.0.0, this method will return
                 *          nullptr if the object is not already a list.
		 */
		BdfNamedList* getNamedList() const noexcept;
	
		BdfObject* setAutoInt(long v);
		long getAutoInt();
	
		// Get
	
		// Primitives
		int32_t getInteger();
		bool getBoolean();
	 	int64_t getLong();
	 	int16_t getShort();
	 	char getByte();
		double getDouble();
		float getFloat();
	
		// Arrays
		void getIntegerArray(int32_t **v, int* s);
		void getBooleanArray(bool **v, int* s);
	 	void getLongArray(int64_t **v, int* s);
	 	void getShortArray(int16_t **v, int* s);
	 	void getByteArray(char **v, int* s);
		void getDoubleArray(double **v, int* s);
		void getFloatArray(float **v, int* s);
	
		// Objects
		std::string getString();
		BdfList* getList();
		BdfNamedList* getNamedList();
	
		// Set
	
		// Primitives
		BdfObject* setInteger(int32_t v);
		BdfObject* setBoolean(bool v);
	 	BdfObject* setLong(int64_t v);
	 	BdfObject* setShort(int16_t v);
	 	BdfObject* setByte(char v);
		BdfObject* setDouble(double v);
		BdfObject* setFloat(float v);
	
		// Arrays
		BdfObject* setIntegerArray(const int32_t *v, int size);
		BdfObject* setBooleanArray(const bool *v, int size);
	 	BdfObject* setLongArray(const int64_t *v, int size);
	 	BdfObject* setShortArray(const int16_t *v, int size);
	 	BdfObject* setByteArray(const char *v, int size);
		BdfObject* setDoubleArray(const double *v, int size);
		BdfObject* setFloatArray(const float *v, int size);
	
		// Objects
		BdfObject* setString(std::string v);
		BdfObject* setList(BdfList* v);
		BdfObject* setNamedList(BdfNamedList* v);

		// Hybrid getters and setters
		/**
		 * If this is a BdfList, returns it. Otherwise, the object is automatically converted to a list.
		 * All data in the original will be lost.
   		 * @return a pointer to a BdfList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note In this version, this method does the same thing as getList(); however, it is recommended that callers switch to calling
		 *       this method over getList(), as its behaviour will change in 2.0.0.
		 */
		BdfList* getOrNewList();
		
		/**
		 * If this is a BdfNamedList, returns it. Otherwise, the object is automatically converted to a named list.
		 * All data in the original will be lost.
   		 * @return a pointer to a BdfNamedList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note In this version, this method does the same thing as getNamedList(); however, it is recommended that callers switch to calling
		 *       this method over getNamedList(), as its behaviour will change in 2.0.0.
		 */
		BdfNamedList* getOrNewNamedList();
	};
}

#endif
