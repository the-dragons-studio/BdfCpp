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
		 * @since 1.5.0
		 */
		BdfCommentCppStyle(Bdf::BdfIndent indenter = {"", "\n"});
		
		/**
		 * Creates a BdfCommentCppStyle comment that consists of the specified single line comment.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle(std::string commentText, Bdf::BdfIndent indenter = {"", "\n"}, bool flatMode = false);
		
		/**
		 * Creates a BdfCommentCppStyle comment that consists of all the lines given in the vector.
		 * @since 1.5.0
		 */
		BdfCommentCppStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter = {"", "\n"}, bool flatMode = false);
		
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
		 * Copies the comment lines given at commentLines into this object.
		 * @since 1.5.0
		 */
		void setCommentLines(const std::vector<std::string> &commentLines);
		
		/**
		 * Moves the comment lines given at commentLines into this object.
		 * @since 1.5.0
		 */
		void setCommentLines(std::vector<std::string> &&commentLines);
		
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