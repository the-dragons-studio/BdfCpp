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
		bool flatMode;
		
		public:
		/**
		 * Creates a BdfCommentCppStyle comment which is empty.
		 * The comment line delimiter will still be created.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle(Bdf::BdfIndent indenter = {" ", ""});
		
		/**
		 * Creates a BdfCommentCppStyle comment that consists of the specified single line comment.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle(std::string commentText, Bdf::BdfIndent indenter = {" ", ""}, bool flatMode = false);
		
		/**
		 * Creates a BdfCommentCppStyle comment that consists of all the lines given in the vector.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter = {"", ""}, bool flatMode = false);
		
		/**
		 * Copies a BdfCommentCppStyle.
		 */
		BdfCommentCppStyle(const BdfCommentCppStyle&);
		
		/**
		 * Destroys a BdfCommentCppStyle.
		 * @since 1.5.0
		 */
		virtual ~BdfCommentCppStyle() noexcept;
		
		/**
		 * Gets the comment lines of this object.
		 * @since 1.5.0
		 */
		std::vector<std::string> getCommentLines() const;
		
		/**
		 * Gets the indenter of this object.
		 * @since 1.5.0
		 */
		Bdf::BdfIndent getIndenter() const;
		
		/**
		 * Gets whether flat mode is enabled.
		 * @since 1.5.0
		 */
		bool getFlatMode() const noexcept;
		
		/**
		 * Sets the comment lines of this object to the lines given at commentLines.
		 * @param commentLines The new set of comment lines.
		 * @since 1.5.0
		 */
		void setCommentLines(std::vector<std::string> commentLines);
		
		/**
		 * Sets the indenter of this object to the indenter given at indenter.
		 * @param indenter The new indenter.
		 * @since 1.5.0
		 */
		void setIndenter(Bdf::BdfIndent indenter);
		
		/**
		 * Sets whether to use flat mode to the bool given at flatMode.
		 * @param flatMode The new state of flatMode.
		 * @since 1.5.0
		 */
		void setFlatMode(bool flatMode) noexcept;
		
				
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