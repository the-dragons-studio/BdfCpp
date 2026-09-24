#include "../include/BdfCommentCppStyle.hpp"

#include "../include/BdfHelpers.hpp"

#include <ostream>

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
			// Print the comment text
			std::print(stream, "//{0}{1}", this->indenter.indent, c);
			// Use both the foreign indenter and the native indenter to break the line.
			std::print(stream, "{0}{1}", foreignIndent.breaker, this->indenter.breaker);
			// Use the foreign indenter to prepare the next comment.
			std::print(stream, "{0}", foreignIndent.calcIndent(upto));
		}
	}
}
