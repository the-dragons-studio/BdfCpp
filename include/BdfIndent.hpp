
#ifndef BDFINDENT_HPP_
#define BDFINDENT_HPP_

#include <string>

namespace Bdf
{
	/**
	 * Class used to configure indenting and line breaking of serialising human-readable BDF files.
  	 * @since 1.0
         * @newable
	 */
	class BdfIndent
	{
	public:
	
		std::string indent;
		std::string breaker;
		
		/**
		 * Creates an indenter which can be passed to various serialising methods to customise
                 * how they will be indented.
		 * @param indent the string that will be used to indent (for example, in a BdfNamedList).
		 * @param breaker the string that will be used to line break at the end of the line.
		 */
		BdfIndent(std::string indent, std::string breaker);

		/**
                 * Returns the indent string repeated n number of times.
                 * @param n the number of times to repeat the indent string.
                 * @since 1.4.0
                 */
                std::string calcIndent(int n) const;
	};
}

#endif
