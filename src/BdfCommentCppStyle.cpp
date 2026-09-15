#include "../include/BdfCommentCppStyle.hpp"

#include "../include/BdfHelpers.hpp"

namespace Bdf {
	BdfCommentCppStyle::BdfCommentCppStyle(Bdf::BdfIndent indenter): indenter(std::move(indenter)) {
	}
	
	// Delegate to vector constructor
	BdfCommentCppStyle::BdfCommentCppStyle(std::string commentText, Bdf::BdfIndent indenter, bool flatMode):
		BdfCommentCppStyle(std::vector<std::string>({commentText}), std::move(indenter), flatMode) {
	}
	
	// Stores the given vector
	BdfCommentCppStyle::BdfCommentCppStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter, bool flatMode): 
		commentLines(std::move(commentLines)),
		indenter(std::move(indenter)),
		flatMode(flatMode) {
	}
	
	std::vector<std::string> BdfCommentCppStyle::getCommentLines() const {
		return this->commentLines;
	}
	
	Bdf::BdfIndent BdfCommentCppStyle::getIndenter() const {
		return this->indenter;
	}
		
	bool BdfCommentCppStyle::getFlatMode() const noexcept {
		return this-flatMode;
	}
	
	void BdfCommentCppStyle::setCommentLines(std::vector<std::string> commentLines) {
		this->commentLines = std::move(commentLines);
	}
	
	void BdfCommentCppStyle::setIndenter(Bdf::BdfIndent indenter) {
		this->indenter = std::move(indenter);
	}
	
	void BdfCommentCppStyle::setFlatMode(bool flatMode) noexcept {
		this->flatMode = flatMode;
	}
	
	BdfCommentCppStyle::~BdfCommentCppStyle() noexcept = default;
	
	void BdfCommentCppStyle::serializeHumanReadable(std::ostream &stream, const BdfIndent &foreignIndent, int upto) const {
		// Iterate through each stored comment line
		for (std::string c : this->commentLines) {
			stream << std::string("// ") << c;
			stream << foreignIndent.breaker << this->indenter.breaker << this->indenter.calcIndent(upto);
		}
	}
}
