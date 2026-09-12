
#ifndef BDFNAMEDLIST_HPP_
#define BDFNAMEDLIST_HPP_

#include "Bdf.hpp"
#include <iostream>
#include <vector>
#include <string>

namespace Bdf
{
	
	class BdfNamedList
	{
	private:
	
		class Item;

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
		BdfObject* get(std::string key);
		BdfNamedList* set(std::string key, BdfObject* value);
		BdfNamedList* set(int key, BdfObject* value);
		BdfObject* remove(std::string key);
		BdfObject* remove(int key);
		std::vector<int> keys();
		bool exists(std::string key);
		bool exists(int key);
	};
}

#endif
