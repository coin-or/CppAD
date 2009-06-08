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
	size_t    num_rec_var_;

	/// Number of operators in the recording. 
	size_t    num_rec_op_;

	/// The operators in the recording.
	OpCode   *rec_op_;

	/// Number of VecAD indices in the recording. 
	size_t    num_rec_vecad_ind_;

	/// The VecAD indices in the recording.
	size_t   *rec_vecad_ind_;

	/// Number of operation indices in the recording. 
	size_t    num_rec_op_ind_;

	/// The operation indices in the recording
	size_t   *rec_op_ind_;

	/// Number of parameters in the recording.
	size_t    num_rec_par_;

	/// The parameters in the recording.
	Base     *rec_par_;

	/// Number of text characters 
	/// (representing '\\0' terminated strings) in the recording.
	size_t    num_rec_text_;

	/// Character in the recording.
	char     *rec_text_;

public:
	/// Default constructor
	player(void) 
	{	
		num_rec_var_       = 0;

		num_rec_op_        = 0;
		rec_op_            = CPPAD_NULL;

		num_rec_vecad_ind_ = 0;
		rec_vecad_ind_     = CPPAD_NULL;

		num_rec_op_ind_    = 0;
		rec_op_ind_        = CPPAD_NULL;

		num_rec_par_       = 0;
		rec_par_           = CPPAD_NULL;

		num_rec_text_      = 0;
		rec_text_          = CPPAD_NULL;

	}

	/// Destructor
	~player(void)
	{	if( num_rec_op_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_);
		if( num_rec_vecad_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_vecad_ind_);
		if( num_rec_op_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_ind_);
		if( num_rec_par_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_par_);
		if( num_rec_text_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_text_);
	}

	/// Creating an operation sequence player from a recorder.
	void operator=(const recorder<Base> &rec)
	{	size_t i;

		if( num_rec_op_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_);
		if( num_rec_vecad_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_vecad_ind_);
		if( num_rec_op_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_ind_);
		if( num_rec_par_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_par_);
		if( num_rec_text_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_text_);

		// Var
		num_rec_var_        = rec.TotalNumberVar_;

		// Op
		num_rec_op_         = rec.NumberOp_;

		// VecInd
		num_rec_vecad_ind_  = rec.NumberVecInd_;

		// Ind
		num_rec_op_ind_     = rec.NumberInd_;

		// Par
		num_rec_par_        = rec.NumberPar_;

		// Txt
		num_rec_text_       = rec.NumberTxt_;

		// Allocate the memory
		if( num_rec_op_ == 0 )
			rec_op_ = CPPAD_NULL;
		else	rec_op_ = 
			CPPAD_TRACK_NEW_VEC(num_rec_op_,        rec_op_);
		if( num_rec_vecad_ind_ == 0 )
			rec_vecad_ind_ = CPPAD_NULL;
		else	rec_vecad_ind_ = 
			CPPAD_TRACK_NEW_VEC(num_rec_vecad_ind_, rec_vecad_ind_);
		if( num_rec_op_ind_ == 0 )
			rec_op_ind_ = CPPAD_NULL;
		else	rec_op_ind_ = 
			CPPAD_TRACK_NEW_VEC(num_rec_op_ind_,    rec_op_ind_);
		if( num_rec_par_ == 0 )
			rec_par_ = CPPAD_NULL;
		else	rec_par_ = 
			CPPAD_TRACK_NEW_VEC(num_rec_par_,       rec_par_);
		if( num_rec_text_ == 0 )
			rec_text_ = CPPAD_NULL;
		else	rec_text_ = 
			CPPAD_TRACK_NEW_VEC(num_rec_text_,      rec_text_);

		// Copy the data
		i = num_rec_op_;
		while(i--)
			rec_op_[i] = rec.Op_[i];
		i = num_rec_vecad_ind_;
		while(i--)
			rec_vecad_ind_[i] = rec.VecInd_[i];
		i = num_rec_op_ind_;
		while(i--)
			rec_op_ind_[i] = rec.Ind_[i];
		i = num_rec_par_;
		while(i--)
			rec_par_[i] = rec.Par_[i];
		i = num_rec_text_;
		while(i--)
			rec_text_[i] = rec.Txt_[i];
	}

	/// Erase all information in an operation sequence player.
	void Erase(void)
	{	
		num_rec_var_  = 0;
		num_rec_op_        = 0;
		num_rec_vecad_ind_    = 0;
		num_rec_op_ind_       = 0;
		num_rec_par_       = 0;
		num_rec_text_       = 0;

		if( num_rec_op_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_);
		if( num_rec_vecad_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_vecad_ind_);
		if( num_rec_op_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_ind_);
		if( num_rec_par_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_par_);
		if( num_rec_text_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_text_);

		num_rec_op_        = 0;
		num_rec_vecad_ind_    = 0;
		num_rec_op_ind_       = 0;
		num_rec_par_       = 0;
		num_rec_text_       = 0;
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
	{	CPPAD_ASSERT_UNKNOWN(i < num_rec_op_);
		return rec_op_[i];
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
	{	CPPAD_ASSERT_UNKNOWN(i < num_rec_vecad_ind_);
		return rec_vecad_ind_[i];
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
	{	CPPAD_ASSERT_UNKNOWN(i < num_rec_par_);
		return rec_par_ + i;
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
	{	CPPAD_ASSERT_UNKNOWN(i + n <= num_rec_op_ind_)
		return rec_op_ind_ + i;
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
	{	CPPAD_ASSERT_UNKNOWN(i < num_rec_text_);
		return rec_text_ + i;
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
	{	CPPAD_ASSERT_UNKNOWN( i < num_rec_op_ind_);
		rec_op_ind_[i] = value;
	}

	/// Fetch number of variables in the recording.
	size_t TotNumVar(void) const
	{	return num_rec_var_; }

	/// Fetch number of operators in the recording.
	size_t NumOp(void) const
	{	return num_rec_op_; }

	/// Fetch number of VecAD indices in the recording.
	size_t NumVecInd(void) const
	{	return num_rec_vecad_ind_; }

	/// Fetch number of normal indices in the recording.
	size_t NumInd(void) const
	{	return num_rec_op_ind_; }

	/// Fetch number of parameters in the recording.
	size_t NumPar(void) const
	{	return num_rec_par_; }

	/// Fetch number of characters (representing strings) in the recording.
	size_t NumTxt(void) const
	{	return num_rec_text_; }

	/// Fetch a rough measure of amount of memory used by the recording. 
	size_t Memory(void) const
	{	return num_rec_op_ * sizeof(OpCode) 
		     + num_rec_vecad_ind_ * sizeof(size_t)
		     + num_rec_op_ind_ * sizeof(size_t)
		     + num_rec_par_ * sizeof(Base)
		     + num_rec_text_ * sizeof(char);
	}

};

} // END CppAD namespace

# endif
