
#include "../include/BdfIndent.hpp"

#include <stdexcept>

using namespace Bdf;

BdfIndent::BdfIndent(std::string pIndent, std::string pBreaker): indent(pIndent), breaker(pBreaker) {
    // Check that all characters in the strings don't have illegal characters
    if (this->indent.find_first_not_of("\t\n ") != std::string::npos) {
        throw std::invalid_argument(std::string("Bdf::BdfIndent: Indenter \"") + this->indenter + "\" contains non-whitespace or newline characters.");
    } else if (this->breaker.find_first_not_of("\t\n ") != std::string::npos) {
        throw std::invalid_argument(std::string("Bdf::BdfIndent: Breaker \"") + this->breaker + "\" contains non-whitespace or newline characters.");
    }
}
