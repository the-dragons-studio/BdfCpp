#include "../include/BdfCommentCppStyle.hpp"

namespace Bdf {
	BdfCommentCppStyle::BdfCommentCppStyle() = default;
	
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
	
	BdfCommentCppStyle::~BdfCommentCppStyle() noexcept = default;
	
	void BdfCommentCppStyle::serializeHumanReadable(std::ostream &stream, const BdfIndent &indent, int upto) const {
		
	}
}
