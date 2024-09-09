
#ifndef BDFNAMEDLIST_HPP_
#define BDFNAMEDLIST_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#if __cplusplus >= 202002L
	#include <compare>
#endif

namespace Bdf
{
	/**
	 * A BdfNamedList represents a BDF named list.
	 * Unlike BdfList objects, BdfNamedList tags are permitted to have string keys.
	 * Items on a BdfNamedList may be accessed either by a string key, or an
	 * integer key as on a BdfList.
	 * However, unlike a BdfLists, the list of valid keys is not guranteed to be in
	 * numerical order.
	 * @since 1.0
	 */
	class BdfNamedList
	{
	private:
	
		class Item;
		
		/** 
		 * Subclass that represents an entry on a BdfList's linked list.
		 * @internal
		 * @since 1.4.0
		 */
		class Item
		{
		public:
			Item* next;
			Item* last;
			BdfObject* object;
			int key;
		
			Item(int key, BdfObject* object, Item* last, Item* next);
			virtual ~Item();
		};
		
		Item* start;
		Item** end;

		BdfLookupTable* lookupTable;

	public:
		/**
		 * Constructs an empty BdfList which will use the lookup table at lookupTable for further operations.
		 * @internal
		 */
		BdfNamedList(BdfLookupTable* lookupTable);
		
		/**
		 * Uses the char data at data to construct a BdfList.
		 * @internal
		 */
		BdfNamedList(BdfLookupTable* lookupTable, const char* data, int size);
		
		/**
		 * Uses the string reader at sr to construct a BdfList.
		 * @internal
		 */
		BdfNamedList(BdfLookupTable* lookupTable, BdfStringReader* sr);

	    /**
		 * Deleted (no copy constructor).
	     */
		BdfNamedList(const BdfNamedList&) = delete;
		
		/**
		 * Destroys the BdfNamedList.
		 * @since 1.0
		 */
		virtual ~BdfNamedList() noexcept;
		
		/**
		 * @internal
		 * @since 1.0
		 */	
		void getLocationUses(int* locations) const noexcept;
		
		/**
		 * @internal
		 * @since 1.0
		 */	
		int serializeSeeker(int* locations) const;
		
		/**
		 * Serialises the named list to data using locations
		 * @internal
		 * @since 1.0
		 */	
		int serialize(char *data, int* locations) const;
		
		/**
		 * Serialises the named list to &stream.
		 * @internal
		 * @since 1.0
		 */	
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent, int upto) const;
		
		/**
		 * Removes all elements in the BdfNamedList.
		 * @return the BdfNamedList, now with all elements removed.
		 * @since 1.4.0
		 */
		BdfNamedList* clear();
		
		/**
		 * Gets the item located at key. If it does not exist, creates it.
		 * @param key the key to search for in the list.
		 * @return a pointer to the object located at key.
		 * @warning Relying on this method's ability to create keys that don't exist is deprecated. From 2.0.0 onwards, this method
		 *          will throw an std::out_of_range exception in that case instead.
		 * @since 1.0
		 */	
		BdfObject* get(int key);
		
		/**
		 * Gets the item located at key. If it does not exist, creates it.
		 * @param key the key to search for in the list.
		 * @return a pointer to the object located at key.
		 * @warning Relying on this method's ability to create keys that don't exist is deprecated. From 2.0.0 onwards, this method
		 *          will throw an std::out_of_range exception in that case instead.
		 * @since 1.0
		 */
		BdfObject* get(std::string key);
		
		/**
		 * Replaces the object located at key with the object located at value.
		 * If key does not exist, create it.
		 * @param key the key to replace or add to the list.
		 * @param value the BdfObject to replace or add at key.
		 * @return the BdfNamedList, now with the object set performed.
		 * @since 1.0
		 */
		BdfNamedList* set(std::string key, BdfObject* value);
		
		/**
		 * Replaces the object located at key with the object located at value.
		 * If key does not exist, create it.
		 * @param key the key to replace or add to the list.
		 * @param value the BdfObject to replace or add at key.
		 * @return the BdfNamedList, now with the object set performed.
		 * @since 1.0
		 */
		BdfNamedList* set(int key, BdfObject* value);
		
		/**
		 * Removes the object located at key.
		 * @param key the location of the BdfObject that needs to be removed.
		 * @return the BdfNamedList, now with the object at key removed.
		 * @since 1.4.0
		 */
		BdfList* remove(std::string key) noexcept;
		
		/**
		 * Removes the object located at key.
		 * @param key the location of the BdfObject that needs to be removed.
		 * @return the BdfNamedList, now with the object at key removed.
		 * @since 1.4.0
		 */
		BdfList* remove(int key) noexcept;
		
		/**
		 * Pops the BdfObject located at the specified key from the list, then removes it.
		 * @param key the location of the BdfObject that needs to be popped.
		 * @return the BdfObject that lived at key.
		 * @since 1.4.0
		 */
		BdfObject* pop(std::string key) noexcept;
		
		/**
		 * Pops the BdfObject located at the specified key from the list, then removes it.
		 * @param key the location of the BdfObject that needs to be popped.
		 * @return the BdfObject that lived at key.
		 * @since 1.4.0
		 */
		BdfObject* pop(int key) noexcept;
		
		/**
		 * Creates a key if it does not already exist.
		 * @param key the key to reserve in the BdfNamedList.
		 */
		void makeKey(std::string key);
		
		/**
		 * Gets a full list of integer keys that exist in the BdfNamedList.
		 * @return a vector containing a list of keys.
		 * @since 1.0
		 */
		std::vector<int> keys() const noexcept;
		
		/**
		 * Checks if key exists in the BdfNamedList.
		 * @param key the key to check for existence in the named list.
		 * @return true if the key exists, false otherwise.
		 * @since 1.0
		 */
		bool exists(std::string key) const noexcept;
		
		/**
		 * Checks if key exists in the BdfNamedList.
		 * @param key the key to check for existence in the named list.
		 * @return true if the key exists, false otherwise.
		 * @since 1.0
		 */
		bool exists(int key) const noexcept;
		
		#if __cplusplus >= 202002L
		
		/**
		 * Attempts to compare two BdfNamedList objects.
		 *
		 * This operator returns:
		 * - the ordering of the first pair of unequal values, as determined by BdfObject::operator<=>
		 *   (which may recursively call this operator on child BdfNamedLists).
		 * - If no such pair was found, the result of this->keys()->size() <=> rhs->keys()->size().
		 *
		 * String keys are ignored for the purpose of this operator. For example, if somehow two BdfNamedLists
		 * had otherwise identical data accessed at the same integer keys, but one stored it at "lhs" and the
		 * other at "rhs", std::partial_ordering::equivalent would still be returned.
		 *
		 * Remember that you need to ensure both BdfObject pointers are dereferenced in all comparisons with * (e.g. if (*lhs <=> *rhs).
		 * Otherwise their pointers will be compared, which will almost never return the intended result.
		 *
		 * @param rhs a pointer to another BdfNamedList to compare.
		 * @return std::partial_ordering determined by the above algorithm.
		 * @since 2.0.0
		 */
		std::partial_ordering operator<=>(const BdfNamedList& rhs) const noexcept;
		
		/**
		 * Returns true if operator<=> returns std::partial_ordering::equivalent.
		 * @param rhs a pointer to another BdfNamedList to compare.
		 * @return bool that represents the outcome of the equality comparison.
		 * @since 2.0.0
		 */
		bool operator==(const BdfNamedList& rhs) const noexcept;
		#endif
	};
}

#endif
