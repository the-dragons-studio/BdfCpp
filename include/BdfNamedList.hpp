
#ifndef BDFNAMEDLIST_HPP_
#define BDFNAMEDLIST_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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
			BdfObject* object;
			int key;
		
			Item(int key, BdfObject* object, Item* next);
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
		void serializeHumanReadable(std::ostream &stream, const BdfIndent &indent, int upto) const;
		
		/**
		 * Removes all elements in the BdfNamedList.
		 * @return the BdfNamedList, now with all elements removed.
		 * @since 1.4.0
		 */
		BdfNamedList* clear() noexcept;
		
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
		BdfNamedList* set(std::string key, BdfObject* value) noexcept;
		
		/**
		 * Replaces the object located at key with the object located at value.
		 * If key does not exist, create it.
		 * @param key the key to replace or add to the list.
		 * @param value the BdfObject to replace or add at key.
		 * @return the BdfNamedList, now with the object set performed.
		 * @since 1.0
		 */
		BdfNamedList* set(int key, BdfObject* value) noexcept;

		/**
		 * Removes the object located at key and returns that object.
		 * @param key the location of the BdfObject that needs to be popped.
		 * @return the BdfObject that lived at key.
		 * @since 1.0
		 * @deprecated since 1.4.0, will be replaced by pop() in 2.0.0 with no behaviour change.
		 */
		#if __cplusplus >= 201402L
		[[deprecated("Use Bdf::BdfNamedList::pop(std::string key) instead.")]]
		#endif
		BdfObject* remove(std::string key) noexcept;
		
		/**
		 * Removes the object located at key and returns that object.
		 * @param key the location of the BdfObject that needs to be popped.
		 * @return the BdfObject that lived at key.
		 * @since 1.0
		 * @deprecated since 1.4.0, will be replaced by pop() in 2.0.0 with no behaviour change.
		 */
		#if __cplusplus >= 201402L
		[[deprecated("Use Bdf::BdfNamedList::pop(int key) instead.")]]
		#endif
		BdfObject* remove(int key) noexcept;
		
		/**
		 * Pops the BdfObject located at the specified key from the list, then removes it.
		 * Remember to delete the object when you're done, as it will no longer belong to any container and won't be deleted on its own.
		 * @param key the location of the BdfObject that needs to be popped.
		 * @return the BdfObject that lived at key, or null if no object was found at key.
		 * @since 1.4.0
		 */
		BdfObject* pop(std::string key) noexcept;
		
		/**
		 * Pops the BdfObject located at the specified key from the list, then removes it.
		 * Remember to delete the object when you're done, as it will no longer belong to any container and won't be deleted on its own.
		 * @param key the location of the BdfObject that needs to be popped.
		 * @return the BdfObject that lived at key, or null if no object was found at key.
		 * @since 1.4.0
		 */
		BdfObject* pop(int key) noexcept;
		
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
	};
}

#endif
