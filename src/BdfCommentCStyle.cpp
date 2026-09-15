#include "../include/BdfCommentCStyle.hpp"

namespace Bdf {
	// Delegate to vector constructor
	BdfCommentCStyle::BdfCommentCStyle(std::string commentText, Bdf::BdfIndent indenter, bool flattenSingleLineComments): 
		BdfCommentCStyle(std::vector<std::string>({commentText}), std::move(indenter), flattenSingleLineComments) {
	}
	
	// Stores the given vector
	BdfCommentCStyle::BdfCommentCStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter, bool flattenSingleLineComments): 
		commentLines(std::move(commentLines)),
		indenter(std::move(indenter)),
		flattenSingleLineComments(flattenSingleLineComments) {
	}
	
	void BdfCommentCStyle::serializeHumanReadable(std::ostream &out, const BdfIndent &indent, int it) const {
		std::size_t commentLinesSize = this->commentLines.size();
		if (flattenSingleLineComments && commentLinesSize <= 1) {
			if (commentLinesSize == 0) {
				
			}
		}
		auto iterator = this->commentLines.cbegin();
		
		// Only proceed if there are actually comments to serialise
		if (iterator != this->commentLines.cend()) {
			
		}
	}
}