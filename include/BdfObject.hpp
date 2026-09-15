
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
		mutable int last_seek;
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
	
		#if __cplusplus >= 202002L
		/**
		 * Compares the two C double pointer arrays of type T given in lhs and rhs.
		 *
		 * This function returns:
		 * - the ordering of the first pair of unequal values, as determined by the inbuilt operator<=> for type T.
		 * - Else, the result of lhsSize <=> rhsSize.
		 *
		 * It is the caller's responsibility to ensure that both arrays are freed after this function runs.
		 *
		 * @param lhs a C double pointer array for the left hand side.
		 * @param rhs a C double pointer array for the right hand side.
		 * @param lhsSize the maximum size to check in lhs.
		 * @param rhsSize the maximum size to check in rhs.
		 * @return std::partial_ordering representing the result of the comparison determined by the above algorithm.
		 *
		 * @since 2.0.0
		 * @internal
		 */
		template<typename T> static std::partial_ordering comparePrimitiveArrays(T** lhs, T** rhs, size_t lhsSize, size_t rhsSize) noexcept; 
		#endif
		
		/**
		 * @since 1.5.0
		 * @unstable, will take the place of getString() in 2.0.0 onwards
		 */
		std::optional<std::string> getStringConst() const noexcept;
		
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
		void serializeHumanReadable(std::ostream &stream, const BdfIndent &indent, int upto) const;

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
		int getKeyLocation(std::string key) const;

		/**
  		 * Gets the name of the key located at key.
     	 * @internal
		 */
		std::string getKeyName(int key) const;
		
		/**
		 * Creates a new object based on this object's BdfLookupTable.
		 * @since 1.0
		 */		
		BdfObject* newObject() const;
		
		/**
		 * Creates a new BdfNamedList based on this object's BdfLookupTable.
		 * Unlike coerceNamedList(), this method does not modify the original object's type or data.
		 * @since 1.0
		 */
		BdfNamedList* newNamedList() const;
		
		/**
		 * Creates a new BdfList based on this object's BdfLookupTable.
		 * Unlike coerceList(), this method does not modify the original object's type or data.
		 * @since 1.0
		 */
		BdfList* newList() const;

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
		int64_t getAutoInt() const;
	
		// Get
	
		// Primitives
		/**
  		 * Return an integer if one is stored in this.
     	 * @return the integer stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      		 */
		int32_t getInteger() const noexcept;

		/**
  		 * Return a boolean if one is stored in this.
     	 * @return the boolean stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on false as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      	 */
		bool getBoolean() const noexcept;

		/**
  		 * Return a long if one is stored in this.
     	 * @return the long stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      	 */
	 	int64_t getLong() const noexcept;

		/**
  		 * Return a short if one is stored in this.
     	 * @return the short stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      	 */
	 	int16_t getShort() const noexcept;

		/**
  		 * Return a byte if one is stored in this.
     	 * @return the byte stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      	 */
	 	char getByte() const noexcept;

		/**
  		 * Return a double if one is stored in this.
     	 * @return the double stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      	 */
		double getDouble() const noexcept;

		/**
  		 * Return a float if one is stored in this.
     	 * @return the float stored if the object is any integer type, or 0 if not an integer.
		 * @deprecated Instead of relying on 0 as a sentinel value, this method will return an std::optional in 2.0.0.
   		 * @since 1.0
      	 */
		float getFloat() const noexcept;
	
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
		/**
		 * Attempts to cast the object to a std::string. If the object is not already a string, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a string that represents the one in the object if already a string, blank if a new one had to be created.
		 * @warning Calling getString() in order to force this object to become a string is deprecated. From 2.0.0, this method will return
         *          std::nullopt if the object is not already a string.
		 */
		std::string getString();
		
		/**
		 * Attempts to cast the object to a BdfList. If the object is not already a list, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a pointer to a BdfList; the current one associated with this object if already a list, or a brand new one if converted.
		 * @warning Calling getList() in order to force this object to become a list is deprecated. From 2.0.0, this method will return
         *          nullptr if the object is not already a list.
		 */
		BdfList* getList();
		
		/**
		 * Attempts to cast the object to a BdfList. If the object is not already a list, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a pointer to a BdfList; the current one associated with this object if already a list, or a brand new one if converted.
		 * @since 1.5.0
		 */
		BdfList* coerceList();
		
		/**
		 * Attempts to cast the object to a BdfNamedList. If the object is not already a named list, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a pointer to a BdfNamedList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @warning Calling getNamedList() in order to force this object to become a named list is deprecated. From 2.0.0, this method will return
         *          nullptr if the object is not already a list.
		 */
		BdfNamedList* getNamedList();
		
		/**
		 * Attempts to cast the object to a BdfNamedList. If the object is not already a named list, it is converted to one, and all data in the
   		 * original will be lost.
		 * @return a pointer to a BdfNamedList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @since 1.5.0
		 */
		BdfNamedList* coerceNamedList();
		
		/**
		 * Attempts to cast the object to a BdfCommentCppStyle. If the object is not already one, return nullptr.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle* getCommentCppStyle() const;
		
		/**
		 * Attempts to cast the object to a BdfCommentCppStyle. If the object is not already one, it is converted to one, and all data in the
		 * original will be lost.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle* coerceCommentCppStyle() noexcept;
		
		/**
		 * Attempts to cast the object to a BdfCommentCStyle. If the object is not already one, return nullptr.
		 * @since 1.5.0
		 */
		BdfCommentCStyle* getCommentCStyle() const;
		
		/**
		 * Attempts to cast the object to a BdfCommentCStyle. If the object is not already one, it is converted to one, and all data in the
		 * original will be lost.
		 * @since 1.5.0
		 */
		BdfCommentCStyle* coerceCommentCStyle() noexcept;
	
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
		 * Sets this BdfObject to a blank BdfCommentCStyle, and returns that BdfCommentCStyle.
		 * All data in the original object will be lost.
		 * @return a pointer to the new BdfCommentCStyle that was written to this BdfObject.
   		 * @since 1.5.0
		 */
		BdfCommentCStyle* startCommentCStyle();
		
		/**
		 * Sets this BdfObject to a new BdfCommentCStyle with the given parameters, and returns that BdfCommentCStyle.
		 * All data in the original object will be lost.
		 * @return a pointer to the new BdfCommentCStyle that was written to this BdfObject.
   		 * @since 1.5.0
		 */
		BdfCommentCStyle* startCommentCStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter, bool flattenSingleLineComments);
		
		#if __cplusplus >= 202002L
		/**
		 * Attempts to compare two BdfObjects with each other.
		 *
		 * This operator returns:
		 * - std::partial_ordering::unordered if the two BdfObjects are completely different types, or if both
		 *   BdfObjects are floating point types and one or both is NaN.
		 * - std::partial_ordering::less if the two BdfObjects are same type, but lhs has less value than rhs.
		 * - std::partial_ordering::greater if the two BdfObjects are same type, but lhs has greater value than rhs.
		 * - std::partial_ordering::equivalent if both BdfObjects have the same type and value (just type if BdfTypes::UNDEFINED).
		 *
		 * Array types as well as BdfList/BdfNamedList objects return the ordering of the first pair of unequal values,
		 * consistent with the behaviour of standard library types such as std::map. If all pairings are equal until the end
		 * of one array is reached, then the result of their sizes is returned. An array type will compare equivalent if both
		 * conditions compare equivalent.
		 * E.g. let this be a BdfList consisting of [ 0.5, 0.7, 2.0 ] and rhs [0.5, 0.8, 1.7 ]. Then
		 * std::partial_ordering::less will be returned, because the first pair of unequivalent values was the pairing at index 1,
		 * and 0.7 < 0.8. The unequal pairing at index 2's ordering of 2.0 > 1.7 is ignored.
		 * On the other hand, let this be [ 1, 2, 3, 4, 5 ] and rhs be [ 1, 2, 3, 4 ]. Then std::partial_ordering::greater
		 * will be returned, because while all pairings within the indexes 0-3 are equivalent, this->getList()->size() = 5 >
		 * rhs.getList()->size() = 4.
		 *
		 * Remember that you need to ensure both BdfObject pointers are dereferenced in all comparisons with * (e.g. "if (*lhs <=> *rhs)").
		 * Otherwise their pointers will be compared, which will almost never return the intended result.
		 *
		 * @since 1.5.0
		 * @param rhs the right hand side value of the comparison. this is automatically treated as the lhs.
		 * @return std::partial_ordering that represents the outcome of the comparison.
		 */
		std::partial_ordering operator<=>(BdfObject& rhs) noexcept;
		
		/**
		 * Returns true if operator<=> returns std::partial_ordering::equal.
		 * @since 2.0.0
		 * @param rhs the right hand side value of the comparison. this is automatically treated as the lhs.
		 * @return bool that represents the outcome of the equality comparison.
		 */
		bool operator==(BdfObject& rhs) noexcept;
		
		#endif
	};
}

#endif
