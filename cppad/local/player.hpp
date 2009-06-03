/* $Id$ */
# ifndef CPPAD_PLAYER_INCLUDED
# define CPPAD_PLAYER_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

/*!
\brief class used to store and play back an operation sequence recording.

*/
template <class Base>
class player {
private:
	/// Number of variables in the recording.
	size_t    TotalNumberVar_;

	/// Number of operators in the recording. 
	size_t    NumberOp_;

	/// The operators in the recording.
	OpCode   *Op_;

	/// Number of VecAD indices in the recording. 
	size_t    NumberVecInd_;

	/// The VecAD indices in the recording.
	size_t   *VecInd_;

	/// Number of operation indices in the recording. 
	size_t    NumberInd_;

	/// The operation indices in the recording
	size_t   *Ind_;

	/// Number of parameters in the recording.
	size_t    NumberPar_;

	/// The parameters in the recording.
	Base     *Par_;

	/// Number of text characters 
	/// (representing '\\0' terminated strings) in the recording.
	size_t    NumberTxt_;

	/// Character in the recording.
	char     *Txt_;

public:
	/// Default constructor
	player(void) 
	{	
		TotalNumberVar_ = 0;

		NumberOp_       = 0;
		Op_             = CPPAD_NULL;

		NumberVecInd_   = 0;
		VecInd_         = CPPAD_NULL;

		NumberInd_      = 0;
		Ind_            = CPPAD_NULL;

		NumberPar_      = 0;
		Par_            = CPPAD_NULL;

		NumberTxt_      = 0;
		Txt_            = CPPAD_NULL;

	}

	/// Destructor
	~player(void)
	{	if( NumberOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( NumberVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( NumberInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( NumberPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( NumberTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);
	}

	/// Creating an operation sequence player from a recorder.
	void operator=(const recorder<Base> &rec)
	{	size_t i;

		if( NumberOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( NumberVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( NumberInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( NumberPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( NumberTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);

		// Var
		TotalNumberVar_  = rec.TotalNumberVar_;

		// Op
		NumberOp_        = rec.NumberOp_;

		// VecInd
		NumberVecInd_    = rec.NumberVecInd_;

		// Ind
		NumberInd_       = rec.NumberInd_;

		// Par
		NumberPar_       = rec.NumberPar_;

		// Txt
		NumberTxt_       = rec.NumberTxt_;

		// Allocate the memory
		if( NumberOp_ == 0 )
			Op_ = CPPAD_NULL;
		else	Op_ = CPPAD_TRACK_NEW_VEC(NumberOp_,      Op_);
		if( NumberVecInd_ == 0 )
			VecInd_ = CPPAD_NULL;
		else	VecInd_ = CPPAD_TRACK_NEW_VEC(NumberVecInd_, VecInd_);
		if( NumberInd_ == 0 )
			Ind_ = CPPAD_NULL;
		else	Ind_ = CPPAD_TRACK_NEW_VEC(NumberInd_,       Ind_);
		if( NumberPar_ == 0 )
			Par_ = CPPAD_NULL;
		else	Par_ = CPPAD_TRACK_NEW_VEC(NumberPar_,       Par_);
		if( NumberTxt_ == 0 )
			Txt_ = CPPAD_NULL;
		else	Txt_ = CPPAD_TRACK_NEW_VEC(NumberTxt_,       Txt_);

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

	/// Erase all information in an operation sequence player.
	void Erase(void)
	{	
		TotalNumberVar_  = 0;
		NumberOp_        = 0;
		NumberVecInd_    = 0;
		NumberInd_       = 0;
		NumberPar_       = 0;
		NumberTxt_       = 0;

		if( NumberOp_ > 0 )
			CPPAD_TRACK_DEL_VEC(Op_);
		if( NumberVecInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(VecInd_);
		if( NumberInd_ > 0 )
			CPPAD_TRACK_DEL_VEC(Ind_);
		if( NumberPar_ > 0 )
			CPPAD_TRACK_DEL_VEC(Par_);
		if( NumberTxt_ > 0 )
			CPPAD_TRACK_DEL_VEC(Txt_);

		NumberOp_        = 0;
		NumberVecInd_    = 0;
		NumberInd_       = 0;
		NumberPar_       = 0;
		NumberTxt_       = 0;
	}

	/*! 
	\brief 
	Fetch an operator from the recording.

	\return 
	the i-th operator in the recording.

	\param i
	the index of the operator in recording
	*/
	OpCode GetOp (size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberOp_);
		return Op_[i];
	}

	/*! 
	\brief 
	Fetch a VecAD index from the recording.

	\return 
	the i-th VecAD index in the recording.

	\param i
	the index of the VecAD index in recording
	*/
	size_t GetVecInd (size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberVecInd_);
		return VecInd_[i];
	}

	/*! 
	\brief 
	Fetch a parameter from the recording.

	\return 
	the i-th parameter in the recording.

	\param i
	the index of the parameter in recording
	*/
	const Base *GetPar(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberPar_);
		return Par_ + i;
	}

	/*! 
	\brief 
	Fetch a subsequence of the normal indices from the recording.

	\return 
	the beginning of the subsequence of normal indices.

	\param n
	the number of normal indices in the subsequence.

	\param i
	the index where the subsequence begins. 
	*/
	const size_t *GetInd(size_t n, size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i + n <= NumberInd_)
		return Ind_ + i;
	}

	/*! 
	\brief 
	Fetch a '\\0' terminated string from the recording.

	\return 
	the beginning of the string.

	\param i
	the index where the string begins. 
	*/
	const char *GetTxt(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < NumberTxt_);
		return Txt_ + i;
	}
	
	/*!
	\brief
	Replace a normal index in the recording.

	\param i
	is the index, in normal indices, that is to be replaced.

	\param value
	is the new normal index value.
	*/
	void ReplaceInd(size_t i, size_t value)
	{	CPPAD_ASSERT_UNKNOWN( i < NumberInd_);
		Ind_[i] = value;
	}

	/// Fetch number of variables in the recording.
	size_t TotNumVar(void) const
	{	return TotalNumberVar_; }

	/// Fetch number of operators in the recording.
	size_t NumOp(void) const
	{	return NumberOp_; }

	/// Fetch number of VecAD indices in the recording.
	size_t NumVecInd(void) const
	{	return NumberVecInd_; }

	/// Fetch number of normal indices in the recording.
	size_t NumInd(void) const
	{	return NumberInd_; }

	/// Fetch number of parameters in the recording.
	size_t NumPar(void) const
	{	return NumberPar_; }

	/// Fetch number of characters (representing strings) in the recording.
	size_t NumTxt(void) const
	{	return NumberTxt_; }

	/// Fetch a rough measure of amount of memory used by the recording. 
	size_t Memory(void) const
	{	return NumberOp_ * sizeof(OpCode) 
		     + NumberVecInd_ * sizeof(size_t)
		     + NumberInd_ * sizeof(size_t)
		     + NumberPar_ * sizeof(Base)
		     + NumberTxt_ * sizeof(char);
	}

};

} // END CppAD namespace

# endif
