
#ifndef BDFTYPES_HPP_
#define BDFTYPES_HPP_

namespace Bdf
{
	/**
	 * Class specifying types used by BdfObject objects.
	 * This class contains public constants which may be compared against
	 * values returned by BdfObject::getType().
	 * Values that attempt to cast objects to certain values will only
	 * be functional if the BdfObject is of the correct type.
	 */
	class BdfTypes
	{
		public:
		
		/**
		 * Represents an undefined type, or a BdfObject which contains unrecognised data.
		 * Can also be tested for via operator bool() returning false.
		 */
		const static char UNDEFINED = 0;
		
		/**
		 * Represents a boolean type BdfObject.
		 * Boolean type objects can be set to either true or false.
		 */
		const static char BOOLEAN = 1;
		
		/**
		 * Represents an integer type BdfObject.
		 * Integer type objects can represent any value from -(2^31) to +(2^31 - 1).
		 */
		const static char INTEGER = 2;
		
		/**
		 * Represents a long type BdfObject.
		 * Long type objects can represent any value from -(2^63) to +(2^63 - 1).
		 */
		const static char LONG = 3;
		
		/**
		 * Represents a short type BdfObject.
		 * Short type objects can represent any value from -(2^15) to +(2^15 - 1).
		 */
		const static char SHORT = 4;
		
		/**
		 * Represents a byte type BdfObject.
		 * Byte type objects can represent any value from -(2^7) to +(2^7 - 1).
		 */
		const static char BYTE = 5;
		
		/**
		 * Represents a double type BdfObject.
		 * Double type objects can represent any value that can be represented in a C++ double (usually, but not always, IEEE 754).
		 */
		const static char DOUBLE = 6;
		
		/**
		 * Represents a float type BdfObject.
		 * Float type objects can represent any value that can be represented in a C++ float (usually, but not always, IEEE 754).
		 */
		const static char FLOAT = 7;
	
		/**
		 * Represents a string type BdfObject.
		 * String type objects can represent a std::string.
		 */
		const static char STRING = 8;
		
		/**
		 * Represents a list type BdfObject.
		 * List type objects can be cast to a BdfList to perform operations on their data.
		 */
		const static char LIST = 9;
		
		/**
		 * Represents a named list type BdfObject.
		 * Named list type objects can be cast to a BdfNamedList to perform operations on their data.
		 */
		const static char NAMED_LIST = 10;
		
		/**
		 * Represents a boolean array type BdfObject.
		 * Boolean array type objects contain an array of booleans.
		 */
		const static char ARRAY_BOOLEAN = 11;
		
		/**
		 * Represents an integer array type BdfObject.
		 * Integer array type objects contain an array of integers.
		 */
		const static char ARRAY_INTEGER = 12;
		
		/**
		 * Represents a long array type BdfObject.
		 * Long array type objects contain an array of longs.
		 */
		const static char ARRAY_LONG = 13;
		
		/**
		 * Represents a short array type BdfObject.
		 * Short array type objects contain an array of shorts.
		 */
		const static char ARRAY_SHORT = 14;
		
		/**
		 * Represents a byte array type BdfObject.
		 * Byte array type objects contain an array of bytes.
		 */
		const static char ARRAY_BYTE = 15;
		
		/**
		 * Represents a double array type BdfObject.
		 * Double array type objects contain an array of doubles.
		 */
		const static char ARRAY_DOUBLE = 16;
		
		/**
		 * Represents a float array type BdfObject.
		 * Float array type objects contain an array of floats.
		 */
		const static char ARRAY_FLOAT = 17;
	};
}

#endif
