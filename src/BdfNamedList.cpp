
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

BdfNamedList::Item::Item(int pKey, BdfObject* pObject, Item* pNext)
{
	next = pNext;
	object = pObject;
	key = pKey;
}

BdfNamedList::BdfNamedList(BdfLookupTable* pLookupTable, const char* data, int size)
{
	lookupTable = pLookupTable;
	start = NULL;
	end = &start;

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
	start = NULL;
	end = &start;

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
				throw BdfError(BdfError::ERROR_SYNTAX, *sr);
			}
	
			std::string key = sr->getQuotedString();
	
			// There should be a colon after this
			sr->ignoreBlanks();
			if(sr->upto[0] != ':') {
				throw BdfError(BdfError::ERROR_SYNTAX, *sr);
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
				throw BdfError(BdfError::ERROR_SYNTAX, *sr);
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

BdfNamedList::~BdfNamedList() noexcept
{
	clear();
}

BdfNamedList* BdfNamedList::clear() noexcept
{
	Item* cur = this->start;
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
	Item* cur = this->start;
	int size = 0;

	while(cur != NULL)
	{
		size += 1;
		cur = cur->next;
	}

	keys.resize(size);
	cur = this->start;

	while(cur != NULL)
	{
		keys.push_back(cur->key);
		cur = cur->next;
	}

	return keys;
}

bool BdfNamedList::exists(std::string key) const noexcept {
	return exists(lookupTable->getLocation(key));
}

bool BdfNamedList::exists(int key) const noexcept
{
	Item* cur = this->start;

	while(cur != NULL)
	{
		if(cur->key == key)
		{
			return true;
		}

		cur = cur->next;
	}

	return false;
}

BdfNamedList* BdfNamedList::set(std::string key, BdfObject* v) {
	return set(lookupTable->getLocation(key), v);
}

BdfNamedList* BdfNamedList::set(int key, BdfObject* v)
{
	Item* cur = this->start;

	while(cur != NULL)
	{
		if(cur->key == key)
		{
			delete cur->object;
			cur->object = v;

			return this;
		}

		cur = cur->next;
	}

	Item* item = new Item(key, v, NULL);

	*this->end = item;
	this->end = &item->next;

	return this;
}

BdfList* BdfNamedList::remove(std::string key) {
	delete this->pop(key);
	return this;
}

BdfList* BdfNamedList::remove(int key) {
	delete this->pop(key);
	return this;
}

BdfObject* BdfNamedList::pop(std::string key) noexcept {
	return this->pop(lookupTable->getLocation(key));
}

BdfObject* BdfNamedList::pop(int key) noexcept
{
	Item** cur = &this->start;

	while(*cur != NULL)
	{
		if((*cur)->key == key)
		{
			BdfObject* object = (*cur)->object;
			Item* next = (*cur)->next;

			delete (*cur)->object;
			delete *cur;

			*cur = next;

			return object;
		}

		cur = &(*cur)->next;
	}

	return NULL;
}

BdfObject* BdfNamedList::get(std::string key) const {
	if(!this->exists(key)) {
		throw std::out_of_range("key " + key + " does not exist");
	}
	
	return get(lookupTable->getLocation(key));
}

BdfObject* BdfNamedList::get(int key) const
{
	if(!this->exists(key)) {
		throw std::out_of_range("key " + std::to_string(key) + " does not exist");
	}
	
	Item* cur = this->start;

	while(cur != NULL)
	{
		if(cur->key == key)
		{
			return cur->object;
		}

		cur = cur->next;
	}
	
	return nullptr;
}

int BdfNamedList::serializeSeeker(int* locations) const
{
	int size = 0;
	Item* cur = this->start;

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
	Item* cur = this->start;

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

void BdfNamedList::serializeHumanReadable(std::ostream &out, BdfIndent indent, int it) const
{
	if(this->start == NULL)
	{
		out << "{}";
		
		return;
	}

	out << "{";

	Item* cur = this->start;

	if(cur != NULL)
	{
		for(;;)
		{
			out << indent.breaker;
	
			for(int n=0;n<=it;n++) {
				out << indent.indent;
			}
			
			std::string name = lookupTable->getName(cur->key);
	
			out << serializeString(name) << ": ";
			cur->object->serializeHumanReadable(out, indent, it + 1);
			cur = cur->next;
	
			if(cur != NULL)
			{
				out << ", ";
			}
	
			else
			{
				break;
			}
		}
	}

 	out << indent.breaker;

	for(int n=0;n<it;n++) {
		out << indent.indent;
	}

	out << "}";
}

void BdfNamedList::getLocationUses(int* locations) const noexcept
{
	Item* cur = this->start;

	while(cur != NULL)
	{
		locations[cur->key] += 1;
		cur->object->getLocationUses(locations);
		cur = cur->next;
	}
}

std::partial_ordering BdfNamedList::operator<=>(const BdfNamedList& rhs) const noexcept {
	std::vector<int> lhsKeys = this->keys();
	std::vector<int> rhsKeys = rhs.keys();
	
	std::vector<int> keysToCheck;
    std::set_intersection(lhsKeys.begin(), lhsKeys.end(), rhsKeys.begin(), rhsKeys.end(), std::back_inserter(keysToCheck));
	
	std::partial_ordering iComparisonResult = std::partial_ordering::unordered;
	
	// Iterate through the keys common to both BdfNamedLists
	for (int i : keysToCheck) {
		iComparisonResult = (this->get(i) <=> rhs.get(i));
		
		// Return the first comparison result that isn't std::partial_ordering::equivalent.
		if (iComparisonResult != std::partial_ordering::equivalent) {
			return iComparisonResult;
		}
	}
	
	// If the above for loop fails to find a result, fall back to the original keys() sizes.
	return (lhsKeys.size() <=> rhsKeys.size());
}

bool BdfNamedList::operator==(const BdfNamedList& rhs) const noexcept {
	return ((*this <=> rhs) == std::partial_ordering::equivalent);
}
