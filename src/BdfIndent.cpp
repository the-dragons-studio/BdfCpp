
#include "../include/BdfIndent.hpp"

#include <utility>

using namespace Bdf;

BdfIndent::BdfIndent(std::string pIndent, std::string pBreaker): indent(std::move(pIndent)), breaker(std::move(pBreaker)) {}

// Moved from BdfObject::calcIndent() to be more useful to other classes that need it.
// @todo: Try to make constexpr?
std::string BdfIndent::calcIndent(int it) const {
	std::string t;

	for(int i=0; i<=it;i ++) {
		t += this->indent;
	}

	return t;
}