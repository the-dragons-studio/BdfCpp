#include "./BdfIndent.hpp"

#include <string>
#include <vector>

#ifndef BDFCOMMENTCPPSTYLE_HPP_
#define BDFCOMMENTCPPSTYLE_HPP_
namespace Bdf {
	/**
	 * Class that represents a single line BDF comment.
	 * @since 1.5.0
	 */
	class BdfCommentCppStyle {
		std::vector<std::string> commentLines;
		Bdf::BdfIndent indenter;
		bool flattenSingleLineComments;
		
		public:
		/**
		 * Creates a BdfCommentCppStyle comment which is empty.
		 * The comment line delimiter will still be created.
		 */
		BdfCommentCppStyle();
		
		/**
		 * Creates a BdfCommentCppStyle comment that consists of the specified single line comment.
		 */
		BdfCommentCppStyle(std::string commentText, Bdf::BdfIndent indenter = {"", ""}, bool flattenSingleLineComments = false);
		
		/**
		 * Creates a BdfCommentCppStyle comment that consists of all the lines given in the vector.
		 */
		BdfCommentCppStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter = {"", ""}, bool flattenSingleLineComments = false);
		
		/**
		 * Deleted (no copy constructor).
		 */
		BdfCommentCppStyle(const BdfCommentCppStyle& that) = delete;
		
		/**
		 * Destroys a BdfCommentCppStyle.
		 */
		virtual ~BdfCommentCppStyle() noexcept;
		
		/**
		 * Sets the comment text.
		 */
		void set(std::string commentText);
		
		std::string get(std::string commentText) const noexcept;
				
		/**
		 * No-op, since BdfCommentCppStyle cannot be serialised to binary format.
		 * @internal
		 */
		int serialize(char *data, int* locations) const;
	    
		/**
		 * Serialises the BdfCommentCppStyle to &stream.
		 * @internal
		 */
		void serializeHumanReadable(std::ostream &stream, const BdfIndent &indent, int upto) const;
	};
}
#endif // BDFCOMMENTCPPSTYLE_HPP_