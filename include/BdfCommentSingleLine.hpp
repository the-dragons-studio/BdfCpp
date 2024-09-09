namespace Bdf {
	/**
	 * A BdfCommentSingle represents a single line BDF comment.
	 * It permits programs to add single line comments, which will be serialised into a human-readable output.
	 * Note that, by design, this object cannot be obtained as a result of unserialisation. It also will have no effect 
	 * in binary serialisation.
	 * @since 2.0.0
	 */
	class BdfNamedList