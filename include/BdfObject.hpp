
#ifndef BDFOBJECT_HPP_
#define BDFOBJECT_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>

namespace Bdf
{
	/**
  	 * Class that represents a standard BDF node, or object.
    	 * They can be cast to values, if they are of the correct type. Alternatively, you can set their type and data.
         * @since 1.0
	 */
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

		/**
  		 * @internal
     		 */
		void getLocationUses(int* locations);

		/**
  		 * @internal
     		 */
		int serializeSeeker(int* locations);

		/**
  		 * @internal
     		 */
		int serialize(char* data, int* locations, unsigned char flags);

		/**
  		 * @internal
     		 */
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent, int upto);

		/**
  		 * @internal
     		 */
		static void getFlagData(const char* data, char* type, char* size_bytes, char* parent_flags);

		/**
  		 * @internal
     		 */
		static char getSizeBytes(char size_bytes);

		/**
  		 * @internal
     		 */
		static int getSize(const char* data);

		/**
  		 * Gets the location of the key specified at key.
     		 * @internal
		 */
		int getKeyLocation(std::string key);

		/**
  		 * Gets the name of the key located at key.
     		 * @internal
		 */
		std::string getKeyName(int key);
	
		/**
		 * Attempts to cast the object to a std::string. If the object is not already a string, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a string that represents the one in the object if already a string, blank if a new one had to be created.
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

		/**
  		 * Automatically chooses the most appropriate integer size for the integer at v.
     		 * @param v the integer to write to the object.
		 * @return the BdfObject, now with v written.
   		 * @since 1.0
      		 */
		BdfObject* setAutoInt(int64_t v);

		/**
  		 * Return any integer type stored in the object.
     		 * @return the integer stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
		int64_t getAutoInt();
	
		// Get
	
		// Primitives
		/**
  		 * Return an integer if one is stored in this.
     		 * @return the integer stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
		int32_t getInteger();

		/**
  		 * Return a boolean if one is stored in this.
     		 * @return the boolean stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on false as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
		bool getBoolean();

		/**
  		 * Return a long if one is stored in this.
     		 * @return the long stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
	 	int64_t getLong();

		/**
  		 * Return a short if one is stored in this.
     		 * @return the short stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
	 	int16_t getShort();

		/**
  		 * Return a byte if one is stored in this.
     		 * @return the byte stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
	 	char getByte();

		/**
  		 * Return a double if one is stored in this.
     		 * @return the double stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
		double getDouble();

		/**
  		 * Return a float if one is stored in this.
     		 * @return the float stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
		float getFloat();
	
		// Arrays
		/**
		 * Provides a pointer to a copy of an integer array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
		void getIntegerArray(int32_t **v, int* s) const;
		
		/**
		 * Provides a pointer to a copy of a boolean array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
		void getBooleanArray(bool **v, int* s) const;
		
		/**
		 * Provides a pointer to a copy of a long array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
	 	void getLongArray(int64_t **v, int* s) const;
		
		/**
		 * Provides a pointer to a copy of a short array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
	 	void getShortArray(int16_t **v, int* s) const;
		
		/**
		 * Provides a pointer to a copy of a byte array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
	 	void getByteArray(char **v, int* s) const;
		
		/**
		 * Provides a pointer to a copy of a double array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
		void getDoubleArray(double **v, int* s) const;
		
		/**
		 * Provides a pointer to a copy of a float array at v.
		 * It is the caller's responsibility to delete the data after calling this function.
		 * @param v pointer to where the array will be written.
		 * @param s an integer to which the size of the array will be written. It is the caller's
		 *        responsibility to ensure this size is never exceeded.
		 * @since 1.0
		 */
		void getFloatArray(float **v, int* s) const;
	
		// Objects
		std::string getString();
		BdfList* getList();
		BdfNamedList* getNamedList();
	
		// Set
	
		// Primitives
		/**
		 * Sets the BdfObject using the integer given at v.
		 * All data in the original object will be lost.
		 * @param v the new integer to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setInteger(int32_t v);

		/**
		 * Sets the BdfObject using the boolean given at v.
		 * All data in the original object will be lost.
		 * @param v the new boolean to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setBoolean(bool v);

		/**
		 * Sets the BdfObject using the long given at v.
		 * All data in the original object will be lost.
		 * @param v the new long to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
	 	BdfObject* setLong(int64_t v);

		/**
		 * Sets the BdfObject using the short given at v.
		 * All data in the original object will be lost.
		 * @param v the new short to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
	 	BdfObject* setShort(int16_t v);

		/**
		 * Sets the BdfObject using the byte given at v.
		 * All data in the original object will be lost.
		 * @param v the new byte to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
	 	BdfObject* setByte(char v);

		/**
		 * Sets the BdfObject using the double given at v.
		 * All data in the original object will be lost.
		 * @param v the new double to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setDouble(double v);

		/**
		 * Sets the BdfObject using the float given at v.
		 * All data in the original object will be lost.
		 * @param v the new float to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setFloat(float v);
	
		// Arrays
		/**
		 * Sets the BdfObject using the integer array given at v.
		 * All data in the original object will be lost.
		 * @param v the new integer array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setIntegerArray(const int32_t *v, int size);

		/**
		 * Sets the BdfObject using the boolean array given at v.
		 * All data in the original object will be lost.
		 * @param v the new boolean array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setBooleanArray(const bool *v, int size);

		/**
		 * Sets the BdfObject using the long array given at v.
		 * All data in the original object will be lost.
		 * @param v the new long array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
	 	BdfObject* setLongArray(const int64_t *v, int size);

		/**
		 * Sets the BdfObject using the short array given at v.
		 * All data in the original object will be lost.
		 * @param v the new short array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
	 	BdfObject* setShortArray(const int16_t *v, int size);

		/**
		 * Sets the BdfObject using the byte array given at v.
		 * All data in the original object will be lost.
		 * @param v the new byte array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
	 	BdfObject* setByteArray(const char *v, int size);

		/**
		 * Sets the BdfObject using the double array given at v.
		 * All data in the original object will be lost.
		 * @param v the new double array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setDoubleArray(const double *v, int size);

		/**
		 * Sets the BdfObject using the float array given at v.
		 * All data in the original object will be lost.
		 * @param v the new float array to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setFloatArray(const float *v, int size);
	
		// Objects
		/**
		 * Sets the BdfObject using the string given at v.
		 * All data in the original object will be lost.
		 * @param v the new string to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setString(std::string v);

		/**
		 * Sets the BdfObject using the BdfList given at v.
		 * All data in the original object will be lost.
		 * @param v the new BdfList to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setList(BdfList* v);

		/**
		 * Sets the BdfObject using the BdfNamedList given at v.
		 * All data in the original object will be lost.
		 * @param v the new BdfNamedList to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setNamedList(BdfNamedList* v);

		// Hybrid getters and setters
		/**
		 * Sets this BdfObject to a blank BdfList, and returns that BdfList.
		 * All data in the original object will be lost.
		 * @return a pointer to the new BdfList that was written to this BdfObject.
   		 * @since 1.4.0
		 */
		BdfList* newSetAndGetList();
		
		/**
		 * Sets this BdfObject to a blank BdfNamedList, and returns that BdfNamedList.
		 * All data in the original object will be lost.
		 * @return a pointer to the new BdfNamedList that was written to this BdfObject.
   		 * @since 1.4.0
		 */
		BdfNamedList* newSetAndGetNamedList();

		/**
		 * If this is a BdfList, returns it. Otherwise, the object is automatically converted to a list.
		 * All data in the original will be lost.
   		 * @return a pointer to a BdfList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note In this version, this method does the same thing as getList(); however, it is recommended that callers switch to calling
		 *       this method over getList(), as its behaviour will change in 2.0.0.
   		 * @since 1.4.0
		 */
		BdfList* getOrNewList();
		
		/**
		 * If this is a BdfNamedList, returns it. Otherwise, the object is automatically converted to a named list.
		 * All data in the original will be lost.
   		 * @return a pointer to a BdfNamedList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note In this version, this method does the same thing as getNamedList(); however, it is recommended that callers switch to calling
		 *       this method over getNamedList(), as its behaviour will change in 2.0.0.
   		 * @since 1.4.0
		 */
		BdfNamedList* getOrNewNamedList();
	};
}

#endif
