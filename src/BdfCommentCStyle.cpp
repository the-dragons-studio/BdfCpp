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
	
	void BdfCommentCStyle::serializeHumanReadable(std::ostream &out, const BdfIndent &indent, int it) const {
		std::size_t commentLinesSize = this->commentLines.size();
		if (flatMode && commentLinesSize <= 1) {
			if (commentLinesSize == 0) {
				
			}
		}
		auto iterator = this->commentLines.cbegin();
		
		// Only proceed if there are actually comments to serialise
		if (iterator != this->commentLines.cend()) {
			
		}
	}
	
	BdfCommentCStyle::~BdfCommentCStyle() noexcept = default;
}