# ifndef CPPAD_PLAYER_INCLUDED
# define CPPAD_PLAYER_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin player$$ $comment CppAD Developer Documentation$$

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

$section Playback a CppAD Operation Sequence$$
$index tape, playback$$
$index playback, tape$$
$index player$$

$head Syntax$$
$codei%player<%Base%> %play%$$


$head Default Constructors$$
The default constructor 
$codei%
	player<%Base%> %play%
%$$
creates an empty operation sequence.

$head Assignment Operator$$
If $icode rec$$ is 
$codei%
	const recorder &%rec%
%$$
object, the assignment operation 
$codei%
	%play% = %rec%
%$$
copies the operation sequence from the 
$cref/recorder/$$ $icode rec$$ to the $code player$$ $icode play$$.

$head Erase$$
$index player, Erase$$
$index Erase, player$$
The function call
$codei%
	void %play%.Erase()
%$$
erases the operation sequence stored in $icode play%$$
(the operation sequence is empty after this operation).
The buffers used to store the tape information are returned
to the system (so as to conserve on memory).

$head GetOp$$
$index GetOp$$
If $icode op$$ and $icode i$$ have prototypes
$codei%
	OpCode %op%
	size_t %i%
%$$
The syntax
$codei%
	%op% = %play%.GetOp(%i%)
%$$
sets $icode op$$ to the operation code for the $th i$$ operation in the 
operation sequence.

$head GetInd$$
$index GetInd$$
If $icode p$$, $icode n$$ and $icode offset$$ have prototypes
$codei%
	const size_t *%p%
	size_t %n%
	size_t %offset%
%$$
The syntax
$codei%
	%p% = %play%.GetInd(%n%, %offset%)
%$$
returns a pointer to the $icode n$$ the values that start at the 
specified offset in the operation sequence index vector.

$head GetPar$$
$index GetPar$$
If $icode p$$ and $icode i$$ have prototypes
$codei%
	const %Base% *%p%
	size_t %i%
%$$
The syntax
$codei%
	%p% = %play%.GetPar(%i%)
%$$
returns a pointer to the value with index $icode i$$
in the operation sequence parameter vector.

$head GetVecInd$$
$index GetVecInd$$
If $icode iv$$ and $icode i$$ have prototypes
$codei%
	size_t %iv%
	size_t %i%
%$$
the syntax
$codei%
	%iv% = %play%.GetVecInd(%i%)
%$$
returns the value with index $icode i$$ in the
operation sequence vec_ind vector.

$head NumOp$$
$index NumOp$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	%n% = %play%.NumOp()
%$$
sets $icode n$$ to the number of 
operations in the operation sequence.

$head NumInd$$
$index NumInd$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	%n% = %play%.NumInd()
%$$
sets $icode n$$ to the number of elements in the 
operation sequence index vector.

$head NumPar$$
$index NumPar$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	%n% = %play%.NumPar()
%$$
sets $icode n$$ to the number of parameters
in the operation sequence parameter vector.

$head NumVecInd$$
$index NumVecInd$$
If $icode n$$ has prototype
$codei%
	size_t %n%
%$$
the syntax
$codei%
	%n% = %play%.NumVecInd()
%$$
sets $icode n$$ to the number of element in the 
operation sequence vec_ind vector.


$head ReplaceInd$$
$index ReplaceInd$$
If $icode i$$ and $icode v$$ have prototypes
$codei%
	size_t %i%
	size_t %v%
%$$
the syntax
$codei%
	%play%.ReplaceInd(%i%, %v%)
%$$
replaces the element with index $icode i$$,
in the operation sequence index vector,
with the value $icode v$$.

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
class player {

public:
	// default
	player(void) 
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
	~player(void)
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
	void operator=(const recorder<Base> &rec)
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
		TotalNumberVar_  = rec.TotalNumberVar_;

		// Op
		NumberOp_        = rec.NumberOp_;
		LengthOp_        = rec.NumberOp_;

		// VecInd
		NumberVecInd_    = rec.NumberVecInd_;
		LengthVecInd_    = rec.NumberVecInd_;

		// Ind
		NumberInd_       = rec.NumberInd_;
		LengthInd_       = rec.NumberInd_;

		// Par
		NumberPar_       = rec.NumberPar_;
		LengthPar_       = rec.NumberPar_;

		// Txt
		NumberTxt_       = rec.NumberTxt_;
		LengthTxt_       = rec.NumberTxt_;

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
			Op_[i] = rec.Op_[i];
		i = NumberVecInd_;
		while(i--)
			VecInd_[i] = rec.VecInd_[i];
		i = NumberInd_;
		while(i--)
			Ind_[i] = rec.Ind_[i];
		i = NumberPar_;
		while(i--)
			Par_[i] = rec.Par_[i];
		i = NumberTxt_;
		while(i--)
			Txt_[i] = rec.Txt_[i];
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

} // END CppAD namespace

# endif
