
#include "../include/Bdf.hpp"
#include "../include/BdfHelpers.hpp"
#include <cstdint>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>

using namespace Bdf;
using namespace BdfHelpers;

BdfNamedList::Item::~Item() {
}

BdfNamedList::Item::Item(int pKey, BdfObject* pObject, Item *pLast, Item* pNext)
{
	last = pLast;
	next = pNext;
	object = pObject;
	key = pKey;
}

BdfNamedList::BdfNamedList(BdfLookupTable* pLookupTable, const char* data, int size)
{
	lookupTable = pLookupTable;
	startItem = NULL;
	endItem = startItem;

	int i = 0;

	while(i < size)
	{
		// Get the object
		char key_size = 0;
		char key_size_bytes;
		char bdf_size = 0;
		char bdf_size_bytes;
		
		BdfObject::getFlagData(data + i, NULL, &bdf_size_bytes, &key_size_bytes);
		key_size = BdfObject::getSizeBytes(key_size_bytes);

		if(i + bdf_size >= size) {
			return;
		}

		int object_size = BdfObject::getSize(data + i);
		const char* object_data = data + i;

		if(object_size <= 0 || i + object_size > size) {
			return;
		}

		i += object_size;

		// Get the key
		int key = 0;

		switch(key_size_bytes)
		{
			case 2:
				key = ((unsigned char*)data)[i];
				break;
			case 1:
				key = get_netus(data + i);
				break;
			case 0:
				key = get_netsi(data + i);
				break;
		}

		if(!lookupTable->hasKeyLocation(key)) {
			return;
		}

		i += key_size;

		// Add the list item
		set(key, new BdfObject(lookupTable, object_data, object_size));
	}
}

BdfNamedList::BdfNamedList(BdfLookupTable* lookupTable) : BdfNamedList(lookupTable, NULL, 0) {

}

BdfNamedList::BdfNamedList(BdfLookupTable* pLookupTable, BdfStringReader* sr)
{
	startItem = NULL;
	endItem = startItem;

	lookupTable = pLookupTable;
	sr->upto += 1;

	// {"key": ..., "key2": ...}
	try
	{
		for(;;)
		{
			sr->ignoreBlanks();
	
			wchar_t c = sr->upto[0];
	
			if(c == '}') {
				sr->upto += 1;
				break;
			}
	
			if(c != '"') {
				throw BdfError(BdfError::ErrorType::SYNTAX, *sr);
			}
	
			std::string key = sr->getQuotedString();
	
			// There should be a colon after this
			sr->ignoreBlanks();
			if(sr->upto[0] != ':') {
				throw BdfError(BdfError::ErrorType::SYNTAX, *sr);
			}
	
			sr->upto += 1;
			sr->ignoreBlanks();
	
			BdfObject* bdf = new BdfObject(lookupTable, sr);
			set(key, bdf);
	
			// There should be a comma after this
			sr->ignoreBlanks();
			c = sr->upto[0];
	
			if(c == '}') {
				sr->upto += 1;
				return;
			}
	
			if(c != ',') {
				throw BdfError(BdfError::ErrorType::SYNTAX, *sr);
			}
	
			sr->upto += 1;
			sr->ignoreBlanks();
		}
	}

	catch(BdfError &e)
	{
		clear();
			
		throw;
	}
}

BdfNamedList::~BdfNamedList()
{
	clear();
}

BdfNamedList* BdfNamedList::clear() noexcept
{
	Item* cur = this->startItem;
	Item* next;

	while(cur != NULL)
	{
		next = cur->next;

		delete cur->object;
		delete cur;

		cur = next;
	}

	return this;
}

std::vector<int> BdfNamedList::keys() const noexcept
{
	std::vector<int> keys;
	Item* cur = this->startItem;
	int size = 0;

	while(cur != NULL)
	{
		size += 1;
		cur = cur->next;
	}

	keys.resize(size);
	cur = this->startItem;

	while(cur != NULL)
	{
		keys.push_back(cur->key);
		cur = cur->next;
	}

	return keys;
}

bool BdfNamedList::exists(std::string key) const noexcept {
	return this->exists(this->lookupTable->getLocation(key));
}

bool BdfNamedList::exists(int key) const noexcept
{
	// Return the operator bool() outcome of the resulting ItemIterator.
	return bool(this->findItemIteratorFromKey(key));
}

std::optional<std::string> BdfNamedList::getNameFromObject(Bdf::BdfObject *needle) {
	std::optional<int> key(this->getKeyFromObject(needle));
	
	// If we manage to get a key, use it to look up the name.
	if (key) {
		return this->lookupTable->getName(*key);
	}
	
	return std::nullopt;
}

std::optional<int> BdfNamedList::getKeyFromObject(Bdf::BdfObject *needle) {
	// Bail immediately if we're given nullptr needle
	if (needle) {
		Bdf::BdfNamedList::ItemIterator iterator;
		// Using our own for loop here
		for (iterator = this->ibegin(); iterator != this->iend(); ++iterator) {
			if (iterator->object == needle) {
				return iterator->key;
			}
		}
	}
	
	return std::nullopt;
}

Bdf::BdfNamedList::ItemIterator Bdf::BdfNamedList::findItemIteratorFromKey(int key) const noexcept {
	// Get an ItemIterator
	BdfNamedList::ItemIterator iterator;
	
	for (iterator = this->ibegin(); iterator != this->iend(); ++iterator) {
		// See if the Item pointed at by iterator matches the given key.
		if (iterator->key == key) {
			// Return the matching ItemIterator straight away.
			return iterator;
		}
	}
	
	return this->iend();
}

bool Bdf::BdfNamedList::serializeHumanReadableShouldPrintComma(ItemIterator iterator) const noexcept {
	// We determine if a comma is required by checking to see if any non-comment items
	// including the current iterator are found.
	
	while (iterator) {
		if (iterator->object->getType() != BdfTypes::COMMENT_CPP_STYLE && iterator->object->getType() != BdfTypes::COMMENT_C_STYLE) {
			return true;
		}
		
		++iterator;
	}
	
	return false;
}

BdfNamedList* BdfNamedList::set(std::string key, BdfObject* v) noexcept {
	return set(lookupTable->getLocation(key), v);
}

BdfNamedList* BdfNamedList::set(int key, BdfObject* v) noexcept
{
	BdfNamedList::ItemIterator toSet = this->findItemIteratorFromKey(key);
	
	if (toSet) {
		delete toSet->object;
		toSet->object = v;
		return this;
	}

	// If the requested key does not exist:
	// Append our new item to the end.
	Item* item = new Item(key, v, this->endItem, NULL);

	// If no Item is the start item yet, point the start of the linked list to it.
	if (!this->startItem) {
		this->startItem = item;
	}
	
	// If no Item is the start item yet, point the end of the linked list to it.
	if (!this->endItem) {
		this->endItem = item;
	// Otherwise, we need to do linked list stuff.
	} else {
		Item *oldEndItem = this->endItem;
		oldEndItem->next = item;
		
		// The new item already contains a pointer to oldEndItem as part of the constructor step
		// We are now good to put our new pointer down.
		this->endItem = item;
	}
	
	return this;
}

BdfObject* BdfNamedList::remove(int key) noexcept {
    return this->pop(key);
}

BdfObject* BdfNamedList::remove(std::string key) noexcept {
	return this->pop(lookupTable->getLocation(key));
}

BdfObject* BdfNamedList::pop(int key) noexcept
{
	BdfNamedList::ItemIterator toPop = this->findItemIteratorFromKey(key);
	
	if (toPop) {
		// Store the pointer of the currently pointed at object.
		BdfObject *toPopObject(toPop->object);
		
		// Store the iterator of the next object.
		ItemIterator toPopNext(toPop++);
		
		// Store the pointer of the last object.
		ItemIterator toPopLast(toPop--);
		
		// Delete toPop (not its object)
		delete *toPop;
		
		// If there is a last object, set its next to toPopNext's
		if (toPopLast) {
			if (toPopNext) {
				toPopLast->next = *toPopNext;
			} else {
				toPopLast->next = nullptr;
			}
		}
		
		// If there is a next object, set its next to toPopLast's
		if (toPopNext) {
			if (toPopLast) {
				toPopNext->last = *toPopLast;
			} else {
				toPopNext->last = nullptr;
			}
		}
		
		// In any case, return the now-orphaned object.
		return toPopObject;
	}
	
	return NULL;
}

BdfObject* BdfNamedList::get(std::string key) {
	return get(lookupTable->getLocation(key));
}

BdfObject* BdfNamedList::get(int key)
{
	BdfNamedList::ItemIterator toGet = this->findItemIteratorFromKey(key);

	if (toGet) {
		return toGet->object;
	} else {
		BdfObject* v = new BdfObject(lookupTable);
		set(key, v);

		return v;
	}
}

int BdfNamedList::serializeSeeker(int* locations) const
{
	int size = 0;
	Item* cur = this->startItem;

	while(cur != NULL)
	{
		int location = locations[cur->key];

		if(location > 65535) {		// >= 2 ^ 16
			size += 4;
		} else if(location > 255) {	// >= 2 ^ 8
			size += 2;
		} else {					// < 2 ^ 8
			size += 1;
		}

		size += cur->object->serializeSeeker(locations);
		cur = cur->next;
	}

	return size;
}

int BdfNamedList::serialize(char* data, int* locations) const
{
	int pos = 0;
	Item* cur = this->startItem;

	while(cur != NULL)
	{
		int location = locations[cur->key];

		char size_bytes_tag;
		char size_bytes;

		if(location > 65535) {
			size_bytes_tag = 0;
			size_bytes = 4;
		} else if(location > 255) {
			size_bytes_tag = 1;
			size_bytes = 2;
		} else {
			size_bytes_tag = 2;
			size_bytes = 1;
		}

		int size = cur->object->serialize(data + pos, locations, size_bytes_tag);
		int offset = pos + size;

		switch(size_bytes_tag)
		{
			case 0:
				put_netsi(data + offset, location);
				break;
			case 1:
				put_netus(data + offset, location);
				break;
			default:
				data[offset] = location & 255;
		}

		pos += size + size_bytes;
		cur = cur->next;
	}

	return pos;
}

void BdfNamedList::serializeHumanReadable(std::ostream &out, const BdfIndent &indent, int it) const
{
	bool lastLoopWasNonComment;
	// Get an iterator (only need const)
	BdfNamedList::ItemIterator iterator = this->ibegin();
	
	// Bail immediately if the iterator is invalid (i.e. the list is empty)
	if(!iterator)
	{
		out << "{}";
		
		return;
	} else {
		// Print start of list tag.
		out << "{";

		do {
			// For the second and onward iterations, print a comma separator. 
			if (lastLoopWasNonComment && this->serializeHumanReadableShouldPrintComma(iterator)) {
				out << ", ";
			}

			// Print a breaker and indenter.
			out << indent.breaker << indent.calcIndent(it);
			
			if (iterator->object->getType() == BdfTypes::COMMENT_CPP_STYLE || iterator->object->getType() == BdfTypes::COMMENT_C_STYLE) {
				lastLoopWasNonComment = false;
				out << indent.breaker;
				iterator->object->serializeHumanReadable(out, indent, it + 1);
			} else {
				lastLoopWasNonComment = true;
				// Serialise the name of the current item
				out << serializeString(this->lookupTable->getName(iterator->key)) << ": ";
				
				// Serialise the object
				iterator->object->serializeHumanReadable(out, indent, it + 1);
			}

			// Iterate. We'll only proceed back to the comma if this doesn't equal the end.
			++iterator;
		} while (iterator != this->iend());					
	
		// Print end of list tag.
		out << indent.breaker << indent.calcIndent(it) << "}";
	}
}

void BdfNamedList::getLocationUses(int* locations) const noexcept
{
	Item* cur = this->startItem;

	while(cur != NULL)
	{
		locations[cur->key] += 1;
		cur->object->getLocationUses(locations);
		cur = cur->next;
	}
}

std::optional<std::string> BdfNamedList::getNameFromKey(int key) {
	return this->lookupTable->getName(key);
}

BdfNamedList::Iterator BdfNamedList::begin() noexcept {
	return Iterator(this->ibegin());
}

BdfNamedList::Iterator BdfNamedList::end() noexcept {
	return Iterator(this->iend());
}

BdfNamedList::ConstIterator BdfNamedList::cbegin() const noexcept {
	return ConstIterator(this->ibegin());
}

BdfNamedList::ConstIterator BdfNamedList::cend() const noexcept {
	return ConstIterator(this->iend());
}

BdfNamedList::ItemIterator BdfNamedList::ibegin() const noexcept {
	return ItemIterator(this->startItem);
}

BdfNamedList::ItemIterator BdfNamedList::iend() const noexcept {
	return ItemIterator(nullptr);
}

BdfNamedList::ItemIterator::ItemIterator(): p(nullptr) {}

BdfNamedList::ItemIterator::ItemIterator(Item *p): p(p) {}

BdfNamedList::Item* BdfNamedList::ItemIterator::operator*() const noexcept {
	return p;
}

BdfNamedList::Item* BdfNamedList::ItemIterator::operator->() const noexcept {
	return p;
}

BdfNamedList::ItemIterator& BdfNamedList::ItemIterator::operator++() {
	if (this->isValid()) {
		this->p = this->p->next;
	}
	return *this;
}

BdfNamedList::ItemIterator BdfNamedList::ItemIterator::operator++(int) {
	ItemIterator tmp = *this;
	++tmp;
	return tmp;
}

BdfNamedList::ItemIterator& BdfNamedList::ItemIterator::operator--() {
	if (this->isValid()) {
		this->p = this->p->last;
	}
	return *this;
}

BdfNamedList::ItemIterator BdfNamedList::ItemIterator::operator--(int) {
	ItemIterator tmp = *this;
	--tmp;
	return tmp;
}

bool BdfNamedList::ItemIterator::isValid() const noexcept {
	return (this->p != nullptr);
}

BdfNamedList::ItemIterator::operator bool() const noexcept {
	return this->isValid();
}

BdfNamedList::ConstIterator::ConstIterator() {}

BdfNamedList::ConstIterator::ConstIterator(const BdfNamedList::ItemIterator &p): p(p) {}

const BdfObject* BdfNamedList::ConstIterator::operator*() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

const BdfObject* BdfNamedList::ConstIterator::operator->() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

BdfNamedList::ConstIterator& BdfNamedList::ConstIterator::operator++() {
	++(this->p);
	return *this;
}

BdfNamedList::ConstIterator BdfNamedList::ConstIterator::operator++(int) {
	ConstIterator tmp = *this;
	++tmp;
	return tmp;
}

BdfNamedList::ConstIterator& BdfNamedList::ConstIterator::operator--() {
	--(this->p);
	return *this;
}

BdfNamedList::ConstIterator BdfNamedList::ConstIterator::operator--(int) {
	ConstIterator tmp = *this;
	++tmp;
	return tmp;
}

bool BdfNamedList::ConstIterator::isValid() const noexcept {
	return (this->p.isValid() && this->p->object != nullptr);
}

BdfNamedList::BdfNamedList::ConstIterator::operator bool() const noexcept {
	return this->isValid();
}

BdfNamedList::Iterator::Iterator() {}

BdfNamedList::Iterator::Iterator(const ItemIterator& p): p(p) {}

BdfObject* BdfNamedList::Iterator::operator*() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

BdfObject* BdfNamedList::Iterator::operator->() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

BdfNamedList::Iterator& BdfNamedList::Iterator::operator++() {
	++(this->p);
	return *this;
}

BdfNamedList::Iterator BdfNamedList::Iterator::operator++(int) {
	Iterator tmp(std::move(*this));
	++tmp;
	return tmp;
}

BdfNamedList::Iterator& BdfNamedList::Iterator::operator--() {
	--(this->p);
	return *this;
}

BdfNamedList::Iterator BdfNamedList::Iterator::operator--(int) {
	Iterator tmp(std::move(*this));
	--tmp;
	return tmp;
}

bool BdfNamedList::Iterator::isValid() const noexcept {
	return (this->p.isValid() && this->p->object != nullptr);
}

BdfNamedList::Iterator::operator bool() const noexcept {
	return this->isValid();
}

BdfNamedList::Iterator::operator ConstIterator() const noexcept {
	return BdfNamedList::ConstIterator(this->p);
}