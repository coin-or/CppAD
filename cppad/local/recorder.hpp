# ifndef CPPAD_RECORDER_INCLUDED
# define CPPAD_RECORDER_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin recorder$$ $comment CppAD Developer Documentation$$

$spell
	Var
	inline
	VecInd
	sizeof
	Num
	Ind
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
	%n% = NumVar(%op%)
%$$
where $icode n$$ is a $code size_t$$ object.
With each call to $code PutOp$$, 
the return index increases by the number of variables corresponding
to the previous call to $code PutOp$$.
This index starts at zero after each $code Erase$$ or default constructor.

$head PutInd$$
$index PutInd$$
If $icode ind_j$$ has prototype
$codei%
	size_t %ind_j%
%$$
for $icode j$$ equal to $icode 0$$, ... , $icode 5$$,
The following syntax
$codei%
	%rec%.PutInd(%ind_0%)
	%rec%.PutInd(%ind_0%, %ind_1%)
	%.%
	%.%
	%.%
	%rec%.PutInd(%ind_0%, %ind_1%, %...%, %ind_5%)
%$$
places the values passed to $codei PutInd$$ at the current end of the
operation sequence index vector in the specified order, i.e., 
$icode ind_0$$ comes before $icode ind_1$$ e.t.c.
The proper number of indices $icode n$$ 
corresponding to the operation $icode op$$ is given by
$codei%
	%n% = NumVar(%op%)
%$$
where $icode n$$ is a $code size_t$$ object and $icode op$$
is an $code OpCode$$ object.
The end of the operation sequence index vector starts at zero
and increases by the number of indices placed in the vector
by each call to $code PutInd$$.
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

$head TotNumVar$$
$index recorder, TotNumVar$$
$index TotNumVar, recorder$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	%n% = %rec%.TotNumVar()
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
		TotalNumberVar_ = 0;

		NumberOp_       = 0;
		LengthOp_       = 0;
		Op_             = CPPAD_NULL;

		NumberVecInd_   = 0;
		LengthVecInd_   = 0;
		VecInd_         = CPPAD_NULL;

		NumberInd_      = 0;
		LengthInd_      = 0;
		Ind_            = CPPAD_NULL;

		NumberPar_      = 0;
		LengthPar_      = 0;
		Par_            = CPPAD_NULL;

		NumberTxt_      = 0;
		LengthTxt_      = 0;
		Txt_            = CPPAD_NULL;

	}

	// destructor
	~recorder(void)
	{	if( LengthOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( LengthVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( LengthInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( LengthPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( LengthTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);
	}

	// erase all information in recording
	void Erase(void)
	{	
		TotalNumberVar_  = 0;
		NumberOp_        = 0;
		NumberVecInd_    = 0;
		NumberInd_       = 0;
		NumberPar_       = 0;
		NumberTxt_       = 0;

		if( LengthOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( LengthVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( LengthInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( LengthPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( LengthTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);

		LengthOp_        = 0;
		LengthVecInd_    = 0;
		LengthInd_       = 0;
		LengthPar_       = 0;
		LengthTxt_       = 0;
	}

	// add information to recording
	inline size_t PutOp(OpCode op);
	inline size_t PutVecInd(size_t vecInd);
	inline size_t PutPar(const Base &par);
	inline void PutInd(size_t ind0); 
	inline void PutInd(size_t ind0, size_t ind1); 
	inline void PutInd(size_t ind0, size_t ind1, size_t ind2); 
	inline void PutInd(size_t ind0, size_t ind1, size_t ind2, size_t ind3); 
	inline void PutInd(size_t ind0, size_t ind1, size_t ind2, size_t ind3,
		size_t ind4);
	inline void PutInd(size_t ind0, size_t ind1, size_t ind2, size_t ind3,
		size_t ind4, size_t ind5);

	inline size_t PutTxt(const char *text);

	// number of values
	size_t TotNumVar(void) const
	{	return TotalNumberVar_; }

	// amount of memory used 
	size_t Memory(void) const
	{	return LengthOp_ * sizeof(OpCode) 
		     + LengthVecInd_ * sizeof(size_t)
		     + LengthInd_ * sizeof(size_t)
		     + LengthPar_ * sizeof(Base)
		     + LengthTxt_ * sizeof(char);
	}

private:
	size_t    TotalNumberVar_;

	size_t    NumberOp_;
	size_t    LengthOp_;
	OpCode   *Op_;

	size_t    NumberVecInd_;
	size_t    LengthVecInd_;
	size_t   *VecInd_;

	size_t    NumberInd_;
	size_t    LengthInd_;
	size_t   *Ind_;

	size_t    NumberPar_;
	size_t    LengthPar_;
	Base     *Par_;

	size_t    NumberTxt_;
	size_t    LengthTxt_;
	char     *Txt_;
};

template <class Base>
inline size_t recorder<Base>::PutOp(OpCode op)
{	size_t varIndex = TotalNumberVar_;
	
	CPPAD_ASSERT_UNKNOWN( NumberOp_ <= LengthOp_ );
	if( NumberOp_ == LengthOp_ )
	{	LengthOp_ = 2 * LengthOp_ + 8;
		Op_ = CPPAD_TRACK_EXTEND(LengthOp_, NumberOp_, Op_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberOp_ < LengthOp_ );
	Op_[NumberOp_++]  = op;
	TotalNumberVar_ += NumVar(op);

	return varIndex;
}

template <class Base>
inline size_t recorder<Base>::PutVecInd(size_t vecInd)
{	
	CPPAD_ASSERT_UNKNOWN( NumberVecInd_ <= LengthVecInd_ );
	if( NumberVecInd_ == LengthVecInd_ )
	{	LengthVecInd_ = 2 * LengthVecInd_ + 8;
		VecInd_ = CPPAD_TRACK_EXTEND(LengthVecInd_, NumberVecInd_, VecInd_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberVecInd_ < LengthVecInd_ );
	VecInd_[NumberVecInd_++] = vecInd;

	return NumberVecInd_ - 1;
}

template <class Base>
inline size_t recorder<Base>::PutPar(const Base &par)
{	size_t i;
	
	CPPAD_ASSERT_UNKNOWN( NumberPar_ <= LengthPar_ );
	
	// check last three values to see if same one came up
	if( NumberPar_ >= 3 )
	{	i = NumberPar_;
		while(i > NumberPar_ - 3)
		{	--i;
			if( IdenticalEqualPar(Par_[i], par) )
				return i;
		}
	}
	
	// place a new value in the table
	if( NumberPar_ == LengthPar_ )
	{	LengthPar_ = 2 * LengthPar_ + 8;
		Par_ = CPPAD_TRACK_EXTEND(LengthPar_, NumberPar_, Par_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberPar_ < LengthPar_ );
	Par_[NumberPar_++] = par;

	return NumberPar_ - 1;
}
 // -------------------------- PutInd --------------------------------------
template <class Base>
inline void recorder<Base>::PutInd(size_t ind0)
{ 
	CPPAD_ASSERT_UNKNOWN( NumberInd_ <= LengthInd_ );
	if( NumberInd_ == LengthInd_ )
	{	LengthInd_ = 2 * LengthInd_ + 8;
		Ind_ = CPPAD_TRACK_EXTEND(LengthInd_, NumberInd_, Ind_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberInd_ < LengthInd_ );
	Ind_[NumberInd_++] = ind0;
}
template <class Base>
inline void recorder<Base>::PutInd(size_t ind0, size_t ind1)
{ 
	CPPAD_ASSERT_UNKNOWN( NumberInd_ <= LengthInd_ );
	if( NumberInd_ + 1 >= LengthInd_ )
	{	LengthInd_ = 2 * LengthInd_ + 8;
		Ind_ = CPPAD_TRACK_EXTEND(LengthInd_, NumberInd_, Ind_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberInd_ + 1 < LengthInd_ );
	Ind_[NumberInd_++] = ind0;
	Ind_[NumberInd_++] = ind1;
}
template <class Base>
inline void recorder<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2)
{ 
	CPPAD_ASSERT_UNKNOWN( NumberInd_ <= LengthInd_ );
	if( NumberInd_ + 2 >= LengthInd_ )
	{	LengthInd_ = 2 * LengthInd_ + 8;
		Ind_ = CPPAD_TRACK_EXTEND(LengthInd_, NumberInd_, Ind_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberInd_ + 2 < LengthInd_ );
	Ind_[NumberInd_++] = ind0;
	Ind_[NumberInd_++] = ind1;
	Ind_[NumberInd_++] = ind2;
}
template <class Base>
inline void recorder<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2,
	size_t ind3)
{ 
	CPPAD_ASSERT_UNKNOWN( NumberInd_ <= LengthInd_ );
	if( NumberInd_ + 3 >= LengthInd_ )
	{	LengthInd_ = 2 * LengthInd_ + 8;
		Ind_ = CPPAD_TRACK_EXTEND(LengthInd_, NumberInd_, Ind_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberInd_ + 3 < LengthInd_ );
	Ind_[NumberInd_++] = ind0;
	Ind_[NumberInd_++] = ind1;
	Ind_[NumberInd_++] = ind2;
	Ind_[NumberInd_++] = ind3;

}
template <class Base>
inline void recorder<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2,
	size_t ind3, size_t ind4)
{ 
	CPPAD_ASSERT_UNKNOWN( NumberInd_ <= LengthInd_ );
	if( NumberInd_ + 4 >= LengthInd_ )
	{	LengthInd_ = 2 * LengthInd_ + 8;
		Ind_ = CPPAD_TRACK_EXTEND(LengthInd_, NumberInd_, Ind_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberInd_ + 4 < LengthInd_ );
	Ind_[NumberInd_++] = ind0;
	Ind_[NumberInd_++] = ind1;
	Ind_[NumberInd_++] = ind2;
	Ind_[NumberInd_++] = ind3;
	Ind_[NumberInd_++] = ind4;

}
template <class Base>
inline void recorder<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2, 
	size_t ind3, size_t ind4, size_t ind5)
{ 
	CPPAD_ASSERT_UNKNOWN( NumberInd_ <= LengthInd_ );
	if( NumberInd_ + 5 >= LengthInd_ )
	{	LengthInd_ = 2 * LengthInd_ + 8;
		Ind_ = CPPAD_TRACK_EXTEND(LengthInd_, NumberInd_, Ind_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberInd_ + 5 < LengthInd_ );
	Ind_[NumberInd_++] = ind0;
	Ind_[NumberInd_++] = ind1;
	Ind_[NumberInd_++] = ind2;
	Ind_[NumberInd_++] = ind3;
	Ind_[NumberInd_++] = ind4;
	Ind_[NumberInd_++] = ind5;
}

template <class Base>
inline size_t recorder<Base>::PutTxt(const char *text)
{	size_t i;

	// determine length of the text including terminating '\0'
	size_t n;
	for(n = 0; text[n] != '\0'; n++)
		CPPAD_ASSERT_UNKNOWN( n < 1000 ); // should check in PrintFor
	n++;

	CPPAD_ASSERT_UNKNOWN( NumberTxt_ <= LengthTxt_ );

	if( NumberTxt_ + n >= LengthTxt_ )
	{	LengthTxt_  = 2 * LengthTxt_ + n + 8;
		Txt_ = CPPAD_TRACK_EXTEND(LengthTxt_, NumberTxt_, Txt_);
	}
	CPPAD_ASSERT_UNKNOWN( NumberTxt_ + n < LengthTxt_ );

	// copy text including terminating '\0'
	for(i = 0; i < n; i++)
		Txt_[NumberTxt_++] = text[i];
	CPPAD_ASSERT_UNKNOWN( text[i-1] == '\0' );

	return NumberTxt_ - n;
}

} // END CppAD namespace

# endif
