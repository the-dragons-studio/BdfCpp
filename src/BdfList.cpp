
#include "../include/Bdf.hpp"
#include "../include/BdfHelpers.hpp"
#include <vector>
#include <cstdint>
#include <iostream>
#include <string.h>

using namespace Bdf;
using namespace BdfHelpers;

BdfList::BdfList(BdfLookupTable* lookupTable, const char* data, int size)
{
	this->startItem = nullptr;
	this->endItem = nullptr;
	this->endptr = &this->startItem;
		
	int i = 0;

	while(i < size)
	{
		char object_size_tag;
		BdfObject::getFlagData(data, nullptr, &object_size_tag, nullptr);

		char object_size_bytes = BdfObject::getSizeBytes(object_size_tag);
		
		if(i + object_size_bytes >= size) {
			return;
		}

		// Get the size of the object
		int object_size = BdfObject::getSize(data + i);
	
		if(object_size <= 0 || i + object_size > size) {
			return;
		}

		// Add the object to the elements list
		add(new BdfObject(lookupTable, data + i, object_size));

		// Increase the iterator by the amount of bytes
		i += object_size;
	}
}

BdfList::BdfList(BdfLookupTable* lookupTable) : BdfList(lookupTable, nullptr, 0) {

}

BdfList::BdfList(BdfLookupTable* lookupTable, BdfStringReader* sr)
{
	this->startItem = nullptr;
	this->endItem = nullptr;
	this->endptr = &this->startItem;
		
	sr->upto += 1;

	// [..., ...]

	try
	{	
		for(;;)
		{
			sr->ignoreBlanks();

			if(sr->upto[0] == ']') {
				sr->upto += 1;
				return;
			}

			BdfObject* bdf = new BdfObject(lookupTable, sr);
			add(bdf);

			// There should be a comma after this
			sr->ignoreBlanks();
			wchar_t c = sr->upto[0];
	
			if(c == ']') {
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

BdfList::~BdfList()
{
	clear();
}

BdfObject* BdfList::get(int index) const
{
	return getAtIndex(index)->object;
}

BdfList* BdfList::set(int index, BdfObject* o)
{
	Item* item = getAtIndex(index);

	delete item->object;
	item->object = o;

	return this;
}

BdfList::Item* BdfList::getStartItem() const noexcept
{
	return this->startItem;
}

BdfList::Item* BdfList::getEndItem() const noexcept
{
	return this->endItem;
}

BdfObject* BdfList::getStart() noexcept {
	if (this->startItem == nullptr) {
		return nullptr;
	}
	return this->startItem->object;
}

BdfObject* BdfList::getEnd() noexcept {
	if (this->endItem == nullptr) {
		return nullptr;
	}
	return this->endItem->object;
}

BdfList* BdfList::insertNext(Item* item, BdfObject* object)
{
	if(item->next == nullptr)
	{
		add(object);
	}

	else
	{
		Item* item_new = new Item();

		item_new->object = object;
		item_new->last = item;
		item_new->next = item->next;

		item->next->last = item_new;
		item->next = item_new;
	}

	return this;
}

BdfList* BdfList::insertNext(uint64_t index, BdfObject* o) {
	return this->insertNext(this->getAtIndex(index), o);
}

BdfList* BdfList::insertNext(BdfObject* needle, BdfObject* o, bool fallbackToAdd) {
	BdfList::ConstIterator it = this->find(needle);
	
	if (it) {
		this->insertNext(it.p.p, o);
	} else if (fallbackToAdd) {
		this->add(o);
	} else {
		throw std::out_of_range("Could not insert in BdfList because the requested object was not found.");
	}
	
	return this;
}

BdfList* BdfList::insertLast(Item* item, BdfObject* object)
{
	Item* item_new = new Item();

	item_new->object = object;

	if(item->last == nullptr)
	{
		item_new->last = nullptr;
		item_new->next = item;

		item->last = item_new;
		this->startItem = item_new;
	}

	else
	{
		item_new->next = item;
		item_new->last = item->last;
		
		item->last->next = item_new;
		item->last = item_new;
	}

	return this;
}

BdfList* BdfList::insertLast(uint64_t index, BdfObject* o) {
	return this->insertLast(this->getAtIndex(index), o);
}

BdfList* BdfList::insertLast(BdfObject* needle, BdfObject* o, bool fallbackToAdd) {
	BdfList::ConstIterator it = this->find(needle);
	
	if (it) {
		this->insertLast(it.p.p, o);
	} else if (fallbackToAdd) {
		this->add(o);
	} else {
		throw std::out_of_range("Could not insert in BdfList because the requested object was not found.");
	}
	
	return this;
}

BdfList* BdfList::remove(Item* item) noexcept
{
	if(item->next == nullptr && item->last == nullptr)
	{
		this->startItem = nullptr;
		this->endItem = nullptr;
		this->endptr = &this->startItem;
	}

	else if(item->last == nullptr)
	{
		item->next->last = nullptr;
		this->startItem = item->next;
	}

	else if(item->next == nullptr)
	{
		item->last->next = nullptr;
		this->endItem = item->last;
		this->endptr = &item->last->next;
	}

	else
	{
		item->next->last = item->last;
		item->last->next = item->next;
	}

	BdfObject* object = item->object;

	delete item;
	delete object;

	return this;
}

BdfList* BdfList::remove(BdfObject* item) noexcept
{
	Item* cur = this->startItem;

	while(cur != nullptr && cur->object != item)
	{
		cur = cur->next;
	}

	if(cur != nullptr)
	{
		remove(cur);
	}
	
	return this;
}

BdfList* BdfList::add(BdfObject* o)
{
	Item* item = new Item();
	
	item->object = o;
	item->last = this->endItem;
	item->next = nullptr;

	*this->endptr = item;
	this->endptr = &item->next;
	this->endItem = item;
		
	return this;
}

BdfList* BdfList::remove(int index)
{
	return remove(getAtIndex(index));
}

BdfList* BdfList::clear() noexcept {
	// Use iterators to clear the list.
	// 1. Get an ItemIterator to the end.
	// 2. Delete our current Item's object.
	// 3. Delete our current Item's next element (deleting nullptr is okay!).
	// 4. Decrement the iterator.
	BdfList::ItemIterator it = BdfList::ItemIterator(this->getEndItem());
	
	while (it) {
		delete (*it)->object;
		delete (*it)->next;
		--it;
	}
	
	delete this->startItem;
	
	this->startItem = nullptr;
	this->endItem = nullptr;
	
	return this;
}

void BdfList::getLocationUses(int* locations) const
{
	Item* upto = this->startItem;

	while(upto != nullptr)
	{
		upto->object->getLocationUses(locations);
		upto = upto->next;
	}
}

int BdfList::serializeSeeker(int* locations) const
{
	Item* upto = this->startItem;
	int size = 0;

	while(upto != nullptr)
	{
		size += upto->object->serializeSeeker(locations);
		upto = upto->next;
	}

	return size;
}

int BdfList::serialize(char *data, int* locations) const
{
	Item* upto = this->startItem;
	int pos = 0;

	while(upto != nullptr)
	{
		pos += upto->object->serialize(data + pos, locations, 0);
		upto = upto->next;
	}

	return pos;
}

void BdfList::serializeHumanReadable(std::ostream &out, BdfIndent indent, int it)
{
	if(this->startItem == nullptr)
	{
		out << "[]";
		
		return;
	}

	out << "[";

	Item* upto = this->startItem;

	if(upto != nullptr)
	{
		for(;;)
		{
			BdfObject *o = upto->object;
	
			out << indent.breaker;
	
			for(int n=0;n<=it;n++) {
				out << indent.indent;
			}
	
			o->serializeHumanReadable(out, indent, it + 1);
			upto = upto->next;
	
			if(upto != nullptr)
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

	out << "]";
}

uint64_t BdfList::size() const noexcept {
	return std::distance(this->ibegin(), this->iend());
}

BdfList* BdfList::reserve(uint64_t size) {
	uint64_t currentSize = this->size();
	
	// 1. currentSize = 5, size = 8
	// 2. currentSize < size (5 < 8)
	// 3. this->add(nullptr)
	// 4. i + 1 = 6
	// 3. this->add(nullptr)
	// 4. i + 1 = 7
	// 3. this->add(nullptr)
	// 4. i + 1 = 8
	// 8 !< 8
	// return this.
	if (currentSize < size) {	
		for (uint64_t i = currentSize; i < size; i++) {
			this->add(new BdfObject(this->lookupTable));
		}
	}
	
	return this;
}

BdfList* BdfList::shrinkUndefinedObjects() {
	BdfList::ItemIterator it(this->endItem);
	
	while (!*(it->object)) {
		delete it->object;
		--it;
		delete (*it)->next;
		(*it)->next = nullptr;
	}
	
	return this;
}

BdfList::ConstIterator BdfList::find(BdfObject* needle) const noexcept {
	// Search the entire BdfList until we find it.
	BdfList::ConstIterator first = this->cbegin();
	BdfList::ConstIterator last = this->cend();
	
	while (first != last) {
		if (**first == *needle) {
			return first;
		}
		++first;
	}
	
	return last;
}

std::optional<uint64_t> BdfList::findIndex(BdfObject* needle) const noexcept {
	// Search the entire BdfList until we find it.
	uint64_t itNo;
	BdfList::ConstIterator first = this->cbegin();
	BdfList::ConstIterator last = this->cend();
	
	for (itNo = 0; first != last; ++first, ++itNo) {
		if (**first == *needle) {
			return itNo;
		}
		
	}
	
	return std::nullopt;
}

std::partial_ordering BdfList::operator<=>(const BdfList& rhs) const noexcept {
	BdfList::ConstIterator lhsIt = this->cbegin();
	BdfList::ConstIterator rhsIt = rhs.cbegin();
	std::partial_ordering itComparisonResult = std::partial_ordering::unordered;
	
	while (lhsIt && rhsIt) {
		itComparisonResult = (**lhsIt <=> **rhsIt);
		if (itComparisonResult != std::partial_ordering::equivalent) {
			return itComparisonResult;
		}
		
		++lhsIt;
		++rhsIt;
	}
	
	return (lhsIt <=> rhsIt);
}

bool BdfList::operator==(const BdfList& rhs) const noexcept {
	return ((*this <=> rhs) == std::partial_ordering::equivalent);
}

BdfList::Iterator BdfList::begin() noexcept {
	return Iterator(this->ibegin());
}

BdfList::Iterator BdfList::end() noexcept {
	return Iterator(this->iend());
}

BdfList::ConstIterator BdfList::cbegin() const noexcept {
	return ConstIterator(this->ibegin());
}

BdfList::ConstIterator BdfList::cend() const noexcept {
	return ConstIterator(this->iend());
}

BdfList::ItemIterator BdfList::ibegin() const noexcept {
	return ItemIterator(this->startItem);
}

BdfList::ItemIterator BdfList::iend() const noexcept {
	return ItemIterator(nullptr);
}

BdfList::ItemIterator::ItemIterator(): p(nullptr) {}

BdfList::ItemIterator::ItemIterator(Item *p): p(p) {}

BdfList::Item* BdfList::ItemIterator::operator*() const noexcept {
	return p;
}

BdfList::Item* BdfList::ItemIterator::operator->() const noexcept {
	return p;
}

BdfList::ItemIterator& BdfList::ItemIterator::operator++() {
	if (this->isValid()) {
		this->p = this->p->next;
	}
	return *this;
}

BdfList::ItemIterator BdfList::ItemIterator::operator++(int) {
	ItemIterator tmp = *this;
	++tmp;
	return tmp;
}

BdfList::ItemIterator& BdfList::ItemIterator::operator--() {
	if (this->isValid()) {
		this->p = this->p->last;
	}
	return *this;
}

BdfList::ItemIterator BdfList::ItemIterator::operator--(int) {
	ItemIterator tmp = *this;
	--tmp;
	return tmp;
}

bool BdfList::ItemIterator::isValid() const noexcept {
	return (this->p != nullptr);
}

BdfList::ItemIterator::operator bool() const noexcept {
	return this->isValid();
}

BdfList::ConstIterator::ConstIterator() {}

BdfList::ConstIterator::ConstIterator(const BdfList::ItemIterator &p): p(p) {}

const BdfObject* BdfList::ConstIterator::operator*() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

const BdfObject* BdfList::ConstIterator::operator->() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

BdfList::ConstIterator& BdfList::ConstIterator::operator++() {
	++(this->p);
	return *this;
}

BdfList::ConstIterator BdfList::ConstIterator::operator++(int) {
	ConstIterator tmp = *this;
	++tmp;
	return tmp;
}

BdfList::ConstIterator& BdfList::ConstIterator::operator--() {
	--(this->p);
	return *this;
}

BdfList::ConstIterator BdfList::ConstIterator::operator--(int) {
	ConstIterator tmp = *this;
	++tmp;
	return tmp;
}

bool BdfList::ConstIterator::isValid() const noexcept {
	return (this->p.isValid() && this->p->object != nullptr);
}

BdfList::BdfList::ConstIterator::operator bool() const noexcept {
	return this->isValid();
}

BdfList::Iterator::Iterator() {}

BdfList::Iterator::Iterator(const ItemIterator& p): p(p) {}

BdfObject* BdfList::Iterator::operator*() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

BdfObject* BdfList::Iterator::operator->() const noexcept {
	if (this->isValid()) {
		return this->p->object;
	} else {
		return nullptr;
	}
}

BdfList::Iterator& BdfList::Iterator::operator++() {
	++(this->p);
	return *this;
}

BdfList::Iterator BdfList::Iterator::operator++(int) {
	Iterator tmp = *this;
	++tmp;
	return tmp;
}

BdfList::Iterator& BdfList::Iterator::operator--() {
	--(this->p);
	return *this;
}

BdfList::Iterator BdfList::Iterator::operator--(int) {
	Iterator tmp = *this;
	--tmp;
	return tmp;
}

bool BdfList::Iterator::isValid() const noexcept {
	return (this->p.isValid() && this->p->object != nullptr);
}

BdfList::Iterator::operator bool() const noexcept {
	return this->isValid();
}

BdfList::Iterator::operator ConstIterator() const noexcept {
	return BdfList::ConstIterator(this->p);
}

BdfList::Item* BdfList::getAtIndex(uint64_t index) const {
	uint64_t itNo = 0;
	BdfList::ItemIterator end = this->iend();
	for (BdfList::ItemIterator it = this->ibegin(); it != end; ++it, ++itNo) {
		if (itNo == index) {
			return *it;
		}
	}
	
	throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
}