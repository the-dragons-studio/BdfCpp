#ifndef BDFLIST_HPP_
#define BDFLIST_HPP_

#include "Bdf.hpp"
#include "BdfHelpers.hpp"
#include <iostream>
#include <vector>

namespace Bdf
{
	/**
	 * Class that represents a BDF list; containing a list of objects ordered by
	 * sequential keys.
	 * Unlike BdfNamedList objects, string keys cannot be used to access items on
	 * the list, but keys are guranteed to be sequential.
  	 * @since 1.0
    	 * @note Called BdfArray prior to 1.3.
	 */
	class BdfList
	{
	public:

		class Item;
		/** 
		 * Subclass that represents an entry on a BdfList's linked list.
		 * @internal
   		 * @since 1.4.0
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
   		 * @since 1.0
		 * @internal
		 */
		BdfList(BdfLookupTable* lookupTable);
		
		/**
		 * Uses the char data at data to construct a BdfList.
		 * @since 1.0
		 * @internal
		 */
		BdfList(BdfLookupTable* lookupTable, const char* data, int size);
		
		/**
		 * Uses the string reader at sr to construct a BdfList.
		 * @since 1.0
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
		 * @since 1.0
		 * @internal
		 */
		void serializeHumanReadable(std::ostream &stream, BdfIndent indent, int upto);
		
		/**
  		 * @since 1.0
		 * @internal
		 */
		void getLocationUses(int* locations);
		
		/**
  		 * @since 1.0
		 * @internal
		 */
		int serializeSeeker(int* locations);
		
		/**
		 * Serialises the BdfList to data using locations.
   		 * @since 1.0
		 * @internal
		 */
		int serialize(char *data, int* locations);
		
		/**
		 * Adds the BdfObject at o to the back of the BdfList.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
   		 * @since 1.0
		 */
		BdfList* add(BdfObject* o);
		
		/**
		 * Clear all items in the list. this->size() == 0 after calling.
		 * @return the BdfList, now emptied of all objects.
   		 * @since 1.0
		 */
		BdfList* clear() noexcept;
		
		/**
		 * Get the item located at the start of the list.
   		 * @return the item at the start of the list.
   		 * @deprecated since 1.4.0, will be marked private in 2.0.0 onwards, as direct access to BdfList's linked list will no longer be allowed.
                 *             In 2.0.0, use BdfObject* getStart() instead.
		 * @since 1.4.0
		 */
		Item* getStart();
		
		/**
		 * Get the item located at the end of the list.
   		 * @return the item at the end of the list.
   		 * @deprecated since 1.4.0, will be marked private in 2.0.0 onwards, as direct access to BdfList's linked list will no longer be allowed.
                 *             In 2.0.0, use BdfObject* getEnd() instead.
		 * @since 1.4.0
		 */
		Item* getEnd();
		
		/**
		 * Inserts a new object after the item given in item.
   		 * @param item the item at which the object will be inserted next to.
		 * @param o the BdfObject to add to the list.
                 * @return the BdfList, now with o added to the list.
                 * @deprecated since 1.4.0, will be marked private in 2.0.0 onwards, as direct access to BdfList's linked list will no longer be allowed.
                 *             Use insertNext(int index, BdfObject* object) instead.
		 * @since 1.4.0
		 */
		#if __cplusplus >= 201402L
		[[deprecated("Use Bdf::BdfList::insertNext(int index, BdfObject* object) instead.")]]
		#endif
		BdfList* insertNext(Item* item, BdfObject* object);

		/**
		 * Adds the BdfObject at o after the item at index.
		 * @param index the index at which the object will be inserted next to.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
   		 * @since 1.4.0
		 */
		BdfList* insertNext(int index, BdfObject* o);
		
		/**
		 * Inserts a new object before the item given in item.
  		 * @param item the item at which the object will be inserted before.
		 * @param o the BdfObject to add to the list.
   		 * @return the BdfList, now with o added to the list.
   		 * @deprecated since 1.4.0, will be marked private in 2.0.0 onwards, as direct access to BdfList's linked list will no longer be allowed.
                 *             Use insertLast(int index, BdfObject* object) instead.
		 * @since 1.4.0
		 */
		#if __cplusplus >= 201402L
		[[deprecated("Use Bdf::BdfList::insertLast(int index, BdfObject* object) instead.")]]
		#endif
		BdfList* insertLast(Item* item, BdfObject* object);

		/**
		 * Adds the BdfObject at o before the item at index.
		 * @param index the index at which the object will be inserted before.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
   		 * @since 1.4.0
		 */
		BdfList* insertLast(int index, BdfObject* o);
		
		/**
		 * Gets a pointer to the BdfObject located at id.
		 * @param int the integer ID of the BdfObject you want to find.
		 * @return a pointer to the BdfObject located at id.
		 * @throw std::out_of_range if id is larger than this->size().
   		 * @since 1.0
		 */
		BdfObject* get(int id) const;
		
		/**
		 * Remove the item located at id.
		 * @param id the ID that this function will attempt to find.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 * @throw std::out_of_range if id is larger than this->size().
		 * @note This function is potentially throwing, unlike the overload that takes a BdfObject.
   		 * @since 1.0
		 */
		BdfList* remove(int id);
		
		/**
		 * Remove the Item given in item.
   		 * @param Item the item to remove from the list.
		 * @return the BdfList, now with item removed.
   		 * @deprecated since 1.4.0, will be marked private in 2.0.0 onwards, as direct access to BdfList's linked list will no longer be allowed.
                 *             Use remove(BdfObject* object) or remove(int id) instead.
   		 * @since 1.4.0
		 */
		#if __cplusplus >= 201402L
		[[deprecated("Use Bdf::BdfList::remove(BdfObject* object) or Bdf::BdfList::remove(int id) instead.")]]
		#endif
		BdfList* remove(Item* item) noexcept;
		
		/**
		 * Finds the BdfObject given in object, then removes it from the BdfList.
		 * @param object the BdfObject that this method will attempt to find and remove.
		 * @return the BdfList, now with the BdfObject located at id removed.
   		 * @since 1.4.0
		 */
		BdfList* remove(BdfObject* object) noexcept;
		
		/**
		 * Replace the BdfObject located at id with object.
   		 * @param id the index whose object will be replaced.
      		 * @param object the object to replace with.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 * @throw std::out_of_range if id is larger than this->size().
   		 * @since 1.0
		 */
		BdfList* set(int id, BdfObject* object);
		
		/**
		 * Return the size of the BdfList.
		 * @return an integer consisting of the size of the BdfList.
   		 * @since 1.0
		 */
		int size() const noexcept;

	private:
		/**
		 * Gets the item located at index.
		 * @internal
   		 * @since 1.4.0
		 */
		Item* getAtIndex(int index) const;

		Item* start;
		Item* end;
		Item** endptr;
	};
}

#endif
