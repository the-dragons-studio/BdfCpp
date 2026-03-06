
#ifndef BDFINDENT_HPP_
#define BDFINDENT_HPP_

#include <string>
#include <utility>

namespace Bdf
{
	/**
	 * Class used to configure indenting and line breaking of serialising human-readable BDF files.
     * @newable
  	 * @since 1.0
	 */
	class BdfIndent
	{
	public:
	
		std::string indent, breaker;
		int baseIndentLevel = 0;
		
		/**
		 * Default constructor. Equivalent to constructing BdfIndent with empty indent and breaker.
		 * @since 1.5.0
		 */
		constexpr BdfIndent() = default;
		
		/**
		 * Creates an indenter which can be passed to various serialising methods to customise
         * how they will be indented.
		 * @param indent the string that will be used to indent (for example, in a BdfNamedList).
		 * @param breaker the string that will be used to line break at the end of the line.
		 * @param baseIndentLevel the base indent level. If you need BDF data to be serialised
		                          with at least this amount of indent, specify it here.
		 * @since 1.0
		 */
		constexpr BdfIndent(std::string pIndent, std::string pBreaker, int pBaseIndentLevel = 0):
			indent(std::move(pIndent)),
			breaker(std::move(pBreaker)),
			baseIndentLevel(pBaseIndentLevel)
		{}

		/**
         * Returns the indent string repeated n number of times.
         * @param it the indent iteration level. 
		 *        The returned string will repeat indent (it + this->baseIndentLevel) number of times.
         * @since 1.5.0
         */
        constexpr std::string calcIndent(int it) const {
			std::string t;

			for(int i = 0; i < it + this->baseIndentLevel; i++) {
				t += this->indent;
			}

			return t;
		}
	};
}

#endif
