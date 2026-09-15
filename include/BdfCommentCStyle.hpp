#include "./BdfIndent.hpp"

#include <string>
#include <vector>

#ifndef BDFCOMMENTCSTYLE_HPP_
#define BDFCOMMENTCSTYLE_HPP_
namespace Bdf {
	/**
	 * Class that represents a C-style BDF comment.
	 * These can be added to BdfObject, BdfNamedList or BdfList objects and will
	 * be serialised if human-readable serialisation is called.
	 * They will not be serialised to binary BDF output, nor will they be unserialised
	 * if the resulting human-readable BDF data is unserialised again.
	 * Therefore, a round-trip BdfObject serialisation will not result in an equal unserialised
	 * object again.
	 * Use BdfObject::coerceCommentCStyle() or similar to start working with objects of this class.
	 * @since 1.5.0
	 */
	class BdfCommentCStyle {
		std::vector<std::string> commentLines;
		Bdf::BdfIndent indenter;
		bool flatMode;
		
		public:
		/**
		 * Creates a BdfCommentCStyle comment which is empty.
		 * The comment line delimiter will still be serialised.
		 * @param indenter The new object's indenter.
		 * @param flatMode The new object's flatMode.
		 * @since 1.5.0
		 * @internal
		 */
		BdfCommentCStyle(Bdf::BdfIndent indenter = {"", "\n"}, bool flatMode = false);
		
		/**
		 * Creates a BdfCommentCStyle comment that consists of the specified single line comment.
		 * @param commentText A single line of text.
		 * @param indenter The new object's indenter.
		 * @param flatMode The new object's flatMode.
		 * @since 1.5.0
		 * @internal
		 */
		BdfCommentCStyle(std::string commentText, Bdf::BdfIndent indenter = {"", ""}, bool flatMode = false);
		
		/**
		 * Creates a BdfCommentCStyle comment that consists of all the lines given in the vector.
		 * @param commentLines A vector containing lines of text that will represent the comment.
		 * @param indenter The new object's indenter.
		 * @param flatMode The new object's flatMode.
		 * @since 1.5.0
		 * @internal
		 */
		BdfCommentCStyle(std::vector<std::string> commentLines, Bdf::BdfIndent indenter = {"", ""}, bool flatMode = false);
		
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