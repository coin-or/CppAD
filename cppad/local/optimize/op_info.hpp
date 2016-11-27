// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_OP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_OP_INFO_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file op_info.hpp
Create operator information tables
*/

// BEGIN_CPPAD_LOCAL__OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/// information for one operator
struct struct_op_info {
	/// op code
	OpCode        op;

	/// arguments
	const addr_t* arg;

	/// Primary (not auxillary) variable index for this operator. If the
	// operator has not results, this is num_var (an invalid variable index).
	size_t        i_var;

	/// Number of times an operator is used. This counts using the results
	/// as well as other factors. Being a dependent variable or independent
	/// varialbe gets one use count. Certain other side effects can get a
	// use count. For example, comparision operators may or may not be counted.
	size_t        usage;

	/// Is this variable use only once, is its parrent a summation,
	/// and is it a summation. In this case it can be removed as part
	/// of a cumulative summation starting at its parent or above.
	bool          csum_connected;
};

/*!
Get variable to operator map and operator basic operator information

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type
\a Base.

\param compare_op
if this is true, arguments are considered used if they appear in compare
operators. This is a side effect because compare operators have boolean
results (and the result is not in the tape; i.e. NumRes(op) is zero
for these operators. (This is an example of a side effect.)

\param play
This is the operation sequence.
It is essentially const, except for play back state which
changes while it plays back the operation seqeunce.

\param dep_taddr
is a vector of variable indices for the dependent variables.

\param var2op
The size of this vector must be equal to the number of variables
in the operation sequence; i.e., num_var = play->nun_var_rec().
The input value of its elements does not matter. Upon return it
maps a variable index to the operator that created the variable.
This is only true for the primary variables.
If the index i_var corresponds to an auxillary variable, var2op[i_var]
is equalt to num_op (which is not a valid operator index).

\param op_info
The size of this vector must be equal to the number of operators
in the operation sequence; i.e., num_op = play->nun_var_rec().
The input value of its elements does not matter. Upon return the value of
have been set to the values corresponding to the operation sequence.
*/

template <class Base>
void get_op_info(
	bool                          compare_op          ,
	player<Base>*                 play                ,
	const vector<size_t>&         dep_taddr           ,
	vector<size_t>&               var2op              ,
	vector<struct_op_info>&       op_info             )
{
	// number of operators in the tape
	const size_t num_op = play->num_op_rec();
	CPPAD_ASSERT_UNKNOWN( op_info.size() == num_op );
	//
	// number of variables in the tape
	const size_t num_var = play->num_var_rec();
	//
	// initialize mapping from variable index to operator index
	CPPAD_ASSERT_UNKNOWN( var2op.size() == num_var );
	for(size_t i = 0; i < num_var; i++)
		var2op[i] = num_op; // invalid (used for auxillary variables)
	//
	// information set by forward_user
	size_t user_old=0, user_m=0, user_n=0, user_i=0, user_j=0;
	enum_user_state user_state;
	//
	// information set by forward_next
	OpCode        op;     // operator
	const addr_t* arg;    // arguments
	size_t        i_op;   // operator index
	size_t        i_var;  // variable index of first result
	//
	// ----------------------------------------------------------------------
	// Forward pass to compute op, arg, i_var for each operator and var2op
	// ----------------------------------------------------------------------
	play->forward_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op              == BeginOp );
	CPPAD_ASSERT_UNKNOWN( NumRes(BeginOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_op            == 0 );
	CPPAD_ASSERT_UNKNOWN( i_var           == 0 );
	op_info[i_op].op    = op;
	op_info[i_op].arg   = arg;
	op_info[i_op].i_var = i_var;
	//
	// This variaible index, 0, is automatically created, but not used because
	// it represents a paraemeter during the recording process. So we set
	var2op[i_var] = num_op;
	//
	user_state = start_user;
	while(op != EndOp)
	{	// next operator
		play->forward_next(op, arg, i_op, i_var);
		//
		// information for this operator
		op_info[i_op].op    = op;
		op_info[i_op].arg   = arg;
		op_info[i_op].i_var = i_var;
		//
		// mapping from variable index to operator index
		if( NumRes(op) > 0 )
			var2op[i_var] = i_op;
		//
		switch( op )
		{	case CSumOp:
			// must correct arg before next operator
			play->forward_csum(op, arg, i_op, i_var);
			break;

			case CSkipOp:
			// must correct arg before next operator
			play->forward_csum(op, arg, i_op, i_var);
			break;

			case UserOp:
			case UsrapOp:
			case UsravOp:
			case UsrrpOp:
			case UsrrvOp:
			play->forward_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			break;

			default:
			break;
		}
	}
	//
	// ----------------------------------------------------------------------
	// Reverse pass to compute usage for each operator
	// ----------------------------------------------------------------------
	//
	// work space used by user defined atomic functions
	typedef std::set<size_t> size_set;
	vector<Base>     user_x;       // parameters in x as integers
	vector<size_t>   user_ix;      // variables indices for argument vector
	vector<size_set> user_r_set;   // set sparsity pattern for result
	vector<size_set> user_s_set;   // set sparisty pattern for argument
	vector<bool>     user_r_bool;  // bool sparsity pattern for result
	vector<bool>     user_s_bool;  // bool sparisty pattern for argument
	vectorBool       user_r_pack;  // pack sparsity pattern for result
	vectorBool       user_s_pack;  // pack sparisty pattern for argument
	//
	atomic_base<Base>* user_atom = CPPAD_NULL; // current user atomic function
	bool               user_pack = false;      // sparsity pattern type is pack
	bool               user_bool = false;      // sparsity pattern type is bool
	bool               user_set  = false;      // sparsity pattern type is set
	// -----------------------------------------------------------------------
	// vecad information
	size_t num_vecad      = play->num_vecad_vec_rec();
	size_t num_vecad_ind  = play->num_vec_ind_rec();
	//
	vector<size_t> vecad_usage(num_vecad);
	for(size_t i = 0; i < num_vecad; i++)
		vecad_usage[i] = 0;
	//
	vector<size_t> arg2vecad(num_vecad_ind);
	for(size_t i = 0; i < num_vecad_ind; i++)
		arg2vecad[i] = num_vecad; // invalid value
	size_t arg_0 = 1; // value of arg[0] for theh first vecad
	for(size_t i = 0; i < num_vecad; i++)
	{	arg2vecad[arg_0] = i; // from arg[0] value to index in vecad_usage
		// length of this vecad object
		size_t length = play->GetVecInd(arg_0 - 1);
		// set to proper index for this VecAD
		// next arg[0] value
		arg_0        += length + 1;
	}
	CPPAD_ASSERT_UNKNOWN( arg_0 == num_vecad_ind + 1 );
	// -----------------------------------------------------------------------
	//
	// parameter information (used by atomic function calls)
	size_t num_par = play->num_par_rec();
	const Base* parameter = CPPAD_NULL;
	if( num_par > 0 )
		parameter = play->GetPar();
	//
	// initialize operator usage
	for(size_t i = 0; i < num_op; i++)
		op_info[i].usage = 0;
	for(size_t i = 0; i < dep_taddr.size(); i++)
	{	i_op                = var2op[ dep_taddr[i] ];
		op_info[i_op].usage = 1;
	}
	// value for BeginOp and EndOp
	CPPAD_ASSERT_UNKNOWN( op_info[0].op == BeginOp);
	CPPAD_ASSERT_UNKNOWN( op_info[num_op-1].op == EndOp);
	op_info[0].usage        = 1;
	op_info[num_op-1].usage = 1;
	//
	// Initialize reverse pass
	user_state = end_user;
	play->reverse_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == EndOp );
	op_info[i_op].usage = 1;
	while( op != BeginOp )
	{	bool   flag;  // temporary boolean value
		size_t j_op;  // temporary operator index
		bool   use;   // a temporary usage variable
		//
		// next op
		play->reverse_next(op, arg, i_op, i_var);
		//
		// is the result of this operation used
		size_t use_result = op_info[i_op].usage;
		switch( op )
		{
			// =============================================================
			// normal operators
			// =============================================================

			// Only one variable with index arg[0]
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
			case LogOp:
			case Log1pOp:
			case PowvpOp:
			case SignOp:
			case SinOp:
			case SinhOp:
			case SqrtOp:
			case SubvpOp:
			case TanOp:
			case TanhOp:
			case ZmulvpOp:
			if( use_result > 0 )
			{	j_op = var2op[ arg[0] ];
				++op_info[j_op].usage;
			}
			break; // --------------------------------------------

			// Only one variable with index arg[1]
			case AddpvOp:
			case DisOp:
			case DivpvOp:
			case MulpvOp:
			case PowpvOp:
			case SubpvOp:
			case ZmulpvOp:
			if( use_result > 0 )
			{	j_op = var2op[ arg[1] ];
				++op_info[j_op].usage;
			}
			break; // --------------------------------------------

			// arg[0] and arg[1] are the only variables
			case AddvvOp:
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case SubvvOp:
			case ZmulvvOp:
			if( use_result > 0 )
			{	j_op = var2op[ arg[0] ];
				++op_info[j_op].usage;
				j_op = var2op[ arg[1] ];
				++op_info[j_op].usage;
			}
			break; // --------------------------------------------

			// Conditional expression operators
			// arg[2], arg[3], arg[4], arg[5] are parameters or variables
			case CExpOp:
			if( use_result > 0 )
			{	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
				addr_t mask[] = {1, 2, 4, 8};
				for(size_t i = 0; i < 4; i++)
				{	if( arg[1] & mask[i] )
					{	j_op = var2op[ arg[2 + i] ];
						++op_info[j_op].usage;
					}
				}
			}
			break;  // --------------------------------------------

			// Operations where there is nothing to do
			case CSkipOp:
			play->reverse_cskip(op, arg, i_op, i_var);
			//
			case BeginOp: // set during initialization of usage
			case EndOp:   // set during initialization of usage
			case ParOp:
			case PriOp:
			break;  // -----------------------------------------------

			// These operats get an extra count for being indpendent variables
			case InvOp:
			++op_info[i_op].usage;
			break; // -----------------------------------------------

			// =============================================================
			// Comparison operators
			// =============================================================

			// Compare operators where arg[1] is only variable
			case LepvOp:
			case LtpvOp:
			case EqpvOp:
			case NepvOp:
			if( compare_op )
			{	j_op = var2op[ arg[1] ];
				++op_info[j_op].usage;
			}
			break; // ----------------------------------------------

			// Compare operators where arg[0] is only variable
			case LevpOp:
			case LtvpOp:
			if( compare_op )
			{	j_op = var2op[ arg[0] ];
				++op_info[j_op].usage;
			}
			break; // ----------------------------------------------

			// Compare operators where arg[0] and arg[1] are variables
			case LevvOp:
			case LtvvOp:
			case EqvvOp:
			case NevvOp:
			if( compare_op ) for(size_t i = 0; i < 2; i++)
			{	j_op = var2op[ arg[i] ];
				++op_info[j_op].usage;
			}
			break; // ----------------------------------------------

			// =============================================================
			// VecAD operators
			// =============================================================

			// load operator using a parameter index
			case LdpOp:
			if( use_result > 0 )
			{	size_t i_vec = arg2vecad[ arg[0] ];
				++vecad_usage[i_vec];
			}
			break; // --------------------------------------------

			// load operator using a variable index
			case LdvOp:
			if( use_result > 0 )
			{	size_t i_vec = arg2vecad[ arg[0] ];
				++vecad_usage[i_vec];
				//
				j_op = var2op[ arg[1] ];
				++op_info[j_op].usage;
			}
			break; // --------------------------------------------

			// Store a variable using a parameter index
			case StpvOp:
			use = vecad_usage[ arg2vecad[ arg[0] ] ];
			if( use )
			{	j_op = var2op[ arg[2] ];
				++op_info[j_op].usage;
			}
			break; // --------------------------------------------

			// Store a variable using a variable index
			case StvvOp:
			use = vecad_usage[ arg2vecad[ arg[0] ] ];
			if( use )
			{	j_op = var2op[ arg[1] ];
				++op_info[j_op].usage;
				j_op = var2op[ arg[2] ];
				++op_info[j_op].usage;
			}
			break; // -----------------------------------------------------

			// =============================================================
			// cumuilative summation operator
			// ============================================================
			case CSumOp:
			play->reverse_csum(op, arg, i_op, i_var);
			{	size_t num_add = size_t( arg[0] );
				size_t num_sub = size_t( arg[1] );
				for(size_t i = 0; i < num_add + num_sub; i++)
				{	j_op = var2op[ arg[3 + i] ];
					++op_info[j_op].usage;
				}
			}
			// =============================================================
			// user defined atomic operators
			// ============================================================

			case UserOp:
			// start or end atomic operation sequence
			flag      = user_state == end_user;
			user_atom = play->reverse_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			if( flag )
			{	// -------------------------------------------------------
				user_x.resize(  user_n );
				user_ix.resize( user_n );
				//
				user_pack  = user_atom->sparsity() ==
							atomic_base<Base>::pack_sparsity_enum;
				user_bool  = user_atom->sparsity() ==
							atomic_base<Base>::bool_sparsity_enum;
				user_set   = user_atom->sparsity() ==
							atomic_base<Base>::set_sparsity_enum;
				CPPAD_ASSERT_UNKNOWN( user_pack || user_bool || user_set );
				//
				// Note that q is one for this call the sparsity calculation
				if( user_pack )
				{	user_r_pack.resize( user_m );
					user_s_pack.resize( user_n );
					for(size_t i = 0; i < user_m; i++)
						user_r_pack[ i ] = false;
				}
				if( user_bool )
				{	user_r_bool.resize( user_m );
					user_s_bool.resize( user_n );
					for(size_t i = 0; i < user_m; i++)
						user_r_bool[ i ] = false;
				}
				if( user_set )
				{	user_s_set.resize(user_n);
					user_r_set.resize(user_m);
					for(size_t i = 0; i < user_m; i++)
						user_r_set[i].clear();
				}
			}
			else
			{	// call users function for this operation
				user_atom->set_old(user_old);
				bool user_ok  = false;
				size_t user_q = 1; // as if sum of dependent variables
				if( user_pack )
				{	user_ok = user_atom->rev_sparse_jac(
						user_q, user_r_pack, user_s_pack, user_x
					);
					if( ! user_ok ) user_ok = user_atom->rev_sparse_jac(
						user_q, user_r_pack, user_s_pack
					);
				}
				if( user_bool )
				{	user_ok = user_atom->rev_sparse_jac(
						user_q, user_r_bool, user_s_bool, user_x
					);
					if( ! user_ok ) user_ok = user_atom->rev_sparse_jac(
						user_q, user_r_bool, user_s_bool
					);
				}
				if( user_set )
				{	user_ok = user_atom->rev_sparse_jac(
						user_q, user_r_set, user_s_set, user_x
					);
					if( ! user_ok ) user_ok = user_atom->rev_sparse_jac(
						user_q, user_r_set, user_s_set
					);
				}
				if( ! user_ok )
				{	std::string s =
						"Optimizing an ADFun object"
						" that contains the atomic function\n\t";
					s += user_atom->afun_name();
					s += "\nCurrent atomic_sparsity is set to ";
					//
					if( user_set )
						s += "set_sparsity_enum.\n";
					if( user_bool )
						s += "bool_sparsity_enum.\n";
					if( user_pack )
						s += "pack_sparsity_enum.\n";
					//
					s += "This version of rev_sparse_jac returned false";
					CPPAD_ASSERT_KNOWN(false, s.c_str() );
				}
				// 2DO: It might be faster if we add set union to var_sparsity
				// where one of the sets is not in var_sparsity.
				for(size_t j = 0; j < user_n; j++) if( user_ix[j] > 0 )
				{	// This user argument is a variable
					if( user_set )
					{	if( ! user_s_set[j].empty() )
							++op_info[ var2op[ user_ix[j] ] ].usage;
					}
					if( user_bool )
					{	if( user_s_bool[j] )
							++op_info[ var2op[ user_ix[j] ] ].usage;
					}
					if( user_pack )
					{	if( user_s_pack[j] )
							++op_info[ var2op[ user_ix[j] ] ].usage;
					}
				}
			}
			break; // -------------------------------------------------------

			case UsrapOp:
			// parameter argument in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			//
			play->reverse_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			//
			user_ix[user_j] = 0;
			//
			// parameter arguments
			user_x[user_j] = parameter[arg[0]];
			//
			break;

			case UsravOp:
			// variable argument in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) <= op_info[i_op].i_var );
			CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
			//
			play->reverse_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			user_ix[user_j] = arg[0];
			//
			// variable arguments as parameters
			user_x[user_j] = CppAD::numeric_limits<Base>::quiet_NaN();
			//
			break;

			case UsrrvOp:
			// variable result in an atomic operation sequence
			//
			play->reverse_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			if( use_result )
			{	if( user_set )
					user_r_set[user_i].insert(0);
				if( user_bool )
					user_r_bool[user_i] = true;
				if( user_pack )
					user_r_pack[user_i] = true;
			}
			break; // --------------------------------------------------------

			case UsrrpOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			play->reverse_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			break;
			// ============================================================

			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
	// ----------------------------------------------------------------------
	// Forward (could use revese) pass to compute csum_connected
	// ----------------------------------------------------------------------
	for(size_t i = 0; i < num_op; i++)
		op_info[i].csum_connected = false;
	//
	play->forward_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
	//
	user_state = start_user;
	while(op != EndOp)
	{
		// next operator
		play->forward_next(op, arg, i_op, i_var);
		//
		switch( op )
		{	case CSumOp:
			// must correct arg before next operator
			play->forward_csum(op, arg, i_op, i_var);
# ifndef NDEBUG
			{	size_t num_add = size_t( arg[0] );
				size_t num_sub = size_t( arg[1] );
				for(size_t i = 0; i < num_add + num_sub; i++)
				{	size_t j_op = var2op[ arg[3 + i] ];
					CPPAD_ASSERT_UNKNOWN( op_info[j_op].usage != 1 );
				}
			}
# endif
			break; // --------------------------------------------------------

			case CSkipOp:
			// must correct arg before next operator
			play->forward_csum(op, arg, i_op, i_var);
			break; // --------------------------------------------------------

			case UserOp:
			case UsrapOp:
			case UsravOp:
			case UsrrpOp:
			case UsrrvOp:
			play->forward_user(op, user_state,
				user_old, user_m, user_n, user_i, user_j
			);
			break; // --------------------------------------------------------

			case AddvvOp:
			case SubvvOp:
			for(size_t i = 0; i < 2; i++)
			{	size_t j_op = var2op[ arg[i] ];
				switch( op_info[j_op].op )
				{
					case AddpvOp:
					case AddvvOp:
					case SubpvOp:
					case SubvpOp:
					case SubvvOp:
					if( op_info[j_op].usage == 1 )
					{	CPPAD_ASSERT_UNKNOWN( ! op_info[j_op].csum_connected );
						op_info[j_op].csum_connected = true;
					}
					break;

					default:
					break;
				}
			}
			break; // --------------------------------------------------------

			case AddpvOp:
			case SubpvOp:
			{
				size_t j_op = var2op[ arg[1] ];
				switch( op_info[j_op].op )
				{
					case AddpvOp:
					case AddvvOp:
					case SubpvOp:
					case SubvpOp:
					case SubvvOp:
					if( op_info[j_op].usage == 1 )
					{	CPPAD_ASSERT_UNKNOWN( ! op_info[j_op].csum_connected );
						op_info[j_op].csum_connected = true;
					}
					break;

					default:
					break;
				}
			}
			break; // --------------------------------------------------------

			case SubvpOp:
			{	size_t j_op = var2op[ arg[0] ];
				switch( op_info[j_op].op )
				{
					case AddpvOp:
					case AddvvOp:
					case SubpvOp:
					case SubvpOp:
					case SubvvOp:
					if( op_info[j_op].usage == 1 )
					{	CPPAD_ASSERT_UNKNOWN( ! op_info[j_op].csum_connected );
						op_info[j_op].csum_connected = true;
					}
					break;

					default:
					break;
				}
			}
			break; // --------------------------------------------------------


			default:
			break;
		}
	}
	return;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
