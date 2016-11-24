// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_USAGE_HPP
# define CPPAD_LOCAL_OPTIMIZE_USAGE_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file usage.hpp
Create operator information tables
*/

// BEGIN_CPPAD_LOCAL__OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/*!
Compute the operator usage.

\param compare_op
if this is true, arguments are considered used if they appear in compare
operators. This is a side effect because compare operators have boolean
results (and the result is not in the tape; i.e. NumRes(op) is zero
for these operators. (This is an example of a side effect.)

\param num_par
is the number of parameters in the tape.

\param parameter
is a vector of lenght num_par containing the parameter values.

\param num_vecad_ind
is the number of vecad indices. This is the sume of the lenghts of the
vectors plus the number of vectors (one vecad index for each vector element
plus one vecad index for the length of each vector).

\param vecad_length
is a vector with size equal to the number of VecAD vectors in the operation
sequence. The value vecad_length[i] is the lenght of the i-th VecAD object
in the recording.

\param dep_taddr
is a vector of variable indices for the dependent variables.

\param op2info
is the operator information for this operation sequence.

\param var2op
maps a variable index to the operator that created the variable.
This is only true for the primary variables.
If the index i_var corresponds to an auxillary variable, var2op[i_var]
is equalt to num_op (which is not a valid operator index).

\param op2usage
maps a operator index to the corresponding enum_usage value.
This size of this vector must be equal to the number of operators.
The input value of its eleemnts does not matter. Upon return,
each operator is mapped to a flag that idenfies if its use case.
Note the BeginOp and EndOp are always used once.
*/
template <class Base>
void get_usage(
	bool                      compare_op    ,
	size_t                    num_par       ,
	const Base*               parameter     ,
	size_t                    num_vecad_ind ,
	const vector<size_t>&     vecad_length  ,
	const vector<size_t>&     dep_taddr     ,
	const vector<op_info>&    op2info       ,
	const vector<size_t>&     var2op        ,
	vector<size_t>&           op2usage      )
{
	// number of VecAD vectors
	const size_t num_vecad = vecad_length.size();

	// number of operators in the tape
	const size_t num_op = op2info.size();
	CPPAD_ASSERT_UNKNOWN( op2usage.size() == num_op );
	//
	// number of dependent variables in the tape
	const size_t num_dep = dep_taddr.size();
	//
	// initialize op2usage
	for(size_t i = 0; i < num_op; i++)
		op2usage[i] = 0;
	for(size_t i = 0; i < num_dep; i++)
	{	size_t i_var   = size_t( dep_taddr[i] );
		size_t i_op    = var2op[i_var];
		op2usage[i_op] = 1;
	}
	op2usage[0] = op2usage[num_op-1] = 1;
	//
	// initialize vecad_usage
	vector<size_t> vecad_usage(num_vecad);
	for(size_t i = 0; i < num_vecad; i++)
		vecad_usage[i] = 0;

	// initialize mapping from arg[0] value to index in vecad_usage.
	vector<size_t> arg2vecad(num_vecad_ind);
	for(size_t i = 0; i < num_vecad_ind; i++)
		arg2vecad[i] = num_vecad; // invalid index into vecad_usage
	size_t arg_0 = 1; // value of arg[0] for first vector
	for(size_t i = 0; i < num_vecad; i++)
	{	arg2vecad[arg_0] = i;
		arg_0           += 1 + vecad_length[i];
	}
	CPPAD_ASSERT_UNKNOWN( arg_0 == num_vecad_ind + 1 );

	// ---------------------------------------------------------------------
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
	//
	size_t user_index=0, user_old=0, user_m=0, user_n=0, user_i=0, user_j=0;
	enum_user_state user_state;
	// ---------------------------------------------------------------------
	//
	// variables that for current operator
	OpCode        op;     // operator
	const addr_t* arg;    // arguments
	//
	user_state  = end_user;
	size_t i_op = num_op;
	while( i_op > 0 )
	{	size_t j_op;  // temporary operator index
		bool   use;   // a temporary usage variable
		//
		--i_op;
		//
		// next op
		op    = op2info[i_op].op;
		arg   = op2info[i_op].arg;
		//
		// is the result of this operation used
		size_t use_result = op2usage[i_op];
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
				++op2usage[j_op];
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
				++op2usage[j_op];
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
				++op2usage[j_op];
				j_op = var2op[ arg[1] ];
				++op2usage[j_op];
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
						++op2usage[j_op];
					}
				}
			}
			break;  // --------------------------------------------

			// Operations where there is nothing to do
			case BeginOp:
			case EndOp:
			case ParOp:
			case PriOp:
			break;  // -----------------------------------------------

			// Operators that always get used
			case InvOp:
			++op2usage[i_op];
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
				++op2usage[j_op];
			}
			break; // ----------------------------------------------

			// Compare operators where arg[0] is only variable
			case LevpOp:
			case LtvpOp:
			if( compare_op )
			{	j_op = var2op[ arg[0] ];
				++op2usage[j_op];
			}
			break; // ----------------------------------------------

			// Compare operators where arg[0] and arg[1] are variables
			case LevvOp:
			case LtvvOp:
			case EqvvOp:
			case NevvOp:
			if( compare_op ) for(size_t i = 0; i < 2; i++)
			{	j_op = var2op[ arg[i] ];
				++op2usage[j_op];
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
				++op2usage[j_op];
			}
			break; // --------------------------------------------

			// Store a variable using a parameter index
			case StpvOp:
			use = vecad_usage[ arg2vecad[ arg[0] ] ];
			if( use )
			{	j_op = var2op[ arg[2] ];
				++op2usage[j_op];
			}
			break; // --------------------------------------------

			// Store a variable using a variable index
			case StvvOp:
			use = vecad_usage[ arg2vecad[ arg[0] ] ];
			if( use )
			{	j_op = var2op[ arg[1] ];
				++op2usage[j_op];
				j_op = var2op[ arg[2] ];
				++op2usage[j_op];
			}
			break; // -----------------------------------------------------

			// =============================================================
			// user defined atomic operators
			// ============================================================

			case UserOp:
			// start or end atomic operation sequence
			if( user_state == end_user )
			{	// reverse_user ------------------------------------------
				user_index = arg[0];
				user_old   = arg[1];
				user_n     = arg[2];
				user_m     = arg[3];
				CPPAD_ASSERT_UNKNOWN( user_n > 0 );

				// other reverse_user arguments
				user_j     = user_n;
				user_i     = user_m;
				user_state = ret_user;

				// the atomic_base object corresponding to this user function
				user_atom = atomic_base<Base>::class_object(user_index);
# ifndef NDEBUG
				if( user_atom == CPPAD_NULL )
				{	std::string msg =
						atomic_base<Base>::class_name(user_index)
						+ ": atomic_base function has been deleted";
					CPPAD_ASSERT_KNOWN(false, msg.c_str() );
				}
# endif
				// -------------------------------------------------------
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
			{	// reverse_user
				CPPAD_ASSERT_UNKNOWN( user_state == start_user );
				user_state = end_user;
				//
				// call users function for this operation
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
							++op2usage[ var2op[ user_ix[j] ] ];
					}
					if( user_bool )
					{	if( user_s_bool[j] )
							++op2usage[ var2op[ user_ix[j] ] ];
					}
					if( user_pack )
					{	if( user_s_pack[j] )
							++op2usage[ var2op[ user_ix[j] ] ];
					}
				}
			}
			break; // -------------------------------------------------------

			case UsrapOp:
			// parameter argument in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
			//
			// reverse_user
			CPPAD_ASSERT_UNKNOWN( user_state == arg_user );
			CPPAD_ASSERT_UNKNOWN( 0 < user_j )
			--user_j;
			if( user_j == 0 )
				user_state = start_user;
			//
			user_ix[user_j] = 0;
			//
			// parameter arguments
			user_x[user_j] = parameter[arg[0]];
			//
			break;

			case UsravOp:
			// variable argument in an atomic operation sequence
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) <= op2info[i_op].i_var );
			CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
			//
			// reverse_user
			CPPAD_ASSERT_UNKNOWN( user_state == arg_user );
			CPPAD_ASSERT_UNKNOWN( 0 < user_j )
			--user_j;
			if( user_j == 0 )
				user_state = start_user;
			//
			user_ix[user_j] = arg[0];
			//
			// variable arguments as parameters
			user_x[user_j] = CppAD::numeric_limits<Base>::quiet_NaN();
			//
			break;

			case UsrrvOp:
			// variable result in an atomic operation sequence
			//
			// reverse_user
			CPPAD_ASSERT_UNKNOWN( user_state == ret_user );
			CPPAD_ASSERT_UNKNOWN( 0 < user_i )
			--user_i;
			if( user_i == 0 )
				user_state = arg_user;
			//
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
			// reverse_user
			CPPAD_ASSERT_UNKNOWN( user_state == ret_user );
			CPPAD_ASSERT_UNKNOWN( 0 < user_i )
			--user_i;
			if( user_i == 0 )
				user_state = arg_user;
			break;
			// ============================================================

			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(0);
		}
	}
	return;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
