# ifndef CPPAD_TAPE_REC_INCLUDED
# define CPPAD_TAPE_REC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin TapeRec$$ $comment CppAD Developer Documentation$$

$spell
	inline
	VecInd
	sizeof
	Num
	Ind
	Cpp
	Rec
	const
	Op
$$

$section A CppAD Program as Recorded on Tape$$
$index tape, record$$
$index record, tape$$
$index TapeRec$$

$head Syntax$$
$syntax%TapeRec<%Base%> %Rec%$$
$pre
$$
$syntax%TapeRec<%Base%> %Rec%(const TapeRec<%Base%> &%Other%)%$$


$head Default Constructors$$
The default constructor 
$syntax%
	TapeRec<%Base%> %Rec%
%$$
creates a program recording called $italic Rec$$ with no contents and some
default setting for the size of its buffers.

$head Copy Constructor$$
The copy constructor
$syntax%
	TapeRec<%Base%> %Rec%(const TapeRec<%Base%> &%Other%)
%$$
creates $italic Rec$$ as a program recording with all the same
information as $italic Other$$ and with the smallest possible buffer sizes
that will hold that information.

$head Erase$$
$index TapeRec, Erase$$
$index Erase, TapeRec$$
The syntax 
$syntax%
	void %Rec%.Erase()
%$$
erases the contents of $italic Rec$$.
The buffers used to store the tape information are returned
to the system (so as to conserve on memory).

$head Put$$
$index TapeRec, Put$$
$index Put, TapeRec$$

$subhead Op$$
$index PutOp$$
The function call
$syntax%
	inline size_t %Rec%.PutOp(OpCode %op%)
%$$
places the value $italic op$$ at the end of the current Op recording
and returns the offset for the corresponding variable 
(if the operator has a resulting variable).
With each call, this index increments by the number of variables required
for the previous call to $code PutOp$$.

$subhead Ind$$
$index PutInd$$
The function call
$syntax%
	inline void %Rec%.PutInd(size_t %ind0%)
	inline void %Rec%.PutInd(size_t %ind0%, size_t %ind1%)
	%.%
	%.%
	%.%
	inline void %Rec%.PutInd(
		size_t %ind0%, 
		size_t %ind1%, 
		size_t %ind2%, 
		size_t %ind3%,
		size_t %ind4%
		size_t %ind5%)
%$$
places the values passed to $code PutInd$$ at the end of the
Ind recording and in the order passed; i.e., $italic ind0$$
comes before $italic ind1$$ e.t.c.


$subhead Par$$
$index PutPar$$
The function call
$syntax%
	inline size_t %Rec%.PutPar(const %Base% &%par%)
%$$
places the value $italic par$$ in the Par recording
and returns its index with in the recording.
This value is not necessarily placed at the end of the recording
so there is no specified pattern to the return values.

$subhead VecInd$$
$index PutVecInd$$
The function call
$syntax%
	inline size_t %Rec%.PutVecInd(size_t %vecInd%)
%$$
places the value $italic vecInd$$ at the end of the current VecInd recording
and returns its index with in the recording.
This index starts at zero after each $code Erase$$ or default constructor
and increments by one for each call to this function.

$head Get$$
$index TapeRec, Get$$
$index Get, TapeRec$$

$subhead Op$$
$index GetOp$$
The syntax
$syntax%
	OpCode %Rec%.GetOp(size_t %i%) const
%$$
returns the value of $italic op$$ in the $th i+1$$ call to 
$syntax%%Rec%.PutOp(%op%)%$$.

$subhead VecInd$$
$index GetVecInd$$
The syntax
$syntax%
	OpCode %Rec%.GetVecInd(size_t %i%) const
%$$
returns the value of $italic vecInd$$ in the corresponding call 
$syntax%%Rec%.PutVecInd(%vecInd%)%$$
where $italic i$$ is the return value of $code PutVecInd$$. 

$subhead Ind$$
$index GetInd$$
The syntax
$syntax%
	const size_t *%Rec%.GetInd(size_t %n%, size_t %i%) const
%$$
returns a pointer to a copy of $italic n$$ values in the Ind recording
starting at the index $italic i$$.

$subhead Par$$
$index GetPar$$
The syntax
$syntax%
	const %Base% *%Rec%.GetPar(size_t %i%) const
%$$
returns a pointer to a value equal to
$italic par$$ in the corresponding call 
$syntax%%Rec%.PutPar(%par%)%$$ 
where $italic i$$ is the return value of $code PutPar$$.
(If $code NDEBUG$$ is not defined, $code GetPar$$ checks that
the argument $italic i$$ is valid.)

$head Num$$
$index TapeRec, Num$$
$index Num, TapeRec$$

$subhead Op$$
$index NumOp$$
The syntax
$syntax%
	size_t %Rec%.NumOp(void) const
%$$
returns the number of Op values
that are currently stored in $italic Rec$$.
This increments by one each time $code PutOp$$ is called; i.e.,
it is the number of calls to $code PutOp$$.

$subhead Ind$$
$index NumInd$$
The syntax
$syntax%
	size_t %Rec%.NumInd(void) const
%$$
returns the number of Ind values
that are currently stored in $italic Rec$$.
This increments by one for each value that is stored by $code PutInd$$.

The syntax
$syntax%
	size_t %Rec%.NumVecInd(void) const
%$$
returns the number of VecInd values
that are currently stored in $italic Rec$$.
This increments by one each time $code PutVecInd$$ is called.

$subhead Par$$
$index NumPar$$
The syntax
$syntax%
	size_t %Rec%.NumPar(void) const
%$$
returns the number of Par values
that are currently stored in $italic Rec$$.
This increment by one or zero each time $code PutPar$$ is called.

$head Replace$$
$index TapeRec, Replace$$
$index Replace, TapeRec$$

$subhead Ind$$
$index ReplaceInd$$
The syntax
$syntax%
	size_t %Rec%.ReplaceInd(size_t %index%, size_t %value%)
%$$
Replaces the single value with index $italic index$$ in the sequence of
Ind values stored by calls to $syntax%%Rec%.PutInd%$$.
The argument $italic index$$ must be less than $syntax%%Rec%.NumInd()%$$.


$head Memory$$
$index TapeRec, Memory$$
$index Memory, TapeRec$$
The syntax
$syntax%
	size_t %Rec%.Memory(void) const
%$$
returns the number of memory units ($code sizeof$$) required to store
the information in $italic Rec$$.



$end
------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
class TapeRec {

public:
	// default
	TapeRec(void) 
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
	~TapeRec(void)
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

	// assignment from another recording
	void operator=(const TapeRec &Other)
	{	size_t i;

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

		// Var
		TotalNumberVar_  = Other.TotalNumberVar_;

		// Op
		NumberOp_        = Other.NumberOp_;
		LengthOp_        = Other.NumberOp_;

		// VecInd
		NumberVecInd_    = Other.NumberVecInd_;
		LengthVecInd_    = Other.NumberVecInd_;

		// Ind
		NumberInd_       = Other.NumberInd_;
		LengthInd_       = Other.NumberInd_;

		// Par
		NumberPar_       = Other.NumberPar_;
		LengthPar_       = Other.NumberPar_;

		// Txt
		NumberTxt_       = Other.NumberTxt_;
		LengthTxt_       = Other.NumberTxt_;

		// Allocate the memory
		if( LengthOp_ == 0 )
			Op_ = CPPAD_NULL;
		else	Op_ = CPPAD_TRACK_NEW_VEC(LengthOp_,      Op_);
		if( LengthVecInd_ == 0 )
			VecInd_ = CPPAD_NULL;
		else	VecInd_ = CPPAD_TRACK_NEW_VEC(LengthVecInd_, VecInd_);
		if( LengthInd_ == 0 )
			Ind_ = CPPAD_NULL;
		else	Ind_ = CPPAD_TRACK_NEW_VEC(LengthInd_,       Ind_);
		if( LengthPar_ == 0 )
			Par_ = CPPAD_NULL;
		else	Par_ = CPPAD_TRACK_NEW_VEC(LengthPar_,       Par_);
		if( LengthTxt_ == 0 )
			Txt_ = CPPAD_NULL;
		else	Txt_ = CPPAD_TRACK_NEW_VEC(LengthTxt_,       Txt_);

		// Copy the data
		i = NumberOp_;
		while(i--)
			Op_[i] = Other.Op_[i];
		i = NumberVecInd_;
		while(i--)
			VecInd_[i] = Other.VecInd_[i];
		i = NumberInd_;
		while(i--)
			Ind_[i] = Other.Ind_[i];
		i = NumberPar_;
		while(i--)
			Par_[i] = Other.Par_[i];
		i = NumberTxt_;
		while(i--)
			Txt_[i] = Other.Txt_[i];
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

	/*
	retrieve information from recording
	*/
	OpCode GetOp (size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberOp_);
		return Op_[i];
	}
	size_t GetVecInd (size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberVecInd_);
		return VecInd_[i];
	}
	const Base *GetPar(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberPar_);
		return Par_ + i;
	}
	const size_t *GetInd(size_t n, size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i + n <= NumberInd_)
		return Ind_ + i;
	}
	const char *GetTxt(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberTxt_);
		return Txt_ + i;
	}
	
	/*
	replace information 
	*/
	void ReplaceInd(size_t i, size_t value)
	{	CPPAD_ASSERT_UNKNOWN( i < NumberInd_);
		Ind_[i] = value;
	}

	// number of values
	size_t TotNumVar(void) const
	{	return TotalNumberVar_; }
	size_t NumOp(void) const
	{	return NumberOp_; }
	size_t NumVecInd(void) const
	{	return NumberVecInd_; }
	size_t NumInd(void) const
	{	return NumberInd_; }
	size_t NumPar(void) const
	{	return NumberPar_; }
	size_t NumTxt(void) const
	{	return NumberTxt_; }

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
inline size_t TapeRec<Base>::PutOp(OpCode op)
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
inline size_t TapeRec<Base>::PutVecInd(size_t vecInd)
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
inline size_t TapeRec<Base>::PutPar(const Base &par)
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
inline void TapeRec<Base>::PutInd(size_t ind0)
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
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1)
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
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2)
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
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2,
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
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2,
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
inline void TapeRec<Base>::PutInd(size_t ind0, size_t ind1, size_t ind2, 
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
inline size_t TapeRec<Base>::PutTxt(const char *text)
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
