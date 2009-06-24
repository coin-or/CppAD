/* $Id$ */
# ifndef CPPAD_PLAYER_INCLUDED
# define CPPAD_PLAYER_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


/*!
Class used to store and play back an operation sequence recording.

\tparam Base
These were AD<Base> operations when recorded. Operations during playback
are done using the type Base.
*/
template <class Base>
class player {

// -------------- Variables that define the recording -----------------------
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

	/// Number of arguments indices in the recording. 
	size_t    num_rec_op_arg_;

	/// The argument indices in the recording
	size_t   *rec_op_arg_;

	/// Number of parameters in the recording.
	size_t    num_rec_par_;

	/// The parameters in the recording.
	Base     *rec_par_;

	/// Number of text characters 
	/// (representing '\\0' terminated strings) in the recording.
	size_t    num_rec_text_;

	/// Character in the recording.
	char     *rec_text_;


// --------------- Functions used to create and maniplate a recording -------
public:
	/// Default constructor
	player(void) 
	{	
		num_rec_var_       = 0;

		num_rec_op_        = 0;
		rec_op_            = CPPAD_NULL;

		num_rec_vecad_ind_ = 0;
		rec_vecad_ind_     = CPPAD_NULL;

		num_rec_op_arg_    = 0;
		rec_op_arg_        = CPPAD_NULL;

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
		if( num_rec_op_arg_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_arg_);
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
		if( num_rec_op_arg_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_arg_);
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
		num_rec_op_arg_     = rec.NumberInd_;

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
		if( num_rec_op_arg_ == 0 )
			rec_op_arg_ = CPPAD_NULL;
		else	rec_op_arg_ = 
			CPPAD_TRACK_NEW_VEC(num_rec_op_arg_,    rec_op_arg_);
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
		i = num_rec_op_arg_;
		while(i--)
			rec_op_arg_[i] = rec.Ind_[i];
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
		num_rec_var_       = 0;
		num_rec_op_        = 0;
		num_rec_vecad_ind_ = 0;
		num_rec_op_arg_    = 0;
		num_rec_par_       = 0;
		num_rec_text_      = 0;

		if( num_rec_op_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_);
		if( num_rec_vecad_ind_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_vecad_ind_);
		if( num_rec_op_arg_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_op_arg_);
		if( num_rec_par_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_par_);
		if( num_rec_text_ > 0 )
			CPPAD_TRACK_DEL_VEC(rec_text_);

		num_rec_op_        = 0;
		num_rec_vecad_ind_ = 0;
		num_rec_op_arg_    = 0;
		num_rec_par_       = 0;
		num_rec_text_      = 0;
	}

// ------------------ Old method of palying back a recording -----------------
public:
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
	Fetch a subsequence of the argument indices from the recording.

	\return 
	the beginning of the subsequence of argument indices.

	\param n
	the number of argument indices in the subsequence.

	\param i
	the index where the subsequence begins. 
	*/
	const size_t *GetInd(size_t n, size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i + n <= num_rec_op_arg_)
		return rec_op_arg_ + i;
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
	Replace an argument index in the recording.

	\param i
	is the index, in argument indices, that is to be replaced.

	\param value
	is the new normal index value.
	*/
	void ReplaceInd(size_t i, size_t value)
	{	CPPAD_ASSERT_UNKNOWN( i < num_rec_op_arg_);
		rec_op_arg_[i] = value;
	}

	/// Fetch number of variables in the recording.
	size_t num_rec_var(void) const
	{	return num_rec_var_; }

	/// Fetch number of operators in the recording.
	size_t num_rec_op(void) const
	{	return num_rec_op_; }

	/// Fetch number of VecAD indices in the recording.
	size_t num_rec_vecad_ind(void) const
	{	return num_rec_vecad_ind_; }

	/// Fetch number of argument indices in the recording.
	size_t num_rec_op_arg(void) const
	{	return num_rec_op_arg_; }

	/// Fetch number of parameters in the recording.
	size_t num_rec_par(void) const
	{	return num_rec_par_; }

	/// Fetch number of characters (representing strings) in the recording.
	size_t num_rec_text(void) const
	{	return num_rec_text_; }

	/// Fetch a rough measure of amount of memory used by the recording. 
	size_t Memory(void) const
	{	return num_rec_op_ * sizeof(OpCode) 
		     + num_rec_vecad_ind_ * sizeof(size_t)
		     + num_rec_op_arg_ * sizeof(size_t)
		     + num_rec_par_ * sizeof(Base)
		     + num_rec_text_ * sizeof(char);
	}

// ------------- Variables used for new methog of playing back a recording ---
private:
	/// Current operator
	OpCode    op_;

	/// Index in recording corresponding to current operator
	size_t    op_index_;

	/// Current offser of the argument indices in rec_op_arg_ 
	size_t    op_arg_;

	/// Index for first variable corresponding to current operator
	size_t    var_index_;

// ----------- Functions used in new method for palying back a recording ---
public:
	/*!
	Start a play back of the recording during a forward sweep.

	Use repeated calls to next_forward to play back one operator at a time.

	\param op
	The input value of op does not matter. Its output value is the
	first operator in the recording; i.e., NonOp.

	\param op_arg
	The input value of *op_arg does not matter. Its output value is the
	beginning of the vector of argument indices for the first operation;
	i.e., 0

	\param op_index
	The input value of op_index does not matter. Its output value
	is the index of the next first operator in the recording; i.e., 0.

	\param var_index
	The input value of var_index does not matter. Its output value is the
	index of the first result variable corresponding to the the first
	operator; i.e., 0.
	*/
	void start_forward(
	OpCode& op, const size_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumVar;
		using CppAD::NumInd;

		op        = op_          = rec_op_[0]; 
		op_arg_   = 0;
		op_arg    = rec_op_arg_;
		op_index  = op_index_    = 0;
		var_index = var_index_   = 0;
# ifndef NDEBUG
		CPPAD_ASSERT_UNKNOWN( op_         == NonOp );
		CPPAD_ASSERT_UNKNOWN( NumVar(op_) == 1     );
		CPPAD_ASSERT_UNKNOWN( NumInd(op_) == 0     );
# endif
		return;
	}

	/*!
	Fetch the next operator during a forward sweep.

	Use start_forward to initialize to the first operator; i.e.,
	the NonOp at the beginning of the recording. 

	\param op
	The input value of op does not matter. Its output value is the
	next operator in the recording.

	\param op_arg
	The input value of *op_arg does not matter. Its output value is the
	beginning of the vector of argument indices for this operation.

	\param op_index
	The input value of op_index does not matter. Its output value
	is the index of the next operator in the recording. Thus the ouput
	value following the previous call to start_forward is one. In addition,
	the output value increases by one with each call to next_forward. 

	\param var_index
	The input value of var_index does not matter. Its output value is the
	index of the first result variable corresponding to the operator op.
	*/

	void next_forward(
	OpCode& op, const size_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumVar;
		using CppAD::NumInd;

		op_index    = ++op_index_;
		op_arg_    += NumInd(op_);
		var_index_ += NumVar(op_);

		op          = op_         = rec_op_[ op_index_ ];
		op_arg      = op_arg_ + rec_op_arg_;
		var_index   = var_index_;

		CPPAD_ASSERT_UNKNOWN( op_index_  < num_rec_op_ );
		CPPAD_ASSERT_UNKNOWN( op_arg_ + NumInd(op) <= num_rec_op_arg_ );
		CPPAD_ASSERT_UNKNOWN( var_index_ + NumVar(op) <= num_rec_var_ );
	}
	/*!
	Start a play back of the recording during a reverse sweep.

	Use repeated calls to next_reverse to play back one operator at a time.
	*/
	void start_reverse(void)
	{
		op_arg_     = num_rec_op_arg_;
		op_index_   = num_rec_op_;
		var_index_  = num_rec_var_;
		return;
	}

	/*!
	Fetch the next operator during a reverse sweep.

	Use start_reverse to initialize to reverse play back.
	The first call to next_reverse (after start_reverse) will give the 
        last operator in the recording.

	\param op
	The input value of op does not matter. Its output value is the
	next operator in the recording (in reverse order).
	The last operator sets op equal to NonOp.

	\param op_arg
	The input value of *op_arg does not matter. Its output value is the
	beginning of the vector of argument indices for this operation.
	The last operator sets op_arg equal to the beginning of the 
	argument indices for the entire recording.

	\param op_index
	The input value of op_index does not matter. Its output value
	is the index of this operator in the recording. Thus the output
	value following the previous call to start_reverse is equal to 
	the number of variables in the recording minus one.
	In addition, the output value decreases by one with each call to
	next_reverse.
	The last operator sets op_index equal to 0.

	\param var_index
	The input value of var_index does not matter. Its output value is the
	index of the first result variable corresponding to the operator op.
	The last operator sets var_index equal to 0.
	*/

	void next_reverse(
	OpCode& op, const size_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumVar;
		using CppAD::NumInd;

		CPPAD_ASSERT_UNKNOWN( op_index_  > 0 );
		op_index    = --op_index_;
		op_         = rec_op_[ op_index_ ];

		CPPAD_ASSERT_UNKNOWN( op_arg_ >= NumInd(op_)  );
		op_arg_    -= NumInd(op_);

		CPPAD_ASSERT_UNKNOWN( var_index_ >= NumVar(op_) );
		var_index_ -= NumVar(op_);

		op          = op_;
		op_arg      = op_arg_ + rec_op_arg_;
		var_index   = var_index_;

	}

};

CPPAD_END_NAMESPACE
# endif
