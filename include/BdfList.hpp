
#ifndef BDFLIST_HPP_
#define BDFLIST_HPP_

#include "Bdf.hpp"
#include "BdfHelpers.hpp"
#include <iostream>
#include <vector>

#if __cplusplus >= 202002L
	#include <compare>
#endif

namespace Bdf
{
	class BdfList
	{
	public:

		class Item;
		/** 
		 * Subclass that represents an entry on a BdfList's linked list.
		 * @internal
		 */
		class Item
		{
		public:

			BdfObject* object;
			Item* last;
			Item* next;
		};
		
		/**
		 * Constructs an empty BdfList which will use the lookup table at lookupTable for further operations.
		 * @internal
		 */
		BdfList(BdfLookupTable* lookupTable);
		
		/**
		 * Uses the char data at data to construct a BdfList.
		 * @internal
		 */
		BdfList(BdfLookupTable* lookupTable, const char* data, int size);
		
		/**
		 * Uses the string reader at sr to construct a BdfList.
		 * @internal
		 */
		BdfList(BdfLookupTable* lookupTable, BdfStringReader* sr);
		
		/**
		 * Deleted (no copy constructor).
		 */
		BdfList(const BdfList& that) = delete;
		
		/**
		 * Destroys a BdfList.
		 */
		virtual ~BdfList();
	    
		/**
		 * Serialises the BdfList to &stream.
		 * @internal
		 */
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent, int upto);
		
		/**
		 * @internal
		 */
		void getLocationUses(int* locations);
		
		/**
		 * @internal
		 */
		int serializeSeeker(int* locations);
		
		/**
		 * Serialises the BdfList to data using locations.
		 * @internal
		 */
		int serialize(char *data, int* locations);
		
		/**
		 * Adds the BdfObject at o to the back of the BdfList.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
		 */
		BdfList* add(BdfObject* o);
		
		/**
		 * Clear all items in the list. this->size() == 0 after calling.
		 * @return the BdfList, now emptied of all objects.
		 */
		BdfList* clear() noexcept;
		
		/**
		 * Get the item located at the start of the list.
		 * @internal
		 */
		Item* getStart();
		
		/**
		 * Get the item located at the end of the list.
		 * @internal
		 */
		Item* getEnd();
		
		/**
		 * Inserts a new object after the item given in item.
		 * @internal
		 */
		BdfList* insertNext(Item* item, BdfObject* object);
		
		/**
		 * Inserts a new object before the item given in item.
		 * @internal
		 */
		BdfList* insertLast(Item* item, BdfObject* object);
		
		/**
		 * Gets a pointer to the BdfObject located at id.
		 * @param int the integer ID of the BdfObject you want to find.
		 * @return a pointer to the BdfObject located at id.
		 * @throw std::out_of_range if id is larger than this->size().
		 */
		BdfObject* get(int id) const;
		
		/**
		 * Remove the item located at id.
		 * @param id the ID that this function will attempt to find.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 * @throw std::out_of_range if id is larger than this->size().
		 * @note This function is potentially throwing, unlike the overload that takes a BdfObject.
		 */
		BdfList* remove(int id);
		
		/**
		 * Remove the Item given in item.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 */
		BdfList* remove(Item* item) noexcept;
		
		/**
		 * Finds the BdfObject given in object, then removes it from the BdfList.
		 * @param object the BdfObject that this method will attempt to find and remove.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 */
		BdfList* remove(BdfObject* object) noexcept;
		
		/**
		 * Replace the BdfObject located at id with object.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 * @throw std::out_of_range if id is larger than this->size().
		 */
		BdfList* set(int id, BdfObject* object);
		
		/**
		 * Return the size of the BdfList.
		 * @return an integer consisting of the size of the BdfList.
		 */
		int size() const noexcept;

	private:
		/**
		 * Gets the item located at index.
		 * @internal
		 */
		Item* getAtIndex(int index) const;

		Item* start;
		Item* end;
		Item** endptr;
	};
}

#endif
