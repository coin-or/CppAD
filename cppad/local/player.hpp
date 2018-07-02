# ifndef CPPAD_LOCAL_PLAYER_HPP
# define CPPAD_LOCAL_PLAYER_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/play/addr_enum.hpp>
# include <cppad/local/play/sequential_iterator.hpp>
# include <cppad/local/play/subgraph_iterator.hpp>
# include <cppad/local/play/random_setup.hpp>
# include <cppad/local/user_state.hpp>
# include <cppad/local/is_pod.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file player.hpp
File used to define the player class.
*/

/*!
Class used to store and play back an operation sequence recording.

\tparam Base
These were AD< Base > operations when recorded. Operations during playback
are done using the type Base .
*/

template <class Base>
class player {
private:
	// ----------------------------------------------------------------------
	// information that defines the recording

	/// Number of dynamic parameters
	size_t num_dynamic_;

	/// Number of variables in the recording.
	size_t num_var_rec_;

	/// number of vecad load opeations in the reconding
	size_t num_load_op_rec_;

	/// Number of VecAD vectors in the recording
	size_t num_vecad_vec_rec_;

	/// The operators in the recording.
	pod_vector<opcode_t> op_vec_;

	/// The operation argument indices in the recording
	pod_vector<addr_t> arg_vec_;

	/// The parameters in the recording.
	/// Note that Base may not be plain old data, so use false in consructor.
	pod_vector_maybe<Base> par_vec_;

	/// Character strings ('\\0' terminated) in the recording.
	pod_vector<char> text_vec_;

	/// The VecAD indices in the recording.
	pod_vector<addr_t> vecad_ind_vec_;

	// ----------------------------------------------------------------------
	// Information needed to use member functions that begin with random_
	// and for using const_subgraph_iterator.

	/// index in arg_vec_ corresonding to the first argument for each operator
	pod_vector<unsigned char> op2arg_vec_;

	/*!
	Index of the result variable for each operator. If the operator has
	no results, this is not defined. The invalid index num_var_rec_ is used
	when NDEBUG is not defined. If the operator has more than one result, this
	is the primary result; i.e., the last result. Auxillary are only used by
	the operator and not used by other operators.
	*/
	pod_vector<unsigned char> op2var_vec_;

	/// Mapping from primary variable index to corresponding operator index.
	/// This is used to traverse sub-graphs of the operation sequence.
	/// This value is valid (invalid) for primary (auxillary) variables.
	pod_vector<unsigned char> var2op_vec_;

public:
	// =================================================================
	/// constructor
	player(void) :
	num_var_rec_(0)      ,
	num_load_op_rec_(0)
	{ }

	// =================================================================
	/// destructor
	~player(void)
	{ }
	// ======================================================================
	/// type used for addressing iterators for this player
	play::addr_enum address_type(void) const
	{
		// required
		size_t required = 0;
		required = std::max(required, num_var_rec_   );  // number variables
		required = std::max(required, op_vec_.size()  ); // number operators
		required = std::max(required, arg_vec_.size() ); // number arguments
		//
		// unsigned short
		if( required <= std::numeric_limits<unsigned short>::max() )
			return play::unsigned_short_enum;
		//
		// unsigned int
		if( required <= std::numeric_limits<unsigned int>::max() )
			return play::unsigned_int_enum;
		//
		// unsigned size_t
		CPPAD_ASSERT_UNKNOWN(
			required <= std::numeric_limits<size_t>::max()
		);
		return play::size_t_enum;
	}
	// ===============================================================
	/*!
	Moving an operation sequence from a recorder to this player

	\param rec
	the object that was used to record the operation sequence. After this
	operation, the state of the recording is no longer defined. For example,
	the pod_vector member variables in this have been swapped with rec.

	\param n_ind
	the number of independent variables (only used for error checking
	when NDEBUG is not defined).

	\par
	Use an assert to check that the length of the following vectors is
	less than the maximum possible value for addr_t; i.e., that an index
	in these vectors can be represented using the type addr_t:
	op_vec_, vecad_ind_vec_, arg_vec_, par_vec_, text_vec_.
	*/
	void get_recording(recorder<Base>& rec, size_t n_ind)
	{
# ifndef NDEBUG
		size_t addr_t_max = size_t( std::numeric_limits<addr_t>::max() );
# endif
		// just set size_t values
		num_dynamic_        = rec.num_dynamic_;
		num_var_rec_        = rec.num_var_rec_;
		num_load_op_rec_    = rec.num_load_op_rec_;

		// op_vec_
		op_vec_.swap(rec.op_vec_);
		CPPAD_ASSERT_UNKNOWN(op_vec_.size() < addr_t_max );

		// op_arg_vec_
		arg_vec_.swap(rec.arg_vec_);
		CPPAD_ASSERT_UNKNOWN(arg_vec_.size()    < addr_t_max );

		// par_vec_
		par_vec_.swap(rec.par_vec_);
		CPPAD_ASSERT_UNKNOWN(par_vec_.size() < addr_t_max );

		// text_rec_
		text_vec_.swap(rec.text_vec_);
		CPPAD_ASSERT_UNKNOWN(text_vec_.size() < addr_t_max );

		// vecad_ind_vec_
		vecad_ind_vec_.swap(rec.vecad_ind_vec_);
		CPPAD_ASSERT_UNKNOWN(vecad_ind_vec_.size() < addr_t_max );

		// num_vecad_vec_rec_
		num_vecad_vec_rec_ = 0;
		{	// vecad_ind_vec_ contains size of each VecAD followed by
			// the parameter indices used to inialize it.
			size_t i = 0;
			while( i < vecad_ind_vec_.size() )
			{	num_vecad_vec_rec_++;
				i += vecad_ind_vec_[i] + 1;
			}
			CPPAD_ASSERT_UNKNOWN( i == vecad_ind_vec_.size() );
		}

		// random access information
		clear_random();

		// some checks
		check_inv_op(n_ind);
		check_dag();
	}
	// ----------------------------------------------------------------------
	/*!
	Check that InvOp operators start with second operator and are contiguous,
	and there are n_ind of them.
	*/
# ifdef NDEBUG
	void check_inv_op(size_t n_ind) const
	{	return; }
# else
	void check_inv_op(size_t n_ind) const
	{	play::const_sequential_iterator itr = begin();
		OpCode        op;
		const addr_t* op_arg;
		size_t        var_index;
		itr.op_info(op, op_arg, var_index);
		CPPAD_ASSERT_UNKNOWN( op = BeginOp );
		size_t i_op = 0;
		while( op != EndOp )
		{	// start at second operator
			(++itr).op_info(op, op_arg, var_index);
			++i_op;
			CPPAD_ASSERT_UNKNOWN( (op == InvOp) == (i_op <= n_ind) );
		}
		return;
	}
# endif
	// ----------------------------------------------------------------------
	/*!
	Check arguments that are variables, to make sure the have value less
	than or equal to the previously created variable. This is the directed
	acyclic graph condition (DAG).
	*/
# ifdef NDEBUG
	void check_dag(void) const
	{	return; }
# else
	void check_dag(void) const
	{	play::const_sequential_iterator itr = begin();
		OpCode        op;
		const addr_t* op_arg;
		size_t        var_index;
		itr.op_info(op, op_arg, var_index);
		CPPAD_ASSERT_UNKNOWN( op = BeginOp );
		//
		addr_t arg_var_bound = 0;
		while( op != EndOp )
		{	(++itr).op_info(op, op_arg, var_index);
			switch(op)
			{
				// cases where nothing to do
				case BeginOp:
				case EndOp:
				case InvOp:
				case LdpOp:
				case ParOp:
				case UserOp:
				case UsrapOp:
				case UsrrpOp:
				case UsrrvOp:
				case StppOp:
				break;

				// only first argument is a variable
				case AbsOp:
				case AcosOp:
				case AcoshOp:
				case AsinOp:
				case AsinhOp:
				case AtanOp:
				case AtanhOp:
				case CosOp:
				case CoshOp:
				case DivvpOp:
				case ErfOp:
				case ExpOp:
				case Expm1Op:
				case LevpOp:
				case LogOp:
				case Log1pOp:
				case LtvpOp:
				case PowvpOp:
				case SignOp:
				case SinOp:
				case SinhOp:
				case SqrtOp:
				case SubvpOp:
				case TanOp:
				case TanhOp:
				case UsravOp:
				case ZmulvpOp:
				CPPAD_ASSERT_UNKNOWN(op_arg[0] <= arg_var_bound );
				break;

				// only second argument is a variable
				case AddpvOp:
				case DisOp:
				case DivpvOp:
				case EqpvOp:
				case LdvOp:
				case LepvOp:
				case LtpvOp:
				case MulpvOp:
				case NepvOp:
				case PowpvOp:
				case StvpOp:
				case SubpvOp:
				case ZmulpvOp:
				CPPAD_ASSERT_UNKNOWN(op_arg[1] <= arg_var_bound );
				break;

				// only first and second arguments are variables
				case AddvvOp:
				case DivvvOp:
				case EqvvOp:
				case LevvOp:
				case LtvvOp:
				case MulvvOp:
				case NevvOp:
				case PowvvOp:
				case SubvvOp:
				case ZmulvvOp:
				CPPAD_ASSERT_UNKNOWN(op_arg[0] <= arg_var_bound );
				CPPAD_ASSERT_UNKNOWN(op_arg[1] <= arg_var_bound );
				break;

				// StpvOp
				case StpvOp:
				CPPAD_ASSERT_UNKNOWN(op_arg[2] <= arg_var_bound );
				break;

				// StvvOp
				case StvvOp:
				CPPAD_ASSERT_UNKNOWN(op_arg[1] <= arg_var_bound );
				CPPAD_ASSERT_UNKNOWN(op_arg[2] <= arg_var_bound );
				break;

				// CSumOp
				case CSumOp:
				{	addr_t num_add = op_arg[0];
					addr_t num_sub = op_arg[1];
					for(addr_t j = 0; j < num_add + num_sub; j++)
						CPPAD_ASSERT_UNKNOWN(op_arg[3+j] <= arg_var_bound);
				}
				itr.correct_before_increment();
				break;

				// CExpOp
				case CExpOp:
				if( op_arg[1] & 1 )
					CPPAD_ASSERT_UNKNOWN( op_arg[2] <= arg_var_bound);
				if( op_arg[1] & 2 )
					CPPAD_ASSERT_UNKNOWN( op_arg[3] <= arg_var_bound);
				if( op_arg[1] & 4 )
					CPPAD_ASSERT_UNKNOWN( op_arg[4] <= arg_var_bound);
				if( op_arg[1] & 8 )
					CPPAD_ASSERT_UNKNOWN( op_arg[5] <= arg_var_bound);
				break;

				// PriOp
				case PriOp:
				if( op_arg[0] & 1 )
					CPPAD_ASSERT_UNKNOWN( op_arg[1] <= arg_var_bound);
				if( op_arg[0] & 2 )
					CPPAD_ASSERT_UNKNOWN( op_arg[3] <= arg_var_bound);
				break;

				// CSkipOp
				case CSkipOp:
				if( op_arg[1] & 1 )
					CPPAD_ASSERT_UNKNOWN( op_arg[2] <= arg_var_bound);
				if( op_arg[1] & 2 )
					CPPAD_ASSERT_UNKNOWN( op_arg[3] <= arg_var_bound);
				itr.correct_before_increment();
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
				break;


			}
			if( NumRes(op) > 0 )
			{	if( var_index > 0 )
				{	CPPAD_ASSERT_UNKNOWN(size_t(arg_var_bound) < var_index);
				}
				else
				{	CPPAD_ASSERT_UNKNOWN(size_t(arg_var_bound) == var_index);
				}
				//
				arg_var_bound = addr_t(var_index);
			}
		}
	}
# endif
	// ===============================================================
	/*!
	Copying an operation sequence from another player to this one

	\param play
	the object that contains the operatoion sequence to copy.
	*/
	void operator=(const player& play)
	{
		num_dynamic_        = play.num_dynamic_;
		num_var_rec_        = play.num_var_rec_;
		num_load_op_rec_    = play.num_load_op_rec_;
		op_vec_             = play.op_vec_;
		num_vecad_vec_rec_  = play.num_vecad_vec_rec_;
		vecad_ind_vec_      = play.vecad_ind_vec_;
		arg_vec_            = play.arg_vec_;
		par_vec_            = play.par_vec_;
		text_vec_           = play.text_vec_;
		op2arg_vec_         = play.op2arg_vec_;
		op2var_vec_         = play.op2var_vec_;
		var2op_vec_         = play.var2op_vec_;
	}
	// ===============================================================
	/// Erase the recording stored in the player
	void Erase(void)
	{
		num_dynamic_       = 0;
		num_var_rec_       = 0;
		num_load_op_rec_   = 0;
		num_vecad_vec_rec_ = 0;

		op_vec_.resize(0);
		vecad_ind_vec_.resize(0);
		arg_vec_.resize(0);
		par_vec_.resize(0);
		text_vec_.resize(0);
		op2arg_vec_.resize(0);
		op2var_vec_.resize(0);
		var2op_vec_.resize(0);
	}
	// =================================================================
	/// Enable use of const_subgraph_iterator and member functions that begin
	// with random_(no work if already setup).
	template <class Addr>
	void setup_random(void)
	{	play::random_setup(
			num_var_rec_                               ,
			op_vec_                                    ,
			arg_vec_                                   ,
			op2arg_vec_.pod_vector_ptr<Addr>()         ,
			op2var_vec_.pod_vector_ptr<Addr>()         ,
			var2op_vec_.pod_vector_ptr<Addr>()
		);
	}
	/// Free memory used for functions that begin with random_
	/// and random iterators and subgraph iterators
	void clear_random(void)
	{
		op2arg_vec_.clear();
		op2var_vec_.clear();
		var2op_vec_.clear();
		CPPAD_ASSERT_UNKNOWN( op2arg_vec_.size() == 0  );
		CPPAD_ASSERT_UNKNOWN( op2var_vec_.size() == 0  );
		CPPAD_ASSERT_UNKNOWN( var2op_vec_.size() == 0  );
	}
	/// Set a dynamic parameter value
	void set_dynamic(size_t i, const Base& par)
	{	CPPAD_ASSERT_UNKNOWN( i < num_dynamic_ );
		par_vec_[i] = par;
	}
	// ================================================================
	// const functions that retrieve infromation from this player
	// ================================================================
	/*!
	\brief
	fetch an operator from the recording.

	\return
	the i-th operator in the recording.

	\param i
	the index of the operator in recording
	*/
	OpCode GetOp (size_t i) const
	{	return OpCode(op_vec_[i]); }

	/*!
	\brief
	Fetch a VecAD index from the recording.

	\return
	the i-th VecAD index in the recording.

	\param i
	the index of the VecAD index in recording
	*/
	size_t GetVecInd (size_t i) const
	{	return vecad_ind_vec_[i]; }

	/*!
	\brief
	Fetch a parameter from the recording.

	\return
	the i-th parameter in the recording.

	\param i
	the index of the parameter in recording
	*/
	Base GetPar(size_t i) const
	{	return par_vec_[i]; }

	/*!
	\brief
	Fetch entire parameter vector from the recording.

	\return
	the entire parameter vector.

	*/
	const Base* GetPar(void) const
	{	return par_vec_.data(); }

	/*!
	\brief
	Fetch a '\\0' terminated string from the recording.

	\return
	the beginning of the string.

	\param i
	the index where the string begins.
	*/
	const char *GetTxt(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < text_vec_.size() );
		return text_vec_.data() + i;
	}

	/// Fetch number of dynamic parameters in the recording
	size_t num_dynamic(void) const
	{	return num_dynamic_; }

	/// Fetch number of variables in the recording.
	size_t num_var_rec(void) const
	{	return num_var_rec_; }

	/// Fetch number of vecad load operations
	size_t num_load_op_rec(void) const
	{	return num_load_op_rec_; }

	/// Fetch number of operators in the recording.
	size_t num_op_rec(void) const
	{	return op_vec_.size(); }

	/// Fetch number of VecAD indices in the recording.
	size_t num_vec_ind_rec(void) const
	{	return vecad_ind_vec_.size(); }

	/// Fetch number of VecAD vectors in the recording
	size_t num_vecad_vec_rec(void) const
	{	return num_vecad_vec_rec_; }

	/// Fetch number of argument indices in the recording.
	size_t num_op_arg_rec(void) const
	{	return arg_vec_.size(); }

	/// Fetch number of parameters in the recording.
	size_t num_par_rec(void) const
	{	return par_vec_.size(); }

	/// Fetch number of characters (representing strings) in the recording.
	size_t num_text_rec(void) const
	{	return text_vec_.size(); }

	/// A measure of amount of memory used to store
	/// the operation sequence, just lengths, not capacities.
	/// In user api as f.size_op_seq(); see the file seq_property.omh.
	size_t size_op_seq(void) const
	{	// check assumptions made by ad_fun<Base>::size_op_seq()
		CPPAD_ASSERT_UNKNOWN( op_vec_.size() == num_op_rec() );
		CPPAD_ASSERT_UNKNOWN( arg_vec_.size()    == num_op_arg_rec() );
		CPPAD_ASSERT_UNKNOWN( par_vec_.size() == num_par_rec() );
		CPPAD_ASSERT_UNKNOWN( text_vec_.size() == num_text_rec() );
		CPPAD_ASSERT_UNKNOWN( vecad_ind_vec_.size() == num_vec_ind_rec() );
		return op_vec_.size()        * sizeof(opcode_t)
		     + arg_vec_.size()       * sizeof(addr_t)
		     + par_vec_.size()       * sizeof(Base)
		     + text_vec_.size()      * sizeof(char)
		     + vecad_ind_vec_.size() * sizeof(addr_t)
		;
	}
	/// A measure of amount of memory used for random access routine
	/// In user api as f.size_random(); see the file seq_property.omh.
	size_t size_random(void) const
	{
# ifndef NDEBUG
		if( op2arg_vec_.size() == 0 )
		{	CPPAD_ASSERT_UNKNOWN( op2var_vec_.size() == 0 );
			CPPAD_ASSERT_UNKNOWN( var2op_vec_.size() == 0 );
		}
		else
		{	size_t size = 0;
			switch( address_type() )
			{	case play::unsigned_short_enum:
				size = sizeof(unsigned short);
				break;
				//
				case play::unsigned_int_enum:
				size = sizeof(unsigned int);
				break;
				//
				case play::size_t_enum:
				size = sizeof(size_t);
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
				break;
			}
			CPPAD_ASSERT_UNKNOWN( op2arg_vec_.size()/size  == num_op_rec() );
			CPPAD_ASSERT_UNKNOWN( op2var_vec_.size()/size  == num_op_rec() );
			CPPAD_ASSERT_UNKNOWN( var2op_vec_.size()/size  == num_var_rec() );
		}
# endif
		CPPAD_ASSERT_UNKNOWN( sizeof(unsigned char) == 1 );
		return op2arg_vec_.size()
		     + op2var_vec_.size()
		     + var2op_vec_.size()
		;
	}
	// -----------------------------------------------------------------------
	/// const sequential iterator begin
	play::const_sequential_iterator begin(void) const
	{	size_t op_index = 0;
		size_t num_var  = num_var_rec_;
		return play::const_sequential_iterator(
			num_var, &op_vec_, &arg_vec_, op_index
		);
	}
	/// const sequential iterator end
	play::const_sequential_iterator end(void) const
	{	size_t op_index = op_vec_.size() - 1;
		size_t num_var  = num_var_rec_;
		return play::const_sequential_iterator(
			num_var, &op_vec_, &arg_vec_, op_index
		);
	}
	// -----------------------------------------------------------------------
	/// const subgraph iterator begin
	play::const_subgraph_iterator<addr_t>  begin_subgraph(
		const play::const_random_iterator<addr_t>& random_itr ,
		const pod_vector<addr_t>*                  subgraph   ) const
	{	size_t subgraph_index = 0;
		return play::const_subgraph_iterator<addr_t>(
			random_itr,
			subgraph,
			subgraph_index
		);
	}
	/// const subgraph iterator end
	template <class Addr>
	play::const_subgraph_iterator<Addr>  end_subgraph(
		const play::const_random_iterator<Addr>&   random_itr ,
		const pod_vector<addr_t>*                  subgraph   ) const
	{	size_t subgraph_index = subgraph->size() - 1;
		return play::const_subgraph_iterator<Addr>(
			random_itr,
			subgraph,
			subgraph_index
		);
	}
	// -----------------------------------------------------------------------
	/// const random iterator
	template <class Addr>
	play::const_random_iterator<Addr> get_random(void) const
	{	return play::const_random_iterator<Addr>(
			op_vec_,
			arg_vec_,
			op2arg_vec_.pod_vector_ptr<Addr>(),
			op2var_vec_.pod_vector_ptr<Addr>(),
			var2op_vec_.pod_vector_ptr<Addr>()
		);
	}
};

} } // END_CPPAD_lOCAL_NAMESPACE
# endif
