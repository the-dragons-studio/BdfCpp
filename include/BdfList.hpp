
#ifndef BDFLIST_HPP_
#define BDFLIST_HPP_

#include "Bdf.hpp"
#include "BdfHelpers.hpp"
#include <iostream>
#include <vector>
#include <optional>

#if __cplusplus >= 202002L
	#include <compare>
	#include <iterator>
#endif

namespace Bdf
{
	/**
	 * Class that represents a BDF list; containing a list of objects ordered by
	 * sequential keys.
	 * Unlike BdfNamedList objects, string keys cannot be used to access items on
	 * the list, but keys are guranteed to be sequential.
	 */
	class BdfList
	{		
	private:
		class Item;
		class ItemIterator;
	
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
		 * Gets the item located at index.
		 * @internal
		 */
		Item* getAtIndex(uint64_t index) const;
		
		/**
		 * Iterator for BdfList Items.
		 * @since 2.0.0
		 * @internal
		 */
		struct ItemIterator {
			friend class BdfList;
			
			using value_type = Item*;
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			
			/**
			 * Creates a blank ItemIterator.
			 */
			ItemIterator();			
			
			/**
			 * Creates a new iterator from a pointer.
			 */
			explicit ItemIterator(Item *p);
			
			/**
			 * Gets a pointer to the item.
			 */
			Item* operator*() const noexcept;
			
			/**
			 * Gets a pointer to the item.
			 */
			Item* operator->() const noexcept;
			
			/**
			 * Prefix increments the Iterator.
			 */
			ItemIterator& operator++();
			
			/**
			 * Postfix increments the Iterator.
			 */
			ItemIterator operator++(int);
			
			/**
			 * Prefix decrements the Iterator.
			 */
			ItemIterator& operator--();
			
			/**
			 * Postfix decrements the Iterator.
			 */
			ItemIterator operator--(int);
			
			/**
			 * Checks if the iterator points at valid data.
			 */
			bool isValid() const noexcept;
			
			/**
			 * Checks if the iterator points at valid data.
			 * Effectively calls this->isValid().
			 */
			explicit operator bool() const noexcept;
				
			friend auto operator<=>(const ItemIterator&, const ItemIterator&) = default;
			
			private:
			Item* p;
		};
		
		static_assert(std::bidirectional_iterator<ItemIterator>);
		
		/**
		 * Returns an ItemIterator to the starting item.
		 */
		ItemIterator ibegin() const noexcept;
		
		/**
		 * Returns an ItemIterator to nullptr.
		 */
		ItemIterator iend() const noexcept;

		Item* startItem;
		Item* endItem;
		Item** endptr;
		BdfLookupTable* lookupTable;
				
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
		 * Remove the Item given in item.
		 * @return the BdfList, now with the BdfObject located at id removed.
		 * @internal
		 */
		BdfList* remove(Item* item) noexcept;
				
		/**
		 * Pops the item given at item; unlike remove(), the object's pointer is released and returned.
		 * @return the BdfObject that lived at item.
		 * @internal
		 * @since 1.4.0
		 */
		BdfObject* pop(uint64_t index) noexcept;
		
	public:
		class Iterator;
		class ConstIterator;
		
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
		void getLocationUses(int* locations) const;
		
		/**
		 * @internal
		 */
		int serializeSeeker(int* locations) const;
		
		/**
		 * Serialises the BdfList to data using locations.
		 * @internal
		 */
		int serialize(char *data, int* locations) const;
		
		/**
		 * Adds the BdfObject at o to the back of the BdfList.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
		 * @since 1.0
		 */
		BdfList* add(BdfObject* o);
		
		/**
		 * Adds the BdfObject at o after the item at index.
		 * @param index the index at which the object will be inserted next to.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
		 */
		BdfList* insertNext(uint64_t index, BdfObject* o);
		
		/**
		 * Finds the BdfObject at needle, then inserts the BdfObject at o next to it.
		 * @param needle the BdfObject at which the object will be inserted next to.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
		 */
		BdfList* insertNext(BdfObject* needle, BdfObject* o, bool fallbackToAdd = false);
		
		/**
		 * Adds the BdfObject at o before the item at index.
		 * @param index the index at which the object will be inserted next to.
		 * @param o the BdfObject to add to the list.
		 * @return the BdfList, now with o added to the list.
		 */
		BdfList* insertLast(uint64_t index, BdfObject* o);
		
		/**
		 * Finds the BdfObject at needle, then inserts the BdfObject at o before it.
		 * @param needle the BdfObject at which the object will be inserted next to.
		 * @param o the BdfObject to add to the list.
		 * @param fallbackToAdd If needle is not found, allow adding to the end instead. Otherwise,
		 *                      throws std::out_of_range.
		 * @return the BdfList, now with o added to the list.
		 */
		BdfList* insertLast(BdfObject* needle, BdfObject* o, bool fallbackToAdd = false);
		
		/**
		 * Clear all items in the list. this->size() == 0 after calling.
		 * @return the BdfList, now emptied of all objects.
		 */
		BdfList* clear() noexcept;
		
		BdfObject* getStart() noexcept;
		const BdfObject* getStart() const noexcept;
		BdfObject* getEnd() noexcept;
		const BdfObject* getEnd() const noexcept;
		
		/**
		 * Get the item located at the start of the list.
		 * @internal
		 */
		Item* getStartItem() const noexcept;
		
		/**
		 * Get the item located at the end of the list.
		 * @internal
		 */
		Item* getEndItem() const noexcept;
		
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
		 * Finds the BdfObject given in object, then removes it from the BdfList.
		 * @param object the BdfObject that this method will attempt to find and remove.
		 * @return the BdfList, now with the BdfObject located at object removed if it ever existed.
		 * @since 1.0
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
		uint64_t size() const noexcept;
		
		/**
		 * Requests that this list be large enough to contain at least size elements.
		 * If the list is already at least the size given, this method is a no-op. Otherwise,
		 * new Items are added to the list. These Items will have objects set to nullptr.
		 * @param size The requested size.
		 */
		BdfList* reserve(uint64_t size);
		
		/**
		 * Requests that this list be resized to exactly the size requested.
		 * - If the list is smaller than the requested size, has the same effect as reserve().
		 * - If the list is equal in size to the requested size, this method has no effect.
		 * - If the list is larger than the requested size, has the same effect as shrink().
		 * @param size The requested size.
		 */
		BdfList* resize(uint64_t size);
		
		/**
		 * Requests that this list be shrunk to the requested size.
		 * If the list is already smaller than or equal to the size given, this method is a no-op. Otherwise,
		 * elements starting from the end of the list until the requested size are discarded.
		 * @param size The requested size.
		 */
		BdfList* shrink(uint64_t size);
		
		/**
		 * Removes elements from the end of the list until the first item whose object isn't undefined.
		 * This method has no effect if the end of the list already contains an object, even if further
		 * items have undefined objects.
		 */
		BdfList* shrinkUndefinedObjects();
		
		/**
		 * Clean up all undefined objects from the list.
		 * After calling, all indicies may change. It's recommended to discard all indicies obtained
		 * prior to calling this method.
		 */
		BdfList* cleanupUndefinedObjects();
		
		/**
		 * Find the specified BdfObject in the list.
		 * @param needle the object to search for.
		 * @return an iterator to the first object which compares (**it == *needle), or equivalent to end() if no such iterator was found.
		 */
		BdfList::ConstIterator find(BdfObject* needle) const noexcept;
		
		/**
		 * Find the specified BdfObject in the list.
		 * @param needle the object to search for.
		 * @return the index to the first object which compares (**it == *needle), or std::nullopt if no such iterator was found.
		 */
		std::optional<uint64_t> findIndex(BdfObject* needle) const noexcept;

		#if __cplusplus >= 202002L
		
		/**
		 * Attempts to compare two BdfList objects.
		 * This operator returns:
		 * - the ordering of the first pair of unequal values, as determined by BdfObject::operator<=>
		 *   (which may recursively call this operator on child BdfLists).
		 * - If no such values were found before one iterator stops being valid, the side whose iterator remained
    	 *   valid is compared greater. If both iterators stopped being valid, then equivalent is returned.
		 *
		 * Remember that you need to ensure both BdfObject pointers are dereferenced in all comparisons with * (e.g. if (*lhs <=> *rhs).
		 * Otherwise their pointers will be compared, which will almost never return the intended result.
		 * 
		 */
		std::partial_ordering operator<=>(const BdfList& rhs) const noexcept;
		
		/**
		 * Returns true if operator<=> returns std::partial_ordering::equal.
		 * @since 2.0.0
		 * @param rhs the right hand side value of the comparison. this is automatically treated as the lhs.
		 * @return bool that represents the outcome of the equality comparison.
		 */
		bool operator==(const BdfList& rhs) const noexcept;
		
		/**
		 * A const iterator for BdfList.
		 *
		 * BdfList objects can be traversed using iterators as an alterantive to key finding.
		 * All iterators in BdfList satisfy std::bidirectional_iterator.
		 *
		 * Unlike modifiabale Iterator objects, ConstIterator objects can always be obtained
		 * regardless of the const-ness of the BdfList. ConstIterators cannot be implicitly
		 * converted to modifiable Iterators, but the opposite is true; modifiable Iterators
		 * can be converted to ConstIterators.
		 *
		 * @since 2.0.0
		 */
		struct ConstIterator {
			friend class BdfList;
			using value_type = const BdfObject*;
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			
			/**
			 * Creates a blank ConstIterator.
			 */
			ConstIterator();
			
			/**
			 * Creates a new iterator from a pointer.
			 */
			explicit ConstIterator(const ItemIterator &p);
			
			/**
			 * Dereferences the BdfObject.
			 * @return a pointer to a BdfObject that cannot be modified.
			 */
			const BdfObject* operator*() const noexcept;
			
			/**
			 * Dereferences the BdfObject.
			 * @return a pointer to a BdfObject that cannot be modified.
			 */
			const BdfObject* operator->() const noexcept;
			
			/**
			 * Prefix increments the ConstIterator.
			 * Using this operator on a nullptr iterator is well-defined; it simply becomes a no-op.
			 */
			ConstIterator& operator++();
			
			/**
			 * Postfix increments the ConstIterator, and returns a new ConstIterator.
			 * Using this operator on a nullptr iterator is well-defined; it simply becomes a no-op.
			 */
			ConstIterator operator++(int);
			
			/**
			 * Prefix decrements the ConstIterator.
			 * Using this operator on a nullptr iterator is well-defined; it simply becomes a no-op.
			 */
			ConstIterator& operator--();
			
			/**
			 * Postfix decrements the ConstIterator.
			 * Using this operator on a nullptr iterator is well-defined; it simply becomes a no-op.
			 */
			ConstIterator operator--(int);
			
			/**
			 * Checks if the iterator points at valid data.
			 * Equivalent to isValid().
			 * @return true if both the stored iterator are valid and its object is not nullptr, false if one or both conditions are not met.
			 */
			explicit operator bool() const noexcept;
			
			/**
			 * Checks if the iterator points at valid data.
			 * @return true if both the stored ItemIterator are valid and its object is not nullptr, false if one or both conditions are not met.
			 */
			bool isValid() const noexcept;
			
			friend auto operator<=>(const ConstIterator&, const ConstIterator&) = default;
			
			private:
			ItemIterator p;
		};
		
		static_assert(std::bidirectional_iterator<ConstIterator>);
		
		/**
		 * A modifiable iterator for BdfList.
		 *
		 * BdfList objects can be traversed using iterators as an alterantive to key finding.
		 * All iterators in BdfList satisfy std::bidirectional_iterator.
		 *
		 * Modifiable Iterator objects cannot be obtained with const BdfList objects. Instead,
		 * you can use ConstIterator. Modifiable Iterator is also implicitly convertible to
		 * ConstIterator. If you don't need to modify the data using an iterator, prefer
		 * ConstIterator.
		 *
		 * @since 2.0.0
		 */
		struct Iterator {
			friend class BdfList;
			using value_type = BdfObject*;
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			
			/**
			 * Creates a blank Iterator.
			 */
			Iterator();
			
			/**
			 * Creates a new iterator from a pointer.
			 */
			explicit Iterator(const ItemIterator &p);
			
			/**
			 * Dereferences the BdfObject contained in the iterator.
			 */
			BdfObject* operator*() const noexcept;
			
			/**
			 * Dereferences the BdfObject contained in the iterator.
			 */
			BdfObject* operator->() const noexcept;
			
			/**
			 * Prefix increments the Iterator.
			 */
			Iterator& operator++();
			
			/**
			 * Postfix increments the Iterator.
			 */
			Iterator operator++(int);
			
			/**
			 * Prefix decrements the Iterator.
			 */
			Iterator& operator--();
			
			/**
			 * Postfix decrements the Iterator.
			 */
			Iterator operator--(int);
			
			/**
			 * Checks if the iterator points at valid data.
			 * Equivalent to isValid().
			 * @return true if both the stored ItemIterator are valid and its object is not nullptr, false if one or both conditions are not met.
			 */
			explicit operator bool() const noexcept;
			
			/**
			 * Checks if the iterator points at valid data.
			 * @return true if both the stored ItemIterator are valid and its object is not nullptr, false if one or both conditions are not met.
			 */
			bool isValid() const noexcept;
			
			/**
			 * Implicitly converts a modifiable Iterator to a ConstIterator.
			 */
			operator ConstIterator() const noexcept;
			
			friend auto operator<=>(const Iterator&, const Iterator&) = default;
			
			private:
			ItemIterator p;
		};
		
		static_assert(std::bidirectional_iterator<Iterator>);
				
		/**
		 * Returns an iterator to the starting object.
		 */
		Iterator begin() noexcept;
		
		/**
		 * Returns an iterator to nullptr.
		 */
		Iterator end() noexcept;
		
		/**
		 * Returns a const iterator to the starting object.
		 */
		ConstIterator cbegin() const noexcept;
		
		/**
		 * Returns a const iterator to nullptr.
		 */
		ConstIterator cend() const noexcept;
		
		private:
				
		/**
		 * Like begin(), except marked const. Used to allow internal methods to take advantage
		 * of Iterator's implicit conversion to ConstIterator.
		 * @since 2.0.0
		 * @internal
		 */
		Iterator fcbegin() const noexcept;
		
		/**
		 * Like end(), except marked const. Used to allow internal methods to take advantage
		 * of Iterator's implicit conversion to ConstIterator.
		 * @since 2.0.0
		 * @internal
		 */
		Iterator fcend() const noexcept;
		
		#endif
	};
}

#endif
