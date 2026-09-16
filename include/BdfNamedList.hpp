
#ifndef BDFNAMEDLIST_HPP_
#define BDFNAMEDLIST_HPP_

#include "Bdf.hpp"
#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

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
			Item* last;
			Item* next;
			BdfObject* object;
			int key;
			std::string name;
		
			Item(int key, BdfObject* object, Item* prev, Item* next);
			virtual ~Item();
		};
		
		/**
		 * Iterator for BdfList Items.
		 * @since 1.5.0
		 * @internal
		 */
		struct ItemIterator {
			friend class BdfNamedList;
			
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
		
		/**
		 * Returns an ItemIterator to the starting item.
		 */
		ItemIterator ibegin() const noexcept;
		
		/**
		 * Returns an ItemIterator to nullptr.
		 */
		ItemIterator iend() const noexcept;
		
		/**
		 * Finds the ItemIterator corresponding to the given key.
		 * @since 1.5.0
		 * @internal
		 */
		ItemIterator findItemIteratorFromKey(int key) const noexcept;
		
		/**
		 * Returns true if a comma is required after the current element.
		 */
		bool serializeHumanReadableShouldPrintComma(ItemIterator iterator) const noexcept;
		
		Item* startItem;
		Item* endItem;

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
		 *          If this functionality is desired, use coerce() instead.
		 * @since 1.0
		 */	
		BdfObject* get(int key);
		
		/**
		 * Gets the item located at key. If it does not exist, creates it.
		 * @param key the key to search for in the list.
		 * @return a pointer to the object located at key.
		 * @since 1.5.0
		 */
		BdfObject* use(size_t key);
		
		/**
		 * Gets the item located at key. If it does not exist, creates it.
		 * @param key the key to search for in the list.
		 * @return a pointer to the object located at key.
		 * @warning Relying on this method's ability to create keys that don't exist is deprecated. From 2.0.0 onwards, this method
		 *          will throw an std::out_of_range exception in that case instead.
		 *          Use BdfNamedList::use() instead.
		 * @since 1.0
		 */
		BdfObject* get(std::string key);
		
		/**
		 * Gets the item located at key. If it does not exist, creates it.
		 * @param key the key to search for in the list.
		 * @return a pointer to the object located at key.
		 * @since 1.5.0
		 */
		BdfObject* use(std::string key);
		
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
		
		/**
		 * Finds the name of the object that one can then retrieve the BdfObject from BdfNamedList::get() by.
		 * If no such name exists (either because the object does not exist in the BdfNamedList, or the needle is nullptr),
		 * return std::nullopt.
		 * @since 1.5.0
		 */
		std::optional<std::string> getNameFromObject(Bdf::BdfObject *needle);
		
		/**
		 * Finds the name equivalent of the given key.
		 * If no such name exists, return nullptr.
		 * @since 1.5.0
		 */
		std::optional<std::string> getNameFromKey(int key);
		
		/**
		 * Finds the key of the object that one can then retrieve the BdfObject from BdfNamedList::get() by.
		 * If no such key exists (either because the object does not exist in the BdfNamedList, or the needle is nullptr),
		 * return std::nullopt.
		 * @since 1.5.0
		 */
		std::optional<int> getKeyFromObject(Bdf::BdfObject *needle);
		
		/**
		 * A const iterator for BdfNamedList.
		 *
		 * BdfNamedList objects can be traversed using iterators as an alternative to key finding.
		 * All iterators in BdfNamedList satisfy std::bidirectional_iterator.
		 *
		 * Unlike modifiabale Iterator objects, ConstIterator objects can always be obtained
		 * regardless of the const-ness of the BdfList. ConstIterators cannot be implicitly
		 * converted to modifiable Iterators, but the opposite is true; modifiable Iterators
		 * can be converted to ConstIterators.
		 *
		 * @since 1.5.0
		 */
		struct ConstIterator {
			friend class BdfNamedList;
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
		
		/**
		 * Returns an iterator to the starting object.
		 */
		ConstIterator cbegin() const noexcept;
		
		/**
		 * Returns an iterator to nullptr.
		 */
		ConstIterator cend() const noexcept;
		
		static_assert(std::bidirectional_iterator<ConstIterator>);
		
		/**
		 * A modifiable iterator for BdfList.
		 *
		 * BdfList objects can be traversed using iterators as an alternative to key finding.
		 * All iterators in BdfList satisfy std::bidirectional_iterator.
		 *
		 * Modifiable Iterator objects cannot be obtained with const BdfList objects. Instead,
		 * you can use ConstIterator. Modifiable Iterator is also implicitly convertible to
		 * ConstIterator. If you don't need to modify the data using an iterator, prefer
		 * ConstIterator.
		 *
		 * @since 1.5.0
		 */
		struct Iterator {
			friend class BdfNamedList;
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
			
			/**
			 * Provides three-way comparison for Iterator objects.
			 * @since 1.5.0
			 */
			friend auto operator<=>(const Iterator&, const Iterator&) = default;
			
			private:
			/** 
			 * The interal ItemIterator held by the iterator.
			 * @since 1.5.0
			 * @internal
			 */
			ItemIterator p;
		};
				
		/**
		 * Returns an iterator to the starting object.
		 */
		Iterator begin() noexcept;
		
		/**
		 * Returns an iterator to nullptr.
		 */
		Iterator end() noexcept;
	};
}

#endif
