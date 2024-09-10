
#include "../include/BdfIndent.hpp"

#include <utility>

using namespace Bdf;

BdfIndent::BdfIndent(std::string pIndent, std::string pBreaker): indent(std::move(pIndent)), breaker(std::move(pBreaker)) {}
