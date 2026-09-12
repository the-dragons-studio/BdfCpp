
#ifndef BDFOBJECT_HPP_
#define BDFOBJECT_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <string>
#include <optional>

#if __cplusplus >= 202002L
#include <compare>
#endif

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
		 * Makes a copy of the BdfObject located at that.
		 * The new object will belong to the same lookupTable and contain the same data.
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
		 * Creates a new object based on this object's BdfLookupTable.
		 * @since 1.0
		 */		
		BdfObject* newObject();
		
		/**
		 * Creates a new BdfNamedList based on this object's BdfLookupTable.
		 * Unlike getOrNewNamedList(), this method does not modify the original object's type or data.
		 * @since 1.0
		 */
		BdfNamedList* newNamedList() const;
		
		/**
		 * Creates a new BdfList based on this object's BdfLookupTable.
		 * Unlike getOrNewList(), this method does not modify the original object's type or data.
		 * @since 1.0
		 */
		BdfList* newList() const;
	
		BdfObject* setAutoInt(long v);
		
		
		std::optional<int64_t> getAutoInt();
		
		/**
		 * Either casts the object to any integer, or returns a preset default value. efectively has the same behaviour as pre-2.0.0 
		 * versions of getAutoInt(), except this method allows you to customise the sentinel value.
		 * @param sentinel the value to return if the object is not an integer.
		 * @return the integer value saved to this BdfObject, or sentinel if the object is not
		 *         an integer.
		 */
		int64_t getAutoIntOr(int64_t sentinel = 0) const noexcept;
	
		// Get
	
		// Primitives
		/**
		 * Attempts to cast the object to a 32 bit signed integer.
		 * @return an std::optional that contains an int32_t if casting succeeded, or nullopt if the object is not
		 *         an integer.
		 */
		std::optional<int32_t> getInteger() const noexcept;
				
		/**
		 * Either casts the object to an integer, or returns a default value.
		 * @param sentinel the value to return if the object is not an integer.
		 */
		int32_t getIntegerOr(int32_t sentinel = 0);
		
		/**
		 * Attempts to cast the object to a bool.
		 * @return an std::optional that contains a bool if casting succeeded.
		 */
		std::optional<bool> getBoolean() const noexcept;
		
	 	std::optional<int64_t> getLong() const noexcept;
	 	std::optional<int16_t> getShort() const noexcept;
	 	std::optional<int8_t> getByte() const noexcept;
		std::optional<double> getDouble() const noexcept;
		std::optional<float> getFloat() const noexcept;
	
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
		 * Attempts to cast the object to a std::string.
		 * @return an std::optional that contains a copy of the string if casting succeeded.
		 * @note Before 2.0.0, calling this method on a non-string BdfObject would actually convert the BdfObject to a string, causing all data 
		 *       in the original to be lost. This is no longer possible. Calling this method from 2.0.0 onwards will return std::nullopt instead.
		 *       Usage of getString() in this way should be replaced with BdfObject::getOrNewString() instead, which will do the same thing as the
		 *       pre-2.0.0 version of this method.
		 */
		std::optional<std::string> getString() const noexcept;
		
		/**
		 * Attempts to cast the object to a std::string. If the object is not a string, instead return sentinel.
		 * @param sentinel the value to return if the object is not an string.
		 * @return the string value saved to this BdfObject, or sentinel if the object is not
		 *         a string.
		 */
		std::optional<std::string> getStringOr(std::string sentinel = "") const noexcept;
		
		/**
		 * Attempts to cast the object to a BdfList.
		 * @return a pointer to a BdfList, or nullptr if casting failed.
		 * @note Before 2.0.0, calling this method on a non-BdfList BdfObject would actually convert the BdfObject to a BdfList, causing all data
		 *       in the original to be lost. This is no longer possible. Calling this method from 2.0.0 onwards will return nullptr instead.
		 *       Usage of getList() in this way should be replaced with BdfObject::getOrNewList() instead, which will do the same thing as the
		 *       pre-2.0.0 version of this method.
		 */
		BdfList* getList() const noexcept;
		
		/**
		 * Attempts to cast the object to a BdfNamedList.
		 * @return a pointer to a BdfNamedList, or nullptr if casting failed.
		 * @note Before 2.0.0, calling this method on a non-BdfNamedList BdfObject would actually convert the BdfObject to a BdfNamedList, causing
		 *       all data in the original to be lost. This is no longer possible. Calling this method from 2.0.0 onwards will return nullptr instead.
		 *       Usage of getNamedList() in this way should be replaced with BdfObject::getOrNewNamedList() instead, which will do the same thing as the
		 *       pre-2.0.0 version of this method.
		 */
		BdfNamedList* getNamedList() const noexcept;
	
		// Set
	
		// Primitives
		/**
		 * Sets the BdfObject using the integer given at v.
		 * All data in the original object will be lost.
		 * @param v the new integer to write.
		 * @return a pointer to this object, now with the new data written.
		 * @since 1.0
		 */
		BdfObject* setInteger(const int32_t v) noexcept;
		BdfObject* setBoolean(const bool v) noexcept;
	 	BdfObject* setLong(const int64_t v) noexcept;
	 	BdfObject* setShort(const int16_t v) noexcept;
	 	BdfObject* setByte(const char v) noexcept;
		BdfObject* setDouble(const double v) noexcept;
		BdfObject* setFloat(const float v) noexcept;
	
		// Arrays
		BdfObject* setIntegerArray(const int32_t *v, int size);
		BdfObject* setBooleanArray(const bool *v, int size);
	 	BdfObject* setLongArray(const int64_t *v, int size);
	 	BdfObject* setShortArray(const int16_t *v, int size);
	 	BdfObject* setByteArray(const char *v, int size);
		BdfObject* setDoubleArray(const double *v, int size);
		BdfObject* setFloatArray(const float *v, int size);
	
		// Objects
		BdfObject* setString(const std::string &v);	
		BdfObject* setString(std::string &&v);			
		BdfObject* setList(BdfList* v);
		BdfObject* setNamedList(BdfNamedList* v);
		
		// Hybrid getters and setters
		/**
		 * Sets this BdfObject to a blank BdfList, and returns that BdfList.
		 * All data in the original object will be lost.
		 * This overload is a replacement to the automatic overwriting behaviour of the pre-2.0.0 version of getList(), which would silently
		 * replace data with a BdfList if it wasn't already.
		 * @warning This method replaced the original newList() which simply returned a new BdfNamedList without writing it to this.
		 *          The old behaviour is available under createList().
		 * @return a pointer to the new BdfList that was written to this BdfObject.
		 */
		BdfList* newSetAndGetList();
		
		/**
		 * Sets this BdfObject to a blank BdfNamedList, and returns that BdfNamedList.
		 * All data in the original object will be lost.
		 * This overload is a replacement to the automatic overwriting behaviour of the pre-2.0.0 version of getNamedList(), which would silently
		 * replace data with a BdfList if it wasn't already.
		 * @return a pointer to the new BdfNamedList that was written to this BdfObject.
		 */
		BdfNamedList* newSetAndGetNamedList();
		
		/**
		 * If this is a BdfList, returns it. Otherwise, the object is automatically converted to a list as specified in
		 * All data in the original will be lost.
		 * @return a pointer to a BdfList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note This method replicates the pre-2.0.0 behaviour of getList(), which as of 2.0.0 returns nullptr instead of insiduously casting
		 *       the object to a named list if it wasn't already one.
		 * @since 1.4.0
		 * @deprecated since 2.0.0, use getNewListOrSet() instead.
		 */
		BdfList* getOrNewList();
		
		/**
		 * If this is a BdfList, returns it. Otherwise, the object is automatically converted to a list specified in newList, or if not provided,
		 * a default-constructed list.
		 * @param newList a pointer to a list to add to the list if the object is not already a list. Leave blank to use a default-constructed list.
		 * @return a pointer to a BdfList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note This method replicates the pre-2.0.0 behaviour of getList(), which as of 2.0.0 returns nullptr instead of insiduously casting
		 *       the object to a named list if it wasn't already one.
		 * @since 2.0.0
		 */
		BdfList* getListOrSet(std::optional<BdfList*> newList = std::nullopt);
		
		/**
		 * If this is a BdfNamedList, returns it. Otherwise, the object is automatically converted to a named list.
		 * All data in the original will be lost.
		 * @return a pointer to a BdfNamedList; the current one associated with this object if already a named list, or a brand new one if converted.
		 * @note This method replicates the pre-2.0.0 behaviour of getNamedList(), which as of 2.0.0 returns nullptr instead of insiduously casting
		 *       the object to a named list if it wasn't already one.
		 * @since 1.4.0
		 */
		BdfNamedList* getOrNewNamedList();
		
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
		 * @since 2.0.0
		 * @param rhs the right hand side value of the comparison. this is automatically treated as the lhs.
		 * @return std::partial_ordering that represents the outcome of the comparison.
		 */
		std::partial_ordering operator<=>(const BdfObject& rhs) const noexcept;
		
		/**
		 * Returns true if operator<=> returns std::partial_ordering::equal.
		 * @since 2.0.0
		 * @param rhs the right hand side value of the comparison. this is automatically treated as the lhs.
		 * @return bool that represents the outcome of the equality comparison.
		 */
		bool operator==(const BdfObject& rhs) const noexcept;
		
		#endif
		
	};
}

#endif
