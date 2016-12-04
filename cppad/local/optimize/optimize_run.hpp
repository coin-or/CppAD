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
# include <cppad/local/optimize/fast_empty_set.hpp>
# include <cppad/local/optimize/old_variable.hpp>
# include <cppad/local/optimize/size_pair.hpp>
# include <cppad/local/optimize/csum_variable.hpp>
# include <cppad/local/optimize/csum_stacks.hpp>
# include <cppad/local/optimize/cskip_info.hpp>
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

	bool compare_op =
		options.find("no_compare_op", 0) == std::string::npos;

	// number of operators in the player
	const size_t num_op = play->num_op_rec();

	// number of variables in the player
	const size_t num_var = play->num_var_rec();

	// number of  VecAD indices
	size_t num_vecad_ind   = play->num_vec_ind_rec();

	// number of VecAD vectors
	size_t num_vecad_vec   = play->num_vecad_vec_rec();

	// operator information
	CppAD::vector<struct_op_info>  op_info;
	CppAD::vector<size_t>          var2op, cexp2op;
	get_op_info(compare_op, play, dep_taddr, var2op, cexp2op, op_info);

	// number of conditonal expressions
	size_t num_cexp_op = cexp2op.size();

	// nan with type Base
	Base base_nan = Base( std::numeric_limits<double>::quiet_NaN() );

	// temporary variables
	OpCode        op;   // current operator
	const addr_t* arg;  // operator arguments
	size_t        i_var;  // index of first result for current operator

	// -------------------------------------------------------------
	// data structure that maps variable index in original operation
	// sequence to corresponding operator information
	CppAD::vector<struct struct_old_variable> tape(num_var);

	// -------------------------------------------------------------
	// Determine how each variable is connected to the dependent variables

	// vecad_connect contains a value for each VecAD object.
	// vecad maps a VecAD index (which corresponds to the beginning of the
	// VecAD object) to the vecad_connect falg for the VecAD object.
	CppAD::vector<enum_connect_type>   vecad_connect(num_vecad_vec);
	CppAD::vector<size_t> vecad(num_vecad_ind);
	{
		size_t j = 0;
		for(size_t i = 0; i < num_vecad_vec; i++)
		{	vecad_connect[i] = not_connected;
			// length of this VecAD
			size_t length = play->GetVecInd(j);
			// set to proper index for this VecAD
			vecad[j] = i;
			for(size_t k = 1; k <= length; k++)
				vecad[j+k] = num_vecad_vec; // invalid index
			// start of next VecAD
			j       += length + 1;
		}
		CPPAD_ASSERT_UNKNOWN( j == num_vecad_ind );
	}

	// work space used by UserOp.
	vector<Base>     user_x;       // parameters in x as integers
	vector<size_t>   user_ix;      // variables indices for argument vector
	//
	// information defined by forward_user
	size_t user_m=0, user_n=0, user_i=0, user_j=0;
	enum_user_state user_state;
	//
	/// During reverse mode, information for each CSkip operation
	CppAD::vector<struct_cskip_info>   cskip_info(num_cexp_op);
	size_t index_cskip_info = num_cexp_op; // initialize for reverse mode

	// Initialize a reverse mode sweep through the operation sequence
	size_t i_op;
	play->reverse_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op == EndOp );
	size_t mask;
	//
	user_state = end_user;
	while( i_op > 0 )
	{
		// next op
		play->reverse_next(op, arg, i_op, i_var);
		//
		// tape info for this operation
		if( NumRes(op) > 0 )
		{	tape[i_var].op = op;
			tape[i_var].arg = arg;
		}
		switch( op )
		{
			// Conditional expression operators
			case CExpOp:
			--index_cskip_info;
			CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
			{	struct_cskip_info info;
				info.i_op       = i_op;
				info.cop        = CompareOp( arg[0] );
				info.flag       = arg[1];
				info.left       = arg[2];
				info.right      = arg[3];
				info.i_arg      = 0; // case where no CSkipOp for this CExpOp
				//
				size_t index    = 0;
				if( arg[1] & 1 )
					index = std::max(index, info.left);
				if( arg[1] & 2 )
					index = std::max(index, info.right);
				CPPAD_ASSERT_UNKNOWN( index > 0 );
				info.max_left_right = index;
				//
				cskip_info[index_cskip_info] = info;
			}
			break;  // --------------------------------------------

			// Load using a parameter index ----------------------
			case LdpOp:
			if( op_info[i_op].usage > 0 )
			{
				size_t i         = vecad[ arg[0] - 1 ];
				vecad_connect[i] = yes_connected;
			}
			break; // --------------------------------------------

			// Load using a variable index
			case LdvOp:
			if( op_info[i_op].usage > 0 )
			{
				size_t i             = vecad[ arg[0] - 1 ];
				vecad_connect[i]     = yes_connected;
			}
			break; // --------------------------------------------

			// ============================================================
			// noting to do in this case
			default:
			break;
		}
	}
	//
	// values corresponding to BeginOp
	CPPAD_ASSERT_UNKNOWN( i_op == 0 && i_var == 0 && op == BeginOp );
	tape[i_var].op           = op;

	// Determine which operators can be conditionally skipped
	for(size_t i = 0; i < num_op; i++)
	{	if( ! op_info[i].cexp_set.empty() )
		{
			fast_empty_set<cexp_compare>::const_iterator itr =
				op_info[i].cexp_set.begin();
			while( itr != op_info[i].cexp_set.end() )
			{	size_t j = itr->index();
				if( itr->compare() == false )
					cskip_info[j].skip_old_op_false.push_back(i);
				else cskip_info[j].skip_old_op_true.push_back(i);
				itr++;
			}
		}
	}
	// -------------------------------------------------------------
	// Sort the conditional skip information by the maximum of the
	// index for the left and right comparision operands
	CppAD::vector<size_t> cskip_info_order( cskip_info.size() );
	{	CppAD::vector<size_t> keys( cskip_info.size() );
		for(size_t i = 0; i < cskip_info.size(); i++)
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
	for(size_t i = 0; i < CPPAD_HASH_TABLE_SIZE; i++)
		hash_table_var[i] = 0;
	CPPAD_ASSERT_UNKNOWN( tape[0].op == BeginOp );

	// initialize mapping from old variable index to new
	// operator and variable index
	for(size_t i = 0; i < num_var; i++)
	{	tape[i].new_op  = 0;       // invalid index (except for BeginOp)
		tape[i].new_var = num_var; // invalid index
	}

	// Erase all information in the old recording
	rec->free();

	// initialize mapping from old VecAD index to new VecAD index
	CppAD::vector<size_t> new_vecad_ind(num_vecad_ind);
	for(size_t i = 0; i < num_vecad_ind; i++)
		new_vecad_ind[i] = num_vecad_ind; // invalid index
	{
		size_t j = 0;     // index into the old set of indices
		for(size_t i = 0; i < num_vecad_vec; i++)
		{	// length of this VecAD
			size_t length = play->GetVecInd(j);
			if( vecad_connect[i] != not_connected )
			{	// Put this VecAD vector in new recording
				CPPAD_ASSERT_UNKNOWN(length < num_vecad_ind);
				new_vecad_ind[j] = rec->PutVecInd(length);
				for(size_t k = 1; k <= length; k++) new_vecad_ind[j+k] =
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
	}

	// start playing the operations in the forward direction
	i_op = 0;
	op   = op_info[i_op].op;
	arg  = op_info[i_op].arg;
	CPPAD_ASSERT_UNKNOWN( op == BeginOp );
	CPPAD_ASSERT_NARG_NRES(BeginOp, 1, 1);
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) == 0 );
	//
	// mapping from old to new operator indices
	vector<size_t> old_op2new_op(num_op);
	for(size_t i = 0; i < num_op; i++)
		old_op2new_op[i] = 0; // some operators are removed
	//
	// Put BeginOp at beginning of recording
	old_op2new_op[i_op] = rec->num_op_rec();
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
		{	size_t j = cskip_info_order[cskip_order_next];
			if( NumRes(op) > 0 )
				skip &= cskip_info[j].max_left_right < i_var;
			else
				skip &= cskip_info[j].max_left_right <= i_var;
		}
		if( skip )
		{	size_t j = cskip_info_order[cskip_order_next];
			cskip_order_next++;
			struct_cskip_info info = cskip_info[j];
			size_t n_true  = info.skip_old_op_true.size();
			size_t n_false = info.skip_old_op_false.size();
			skip &= n_true > 0 || n_false > 0;
			if( skip )
			{	CPPAD_ASSERT_UNKNOWN( NumRes(CSkipOp) == 0 );
				size_t n_arg   = 7 + n_true + n_false;
				// reserve space for the arguments to this operator but
				// delay setting them until we have all the new addresses
				cskip_info[j].i_arg = rec->ReserveArg(n_arg);
				CPPAD_ASSERT_UNKNOWN( cskip_info[j].i_arg > 0 );
				// There is no corresponding old operator in this case
				rec->PutOp(CSkipOp);
			}
		}
		//
		unsigned short code         = 0;
		bool           replace_hash = false;
		addr_t         match_var;
		tape[i_var].match = false;
		//
		if( op_info[i_op].usage > 0 ) switch( op )
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
				old_op2new_op[i_op] = rec->num_op_rec();
				tape[i_var].new_op  = rec->num_op_rec();
				tape[i_var].new_var = rec->PutOp(op);
				CPPAD_ASSERT_UNKNOWN(new_arg[0] < tape[i_var].new_var);
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
			// check if this is the top of a csum connection
			if( op_info[i_op].csum_connected )
				break;
			if( op_info[ var2op[arg[0]] ].csum_connected )
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
				old_op2new_op[i_op] = size_pair.i_op;
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
				old_op2new_op[i_op] = size_pair.i_op;
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
				old_op2new_op[i_op] = rec->num_op_rec();
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
			// check if this is the top of a csum connection
			if( op_info[i_op].csum_connected )
				break;
			if( op_info[ var2op[arg[1]] ].csum_connected )
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
				old_op2new_op[i_op] = size_pair.i_op;
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
				old_op2new_op[i_op] = size_pair.i_op;
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
			// check if this is the top of a csum connection
			if( op_info[i_op].csum_connected )
				break;
			if(
				op_info[ var2op[arg[0]] ].csum_connected |
				op_info[ var2op[arg[1]] ].csum_connected
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
				old_op2new_op[i_op] = size_pair.i_op;
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
				old_op2new_op[i_op] = size_pair.i_op;
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
			for(size_t i = 2; i < 6; i++)
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
			rec->PutOp(op);
			break;
			//
			case LevpOp:
			case LtvpOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			new_arg[0] = tape[arg[0]].new_var;
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			rec->PutArg(new_arg[0], new_arg[1]);
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
			rec->PutOp(op);
			break;

			// ---------------------------------------------------
			// Operations with no arguments and one result
			case InvOp:
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			old_op2new_op[i_op] = rec->num_op_rec();
			tape[i_var].new_op  = rec->num_op_rec();
			tape[i_var].new_var = rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Operations with one argument that is a parameter
			case ParOp:
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0] ) );

			rec->PutArg( new_arg[0] );
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
			tape[i_var].new_op  = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			old_op2new_op[i_op] = rec->num_op_rec();
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
			}
			else
			{	// forward_user
				CPPAD_ASSERT_UNKNOWN( user_state == end_user );
				user_state = start_user;
				//
			}
			// user_old, user_n, user_m
			if( op_info[i_op].usage > 0 )
			{	rec->PutArg(arg[0], arg[1], arg[2], arg[3]);
				old_op2new_op[i_op] = rec->num_op_rec();
				rec->PutOp(UserOp);
			}
			break;

			case UsrapOp:
			// forward_user
			++user_j;
			if( user_j == user_n )
				user_state = ret_user;
			//
			if( op_info[i_op].usage > 0 )
			{	new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
				rec->PutArg(new_arg[0]);
				old_op2new_op[i_op] = rec->num_op_rec();
				rec->PutOp(UsrapOp);
			}
			break;

			case UsravOp:
			// forward_user
			++user_j;
			if( user_j == user_n )
				user_state = ret_user;
			//
			if( op_info[i_op].usage > 0 )
			{	new_arg[0] = tape[arg[0]].new_var;
				if( size_t(new_arg[0]) < num_var )
				{	rec->PutArg(new_arg[0]);
					old_op2new_op[i_op] = rec->num_op_rec();
					rec->PutOp(UsravOp);
				}
				else
				{	// This argument does not affect the result and
					// has been optimized out so use nan in its place.
					new_arg[0] = rec->PutPar( base_nan );
					rec->PutArg(new_arg[0]);
					old_op2new_op[i_op] = rec->num_op_rec();
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
			if( op_info[i_op].usage > 0 )
			{	new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
				rec->PutArg(new_arg[0]);
				old_op2new_op[i_op] = rec->num_op_rec();
				rec->PutOp(UsrrpOp);
			}
			break;

			case UsrrvOp:
			// forward_user
			++user_i;
			if( user_i == user_m )
				user_state = end_user;
			//
			if( op_info[i_op].usage > 0 )
			{	old_op2new_op[i_op] = rec->num_op_rec();
				tape[i_var].new_op  = rec->num_op_rec();
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
	for(size_t i = 0; i < dep_taddr.size(); i++ )
	{	CPPAD_ASSERT_UNKNOWN( size_t(tape[dep_taddr[i]].new_var) < num_var );
		dep_taddr[i] = tape[ dep_taddr[i] ].new_var;
	}

# ifndef NDEBUG
	for(i_op = 0; i_op < num_op; i_op++) if( NumRes( op_info[i_op].op ) > 0 )
	{	i_var = op_info[i_op].i_var;
		CPPAD_ASSERT_UNKNOWN( tape[i_var].new_op < rec->num_op_rec() );
		CPPAD_ASSERT_UNKNOWN( old_op2new_op[i_op] == tape[i_var].new_op );
	}
# endif
	// fill in the arguments for the CSkip operations
	CPPAD_ASSERT_UNKNOWN( cskip_order_next == cskip_info.size() );
	for(size_t i = 0; i < cskip_info.size(); i++)
	{	struct_cskip_info info = cskip_info[i];
		if( info.i_arg > 0 )
		{
			size_t n_true  = info.skip_old_op_true.size();
			size_t n_false = info.skip_old_op_false.size();
			size_t i_arg   = info.i_arg;
			rec->ReplaceArg(i_arg++, info.cop   );
			rec->ReplaceArg(i_arg++, info.flag  );
			rec->ReplaceArg(i_arg++, info.left  );
			rec->ReplaceArg(i_arg++, info.right );
			rec->ReplaceArg(i_arg++, n_true     );
			rec->ReplaceArg(i_arg++, n_false    );
			for(size_t j = 0; j < info.skip_old_op_true.size(); j++)
			{	i_op = info.skip_old_op_true[j];
				bool remove = old_op2new_op[i_op] == 0;
				if( NumRes( op_info[i_op].op ) > 0 )
					if( tape[ op_info[i_op].i_var].match )
						remove = true;
				if( remove )
				{	// This operation has been removed or matched,
					// so use an operator index that never comes up.
					rec->ReplaceArg(i_arg++, rec->num_op_rec());
				}
				else
					rec->ReplaceArg(i_arg++, old_op2new_op[i_op] );
			}
			for(size_t j = 0; j < info.skip_old_op_false.size(); j++)
			{	i_op   = info.skip_old_op_false[j];
				bool remove = old_op2new_op[i_op] == 0;
				if( NumRes( op_info[i_op].op ) > 0 )
					if( tape[ op_info[i_op].i_var].match )
						remove = true;
				if( remove )
				{	// This operation has been removed or matched,
					// so use an operator index that never comes up.
					rec->ReplaceArg(i_arg++, rec->num_op_rec());
				}
				else
					rec->ReplaceArg(i_arg++, old_op2new_op[i_op] );
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
