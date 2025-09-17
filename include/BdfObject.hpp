
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
	
		void freeAll();
	
	public:
	
		BdfObject(BdfLookupTable* lookupTable);
		BdfObject(BdfLookupTable* lookupTable, const char *data, int size);
		BdfObject(BdfLookupTable* lookupTable, BdfStringReader* sr);
	
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
		void getLocationUses(int* locations) const;

		/**
  		 * @internal
     	 */
		int serializeSeeker(int* locations) const;

		/**
  		 * @internal
     	 */
		int serialize(char* data, int* locations, unsigned char flags) const;

		/**
  		 * @internal
         */
		void serializeHumanReadable(std::ostream &stream, const BdfIndent &indent, int upto);

		/**
  		 * @internal
     	 */
		static void getFlagData(const char* data, char* type, char* size_bytes, char* parent_flags);
		static char getSizeBytes(char size_bytes);
		static int getSize(const char* data);
		
		int getKeyLocation(std::string key);
		std::string getKeyName(int key);
	
		BdfObject* newObject();
		BdfNamedList* newNamedList();
		BdfList* newList();
	
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
	};
}

#endif
