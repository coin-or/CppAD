// $Id$

# ifndef CPPAD_LOCAL_OPTIMIZE_OPTIMIZE_RUN_HPP
# define CPPAD_LOCAL_OPTIMIZE_OPTIMIZE_RUN_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <stack>
# include <iterator>
# include <cppad/local/optimize/op_info.hpp>
# include <cppad/local/optimize/connect_type.hpp>
# include <cppad/local/optimize/cexp_pair.hpp>
# include <cppad/local/optimize/fast_empty_set.hpp>
# include <cppad/local/optimize/old_variable.hpp>
# include <cppad/local/optimize/size_pair.hpp>
# include <cppad/local/optimize/csum_variable.hpp>
# include <cppad/local/optimize/csum_stacks.hpp>
# include <cppad/local/optimize/cskip_info.hpp>
# include <cppad/local/optimize/user_info.hpp>
# include <cppad/local/optimize/unary_match.hpp>
# include <cppad/local/optimize/binary_match.hpp>
# include <cppad/local/optimize/record_pv.hpp>
# include <cppad/local/optimize/record_vp.hpp>
# include <cppad/local/optimize/record_vv.hpp>
# include <cppad/local/optimize/record_csum.hpp>

/*!
\file optimize_run.hpp
Convert a player object to an optimized recorder object
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Convert a player object to an optimized recorder object

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type
\a Base.

\param options
The possible values for this string are:
"", "no_conditional_skip".
If it is "no_conditional_skip", then no conditional skip operations
will be generated.

\param n
is the number of independent variables on the tape.

\param dep_taddr
On input this vector contains the indices for each of the dependent
variable values in the operation sequence corresponding to \a play.
Upon return it contains the indices for the same variables but in
the operation sequence corresponding to \a rec.

\param play
This is the operation sequence that we are optimizing.
It is essentially const, except for play back state which
changes while it plays back the operation seqeunce.

\param rec
The input contents of this recording does not matter.
Upon return, it contains an optimized verison of the
operation sequence corresponding to \a play.
*/

template <class Base>
void optimize_run(
	const std::string&           options   ,
	size_t                       n         ,
	CppAD::vector<size_t>&       dep_taddr ,
	player<Base>*                play      ,
	recorder<Base>*              rec       )
{

	// check options
	bool conditional_skip =
		options.find("no_conditional_skip", 0) == std::string::npos;
	bool compare_op =
		options.find("no_compare_op", 0) == std::string::npos;

	// number of operators in the player
	const size_t num_op = play->num_op_rec();

	// number of variables in the player
	const size_t num_var = play->num_var_rec();

	// number of parameters in the player
	const size_t num_par = play->num_par_rec();

	// number of  VecAD indices
	size_t num_vecad_ind   = play->num_vec_ind_rec();

	// number of VecAD vectors
	size_t num_vecad_vec   = play->num_vecad_vec_rec();

	// pointer to the beginning of the parameter vector
	// (used by atomic functions
	const Base* parameter = CPPAD_NULL;
	if( num_par > 0 )
		parameter = play->GetPar();

	// operator information
	CppAD::vector<struct_op_info>  op_info(num_op);
	CppAD::vector<size_t>          var2op(num_var);
	get_op_info(compare_op, play, dep_taddr, var2op, op_info);

	// nan with type Base
	Base base_nan = Base( std::numeric_limits<double>::quiet_NaN() );

	// temporary indices
	size_t i, j, k;

	// temporary variables
	OpCode        op;   // current operator
	const addr_t* arg;  // operator arguments
	size_t        i_var;  // index of first result for current operator

	// range and domain dimensions for F
	size_t m = dep_taddr.size();

	// -------------------------------------------------------------
	// data structure that maps variable index in original operation
	// sequence to corresponding operator information
	CppAD::vector<struct struct_old_variable> tape(num_var);

	// if tape[i].connect_type == exp_connected, cexp_set[i] is the
	// corresponding information for the conditional connection.
	CppAD::vector< fast_empty_set<class_cexp_pair> > cexp_vec_set;
	if( conditional_skip )
		cexp_vec_set.resize(num_var);
	// -------------------------------------------------------------
	// Determine how each variable is connected to the dependent variables

	// initialize all variables has having no connections
	for(i = 0; i < num_var; i++)
		tape[i].connect_type = not_connected;

	for(j = 0; j < m; j++)
	{	// mark dependent variables as having one or more connections
		tape[ dep_taddr[j] ].connect_type = yes_connected;
	}

	// vecad_connect contains a value for each VecAD object.
	// vecad maps a VecAD index (which corresponds to the beginning of the
	// VecAD object) to the vecad_connect falg for the VecAD object.
	CppAD::vector<enum_connect_type>   vecad_connect(num_vecad_vec);
	CppAD::vector<size_t> vecad(num_vecad_ind);
	j = 0;
	for(i = 0; i < num_vecad_vec; i++)
	{	vecad_connect[i] = not_connected;
		// length of this VecAD
		size_t length = play->GetVecInd(j);
		// set to proper index for this VecAD
		vecad[j] = i;
		for(k = 1; k <= length; k++)
			vecad[j+k] = num_vecad_vec; // invalid index
		// start of next VecAD
		j       += length + 1;
	}
	CPPAD_ASSERT_UNKNOWN( j == num_vecad_ind );

	// work space used by UserOp.
	vector<Base>     user_x;       // parameters in x as integers
	vector<size_t>   user_ix;      // variables indices for argument vector
	//
	// information defined by forward_user
	size_t user_m=0, user_n=0, user_i=0, user_j=0;
	enum_user_state user_state;
	//
	// During reverse mode, compute type of connection for each call to
	// a user atomic function.
	CppAD::vector<struct_user_info>    user_info;
	size_t                             user_curr = 0;

	/// During reverse mode, information for each CSkip operation
	CppAD::vector<struct_cskip_info>   cskip_info;

	// Initialize a reverse mode sweep through the operation sequence
	size_t i_op;
	play->reverse_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == EndOp );
	size_t mask;
	//
	user_state = end_user;
	i_op       = num_op;
	while( i_op > 0 )
	{	--i_op;
		//
		bool flag; // temporary for use in switch cases
		//
		// next op
		op    = op_info[i_op].op;
		arg   = op_info[i_op].arg;
		i_var = op_info[i_op].i_var;
		//
		// Store the operator corresponding to each variable
		if( NumRes(op) > 0 )
		{	tape[i_var].op = op;
			tape[i_var].arg = arg;
		}
# ifndef NDEBUG
		if( i_op <= n )
		{	CPPAD_ASSERT_UNKNOWN((op == InvOp) | (op == BeginOp));
		}
		else	CPPAD_ASSERT_UNKNOWN((op != InvOp) & (op != BeginOp));
# endif
		enum_connect_type connect_type      = tape[i_var].connect_type;
		fast_empty_set<class_cexp_pair>* cexp_set = CPPAD_NULL;
		if( conditional_skip )
			cexp_set = &cexp_vec_set[i_var];
		switch( op )
		{
			// One variable corresponding to arg[0]
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
			case TanOp:
			case TanhOp:
			case ZmulvpOp:
			switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				tape[arg[0]].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip )
				if( tape[arg[0]].connect_type == not_connected )
				{	tape[arg[0]].connect_type = cexp_connected;
					cexp_vec_set[arg[0]]     = *cexp_set;
				}
				else if( tape[arg[0]].connect_type == cexp_connected )
				{	cexp_vec_set[arg[0]].intersection(*cexp_set);
					if( cexp_vec_set[arg[0]].empty() )
						tape[arg[0]].connect_type = yes_connected;
				}
				else	tape[arg[0]].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			break; // --------------------------------------------

			// One variable corresponding to arg[1]
			case DisOp:
			case DivpvOp:
			case MulpvOp:
			case PowpvOp:
			case ZmulpvOp:
			switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				tape[arg[1]].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip )
				if( tape[arg[1]].connect_type == not_connected )
				{	tape[arg[1]].connect_type = cexp_connected;
					cexp_vec_set[arg[1]]     = *cexp_set;
				}
				else if( tape[arg[1]].connect_type == cexp_connected )
				{	cexp_vec_set[arg[1]].intersection(*cexp_set);
					if( cexp_vec_set[arg[1]].empty() )
						tape[arg[1]].connect_type = yes_connected;
				}
				else	tape[arg[1]].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			break; // --------------------------------------------

			// Special case for SubvpOp
			case SubvpOp:
			switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				if( tape[arg[0]].connect_type == not_connected )
					tape[arg[0]].connect_type = sum_connected;
				else	tape[arg[0]].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip )
				if( tape[arg[0]].connect_type == not_connected )
				{	tape[arg[0]].connect_type = cexp_connected;
					cexp_vec_set[arg[0]]     = *cexp_set;
				}
				else if( tape[arg[0]].connect_type == cexp_connected )
				{	cexp_vec_set[arg[0]].intersection(*cexp_set);
					if( cexp_vec_set[arg[0]].empty() )
						tape[arg[0]].connect_type = yes_connected;
				}
				else	tape[arg[0]].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			if( connect_type == sum_connected )
			{	// convert sum to csum connection for this variable
				tape[i_var].connect_type = connect_type = csum_connected;
			}
			break; // --------------------------------------------

			// Special case for AddpvOp and SubpvOp
			case AddpvOp:
			case SubpvOp:
			switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				if( tape[arg[1]].connect_type == not_connected )
					tape[arg[1]].connect_type = sum_connected;
				else	tape[arg[1]].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip )
				if( tape[arg[1]].connect_type == not_connected )
				{	tape[arg[1]].connect_type = cexp_connected;
					cexp_vec_set[arg[1]]     = *cexp_set;
				}
				else if( tape[arg[1]].connect_type == cexp_connected )
				{	cexp_vec_set[arg[1]].intersection(*cexp_set);
					if( cexp_vec_set[arg[1]].empty() )
						tape[arg[1]].connect_type = yes_connected;
				}
				else	tape[arg[1]].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			if( connect_type == sum_connected )
			{	// convert sum to csum connection for this variable
				tape[i_var].connect_type = connect_type = csum_connected;
			}
			break; // --------------------------------------------


			// Special case for AddvvOp and SubvvOp
			case AddvvOp:
			case SubvvOp:
			for(i = 0; i < 2; i++) switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				if( tape[arg[i]].connect_type == not_connected )
					tape[arg[i]].connect_type = sum_connected;
				else	tape[arg[i]].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip )
				if( tape[arg[i]].connect_type == not_connected )
				{	tape[arg[i]].connect_type = cexp_connected;
					cexp_vec_set[arg[i]]     = *cexp_set;
				}
				else if( tape[arg[i]].connect_type == cexp_connected )
				{	cexp_vec_set[arg[i]].intersection(*cexp_set);
					if( cexp_vec_set[arg[i]].empty() )
						tape[arg[i]].connect_type = yes_connected;
				}
				else	tape[arg[i]].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			if( connect_type == sum_connected )
			{	// convert sum to csum connection for this variable
				tape[i_var].connect_type = connect_type = csum_connected;
			}
			break; // --------------------------------------------

			// Other binary operators
			// where operands are arg[0], arg[1]
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case ZmulvvOp:
			for(i = 0; i < 2; i++) switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				tape[arg[i]].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip )
				if( tape[arg[i]].connect_type == not_connected )
				{	tape[arg[i]].connect_type = cexp_connected;
					cexp_vec_set[arg[i]]     = *cexp_set;
				}
				else if( tape[arg[i]].connect_type == cexp_connected )
				{	cexp_vec_set[arg[i]].intersection(*cexp_set);
					if( cexp_vec_set[arg[i]].empty() )
						tape[arg[i]].connect_type = yes_connected;
				}
				else	tape[arg[i]].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			break; // --------------------------------------------

			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
			if( connect_type != not_connected )
			{	struct_cskip_info info;
				info.i_op       = i_op;
				info.cop        = CompareOp( arg[0] );
				info.flag       = arg[1];
				info.left       = arg[2];
				info.right      = arg[3];
				info.n_op_true  = 0;
				info.n_op_false = 0;
				info.i_arg      = 0; // case where no CSkipOp for this CExpOp
				//
				size_t index    = 0;
				if( arg[1] & 1 )
				{	index = std::max(index, info.left);
					tape[info.left].connect_type = yes_connected;
				}
				if( arg[1] & 2 )
				{	index = std::max(index, info.right);
					tape[info.right].connect_type = yes_connected;
				}
				CPPAD_ASSERT_UNKNOWN( index > 0 );
				info.max_left_right = index;
				//
				index = cskip_info.size();
				cskip_info.push_back(info);
				//
				if( arg[1] & 4 )
				{	if( conditional_skip &&
						tape[arg[4]].connect_type == not_connected )
					{	tape[arg[4]].connect_type = cexp_connected;
						cexp_vec_set[arg[4]]     = *cexp_set;
						cexp_vec_set[arg[4]].insert(
							class_cexp_pair(true, index)
						);
					}
					else
					{	// if arg[4] is cexp_connected, it could be
						// connected for both the true and false case
						// 2DO: if previously cexp_connected
						// and the true/false sense is the same, should
						// keep this conditional connnection.
						if(conditional_skip)
							cexp_vec_set[arg[4]].clear();
						tape[arg[4]].connect_type = yes_connected;
					}
				}
				if( arg[1] & 8 )
				{	if( conditional_skip &&
						tape[arg[5]].connect_type == not_connected )
					{	tape[arg[5]].connect_type = cexp_connected;
						cexp_vec_set[arg[5]]     = *cexp_set;
						cexp_vec_set[arg[5]].insert(
							class_cexp_pair(false, index)
						);
					}
					else
					{	if(conditional_skip)
							cexp_vec_set[arg[5]].clear();
						tape[arg[5]].connect_type = yes_connected;
					}
				}
			}
			break;  // --------------------------------------------

			// Operations where there is nothing to do
			case EndOp:
			case ParOp:
			case PriOp:
			break;  // --------------------------------------------

			// Operators that never get removed
			case BeginOp:
			case InvOp:
			tape[i_var].connect_type = yes_connected;
			break;

			// Compare operators never get removed -----------------
			case LepvOp:
			case LtpvOp:
			case EqpvOp:
			case NepvOp:
			tape[arg[1]].connect_type = yes_connected;
			break;

			case LevpOp:
			case LtvpOp:
			tape[arg[0]].connect_type = yes_connected;
			break;

			case LevvOp:
			case LtvvOp:
			case EqvvOp:
			case NevvOp:
			tape[arg[0]].connect_type = yes_connected;
			tape[arg[1]].connect_type = yes_connected;
			break;

			// Load using a parameter index ----------------------
			case LdpOp:
			if( tape[i_var].connect_type != not_connected )
			{
				i                = vecad[ arg[0] - 1 ];
				vecad_connect[i] = yes_connected;
			}
			break; // --------------------------------------------

			// Load using a variable index
			case LdvOp:
			if( tape[i_var].connect_type != not_connected )
			{
				i                    = vecad[ arg[0] - 1 ];
				vecad_connect[i]     = yes_connected;
				tape[arg[1]].connect_type = yes_connected;
			}
			break; // --------------------------------------------

			// Store a variable using a parameter index
			case StpvOp:
			i = vecad[ arg[0] - 1 ];
			if( vecad_connect[i] != not_connected )
				tape[arg[2]].connect_type = yes_connected;
			break; // --------------------------------------------

			// Store a variable using a variable index
			case StvvOp:
			i = vecad[ arg[0] - 1 ];
			if( vecad_connect[i] )
			{	tape[arg[1]].connect_type = yes_connected;
				tape[arg[2]].connect_type = yes_connected;
			}
			break;
			// ============================================================
			case UserOp:
			// start or end atomic operation sequence
			flag = user_state == end_user;
			if( flag )
			{	// reverse_user ------------------------------------------
				user_n     = arg[2];
				user_m     = arg[3];
				CPPAD_ASSERT_UNKNOWN( user_n > 0 );

				// other reverse_user arguments
				user_j     = user_n;
				user_i     = user_m;
				user_state = ret_user;
				// -----------------------------------------------------------


				user_x.resize( user_n );
				user_ix.resize( user_n );
				//
				struct_user_info info;
				info.connect_type = not_connected;
				info.op_end       = i_op + 1;
				user_info.push_back(info);
			}
			else
			{	// reverse_user
				CPPAD_ASSERT_UNKNOWN( user_state == start_user );
				user_state = end_user;
				//
				for(j = 0; j < user_n; j++) if( user_ix[j] > 0 )
				{	if( op_info[ var2op[ user_ix[j] ] ].usage > 0 )
						tape[ user_ix[j] ].connect_type = yes_connected;
				}
				//
				CPPAD_ASSERT_UNKNOWN( user_curr + 1 == user_info.size() );
				user_info[user_curr].op_begin = i_op;
				user_curr                     = user_info.size();
			}
			break;

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
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) <= i_var );
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
			switch( connect_type )
			{	case not_connected:
				break;

				case yes_connected:
				case sum_connected:
				case csum_connected:
				user_info[user_curr].connect_type = yes_connected;
				break;

				case cexp_connected:
				CPPAD_ASSERT_UNKNOWN( conditional_skip );
				if( user_info[user_curr].connect_type == not_connected )
				{	user_info[user_curr].connect_type  = connect_type;
					user_info[user_curr].cexp_set      = *cexp_set;
				}
				else if(user_info[user_curr].connect_type==cexp_connected)
				{	user_info[user_curr].cexp_set.intersection(*cexp_set);
					if( user_info[user_curr].cexp_set.empty() )
						user_info[user_curr].connect_type = yes_connected;
				}
				else	user_info[user_curr].connect_type = yes_connected;
				break;

				default:
				CPPAD_ASSERT_UNKNOWN(false);
			}
			break;

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
	//
	// values corresponding to BeginOp
	CPPAD_ASSERT_UNKNOWN( i_op == 0 && i_var == 0 && op == BeginOp );
	tape[i_var].op           = op;
	tape[i_var].connect_type = yes_connected;

	// Determine which variables can be conditionally skipped
	for(i = 0; i < num_var; i++)
	{	if( tape[i].connect_type == cexp_connected &&
		  ! cexp_vec_set[i].empty() )
		{	std::set<class_cexp_pair>::const_iterator itr =
				cexp_vec_set[i].begin();
			while( itr != cexp_vec_set[i].end() )
			{	j = itr->index();
				if( itr->compare() == true )
					cskip_info[j].skip_var_false.push_back(i);
				else cskip_info[j].skip_var_true.push_back(i);
				itr++;
			}
		}
	}
	// -------------------------------------------------------------
	// Check op_info conditional skip information
	for(i = 0; i < cskip_info.size(); i++)
	{	i_op = cskip_info[i].i_op;
		//
		for(j = 0; j < cskip_info[i].skip_var_true.size(); j++)
		{	size_t j_var = cskip_info[i].skip_var_true[j];
			size_t j_op  = var2op[j_var];
			fast_empty_set<cexp_compare> cexp_set( op_info[j_op].cexp_set );
			cexp_compare element(i_op, true);
			CPPAD_ASSERT_UNKNOWN( cexp_set.find(element) != cexp_set.end() );
			// std::cout << "i_op = " << i_op;
			// std::cout << ", (j_op, compare) = (" << j_op << ",true)\n";

		}
		for(j = 0; j < cskip_info[i].skip_var_false.size(); j++)
		{	size_t j_var = cskip_info[i].skip_var_false[j];
			size_t j_op  = var2op[j_var];
			fast_empty_set<cexp_compare> cexp_set( op_info[j_op].cexp_set );
			cexp_compare element(i_op, false);
			CPPAD_ASSERT_UNKNOWN( cexp_set.find(element) != cexp_set.end() );
			// std::cout << "i_op = " << i_op;
			// std::cout << ", (j_op, compare) = (" << j_op << ",false)\n";
		}
	}
	// -------------------------------------------------------------
	// Determine size of skip information in user_info
	for(i = 0; i < user_info.size(); i++)
	{	if( user_info[i].connect_type == cexp_connected &&
		  ! user_info[i].cexp_set.empty() )
		{	std::set<class_cexp_pair>::const_iterator itr =
				user_info[i].cexp_set.begin();
			while( itr != user_info[i].cexp_set.end() )
			{	j = itr->index();
				if( itr->compare() == true )
					cskip_info[j].n_op_false =
						user_info[i].op_end - user_info[i].op_begin;
				else
					cskip_info[j].n_op_true =
						user_info[i].op_end - user_info[i].op_begin;
				itr++;
			}
		}
	}

	// Sort the conditional skip information by the maximum of the
	// index for the left and right comparision operands
	CppAD::vector<size_t> cskip_info_order( cskip_info.size() );
	{	CppAD::vector<size_t> keys( cskip_info.size() );
		for(i = 0; i < cskip_info.size(); i++)
			keys[i] = std::max( cskip_info[i].left, cskip_info[i].right );
		CppAD::index_sort(keys, cskip_info_order);
	}
	// index in sorted order
	size_t cskip_order_next = 0;
	// index in order during reverse sweep
	size_t cskip_info_index = cskip_info.size();


	// Initilaize table mapping hash code to variable index in tape
	// as pointing to the BeginOp at the beginning of the tape
	CppAD::vector<size_t>  hash_table_var(CPPAD_HASH_TABLE_SIZE);
	for(i = 0; i < CPPAD_HASH_TABLE_SIZE; i++)
		hash_table_var[i] = 0;
	CPPAD_ASSERT_UNKNOWN( tape[0].op == BeginOp );

	// initialize mapping from old variable index to new
	// operator and variable index
	for(i = 0; i < num_var; i++)
	{	tape[i].new_op  = 0;       // invalid index (except for BeginOp)
		tape[i].new_var = num_var; // invalid index
	}

	// Erase all information in the old recording
	rec->free();

	// initialize mapping from old VecAD index to new VecAD index
	CppAD::vector<size_t> new_vecad_ind(num_vecad_ind);
	for(i = 0; i < num_vecad_ind; i++)
		new_vecad_ind[i] = num_vecad_ind; // invalid index

	j = 0;     // index into the old set of indices
	for(i = 0; i < num_vecad_vec; i++)
	{	// length of this VecAD
		size_t length = play->GetVecInd(j);
		if( vecad_connect[i] != not_connected )
		{	// Put this VecAD vector in new recording
			CPPAD_ASSERT_UNKNOWN(length < num_vecad_ind);
			new_vecad_ind[j] = rec->PutVecInd(length);
			for(k = 1; k <= length; k++) new_vecad_ind[j+k] =
				rec->PutVecInd(
					rec->PutPar(
						play->GetPar(
							play->GetVecInd(j+k)
			) ) );
		}
		// start of next VecAD
		j       += length + 1;
	}
	CPPAD_ASSERT_UNKNOWN( j == num_vecad_ind );

	// start playing the operations in the forward direction
	CPPAD_ASSERT_UNKNOWN( user_curr == user_info.size() );
	i_op = 0;
	op   = op_info[i_op].op;
	arg  = op_info[i_op].arg;
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
	CPPAD_ASSERT_NARG_NRES(BeginOp, 1, 1);
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) == 0 );
	//
	// Put BeginOp at beginning of recording
	tape[i_var].new_op  = rec->num_op_rec();
	tape[i_var].new_var = rec->PutOp(BeginOp);
	rec->PutArg(arg[0]);


	// temporary buffer for new argument values
	addr_t new_arg[6];

	// temporary work space used by record_csum
	// (decalared here to avoid realloaction of memory)
	struct_csum_stacks csum_work;

	// tempory used to hold a size_pair
	struct_size_pair size_pair;

	user_state = start_user;
	for( i_op = 1; i_op < num_op; i_op++)
	{	bool flag; // for temporary in switch cases
		//
		// next op
		op    = op_info[i_op].op;
		arg   = op_info[i_op].arg;
		i_var = op_info[i_op].i_var;
		//
		CPPAD_ASSERT_UNKNOWN( (i_op > n)  | (op == InvOp) );
		CPPAD_ASSERT_UNKNOWN( (i_op <= n) | (op != InvOp) );

		// determine if we should insert a conditional skip here
		bool skip = cskip_order_next < cskip_info.size();
		skip     &= op != BeginOp;
		skip     &= op != InvOp;
		skip     &= user_state == start_user;
		if( skip )
		{	j     = cskip_info_order[cskip_order_next];
			if( NumRes(op) > 0 )
				skip &= cskip_info[j].max_left_right < i_var;
			else
				skip &= cskip_info[j].max_left_right <= i_var;
		}
		if( skip )
		{	cskip_order_next++;
			struct_cskip_info info = cskip_info[j];
			size_t n_true  = info.skip_var_true.size() + info.n_op_true;
			size_t n_false = info.skip_var_false.size() + info.n_op_false;
			skip &= n_true > 0 || n_false > 0;
			if( skip )
			{	CPPAD_ASSERT_UNKNOWN( NumRes(CSkipOp) == 0 );
				size_t n_arg   = 7 + n_true + n_false;
				// reserve space for the arguments to this operator but
				// delay setting them until we have all the new addresses
				cskip_info[j].i_arg = rec->ReserveArg(n_arg);
				CPPAD_ASSERT_UNKNOWN( cskip_info[j].i_arg > 0 );
				rec->PutOp(CSkipOp);
			}
		}

		// determine if we should keep this operation in the new
		// operation sequence
		bool keep;
		switch( op )
		{	// see wish_list/Optimize/CompareChange entry.
			case EqpvOp:
			case EqvvOp:
			case LepvOp:
			case LevpOp:
			case LevvOp:
			case LtpvOp:
			case LtvpOp:
			case LtvvOp:
			case NepvOp:
			case NevvOp:
			keep = true;
			break;

			case PriOp:
			keep = false;
			break;

			case InvOp:
			case EndOp:
			keep = true;
			break;

			case StppOp:
			case StvpOp:
			case StpvOp:
			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
			i = vecad[ arg[0] - 1 ];
			keep = vecad_connect[i] != not_connected;
			break;

			case AddpvOp:
			case AddvvOp:
			case SubpvOp:
			case SubvpOp:
			case SubvvOp:
			keep  = tape[i_var].connect_type != not_connected;
			keep &= tape[i_var].connect_type != csum_connected;
			break;

			case UserOp:
			case UsrapOp:
			case UsravOp:
			case UsrrpOp:
			case UsrrvOp:
			keep = true;
			break;

			default:
			keep = tape[i_var].connect_type != not_connected;
			break;
		}

		unsigned short code         = 0;
		bool           replace_hash = false;
		addr_t         match_var;
		tape[i_var].match = false;
		if( keep ) switch( op )
		{
			// Unary operator where operand is arg[0]
			case AbsOp:
			case AcosOp:
			case AcoshOp:
			case AsinOp:
			case AsinhOp:
			case AtanOp:
			case AtanhOp:
			case CosOp:
			case CoshOp:
			case ErfOp:
			case ExpOp:
			case Expm1Op:
			case LogOp:
			case Log1pOp:
			case SignOp:
			case SinOp:
			case SinhOp:
			case SqrtOp:
			case TanOp:
			case TanhOp:
			match_var = unary_match(
				tape                ,  // inputs
				i_var               ,
				play->num_par_rec() ,
				play->GetPar()      ,
				hash_table_var      ,
				code                  // outputs
			);
			if( match_var > 0 )
			{	tape[i_var].match     = true;
				tape[match_var].match = true;
				tape[i_var].new_var   = tape[match_var].new_var;
			}
			else
			{
				replace_hash = true;
				new_arg[0]   = tape[ arg[0] ].new_var;
				rec->PutArg( new_arg[0] );
				tape[i_var].new_op  = rec->num_op_rec();
				tape[i_var].new_var = i = rec->PutOp(op);
				CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < i );
				if( op == ErfOp )
				{	// Error function is a special case
					// second argument is always the parameter 0
					// third argument is always the parameter 2 / sqrt(pi)
					CPPAD_ASSERT_UNKNOWN( NumArg(ErfOp) == 3 );
					rec->PutArg( rec->PutPar( Base(0) ) );
					rec->PutArg( rec->PutPar(
						Base( 1.0 / std::sqrt( std::atan(1.0) ) )
					) );
				}
			}
			break;
			// ---------------------------------------------------
			// Binary operators where
			// left is a variable and right is a parameter
			case SubvpOp:
			if( tape[arg[0]].connect_type == csum_connected )
			{
				// convert to a sequence of summation operators
				size_pair = record_csum(
					tape                , // inputs
					i_var               ,
					play->num_par_rec() ,
					play->GetPar()      ,
					rec                 ,
					csum_work           ,
					var2op              ,
					op_info
				);
				tape[i_var].new_op  = size_pair.i_op;
				tape[i_var].new_var = size_pair.i_var;
				// abort rest of this case
				break;
			}
			case DivvpOp:
			case PowvpOp:
			case ZmulvpOp:
			match_var = binary_match(
				tape                ,  // inputs
				i_var               ,
				play->num_par_rec() ,
				play->GetPar()      ,
				hash_table_var      ,
				code                  // outputs
			);
			if( match_var > 0 )
			{	tape[i_var].match     = true;
				tape[match_var].match = true;
				tape[i_var].new_var   = tape[match_var].new_var;
			}
			else
			{	size_pair = record_vp(
					tape                , // inputs
					i_var               ,
					play->num_par_rec() ,
					play->GetPar()      ,
					rec                 ,
					op                  ,
					arg
				);
				tape[i_var].new_op  = size_pair.i_op;
				tape[i_var].new_var = size_pair.i_var;
				replace_hash = true;
			}
			break;
			// ---------------------------------------------------
			// Binary operators where
			// left is an index and right is a variable
			case DisOp:
			match_var = binary_match(
				tape                ,  // inputs
				i_var               ,
				play->num_par_rec() ,
				play->GetPar()      ,
				hash_table_var      ,
				code                  // outputs
			);
			if( match_var > 0 )
			{	tape[i_var].match     = true;
				tape[match_var].match = true;
				tape[i_var].new_var   = tape[match_var].new_var;
			}
			else
			{	new_arg[0] = arg[0];
				new_arg[1] = tape[ arg[1] ].new_var;
				rec->PutArg( new_arg[0], new_arg[1] );
				tape[i_var].new_op  = rec->num_op_rec();
				tape[i_var].new_var = rec->PutOp(op);
				CPPAD_ASSERT_UNKNOWN(
					new_arg[1] < tape[i_var].new_var
				);
				replace_hash = true;
			}
			break;

			// ---------------------------------------------------
			// Binary operators where
			// left is a parameter and right is a variable
			case SubpvOp:
			case AddpvOp:
			if( tape[arg[1]].connect_type == csum_connected )
			{
				// convert to a sequence of summation operators
				size_pair = record_csum(
					tape                , // inputs
					i_var               ,
					play->num_par_rec() ,
					play->GetPar()      ,
					rec                 ,
					csum_work           ,
					var2op              ,
					op_info
				);
				tape[i_var].new_op  = size_pair.i_op;
				tape[i_var].new_var = size_pair.i_var;
				// abort rest of this case
				break;
			}
			case DivpvOp:
			case MulpvOp:
			case PowpvOp:
			case ZmulpvOp:
			match_var = binary_match(
				tape                ,  // inputs
				i_var               ,
				play->num_par_rec() ,
				play->GetPar()      ,
				hash_table_var      ,
				code                  // outputs
			);
			if( match_var > 0 )
			{	tape[i_var].match     = true;
				tape[match_var].match = true;
				tape[i_var].new_var   = tape[match_var].new_var;
			}
			else
			{	size_pair = record_pv(
					tape                , // inputs
					i_var               ,
					play->num_par_rec() ,
					play->GetPar()      ,
					rec                 ,
					op                  ,
					arg
				);
				tape[i_var].new_op  = size_pair.i_op;
				tape[i_var].new_var = size_pair.i_var;
				replace_hash = true;
			}
			break;
			// ---------------------------------------------------
			// Binary operator where
			// both operators are variables
			case AddvvOp:
			case SubvvOp:
			if( (tape[arg[0]].connect_type == csum_connected) |
			    (tape[arg[1]].connect_type == csum_connected)
			)
			{
				// convert to a sequence of summation operators
				size_pair = record_csum(
					tape                , // inputs
					i_var               ,
					play->num_par_rec() ,
					play->GetPar()      ,
					rec                 ,
					csum_work           ,
					var2op              ,
					op_info
				);
				tape[i_var].new_op  = size_pair.i_op;
				tape[i_var].new_var = size_pair.i_var;
				// abort rest of this case
				break;
			}
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case ZmulvvOp:
			match_var = binary_match(
				tape                ,  // inputs
				i_var               ,
				play->num_par_rec() ,
				play->GetPar()      ,
				hash_table_var      ,
				code                  // outputs
			);
			if( match_var > 0 )
			{	tape[i_var].match     = true;
				tape[match_var].match = true;
				tape[i_var].new_var   = tape[match_var].new_var;
			}
			else
			{	size_pair = record_vv(
					tape                , // inputs
					i_var               ,
					play->num_par_rec() ,
					play->GetPar()      ,
					rec                 ,
					op                  ,
					arg
				);
				tape[i_var].new_op  = size_pair.i_op;
				tape[i_var].new_var = size_pair.i_var;
				replace_hash = true;
			}
			break;
			// ---------------------------------------------------
			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_NARG_NRES(op, 6, 1);
			new_arg[0] = arg[0];
			new_arg[1] = arg[1];
			mask = 1;
			for(i = 2; i < 6; i++)
			{	if( arg[1] & mask )
				{	new_arg[i] = tape[arg[i]].new_var;
					CPPAD_ASSERT_UNKNOWN(
						size_t(new_arg[i]) < num_var
					);
				}
				else	new_arg[i] = rec->PutPar(
						play->GetPar( arg[i] )
				);
				mask = mask << 1;
			}
			rec->PutArg(
				new_arg[0] ,
				new_arg[1] ,
				new_arg[2] ,
				new_arg[3] ,
				new_arg[4] ,
				new_arg[5]
			);
			tape[i_var].new_op  = rec->num_op_rec();
			tape[i_var].new_var = rec->PutOp(op);
			//
			// The new addresses for left and right are used during
			// fill in the arguments for the CSkip operations. This does not
			// affect max_left_right which is used during this sweep.
			CPPAD_ASSERT_UNKNOWN( cskip_info_index > 0 );
			cskip_info_index--;
			cskip_info[ cskip_info_index ].left  = new_arg[2];
			cskip_info[ cskip_info_index ].right = new_arg[3];
			break;
			// ---------------------------------------------------
			// Operations with no arguments and no results
			case EndOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 0);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Operations with two arguments and no results
			case LepvOp:
			case LtpvOp:
			case EqpvOp:
			case NepvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
			new_arg[1] = tape[arg[1]].new_var;
			rec->PutArg(new_arg[0], new_arg[1]);
			rec->PutOp(op);
			break;
			//
			case LevpOp:
			case LtvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			new_arg[0] = tape[arg[0]].new_var;
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			rec->PutArg(new_arg[0], new_arg[1]);
			rec->PutOp(op);
			break;
			//
			case LevvOp:
			case LtvvOp:
			case EqvvOp:
			case NevvOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			new_arg[0] = tape[arg[0]].new_var;
			new_arg[1] = tape[arg[1]].new_var;
			rec->PutArg(new_arg[0], new_arg[1]);
			rec->PutOp(op);
			break;

			// ---------------------------------------------------
			// Operations with no arguments and one result
			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			tape[i_var].new_op  = rec->num_op_rec();
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Operations with one argument that is a parameter
			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0] ) );

			rec->PutArg( new_arg[0] );
			tape[i_var].new_op  = rec->num_op_rec();
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Load using a parameter index
			case LdpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = arg[1];
			new_arg[2] = rec->num_load_op_rec();
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			tape[i_var].new_op  = rec->num_op_rec();
			tape[i_var].new_var = rec->PutLoadOp(op);
			break;
			// ---------------------------------------------------
			// Load using a variable index
			case LdvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = tape[arg[1]].new_var;
			new_arg[2] = rec->num_load_op_rec();
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			tape[i_var].new_var = rec->num_op_rec();
			tape[i_var].new_var = rec->PutLoadOp(op);
			break;
			// ---------------------------------------------------
			// Store a parameter using a parameter index
			case StppOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			new_arg[2] = rec->PutPar( play->GetPar(arg[2]) );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a parameter using a variable index
			case StvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = tape[arg[1]].new_var;
			new_arg[2] = rec->PutPar( play->GetPar(arg[2]) );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a variable using a parameter index
			case StpvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			new_arg[2] = tape[arg[2]].new_var;
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[2]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Store a variable using a variable index
			case StvvOp:
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = tape[arg[1]].new_var;
			new_arg[2] = tape[arg[2]].new_var;
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[2]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			rec->PutOp(op);
			break;

			// -----------------------------------------------------------
			case UserOp:
			flag = user_state == start_user;
			if( flag )
			{	// forward_user
				user_n     = arg[2];
				user_m     = arg[3];
				user_j     = 0;
				user_i     = 0;
				user_state = arg_user;


				CPPAD_ASSERT_UNKNOWN( user_curr > 0 );
				user_curr--;
				user_info[user_curr].op_begin = rec->num_op_rec();
			}
			else
			{	// forward_user
				CPPAD_ASSERT_UNKNOWN( user_state == end_user );
				user_state = start_user;
				//
				user_info[user_curr].op_end = rec->num_op_rec() + 1;
			}
			// user_old, user_n, user_m
			if( user_info[user_curr].connect_type != not_connected )
			{	rec->PutArg(arg[0], arg[1], arg[2], arg[3]);
				rec->PutOp(UserOp);
			}
			break;

			case UsrapOp:
			// forward_user
			++user_j;
			if( user_j == user_n )
				user_state = ret_user;
			//
			if( user_info[user_curr].connect_type != not_connected )
			{	new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
				rec->PutArg(new_arg[0]);
				rec->PutOp(UsrapOp);
			}
			break;

			case UsravOp:
			// forward_user
			++user_j;
			if( user_j == user_n )
				user_state = ret_user;
			//
			if( user_info[user_curr].connect_type != not_connected )
			{	new_arg[0] = tape[arg[0]].new_var;
				if( size_t(new_arg[0]) < num_var )
				{	rec->PutArg(new_arg[0]);
					rec->PutOp(UsravOp);
				}
				else
				{	// This argument does not affect the result and
					// has been optimized out so use nan in its place.
					new_arg[0] = rec->PutPar( base_nan );
					rec->PutArg(new_arg[0]);
					rec->PutOp(UsrapOp);
				}
			}
			break;

			case UsrrpOp:
			// forward_user
			++user_i;
			if( user_i == user_m )
				user_state = end_user;
			//
			if( user_info[user_curr].connect_type != not_connected )
			{	new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
				rec->PutArg(new_arg[0]);
				rec->PutOp(UsrrpOp);
			}
			break;

			case UsrrvOp:
			// forward_user
			++user_i;
			if( user_i == user_m )
				user_state = end_user;
			//
			if( user_info[user_curr].connect_type != not_connected )
			{	tape[i_var].new_op  = rec->num_op_rec();
				tape[i_var].new_var = rec->PutOp(UsrrvOp);
			}
			break;
			// ---------------------------------------------------

			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(false);

		}
		if( replace_hash )
		{	// The old variable index i_var corresponds to the
			// new variable index tape[i_var].new_var. In addition
			// this is the most recent variable that has this code.
			hash_table_var[code] = i_var;
		}

	}
	// modify the dependent variable vector to new indices
	for(i = 0; i < dep_taddr.size(); i++ )
	{	CPPAD_ASSERT_UNKNOWN( size_t(tape[dep_taddr[i]].new_var) < num_var );
		dep_taddr[i] = tape[ dep_taddr[i] ].new_var;
	}

# ifndef NDEBUG
	size_t num_new_op = rec->num_op_rec();
	for(i_var = 0; i_var < tape.size(); i_var++)
		CPPAD_ASSERT_UNKNOWN( tape[i_var].new_op < num_new_op );
# endif

	// Move skip information from user_info to cskip_info
	for(i = 0; i < user_info.size(); i++)
	{	if( user_info[i].connect_type == cexp_connected &&
		  ! user_info[i].cexp_set.empty() )
		{	std::set<class_cexp_pair>::const_iterator itr =
				user_info[i].cexp_set.begin();
			while( itr != user_info[i].cexp_set.end() )
			{	j = itr->index();
				k = user_info[i].op_begin;
				while(k < user_info[i].op_end)
				{	if( itr->compare() == true )
						cskip_info[j].skip_op_false.push_back(k++);
					else	cskip_info[j].skip_op_true.push_back(k++);
				}
				itr++;
			}
		}
	}

	// fill in the arguments for the CSkip operations
	CPPAD_ASSERT_UNKNOWN( cskip_order_next == cskip_info.size() );
	for(i = 0; i < cskip_info.size(); i++)
	{	struct_cskip_info info = cskip_info[i];
		if( info.i_arg > 0 )
		{	CPPAD_ASSERT_UNKNOWN( info.n_op_true==info.skip_op_true.size() );
			CPPAD_ASSERT_UNKNOWN(info.n_op_false==info.skip_op_false.size());
			size_t n_true  =
				info.skip_var_true.size() + info.skip_op_true.size();
			size_t n_false =
				info.skip_var_false.size() + info.skip_op_false.size();
			size_t i_arg   = info.i_arg;
			rec->ReplaceArg(i_arg++, info.cop   );
			rec->ReplaceArg(i_arg++, info.flag  );
			rec->ReplaceArg(i_arg++, info.left  );
			rec->ReplaceArg(i_arg++, info.right );
			rec->ReplaceArg(i_arg++, n_true     );
			rec->ReplaceArg(i_arg++, n_false    );
			for(j = 0; j < info.skip_var_true.size(); j++)
			{	i_var = info.skip_var_true[j];
				if( tape[i_var].match )
				{	// The operation for this argument has been removed,
					// so use an operator index that never comes up.
					rec->ReplaceArg(i_arg++, rec->num_op_rec());
				}
				else
				{	CPPAD_ASSERT_UNKNOWN( tape[i_var].new_op > 0 );
					rec->ReplaceArg(i_arg++, tape[i_var].new_op );
				}
			}
			for(j = 0; j < info.skip_op_true.size(); j++)
			{	i_op = info.skip_op_true[j];
				rec->ReplaceArg(i_arg++, i_op);
			}
			for(j = 0; j < info.skip_var_false.size(); j++)
			{	i_var = info.skip_var_false[j];
				if( tape[i_var].match )
				{	// The operation for this argument has been removed,
					// so use an operator index that never comes up.
					rec->ReplaceArg(i_arg++, rec->num_op_rec());
				}
				else
				{	CPPAD_ASSERT_UNKNOWN( tape[i_var].new_op > 0 );
					rec->ReplaceArg(i_arg++, tape[i_var].new_op );
				}
			}
			for(j = 0; j < info.skip_op_false.size(); j++)
			{	i_op = info.skip_op_false[j];
				rec->ReplaceArg(i_arg++, i_op);
			}
			rec->ReplaceArg(i_arg++, n_true + n_false);
# ifndef NDEBUG
			size_t n_arg   = 7 + n_true + n_false;
			CPPAD_ASSERT_UNKNOWN( info.i_arg + n_arg == i_arg );
# endif
		}
	}
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
