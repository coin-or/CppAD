/* $Id$ */
# ifndef CPPAD_PLAYER_INCLUDED
# define CPPAD_PLAYER_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup player_hpp player.hpp
\{
\file player.hpp
File used to define the player class.
*/


/*!
Class used to store and play back an operation sequence recording.

\tparam Base
These were AD< \a Base > operations when recorded. Operations during playback
are done using the type  \a Base .
*/
template <class Base>
class player {

// -------------- Variables that define the recording -----------------------
private:
	/// Number of variables in the recording.
	size_t    num_rec_var_;

	/// The operators in the recording.
	pod_vector<CPPAD_OP_CODE_TYPE> rec_op_;

	/// Number of VecAD vectors in the recording
	size_t    num_rec_vecad_vec_;

	/// The VecAD indices in the recording.
	pod_vector<addr_t> rec_vecad_ind_;

	/// The operation argument indices in the recording
	pod_vector<addr_t> rec_op_arg_;

	/// The parameters in the recording.
	/// Note that Base may not be plain old data, so use false in consructor.
	pod_vector<Base> rec_par_;

	/// Character strings ('\\0' terminated) in the recording.
	pod_vector<char> rec_text_;


// --------------- Functions used to create and maniplate a recording -------
public:
	/// Default constructor
	player(void) : 
	num_rec_var_(0)                                      ,
	rec_op_( std::numeric_limits<addr_t>::max() )        ,
	rec_vecad_ind_( std::numeric_limits<addr_t>::max() ) ,
	rec_op_arg_( std::numeric_limits<addr_t>::max() )    ,
	rec_par_( std::numeric_limits<addr_t>::max() )       ,
	rec_text_( std::numeric_limits<addr_t>::max() )
	{ }

	/// Destructor
	~player(void)
	{ }

	// ===============================================================
	// Begin two functions with idential code but different argument types.
	/*!  
 	Moving an operation sequence from a recorder to a player
 
	\param rec
	the object that was used to record the operation sequence.  After this 
	operation, the state of the recording is no longer defined. For example,
	the \c pod_vector member variables in \c this have been swapped with
	\c rec .
 	*/
	void get(recorder<Base>& rec)
	{	size_t i;

		// Var
		num_rec_var_        = rec.num_rec_var_;

		// Op
		rec_op_.swap(rec.rec_op_);

		// VecInd
		rec_vecad_ind_.swap(rec.rec_vecad_ind_);

		// Arg
		rec_op_arg_.swap(rec.rec_op_arg_);

		// Par
		rec_par_.swap(rec.rec_par_);

		// Txt
		rec_text_.swap(rec.rec_text_);

		// set the number of VecAD vectors
		num_rec_vecad_vec_ = 0;
		for(i = 0; i < rec_vecad_ind_.size(); i += rec_vecad_ind_[i] + 1)
			num_rec_vecad_vec_++;
		// rec_vecad_ind_ contains size of each VecAD followed by
		// the parameter indices used to iniialize it.
		CPPAD_ASSERT_UNKNOWN( i == rec_vecad_ind_.size() );
	}

	/*!  
 	Copying an operation sequence from one player to another
 
	\param play
	the object that contains the operatoion sequence to copy.
 	*/
	void operator=(const player& play)
	{	
		// Var
		num_rec_var_        = play.num_rec_var_;

		// Op
		rec_op_             = play.rec_op_;

		// VecInd
		num_rec_vecad_vec_  = play.num_rec_vecad_vec_;
		rec_vecad_ind_      = play.rec_vecad_ind_;

		// Arg
		rec_op_arg_         = play.rec_op_arg_;

		// Par
		rec_par_            = play.rec_par_;

		// Txt
		rec_text_           = play.rec_text_;
	}
	// End two functions with idential code but different argument types.
	// ===============================================================

	/// Erase all information in an operation sequence player.
	void Erase(void)
	{	
		num_rec_var_       = 0;
		num_rec_vecad_vec_ = 0;

		rec_op_.erase();
		rec_vecad_ind_.erase();
		rec_op_arg_.erase();
		rec_par_.erase();
		rec_text_.erase();
	}

public:
	/*! 
	\brief 
	Old method of fetching an operator from the recording.

	\return 
	the i-th operator in the recording.

	\param i
	the index of the operator in recording
	*/
	OpCode GetOp (size_t i) const
	{	return OpCode(rec_op_[i]); }

	/*! 
	\brief 
	Fetch a VecAD index from the recording.

	\return 
	the i-th VecAD index in the recording.

	\param i
	the index of the VecAD index in recording
	*/
	size_t GetVecInd (size_t i) const
	{	return rec_vecad_ind_[i]; }

	/*! 
	\brief 
	Fetch a parameter from the recording.

	\return 
	the i-th parameter in the recording.

	\param i
	the index of the parameter in recording
	*/
	Base GetPar(size_t i) const
	{	return rec_par_[i]; }

	/*! 
	\brief 
	Fetch entire parameter vector from the recording.

	\return 
	the entire parameter vector.

	*/
	const Base* GetPar(void) const
	{	return rec_par_.data(); }

	/*! 
	\brief 
	Fetch a '\\0' terminated string from the recording.

	\return 
	the beginning of the string.

	\param i
	the index where the string begins. 
	*/
	const char *GetTxt(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < rec_text_.size() );
		return rec_text_.data() + i;
	}
	
	/// Fetch number of variables in the recording.
	size_t num_rec_var(void) const
	{	return num_rec_var_; }

	/// Fetch number of operators in the recording.
	size_t num_rec_op(void) const
	{	return rec_op_.size(); }

	/// Fetch number of VecAD indices in the recording.
	size_t num_rec_vecad_ind(void) const
	{	return rec_vecad_ind_.size(); }

	/// Fetch number of VecAD vectors in the recording
	size_t num_rec_vecad_vec(void) const
	{	return num_rec_vecad_vec_; }

	/// Fetch number of argument indices in the recording.
	size_t num_rec_op_arg(void) const
	{	return rec_op_arg_.size(); }

	/// Fetch number of parameters in the recording.
	size_t num_rec_par(void) const
	{	return rec_par_.size(); }

	/// Fetch number of characters (representing strings) in the recording.
	size_t num_rec_text(void) const
	{	return rec_text_.size(); }

	/// Fetch a rough measure of amount of memory used to store recording
	/// (just lengths, not capacities). 
	size_t Memory(void) const
	{	return rec_op_.size()        * sizeof(OpCode) 
		     + rec_op_arg_.size()    * sizeof(addr_t)
		     + rec_par_.size()       * sizeof(Base)
		     + rec_text_.size()      * sizeof(char)
		     + rec_vecad_ind_.size() * sizeof(addr_t)
		;
	}

// ------------- Variables used for new methog of playing back a recording ---
private:
	/// Current operator
	OpCode    op_;

	/// Index in recording corresponding to current operator
	size_t    op_index_;

	/// Current offset of the argument indices in rec_op_arg_ 
	addr_t*   op_arg_;

	/// Index for primary (last) variable corresponding to current operator
	size_t    var_index_;

// ----------- Functions used in new method for palying back a recording ---
public:
	/*!
	Start a play back of the recording during a forward sweep.

	Use repeated calls to forward_next to play back one operator at a time.

	\param op [out]
	The input value of \c op does not matter. Its output value is the
	first operator in the recording; i.e., BeginOp.

	\param op_arg [out]
	The input value of \c op_arg does not matter. Its output value is the
	beginning of the vector of argument indices for the first operation;
	i.e., 0

	\param op_index [out]
	The input value of \c op_index does not matter. Its output value
	is the index of the next first operator in the recording; i.e., 0.

	\param var_index [out]
	The input value of \c var_index does not matter. Its output value is the
	index of the primary (last) result corresponding to the the first
	operator (which must be a BeginOp); i.e., 0.
	*/
	void forward_start(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{
		op        = op_          = OpCode( rec_op_[0] ); 
		op_arg    = op_arg_      = rec_op_arg_.data();
		op_index  = op_index_    = 0;
		var_index = var_index_   = 0;

		CPPAD_ASSERT_UNKNOWN( op_  == BeginOp );
		CPPAD_ASSERT_NARG_NRES(op_, 1, 1);

		return;
	}

	/*!
	Fetch the next operator during a forward sweep.

	Use forward_start to initialize to the first operator; i.e.,
	the BeginOp at the beginning of the recording. 
	We use the notation forward_routine to denote the set
	forward_start, forward_next, forward_csum, forward_cskip.

	\param op [in,out]
	The input value of \c op must be its output value from the 
	previous call to a forward_routine.
	Its output value is the next operator in the recording.
	For speed, \c forward_next does not check for the special cases
	where  <tt>op == CSumOp</tt> or <tt>op == CSkipOp</tt>. In these cases, 
	the other return values from \c forward_next must be corrected by a call 
	to \c forward_csum or \c forward_cskip respectively.

	\param op_arg [in,out]
	The input value of \c op_arg must be its output value form the
	previous call to a forward routine. 
	Its output value is the
	beginning of the vector of argument indices for this operation.

	\param op_index [in,out]
	The input value of \c op_index must be its output value form the
	previous call to a forward routine. 
	Its output value is the index of the next operator in the recording. 
	Thus the ouput value following the previous call to forward_start is one.
	In addition,
	the output value increases by one with each call to forward_next. 

	\param var_index [in,out]
	The input value of \c var_index must be its output value form the
	previous call to a forward routine. 
	Its output value is the
	index of the primary (last) result corresponding to the operator op.
	*/
	void forward_next(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumRes;
		using CppAD::NumArg;
		CPPAD_ASSERT_UNKNOWN( op_       == op );
		CPPAD_ASSERT_UNKNOWN( op_arg    == op_arg_ );
		CPPAD_ASSERT_UNKNOWN( op_index  == op_index_ );
		CPPAD_ASSERT_UNKNOWN( var_index == var_index_ );

		// index for the next operator 
		op_index    = ++op_index_;

		// first argument for next operator 
		op_arg      = op_arg_    += NumArg(op_);

		// next operator
		op          = op_         = OpCode( rec_op_[ op_index_ ] );

		// index for last result for next operator
		var_index   = var_index_ += NumRes(op);
		

		CPPAD_ASSERT_UNKNOWN( rec_op_arg_.data() <= op_arg_ );
		CPPAD_ASSERT_UNKNOWN( 
			op_arg_ + NumArg(op) <= rec_op_arg_.data() + rec_op_arg_.size() 
		);
		CPPAD_ASSERT_UNKNOWN( var_index_  < num_rec_var_ );
	}
	/*!
	Correct \c forward_next return values when <tt>op == CSumOp</tt>.

	\param op [in,out]
	The input value of op must be the return value from the previous
	call to \c forward_next and must be \c CSumOp. It is not modified.

	\param op_arg [in,out]
	The input value of \c op_arg must be the return value from the 
	previous call to \c forward_next. Its output value is the
	beginning of the vector of argument indices for the next operation.

	\param op_index [in,out]
	The input value of \c op_index does must be the return value from the
	previous call to \c forward_next. Its is not modified.

	\param var_index [in,out]
	The input value of \c var_index must be the return value from the
	previous call to \c forward_next. It is not modified.
	*/
	void forward_csum(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumRes;
		using CppAD::NumArg;
		CPPAD_ASSERT_UNKNOWN( op_       == op );
		CPPAD_ASSERT_UNKNOWN( op_arg    == op_arg_ );
		CPPAD_ASSERT_UNKNOWN( op_index  == op_index_ );
		CPPAD_ASSERT_UNKNOWN( var_index == var_index_ );

		CPPAD_ASSERT_UNKNOWN( op == CSumOp );
		CPPAD_ASSERT_UNKNOWN( NumArg(CSumOp) == 0 );
		CPPAD_ASSERT_UNKNOWN(
		op_arg[0] + op_arg[1] == op_arg[ 3 + op_arg[0] + op_arg[1] ]
		);
		/*
		The only thing that really needs fixing is op_arg_.
		Actual number of arugments for this operator is
			op_arg[0] + op_arg[1] + 4.
 		We must change op_arg_ so that when you add NumArg(CSumOp)
		you get first argument for next operator in sequence.
		*/
		op_arg = op_arg_   += op_arg[0] + op_arg[1] + 4;

		CPPAD_ASSERT_UNKNOWN( rec_op_arg_.data() <= op_arg_ );
		CPPAD_ASSERT_UNKNOWN( 
			op_arg_ + NumArg(op) <= rec_op_arg_.data() + rec_op_arg_.size()
		);
		CPPAD_ASSERT_UNKNOWN( var_index_  < num_rec_var_ );
	}
	/*!
	Correct \c forward_next return values when <tt>op == CSkipOp</tt>.

	\param op [in,out]
	The input value of op must be the return value from the previous
	call to \c forward_next and must be \c CSkipOp. It is not modified.

	\param op_arg [in,out]
	The input value of \c op_arg must be the return value from the 
	previous call to \c forward_next. Its output value is the
	beginning of the vector of argument indices for the next operation.

	\param op_index [in,out]
	The input value of \c op_index does must be the return value from the
	previous call to \c forward_next. Its is not modified.

	\param var_index [in,out]
	The input value of \c var_index must be the return value from the
	previous call to \c forward_next. It is not modified.
	*/
	void forward_cskip(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumRes;
		using CppAD::NumArg;
		CPPAD_ASSERT_UNKNOWN( op_       == op );
		CPPAD_ASSERT_UNKNOWN( op_arg    == op_arg_ );
		CPPAD_ASSERT_UNKNOWN( op_index  == op_index_ );
		CPPAD_ASSERT_UNKNOWN( var_index == var_index_ );

		CPPAD_ASSERT_UNKNOWN( op == CSkipOp );
		CPPAD_ASSERT_UNKNOWN( NumArg(CSkipOp) == 0 );
		CPPAD_ASSERT_UNKNOWN(
		op_arg[4] + op_arg[5] == op_arg[ 6 + op_arg[4] + op_arg[5] ]
		);
		/*
		The only thing that really needs fixing is op_arg_.
		Actual number of arugments for this operator is
			7 + op_arg[4] + op_arg[5]
 		We must change op_arg_ so that when you add NumArg(CSkipOp)
		you get first argument for next operator in sequence.
		*/
		op_arg  = op_arg_  += 7 + op_arg[4] + op_arg[5];

		CPPAD_ASSERT_UNKNOWN( rec_op_arg_.data() <= op_arg_ );
		CPPAD_ASSERT_UNKNOWN( 
			op_arg_ + NumArg(op) <= rec_op_arg_.data() + rec_op_arg_.size()
		);
		CPPAD_ASSERT_UNKNOWN( var_index_  < num_rec_var_ );
	}
	/*!
	Get a non-constant version of op_arg returned by previous forward_next

	\return
	The return value is equal to the return value of \c op_arg 
	corresponding to the previous call to forward_next.
	*/
	addr_t* forward_non_const_arg(void)
	{	return op_arg_; }

	/*!
	Start a play back of the recording during a reverse sweep.

	Use repeated calls to reverse_next to play back one operator at a time.

	\param op [out]
	The input value of \c op does not matter. Its output value is the
	last operator in the recording; i.e., EndOp.

	\param op_arg [out]
	The input value of \c op_arg does not matter. Its output value is the
	beginning of the vector of argument indices for the last operation;
	(there are no arguments for the last operation so \a op_arg is invalid).

	\param op_index [out[
	The input value of \c op_index does not matter. Its output value
	is the index of the last operator in the recording.

	\param var_index [out]
	The input value of \c var_index does not matter. Its output value is the
	index of the primary (last) result corresponding to the the last
	operator (which must be a EndOp).
	(there are no results for the last operation so \a var_index is invalid).
	*/

	void reverse_start(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{
		op_arg      = op_arg_     = rec_op_arg_.data() + rec_op_arg_.size();
		op_index    = op_index_   = rec_op_.size() - 1; 
		var_index   = var_index_  = num_rec_var_ - 1;
		op          = op_         = OpCode( rec_op_[ op_index_ ] );
		CPPAD_ASSERT_UNKNOWN( op_ == EndOp );
		CPPAD_ASSERT_NARG_NRES(op, 0, 0);
		return;
	}

	/*!
	Fetch the next operator during a reverse sweep.

	Use reverse_start to initialize to reverse play back.
	The first call to reverse_next (after reverse_start) will give the 
	last operator in the recording.
	We use the notation reverse_routine to denote the set
	reverse_start, reverse_next, reverse_csum, reverse_cskip.

	\param op [in,out]
	The input value of \c op must be its output value from the 
	previous call to a reverse_routine.
	Its output value is the next operator in the recording (in reverse order).
	The last operator sets op equal to EndOp.

	\param op_arg [in,out]
	The input value of \c op_arg must be its output value from the 
	previous call to a reverse_routine.
	Its output value is the
	beginning of the vector of argument indices for this operation.
	The last operator sets op_arg equal to the beginning of the 
	argument indices for the entire recording.
	For speed, \c reverse_next does not check for the special cases
	<tt>op == CSumOp</tt> or <tt>op == CSkipOp</tt>. In these cases, the other
	return values from \c reverse_next must be corrected by a call to 
	\c reverse_csum or \c reverse_cskip respectively.


	\param op_index [in,out]
	The input value of \c op_index must be its output value from the 
	previous call to a reverse_routine.
	Its output value
	is the index of this operator in the recording. Thus the output
	value following the previous call to reverse_start is equal to 
	the number of variables in the recording minus one.
	In addition, the output value decreases by one with each call to
	reverse_next.
	The last operator sets op_index equal to 0.

	\param var_index [in,out]
	The input value of \c var_index must be its output value from the 
	previous call to a reverse_routine.
	Its output value is the
	index of the primary (last) result corresponding to the operator op.
	The last operator sets var_index equal to 0 (corresponding to BeginOp
	at beginning of operation sequence).
	*/
	void reverse_next(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumRes;
		using CppAD::NumArg;
		CPPAD_ASSERT_UNKNOWN( op_       == op );
		CPPAD_ASSERT_UNKNOWN( op_arg    == op_arg_ );
		CPPAD_ASSERT_UNKNOWN( op_index  == op_index_ );
		CPPAD_ASSERT_UNKNOWN( var_index == var_index_ );

		// index of the last result for the next operator
		CPPAD_ASSERT_UNKNOWN( var_index_ >= NumRes(op_) );
		var_index   = var_index_ -= NumRes(op_);

		// next operator
		CPPAD_ASSERT_UNKNOWN( op_index_  > 0 );
		op_index    = --op_index_;                                  // index
		op          = op_         = OpCode( rec_op_[ op_index_ ] ); // value

		// first argument for next operator
		op_arg      = op_arg_    -= NumArg(op);
		CPPAD_ASSERT_UNKNOWN( rec_op_arg_.data() <= op_arg_ );
		CPPAD_ASSERT_UNKNOWN( 
			op_arg_ + NumArg(op) <= rec_op_arg_.data() + rec_op_arg_.size() 
		);
	}
	/*!
	Correct \c reverse_next return values when <tt>op == CSumOp</tt>.

	\param op [in,out]
	The input value of \c op must be the return value from the previous
	call to \c reverse_next and must be \c CSumOp. It is not modified.

	\param op_arg
	The input value of \c op_arg must be the return value from the 
	previous call to \c reverse_next. Its output value is the
	beginning of the vector of argument indices for this operation.

	\param op_index [in,out]
	The input value of \c op_index must be the return value from the
	previous call to \c reverse_next. It is not modified.

	\param var_index
	The input value of \c var_index must be the return value from the 
	previous call to \c reverse_next. It is not modified.
	*/

	void reverse_csum(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumRes;
		using CppAD::NumArg;
		CPPAD_ASSERT_UNKNOWN( op_       == op );
		CPPAD_ASSERT_UNKNOWN( op_arg    == op_arg_ );
		CPPAD_ASSERT_UNKNOWN( op_index  == op_index_ );
		CPPAD_ASSERT_UNKNOWN( var_index == var_index_ );

		CPPAD_ASSERT_UNKNOWN( op == CSumOp );
		CPPAD_ASSERT_UNKNOWN( NumArg(CSumOp) == 0 );
		/*
		The variables that need fixing are op_arg_ and op_arg. Currently, 
		op_arg points first argument for the previous operator.
		*/
		--op_arg;
		op_arg      = op_arg_    -= (op_arg[0] + 4);

		CPPAD_ASSERT_UNKNOWN(
		op_arg[0] + op_arg[1] == op_arg[ 3 + op_arg[0] + op_arg[1] ]
		);
		CPPAD_ASSERT_UNKNOWN( op_index_  < rec_op_.size() );
		CPPAD_ASSERT_UNKNOWN( rec_op_arg_.data() <= op_arg_ );
		CPPAD_ASSERT_UNKNOWN( var_index_  < num_rec_var_ );
	}
	/*!
	Correct \c reverse_next return values when <tt>op == CSkipOp</tt>.


	\param op [in,out]
	The input value of \c op must be the return value from the previous
	call to \c reverse_next and must be \c CSkipOp. It is not modified.

	\param op_arg
	The input value of \c op_arg must be the return value from the 
	previous call to \c reverse_next. Its output value is the
	beginning of the vector of argument indices for this operation.

	\param op_index [in,out]
	The input value of \c op_index must be the return value from the
	previous call to \c reverse_next. It is not modified.

	\param var_index
	The input value of \c var_index must be the return value from the 
	previous call to \c reverse_next. It is not modified.
	*/

	void reverse_cskip(
	OpCode& op, const addr_t*& op_arg, size_t& op_index, size_t& var_index)
	{	using CppAD::NumRes;
		using CppAD::NumArg;
		CPPAD_ASSERT_UNKNOWN( op_       == op );
		CPPAD_ASSERT_UNKNOWN( op_arg    == op_arg_ );
		CPPAD_ASSERT_UNKNOWN( op_index  == op_index_ );
		CPPAD_ASSERT_UNKNOWN( var_index == var_index_ );

		CPPAD_ASSERT_UNKNOWN( op == CSkipOp );
		CPPAD_ASSERT_UNKNOWN( NumArg(CSkipOp) == 0 );
		/*
		The variables that need fixing are op_arg_ and op_arg. Currently, 
		op_arg points first arugment for the previous operator.
		*/
		--op_arg;
		op_arg      = op_arg_    -= (op_arg[0] + 4);

		CPPAD_ASSERT_UNKNOWN(
		op_arg[1] + op_arg[2] == op_arg[ 3 + op_arg[1] + op_arg[2] ]
		);
		CPPAD_ASSERT_UNKNOWN( op_index_  < rec_op_.size() );
		CPPAD_ASSERT_UNKNOWN( rec_op_arg_.data() <= op_arg_ );
		CPPAD_ASSERT_UNKNOWN( var_index_  < num_rec_var_ );
	}

};

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
