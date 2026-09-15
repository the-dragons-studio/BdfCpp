#include "./BdfIndent.hpp"

#include <string>
#include <vector>

#ifndef BDFCOMMENTCSTYLE_HPP_
#define BDFCOMMENTCSTYLE_HPP_
namespace Bdf {
	/**
	 * Class that represents a single line BDF comment.
	 * @since 1.5.0
	 */
	class BdfCommentCStyle {
		std::vector<std::string> commentLines;
		Bdf::BdfIndent indenter;
		bool flattenSingleLineComments;
		
		public:
		/**
		 * Creates a BdfCommentCStyle comment which is empty.
		 * The comment line delimiter will still be created.
		 */
		BdfCommentCStyle();
		
		/**
		 * Creates a BdfCommentCStyle comment that consists of the specified single line comment.
		 */
		BdfCommentCStyle(std::string commentText, Bdf::BdfIndent indenter = {"", ""}, bool flattenSingleLineComments = false);
		
		/**
		 * Creates a BdfCommentCStyle comment that consists of all the lines given in the vector.
		 */
		BdfCommentCStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter = {"", ""}, bool flattenSingleLineComments = false);
		
		/**
		 * Deleted (no copy constructor).
		 */
		BdfCommentCStyle(const BdfCommentCStyle& that) = delete;
		
		/**
		 * Destroys a BdfCommentCStyle.
		 */
		virtual ~BdfCommentCStyle() noexcept;
		
		/**
		 * Sets the comment text.
		 */
		void set(std::string commentText);
		
		std::string get(std::string commentText) const noexcept;
				
		/**
		 * No-op, since BdfCommentCStyle cannot be serialised to binary format.
		 * @internal
		 */
		int serialize(char *data, int* locations) const;
	    
		/**
		 * Serialises the BdfCommentCStyle to &stream.
		 * @internal
		 */
		void serializeHumanReadable(std::ostream &stream, const BdfIndent &indent, int upto) const;
	};
}
#endif // BDFCOMMENTCSTYLE_HPP_