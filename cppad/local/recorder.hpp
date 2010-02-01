/* $Id$ */
# ifndef CPPAD_RECORDER_INCLUDED
# define CPPAD_RECORDER_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/hash_code.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file recorder.hpp
File used to define the recorder class.
*/

/*!
Class used to store an operation sequence while it is being recorded
(the operation sequence is copied to the player class for playback).

\tparam Base
This is an AD< \a Base > operation sequence recording; i.e.,
it records operations of type AD< \a Base >.
*/
template <class Base>
class recorder {
	friend class player<Base>;

private:
	/// Number of variables in the recording.
	size_t    num_rec_var_;

	/// Length of the operation vector rec_op_.
	size_t    len_rec_op_;

	/// Number of operators currently in the recording. 
	size_t    num_rec_op_;

	/// The operators in the recording.
	OpCode   *rec_op_;

	/// Length of the VecAD index vector rec_vecad_ind_
	size_t    len_rec_vecad_ind_;

	/// Number of VecAD indices currently in the recording. 
	size_t    num_rec_vecad_ind_;

	/// The VecAD indices in the recording.
	size_t   *rec_vecad_ind_;

	/// Length of operation argument index vector rec_op_arg_.
	size_t    len_rec_op_arg_;

	/// Number of operation arguments indices currently in the recording. 
	size_t    num_rec_op_arg_;

	/// The argument indices in the recording
	size_t   *rec_op_arg_;

	/// Length of parameter vector rec_par_.
	size_t    len_rec_par_;

	/// Number of parameters currently in the recording.
	size_t    num_rec_par_;

	/// The parameters in the recording.
	Base     *rec_par_;

	/// Length of text character vector rec_text_.
	size_t    len_rec_text_;

	/// Number of text characters currently in recording.
	size_t    num_rec_text_;

	/// Character strings ('\\0' terminated) in the recording.
	char     *rec_text_;

// ---------------------- Public Functions -----------------------------------
public:
	/// Default constructor
	recorder(void) 
	{	
		num_rec_var_         = 0;

		num_rec_op_          = 0;
		len_rec_op_          = 0;
		rec_op_              = CPPAD_NULL;

		num_rec_vecad_ind_   = 0;
		len_rec_vecad_ind_   = 0;
		rec_vecad_ind_       = CPPAD_NULL;

		num_rec_op_arg_      = 0;
		len_rec_op_arg_      = 0;
		rec_op_arg_          = CPPAD_NULL;

		num_rec_par_         = 0;
		len_rec_par_         = 0;
		rec_par_             = CPPAD_NULL;

		num_rec_text_        = 0;
		len_rec_text_        = 0;
		rec_text_            = CPPAD_NULL;

	}

	/// Destructor
	~recorder(void)
	{	if( len_rec_op_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_);
		if( len_rec_vecad_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_vecad_ind_);
		if( len_rec_op_arg_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_arg_);
		if( len_rec_par_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_par_);
		if( len_rec_text_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_text_);
	}

	/*!
	Erase all information in recording.

	Erases the operation sequence store in this recording 
	(the operation sequence is empty after this operation).
	The buffers used to store the current recording are returned
	to the system (so as to conserve on memory).
	*/
	void Erase(void)
	{	
		num_rec_var_          = 0;
		num_rec_op_           = 0;
		num_rec_vecad_ind_    = 0;
		num_rec_op_arg_       = 0;
		num_rec_par_          = 0;
		num_rec_text_         = 0;

		if( len_rec_op_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_);
		if( len_rec_vecad_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_vecad_ind_);
		if( len_rec_op_arg_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_arg_);
		if( len_rec_par_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_par_);
		if( len_rec_text_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_text_);

		len_rec_op_           = 0;
		len_rec_vecad_ind_    = 0;
		len_rec_op_arg_       = 0;
		len_rec_par_          = 0;
		len_rec_text_         = 0;
	}
	/// Start recording the next operator in the operation sequence.
	inline size_t PutOp(OpCode op);
	/// Add a value to the end of the current vector of VecAD indices.
	inline size_t PutVecInd(size_t vec_ind);
	/// Find or add a parameter to the current vector of parameters.
	inline size_t PutPar(const Base &par);
	/// Put one operation argument index in the recording
	inline void PutArg(size_t arg0); 
	/// Put two operation argument index in the recording
	inline void PutArg(size_t arg0, size_t arg1); 
	/// Put three operation argument index in the recording
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2); 
	/// Put four operation argument index in the recording
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2, size_t arg3); 
	/// Put five operation argument index in the recording
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2, size_t arg3,
		size_t arg4);
	/// Put six operation argument index in the recording
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2, size_t arg3,
		size_t arg4, size_t arg5);

	/// Put a character string in the text for this recording.
	inline size_t PutTxt(const char *text);

	/// Number of variables currently stored in the recording.
	size_t num_rec_var(void) const
	{	return num_rec_var_; }

	/// Approximate amount of memory used by the recording 
	size_t Memory(void) const
	{	return len_rec_op_ * sizeof(OpCode) 
		     + len_rec_vecad_ind_ * sizeof(size_t)
		     + len_rec_op_arg_ * sizeof(size_t)
		     + len_rec_par_ * sizeof(Base)
		     + len_rec_text_ * sizeof(char);
	}
};

/*!
Start recording the next operator in the operation sequence.

This sets the op code for the next operation in this recording.
This call must be followed by putting the corresponding 
\verbatim
	NumArg(op)
\endverbatim
argument indices in the recording.

\param op
Is the op code corresponding to the the operation that is being
recorded. 

\return
The return value is the index of the primary (last) variable 
corresponding to the result of this operation. 
The number of variables corresponding to the operation is given by
\verbatim
	NumRes(op)
\endverbatim
With each call to PutOp 
the return index increases by the number of variables corresponding
to this call to PutOp.
This index starts at zero after the default constructor
and after each call to Erase.
*/
template <class Base>
inline size_t recorder<Base>::PutOp(OpCode op)
{
	CPPAD_ASSERT_UNKNOWN( num_rec_op_ <= len_rec_op_ );
	if( num_rec_op_ == len_rec_op_ )
	{	len_rec_op_ = 2 * len_rec_op_ + 8;
		rec_op_ = CPPAD_TRACK_EXTEND(
			len_rec_op_, num_rec_op_, rec_op_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_ < len_rec_op_ );
	rec_op_[num_rec_op_++]  = op;
	num_rec_var_ += NumRes(op);

	// first operator should be a BeginOp and NumRes( BeginOp ) > 0
	CPPAD_ASSERT_UNKNOWN( num_rec_var_ > 0 );

	return num_rec_var_ - 1;
}

/*!
Add a value to the end of the current vector of VecAD indices.

For each VecAD vector, this routine is used to store the length
of the vector followed by the parameter index corresponding to each
value in the vector.
This value for the elements of the VecAD vector corresponds to the
beginning of the operation sequence.

\param vec_ind
is the index to be palced at the end of the vector of VecAD indices.

\return
is the index in the vector of VecAD indices corresponding to this value.
This index starts at zero after the recorder default constructor
and after each call to Erase.
It increments by one for each call to PutVecInd..
*/
template <class Base>
inline size_t recorder<Base>::PutVecInd(size_t vec_ind)
{	
	CPPAD_ASSERT_UNKNOWN( num_rec_vecad_ind_ <= len_rec_vecad_ind_ );
	if( num_rec_vecad_ind_ == len_rec_vecad_ind_ )
	{	len_rec_vecad_ind_ = 2 * len_rec_vecad_ind_ + 8;
		rec_vecad_ind_ = CPPAD_TRACK_EXTEND(
			len_rec_vecad_ind_, num_rec_vecad_ind_, rec_vecad_ind_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_vecad_ind_ < len_rec_vecad_ind_ );
	rec_vecad_ind_[num_rec_vecad_ind_++] = vec_ind;

	return num_rec_vecad_ind_ - 1;
}

/*!
Find or add a parameter to the current vector of parameters.

\param par
is the parameter to be found or placed in the vector of parameters.

\return
is the index in the parameter vector corresponding to this parameter value.
This value is not necessarily placed at the end of the vector
(because values that are identically equal may be reused).
*/
template <class Base>
size_t recorder<Base>::PutPar(const Base &par)
{	static size_t   hash_table[CPPAD_HASH_TABLE_SIZE];
	static bool     init = true;
	size_t          i;
	unsigned short  code;
	CPPAD_ASSERT_UNKNOWN( num_rec_par_ <= len_rec_par_ );

	if( init )
	{	// initialize hash table
		for(i = 0; i < CPPAD_HASH_TABLE_SIZE; i++)
			hash_table[i] = 0;
		init = false;
	}
	
	// get hash code for this value
	code = hash_code(par);

	// If we have a match, return the parameter index
	i = hash_table[code];
	if( i < num_rec_par_ )
	{	if( IdenticalEqualPar(rec_par_[i], par) )
			return i;
	}
	
	// place a new value in the table
	if( num_rec_par_ == len_rec_par_ )
	{	len_rec_par_ = 2 * len_rec_par_ + 8;
		rec_par_ = CPPAD_TRACK_EXTEND(
			len_rec_par_, num_rec_par_, rec_par_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_par_ < len_rec_par_ );
	i           = num_rec_par_++;
	rec_par_[i] = par;

	// make the hash code point to this new value
	hash_table[code] = i;

	// return the parameter index
	return i;
}
// -------------------------- PutArg --------------------------------------
/*!
Prototype for putting operation argument indices in the recording.

The following syntax
\verbatim
	rec.PutArg(arg0)
	rec.PutArg(arg0, arg1)
	.
	.
	.
	rec.PutArg(arg0, arg1, ..., arg5)
\endverbatim
places the values passed to PutArg at the current end of the
operation argument indices for the recording.
\a arg0 comes before \a arg1, etc.
The proper number of operation argument indices 
corresponding to the operation code op is given by
\verbatim
	NumArg(op)
\endverbatim
The number of the operation argument indices starts at zero
after the default constructor and each call to Erase.
It increases by the number of indices placed by each call to PutArg.
*/
inline void prototype_put_arg(void)
{	// This routine should not be called
	CPPAD_ASSERT_UNKNOWN(false);
}
/*!
Put one operation argument index in the recording

\param arg0
The operation argument index

\copydetails prototype_put_arg 
*/
template <class Base>
inline void recorder<Base>::PutArg(size_t arg0)
{ 
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ <= len_rec_op_arg_ );
	if( num_rec_op_arg_ == len_rec_op_arg_ )
	{	len_rec_op_arg_ = 2 * len_rec_op_arg_ + 8;
		rec_op_arg_ = CPPAD_TRACK_EXTEND(
			len_rec_op_arg_, num_rec_op_arg_, rec_op_arg_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ < len_rec_op_arg_ );
	rec_op_arg_[num_rec_op_arg_++] = arg0;
}
/*!
Put two operation argument index in the recording

\param arg0
First operation argument index.

\param arg1
Second operation argument index.

\copydetails prototype_put_arg 
*/
template <class Base>
inline void recorder<Base>::PutArg(size_t arg0, size_t arg1)
{ 
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ <= len_rec_op_arg_ );
	if( num_rec_op_arg_ + 1 >= len_rec_op_arg_ )
	{	len_rec_op_arg_ = 2 * len_rec_op_arg_ + 8;
		rec_op_arg_ = CPPAD_TRACK_EXTEND(
			len_rec_op_arg_, num_rec_op_arg_, rec_op_arg_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ + 1 < len_rec_op_arg_ );
	rec_op_arg_[num_rec_op_arg_++] = arg0;
	rec_op_arg_[num_rec_op_arg_++] = arg1;
}
/*!
Put three operation argument index in the recording

\param arg0
First operation argument index.

\param arg1
Second operation argument index.

\param arg2
Third operation argument index.

\copydetails prototype_put_arg 
*/
template <class Base>
inline void recorder<Base>::PutArg(size_t arg0, size_t arg1, size_t arg2)
{ 
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ <= len_rec_op_arg_ );
	if( num_rec_op_arg_ + 2 >= len_rec_op_arg_ )
	{	len_rec_op_arg_ = 2 * len_rec_op_arg_ + 8;
		rec_op_arg_ = CPPAD_TRACK_EXTEND(
			len_rec_op_arg_, num_rec_op_arg_, rec_op_arg_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ + 2 < len_rec_op_arg_ );
	rec_op_arg_[num_rec_op_arg_++] = arg0;
	rec_op_arg_[num_rec_op_arg_++] = arg1;
	rec_op_arg_[num_rec_op_arg_++] = arg2;
}
/*!
Put four operation argument index in the recording

\param arg0
First operation argument index.

\param arg1
Second operation argument index.

\param arg2
Third operation argument index.

\param arg3
Fourth operation argument index.

\copydetails prototype_put_arg 
*/
template <class Base>
inline void recorder<Base>::PutArg(size_t arg0, size_t arg1, size_t arg2,
	size_t arg3)
{ 
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ <= len_rec_op_arg_ );
	if( num_rec_op_arg_ + 3 >= len_rec_op_arg_ )
	{	len_rec_op_arg_ = 2 * len_rec_op_arg_ + 8;
		rec_op_arg_ = CPPAD_TRACK_EXTEND(
			len_rec_op_arg_, num_rec_op_arg_, rec_op_arg_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ + 3 < len_rec_op_arg_ );
	rec_op_arg_[num_rec_op_arg_++] = arg0;
	rec_op_arg_[num_rec_op_arg_++] = arg1;
	rec_op_arg_[num_rec_op_arg_++] = arg2;
	rec_op_arg_[num_rec_op_arg_++] = arg3;

}
/*!
Put five operation argument index in the recording

\param arg0
First operation argument index.

\param arg1
Second operation argument index.

\param arg2
Third operation argument index.

\param arg3
Fourth operation argument index.

\param arg4
Fifth operation argument index.

\copydetails prototype_put_arg 
*/
template <class Base>
inline void recorder<Base>::PutArg(size_t arg0, size_t arg1, size_t arg2,
	size_t arg3, size_t arg4)
{ 
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ <= len_rec_op_arg_ );
	if( num_rec_op_arg_ + 4 >= len_rec_op_arg_ )
	{	len_rec_op_arg_ = 2 * len_rec_op_arg_ + 8;
		rec_op_arg_ = CPPAD_TRACK_EXTEND(
			len_rec_op_arg_, num_rec_op_arg_, rec_op_arg_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ + 4 < len_rec_op_arg_ );
	rec_op_arg_[num_rec_op_arg_++] = arg0;
	rec_op_arg_[num_rec_op_arg_++] = arg1;
	rec_op_arg_[num_rec_op_arg_++] = arg2;
	rec_op_arg_[num_rec_op_arg_++] = arg3;
	rec_op_arg_[num_rec_op_arg_++] = arg4;

}
/*!
Put six operation argument index in the recording

\param arg0
First operation argument index.

\param arg1
Second operation argument index.

\param arg2
Third operation argument index.

\param arg3
Fourth operation argument index.

\param arg4
Fifth operation argument index.

\param arg5
Sixth operation argument index.

\copydetails prototype_put_arg 
*/
template <class Base>
inline void recorder<Base>::PutArg(size_t arg0, size_t arg1, size_t arg2, 
	size_t arg3, size_t arg4, size_t arg5)
{ 
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ <= len_rec_op_arg_ );
	if( num_rec_op_arg_ + 5 >= len_rec_op_arg_ )
	{	len_rec_op_arg_ = 2 * len_rec_op_arg_ + 8;
		rec_op_arg_ = CPPAD_TRACK_EXTEND(
			len_rec_op_arg_, num_rec_op_arg_, rec_op_arg_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_op_arg_ + 5 < len_rec_op_arg_ );
	rec_op_arg_[num_rec_op_arg_++] = arg0;
	rec_op_arg_[num_rec_op_arg_++] = arg1;
	rec_op_arg_[num_rec_op_arg_++] = arg2;
	rec_op_arg_[num_rec_op_arg_++] = arg3;
	rec_op_arg_[num_rec_op_arg_++] = arg4;
	rec_op_arg_[num_rec_op_arg_++] = arg5;
}
// --------------------------------------------------------------------------
/*!
Put a character string in the text for this recording.

\param text
is a '\\0' terminated character string that is to be put in the
vector of characters corresponding to this recording.
The terminator '\\0' will be included.

\return
is the offset with in the text vector for this recording at which
the character string starts.
*/
template <class Base>
inline size_t recorder<Base>::PutTxt(const char *text)
{	size_t i;

	// determine length of the text including terminating '\0'
	size_t n;
	for(n = 0; text[n] != '\0'; n++)
		CPPAD_ASSERT_UNKNOWN( n < 1000 ); // should check in PrintFor
	n++;

	CPPAD_ASSERT_UNKNOWN( num_rec_text_ <= len_rec_text_ );

	if( num_rec_text_ + n >= len_rec_text_ )
	{	len_rec_text_  = 2 * len_rec_text_ + n + 8;
		rec_text_ = CPPAD_TRACK_EXTEND(
			len_rec_text_, num_rec_text_, rec_text_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_text_ + n < len_rec_text_ );

	// copy text including terminating '\0'
	for(i = 0; i < n; i++)
		rec_text_[num_rec_text_++] = text[i];
	CPPAD_ASSERT_UNKNOWN( text[i-1] == '\0' );

	return num_rec_text_ - n;
}
// -------------------------------------------------------------------------


CPPAD_END_NAMESPACE
# endif
