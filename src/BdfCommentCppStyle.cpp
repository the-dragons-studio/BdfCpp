#include "../include/BdfCommentCppStyle.hpp"

#include "../include/BdfHelpers.hpp"

namespace Bdf {
	BdfCommentCppStyle::BdfCommentCppStyle(Bdf::BdfIndent indenter): indenter(std::move(indenter)) {
	}
	
	// Delegate to vector constructor
	BdfCommentCppStyle::BdfCommentCppStyle(std::string commentText, Bdf::BdfIndent indenter, bool flattenSingleLineComments):
		BdfCommentCppStyle(std::vector<std::string>({commentText}), std::move(indenter), flattenSingleLineComments) {
	}
	
	// Stores the given vector
	BdfCommentCppStyle::BdfCommentCppStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter, bool flattenSingleLineComments): 
		commentLines(std::move(commentLines)),
		indenter(std::move(indenter)),
		flattenSingleLineComments(flattenSingleLineComments) {
	}
	
	void BdfCommentCppStyle::setCommentLines(const std::vector<std::string> &commentLines) {
		this->commentLines = commentLines;
	}
	
	void BdfCommentCppStyle::setCommentLines(std::vector<std::string> &&commentLines) {
		this->commentLines = std::move(commentLines);
	}
	
	
	BdfCommentCppStyle::~BdfCommentCppStyle() noexcept = default;
	
	void BdfCommentCppStyle::serializeHumanReadable(std::ostream &stream, const BdfIndent &foreignIndent, int upto) const {
		// Iterate through each stored comment line
		for (std::string c : this->commentLines) {
			stream << foreignIndent.breaker << this->indenter.breaker << this->indenter.calcIndent(upto);
			stream << std::string("// ") << c;
		}
	}
}
