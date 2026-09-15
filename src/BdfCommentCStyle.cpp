#include "../include/BdfCommentCStyle.hpp"

namespace Bdf {
	// Delegate to vector constructor
	BdfCommentCStyle::BdfCommentCStyle(std::string commentText, Bdf::BdfIndent indenter, bool flatMode): 
		BdfCommentCStyle(std::vector<std::string>({commentText}), std::move(indenter), flatMode) {
	}
	
	// Stores the given vector
	BdfCommentCStyle::BdfCommentCStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter, bool flatMode): 
		commentLines(std::move(commentLines)),
		indenter(std::move(indenter)),
		flatMode(flatMode) {
	}
		
	std::vector<std::string> BdfCommentCStyle::getCommentLines() const {
		return this->commentLines;
	}
	
	Bdf::BdfIndent BdfCommentCStyle::getIndenter() const {
		return this->indenter;
	}
		
	bool BdfCommentCStyle::getFlatMode() const noexcept {
		return this-flatMode;
	}
	
	void BdfCommentCStyle::setCommentLines(std::vector<std::string> commentLines) {
		this->commentLines = std::move(commentLines);
	}
	
	void BdfCommentCStyle::setIndenter(Bdf::BdfIndent indenter) {
		this->indenter = std::move(indenter);
	}
	
	void BdfCommentCStyle::setFlatMode(bool flatMode) noexcept {
		this->flatMode = flatMode;
	}
	
	void BdfCommentCStyle::serializeHumanReadable(std::ostream &stream, const BdfIndent &foreignIndent, int it) const {
		stream << std::string("/*");
		
		// If not flatMode, add dummy line at the start of the vector.
		if (!this->flatMode) {
			stream << this->indenter.indent;
		}
		
		// Iterate through each stored comment line
		for (std::string c : this->commentLines) {
			stream << this->indenter.indent << c;
			stream << foreignIndent.breaker << this->indenter.breaker << this->indenter.calcIndent(it);
		}
		
		stream << std::string("*/") << foreignIndent.breaker;
	}
	
	BdfCommentCStyle::~BdfCommentCStyle() noexcept = default;
}