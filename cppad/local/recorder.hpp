/* $Id$ */
# ifndef CPPAD_RECORDER_INCLUDED
# define CPPAD_RECORDER_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin recorder$$ $comment CppAD Developer Documentation$$

$spell
	var
	Res
	Arg
	inline
	VecInd
	sizeof
	Num
	Cpp
	const
	Op
$$

$section Record a CppAD Operation Sequence$$
$index tape, record$$
$index record, tape$$
$index recorder$$

$head Syntax$$
$codei%recorder<%Base%> %rec%$$


$head Default Constructors$$
The default constructor 
$codei%
	recorder<%Base%> %rec%
%$$
creates an empty operation sequence.

$head Erase$$
$index recorder, Erase$$
$index Erase, recorder$$
The syntax 
$codei%
	void %rec%.Erase()
%$$
erases the operation sequence store in $icode rec$$
(the operation sequence is empty after this operation).
The buffers used to store the tape information are returned
to the system (so as to conserve on memory).

$head PutOp$$
$index PutOp$$
If $icode op$$ and $icode i$$ have prototypes
$codei%
        OpCode %op%
        size_t %i%
%$$
The syntax
$codei%
	%i% = %rec%.PutOp(%op%)
%$$
sets the code for the next operation in the sequence.
The return value $icode i$$ is the index of the first variable 
corresponding to the result of this operation. 
The number of variables $icode n$$ 
corresponding to the operation is given by
$codei%
	%n% = NumRes(%op%)
%$$
where $icode n$$ is a $code size_t$$ object.
With each call to $code PutOp$$, 
the return index increases by the number of variables corresponding
to the previous call to $code PutOp$$.
This index starts at zero after each $code Erase$$ or default constructor.

$head PutArg$$
$index PutArg$$
If $icode arg_j$$ has prototype
$codei%
	size_t %arg_j%
%$$
for $icode j$$ equal to $icode 0$$, ... , $icode 5$$,
The following syntax
$codei%
	%rec%.PutArg(%arg_0%)
	%rec%.PutArg(%arg_0%, %arg_1%)
	%.%
	%.%
	%.%
	%rec%.PutArg(%arg_0%, %arg_1%, %...%, %arg_5%)
%$$
places the values passed to $codei PutArg$$ at the current end of the
operation sequence index vector in the specified order, i.e., 
$icode arg_0$$ comes before $icode arg_1$$ e.t.c.
The proper number of indices $icode n$$ 
corresponding to the operation $icode op$$ is given by
$codei%
	%n% = NumRes(%op%)
%$$
where $icode n$$ is a $code size_t$$ object and $icode op$$
is an $code OpCode$$ object.
The end of the operation sequence index vector starts at zero
and increases by the number of indices placed in the vector
by each call to $code PutArg$$.
The end of the vector starts at zero after each $code Erase$$ 
or default constructor. 

$head PutPar$$
$index PutPar$$
If $icode p$$ and $icode i$$ have prototypes
$codei%
	const %Base% &%p%
	size_t %i%
%$$
The syntax
$codei%
	%i% = %rec%.PutPar(%p%)
%$$
places the value $icode p$$ in the 
operation sequence parameter vector
and returns its index in the vector $icode i$$.
This value is not necessarily placed at the end of the vector
(because values that are identically equal can be reused).

$head PutVecInd$$
$index PutVecInd$$
If $icode i$$ and $icode iv$$ have prototypes
$codei%
	size_t %i%
	size_t %iv%
%$$
the syntax
$codei%
	%i% = %rec%.PutVecInd(%iv%)
%$$
places the value $icode iv$$ at the current end of the
operation sequence vec_ind vector
and returns its index in this vector.
This index starts at zero after each $code Erase$$ or default constructor
and increments by one for each call to this function.

$head num_rec_var$$
$index recorder, num_rec_var$$
$index num_rec_var, recorder$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	%n% = %rec%.num_rec_var()
%$$
sets $icode n$$ to the number of variables that are in the 
operation sequence.


$head Memory$$
$index recorder, Memory$$
$index Memory, recorder$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	 %n% = %rec%.Memory()
%$$
sets $icode n$$ to the number of memory units ($code sizeof$$) 
required to store the current operation sequence in $icode rec$$.


$end
------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class recorder {

	friend class player<Base>;

public:
	// default
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

	// destructor
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

	// erase all information in recording
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

	// add information to recording
	inline size_t PutOp(OpCode op);
	inline size_t PutVecInd(size_t vecInd);
	inline size_t PutPar(const Base &par);
	inline void PutArg(size_t arg0); 
	inline void PutArg(size_t arg0, size_t arg1); 
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2); 
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2, size_t arg3); 
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2, size_t arg3,
		size_t arg4);
	inline void PutArg(size_t arg0, size_t arg1, size_t arg2, size_t arg3,
		size_t arg4, size_t arg5);

	inline size_t PutTxt(const char *text);

	// number of values
	size_t num_rec_var(void) const
	{	return num_rec_var_; }

	// amount of memory used 
	size_t Memory(void) const
	{	return len_rec_op_ * sizeof(OpCode) 
		     + len_rec_vecad_ind_ * sizeof(size_t)
		     + len_rec_op_arg_ * sizeof(size_t)
		     + len_rec_par_ * sizeof(Base)
		     + len_rec_text_ * sizeof(char);
	}

private:
	size_t    num_rec_var_;

	size_t    num_rec_op_;
	size_t    len_rec_op_;
	OpCode   *rec_op_;

	size_t    num_rec_vecad_ind_;
	size_t    len_rec_vecad_ind_;
	size_t   *rec_vecad_ind_;

	size_t    num_rec_op_arg_;
	size_t    len_rec_op_arg_;
	size_t   *rec_op_arg_;

	size_t    num_rec_par_;
	size_t    len_rec_par_;
	Base     *rec_par_;

	size_t    num_rec_text_;
	size_t    len_rec_text_;
	char     *rec_text_;
};

template <class Base>
inline size_t recorder<Base>::PutOp(OpCode op)
{	size_t varIndex = num_rec_var_;
	
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

	return varIndex;
}

template <class Base>
inline size_t recorder<Base>::PutVecInd(size_t vecInd)
{	
	CPPAD_ASSERT_UNKNOWN( num_rec_vecad_ind_ <= len_rec_vecad_ind_ );
	if( num_rec_vecad_ind_ == len_rec_vecad_ind_ )
	{	len_rec_vecad_ind_ = 2 * len_rec_vecad_ind_ + 8;
		rec_vecad_ind_ = CPPAD_TRACK_EXTEND(
			len_rec_vecad_ind_, num_rec_vecad_ind_, rec_vecad_ind_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_vecad_ind_ < len_rec_vecad_ind_ );
	rec_vecad_ind_[num_rec_vecad_ind_++] = vecInd;

	return num_rec_vecad_ind_ - 1;
}

template <class Base>
inline size_t recorder<Base>::PutPar(const Base &par)
{	size_t i;
	
	CPPAD_ASSERT_UNKNOWN( num_rec_par_ <= len_rec_par_ );
	
	// check last three values to see if same one came up
	if( num_rec_par_ >= 3 )
	{	i = num_rec_par_;
		while(i > num_rec_par_ - 3)
		{	--i;
			if( IdenticalEqualPar(rec_par_[i], par) )
				return i;
		}
	}
	
	// place a new value in the table
	if( num_rec_par_ == len_rec_par_ )
	{	len_rec_par_ = 2 * len_rec_par_ + 8;
		rec_par_ = CPPAD_TRACK_EXTEND(
			len_rec_par_, num_rec_par_, rec_par_
		);
	}
	CPPAD_ASSERT_UNKNOWN( num_rec_par_ < len_rec_par_ );
	rec_par_[num_rec_par_++] = par;

	return num_rec_par_ - 1;
}
 // -------------------------- PutArg --------------------------------------
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

} // END CppAD namespace

# endif
