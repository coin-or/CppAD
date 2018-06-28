# ifndef CPPAD_LOCAL_OPTIMIZE_OPTIMIZE_RUN_HPP
# define CPPAD_LOCAL_OPTIMIZE_OPTIMIZE_RUN_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <stack>
# include <iterator>
# include <cppad/local/optimize/get_usage.hpp>
# include <cppad/local/optimize/get_previous.hpp>
# include <cppad/local/optimize/get_cexp_info.hpp>
# include <cppad/local/optimize/size_pair.hpp>
# include <cppad/local/optimize/csum_variable.hpp>
# include <cppad/local/optimize/csum_stacks.hpp>
# include <cppad/local/optimize/cexp_info.hpp>
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

\tparam Addr
Type to use with player random iterators. Must correspond to the
result for play->addr_type().

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type
\a Base.

\param options
\li
If the sub-string "no_conditional_skip" appears,
conditional skip operations will not be generated.
This may make the optimize routine use significantly less memory
and take significantly less time.
\li
If the sub-string "no_compare_op" appears,
then comparison operators will be removed from the optimized tape.
These operators are necessary for the compare_change function to be
be meaningful in the resulting recording.
On the other hand, they are not necessary and take extra time
when compare_change is not used.
\li
If the sub-string "no_print_for" appears,
then print forward (PriOp) operators will be removed from the optimized tape.
These operators are useful for reporting problems evaluating derivatives
at independent variable values different from those used to record a function.

\param n
is the number of independent variables on the tape.

\param dep_taddr
On input this vector contains the indices for each of the dependent
variable values in the operation sequence corresponding to \a play.
Upon return it contains the indices for the same variables but in
the operation sequence corresponding to \a rec.

\param play
This is the operation sequence that we are optimizing.
It is const except for the fact that play->setup_random is called.

\param rec
The input contents of this recording must be empty; i.e.,
it corresponds to directly after the default constructor.
Upon return, it contains an optimized verison of the
operation sequence corresponding to \a play.
*/

template <class Addr, class Base>
void optimize_run(
	const std::string&                         options    ,
	size_t                                     n          ,
	vector<size_t>&                            dep_taddr  ,
	player<Base>*                              play       ,
	recorder<Base>*                            rec        )
{	CPPAD_ASSERT_UNKNOWN( rec->num_op_rec() == 0 );
	//
	// get a random iterator for this player
	play->template setup_random<Addr>();
	local::play::const_random_iterator<Addr> random_itr =
		play->template get_random<Addr>();

	bool conditional_skip = true;
	bool compare_op       = true;
	bool print_for_op     = true;
	size_t index = 0;
	while( index < options.size() )
	{	while( index < options.size() && options[index] == ' ' )
			++index;
		std::string option;
		while( index < options.size() && options[index] != ' ' )
			option += options[index++];
		if( option != "" )
		{	if( option == "no_conditional_skip" )
				conditional_skip = false;
			else if( option == "no_compare_op" )
				compare_op = false;
			else if( option == "no_print_for_op" )
				print_for_op = false;
			else
			{	option += " is not a valid optimize option";
				CPPAD_ASSERT_KNOWN( false , option.c_str() );
			}
		}
	}
	// number of operators in the player
	const size_t num_op = play->num_op_rec();
	CPPAD_ASSERT_UNKNOWN(
		num_op < size_t( std::numeric_limits<addr_t>::max() )
	);

	// number of variables in the player
	const size_t num_var = play->num_var_rec();

	// number of  VecAD indices
	size_t num_vecad_ind   = play->num_vec_ind_rec();

	// number of VecAD vectors
	size_t num_vecad_vec   = play->num_vecad_vec_rec();

	// operator information
	pod_vector<addr_t>        cexp2op;
	sparse_list               cexp_set;
	pod_vector<bool>          vecad_used;
	pod_vector<usage_t>       op_usage;
	get_usage(
		conditional_skip,
		compare_op,
		print_for_op,
		play,
		random_itr,
		dep_taddr,
		cexp2op,
		cexp_set,
		vecad_used,
		op_usage
	);
	pod_vector<addr_t>        op_previous;
	get_previous(
		play,
		random_itr,
		cexp_set,
		op_previous,
		op_usage
	);
	size_t num_cexp = cexp2op.size();
	CPPAD_ASSERT_UNKNOWN( conditional_skip || num_cexp == 0 );
	vector<struct_cexp_info>  cexp_info; // struct_cexp_info not POD
	sparse_list               skip_op_true;
	sparse_list               skip_op_false;
	//
	if( cexp2op.size() > 0 ) get_cexp_info(
		play,
		random_itr,
		op_previous,
		op_usage,
		cexp2op,
		cexp_set,
		cexp_info,
		skip_op_true,
		skip_op_false
	);

	// We no longer need cexp_set, and cexp2op, so free their memory
	cexp_set.resize(0, 0);
	cexp2op.clear();

	// nan with type Base
	Base base_nan = Base( std::numeric_limits<double>::quiet_NaN() );

	// -------------------------------------------------------------
	// information for current operator
	size_t        i_op;   // index
	OpCode        op;     // operator
	const addr_t* arg;    // arguments
	size_t        i_var;  // variable index of primary (last) result

	enum_user_state user_state;
	// -------------------------------------------------------------
	// conditional expression information
	//
	// Size of the conditional expression information structure.
	// This is equal to the number of conditional expressions when
	// conditional_skip is true, otherwise it is zero.
	//
	// sort the conditional expression information by max_left_right
	// this is the conditional skip order
	vector<size_t> cskip_order(num_cexp);
	if( num_cexp > 0 )
	{	vector<size_t> keys(num_cexp);
		for(size_t i = 0; i < num_cexp; i++)
			keys[i] = cexp_info[i].max_left_right;
		CppAD::index_sort(keys, cskip_order);
	}
	// initial index in conditional skip order
	size_t cskip_order_next = 0;
	//
	// initialize index in conditional expression order
	size_t cexp_next = 0;

	// mapping from conditional expression index to conditional skip
	// information on new tape
	pod_vector<struct_cskip_new> cskip_new(num_cexp);
	//
	// flag used to indicate that there is no conditional skip
	// for this conditional expression
	for(size_t i = 0; i < num_cexp; i++)
		cskip_new[i].i_arg = 0;
	// -------------------------------------------------------------

	// initialize mapping from old VecAD index to new VecAD index
	CPPAD_ASSERT_UNKNOWN(
		size_t( std::numeric_limits<addr_t>::max() ) >= num_vecad_ind
	);
	pod_vector<addr_t> new_vecad_ind(num_vecad_ind);
	for(size_t i = 0; i < num_vecad_ind; i++)
		new_vecad_ind[i] = addr_t( num_vecad_ind ); // invalid index
	{
		size_t j = 0;     // index into the old set of indices
		for(size_t i = 0; i < num_vecad_vec; i++)
		{	// length of this VecAD
			size_t length = play->GetVecInd(j);
			if( vecad_used[i] )
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

	// temporary buffer for new argument values
	addr_t new_arg[6];

	// temporary work space used by record_csum
	// (decalared here to avoid realloaction of memory)
	struct_csum_stacks csum_work;

	// tempory used to hold a size_pair
	struct_size_pair size_pair;
	//
	// Mapping from old operator index to new variable index,
	// zero is invalid except for new_var[0].
	pod_vector<addr_t> new_var(num_op);
	//
	// Mapping from old operator index to new operator index will share
	// memory with op_previous. Must get op_previous[i_op] for this operator
	// before over writting it with new_op[i_op].
	pod_vector<addr_t>& new_op( op_previous );
	CPPAD_ASSERT_UNKNOWN( new_op.size() == num_op );
	//
	user_state = start_user;
	i_var      = 0;
	for(i_op = 0; i_op < num_op; ++i_op)
	{	// if non-zero, use previous result in place of this operator.
		// Must get this information before writing new_op[i_op].
		size_t previous = op_previous[i_op];
		//
		// zero is invalid except for new_op[0].
		new_op[i_op] = 0;
		//
		// Zero is invalid except for new_var[0] and previous is zero unless
		// this operator is replace by a previous operator.
		new_var[i_op] = 0;
		if( op_usage[i_op] == usage_t(yes_usage) )
			new_var[i_op] = new_var[previous];
		//
		// temporary used in some switch cases
		addr_t mask;
		//
		// this operator information
		size_t i_tmp;
		random_itr.op_info(i_op, op, arg, i_tmp);
		if( NumRes(op) > 0 )
			i_var = i_tmp;
		//
		// determine if we should insert a conditional skip here
		bool skip  = conditional_skip;
		if( skip )
		{	skip      &= cskip_order_next < num_cexp;
			skip      &= op != BeginOp;
			skip      &= op != InvOp;
			skip      &= user_state == start_user;
			if( skip )
			{	size_t j = cskip_order[cskip_order_next];
				if( NumRes(op) > 0 )
					skip &= cexp_info[j].max_left_right < i_var;
				else
					skip &= cexp_info[j].max_left_right <= i_var;
			}
			if( skip )
			{	size_t j = cskip_order[cskip_order_next];
				cskip_order_next++;
				size_t n_true   = skip_op_true.number_elements(j);
				size_t n_false  = skip_op_false.number_elements(j);
				skip &= n_true > 0 || n_false > 0;
				if( skip )
				{	CPPAD_ASSERT_UNKNOWN( NumRes(CSkipOp) == 0 );
					size_t n_arg   = 7 + n_true + n_false;
					// reserve space for the arguments to this operator but
					// delay setting them until we have all the new addresses
					cskip_new[j].i_arg = rec->ReserveArg(n_arg);
					// i_arg == 0 is used to check if conditional expression
					// has been skipped.
					CPPAD_ASSERT_UNKNOWN( cskip_new[j].i_arg > 0 );
					// There is no corresponding old operator in this case
					rec->PutOp(CSkipOp);
				}
			}
		}
		if( op == UserOp )
		{	if( user_state == start_user )
				user_state = end_user;
			else
			{	CPPAD_ASSERT_UNKNOWN( user_state == end_user );
				user_state = start_user;
			}
		}
		//
		CPPAD_ASSERT_UNKNOWN(
			size_t( std::numeric_limits<addr_t>::max() ) >= rec->num_op_rec()
		);
		//
		if( op_usage[i_op] != usage_t(yes_usage) )
		{	if( op == CExpOp )
				++cexp_next;
		}
		else switch( op )
		{	// op_usage[i_op] == usage_t(yes_usage)

			case BeginOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			// Put BeginOp at beginning of recording
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutOp(BeginOp);
			rec->PutArg(arg[0]);
			break;

			// --------------------------------------------------------------
			// Unary operators, argument a variable, one result
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
			if( previous == 0 )
			{	//
				new_arg[0]   = new_var[ random_itr.var2op(arg[0]) ];
				rec->PutArg( new_arg[0] );
				//
				new_op[i_op]  = addr_t( rec->num_op_rec() );
				new_var[i_op] = rec->PutOp(op);
				CPPAD_ASSERT_UNKNOWN(
					new_arg[0] < new_var[random_itr.var2op(i_var)]
				);
				if( op == ErfOp )
				{	CPPAD_ASSERT_NARG_NRES(op, 3, 5);
					// Error function is a special case
					// second argument is always the parameter 0
					// third argument is always the parameter 2 / sqrt(pi)
					CPPAD_ASSERT_UNKNOWN( NumArg(ErfOp) == 3 );
					rec->PutArg( rec->PutPar( Base(0.0) ) );
					rec->PutArg( rec->PutPar(
						Base( 1.0 / std::sqrt( std::atan(1.0) ) )
					) );
				}
				else
				{	// some of these operators have an auxillary result;
					// e.g. sine and cosine are computed together.
					CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
					CPPAD_ASSERT_UNKNOWN( NumRes(op) ==1 || NumRes(op) == 2 );
				}
			}
			break;
			// ---------------------------------------------------
			// Binary operators, left variable, right parameter, one result
			case SubvpOp:
			// check if this is the top of a csum connection
			if( op_usage[ random_itr.var2op(arg[0]) ] == usage_t(csum_usage) )
			{	CPPAD_ASSERT_UNKNOWN( previous == 0 );
				//
				// convert to a sequence of summation operators
				size_pair = record_csum(
					play                ,
					random_itr          ,
					op_usage            ,
					new_var             ,
					i_var               ,
					rec                 ,
					csum_work
				);
				new_op[i_op]  = addr_t( size_pair.i_op );
				new_var[i_op] = addr_t( size_pair.i_var );
				// abort rest of this case
				break;
			}
			case DivvpOp:
			case PowvpOp:
			case ZmulvpOp:
			if( previous == 0 )
			{	//
				size_pair = record_vp(
					play                ,
					random_itr          ,
					new_var             ,
					i_op                ,
					rec
				);
				new_op[i_op]  = addr_t( size_pair.i_op );
				new_var[i_op] = addr_t( size_pair.i_var );
			}
			break;
			// ---------------------------------------------------
			// Binary operators, left index, right variable, one result
			case DisOp:
			CPPAD_ASSERT_NARG_NRES(op, 2, 1);
			if( previous == 0 )
			{	//
				new_arg[0] = arg[0];
				new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
				rec->PutArg( new_arg[0], new_arg[1] );
				//
				new_op[i_op]  = addr_t( rec->num_op_rec() );
				new_var[i_op] = rec->PutOp(op);
				CPPAD_ASSERT_UNKNOWN(
					new_arg[1] < new_var[random_itr.var2op(i_var)]
				);
			}
			break;

			// ---------------------------------------------------
			// Binary operators, left parameter, right variable, one result
			case SubpvOp:
			case AddpvOp:
			// check if this is the top of a csum connection
			if( op_usage[ random_itr.var2op(arg[1]) ] == usage_t(csum_usage) )
			{	CPPAD_ASSERT_UNKNOWN( previous == 0 );
				//
				// convert to a sequence of summation operators
				size_pair = record_csum(
					play                ,
					random_itr          ,
					op_usage            ,
					new_var             ,
					i_var               ,
					rec                 ,
					csum_work
				);
				new_op[i_op]  = addr_t( size_pair.i_op );
				new_var[i_op] = addr_t( size_pair.i_var );
				// abort rest of this case
				break;
			}
			case DivpvOp:
			case MulpvOp:
			case PowpvOp:
			case ZmulpvOp:
			if( previous == 0 )
			{	//
				size_pair = record_pv(
					play                ,
					random_itr          ,
					new_var             ,
					i_op                ,
					rec
				);
				new_op[i_op]  = addr_t( size_pair.i_op );
				new_var[i_op] = addr_t( size_pair.i_var );
			}
			break;
			// ---------------------------------------------------
			// Binary operator, left and right variables, one result
			case AddvvOp:
			case SubvvOp:
			// check if this is the top of a csum connection
			if(
				op_usage[ random_itr.var2op(arg[0]) ] == usage_t(csum_usage)
				||
				op_usage[ random_itr.var2op(arg[1]) ] == usage_t(csum_usage)
			)
			{	CPPAD_ASSERT_UNKNOWN( previous == 0 );
				//
				// convert to a sequence of summation operators
				size_pair = record_csum(
					play                ,
					random_itr          ,
					op_usage            ,
					new_var             ,
					i_var               ,
					rec                 ,
					csum_work
				);
				new_op[i_op]  = addr_t( size_pair.i_op );
				new_var[i_op] = addr_t( size_pair.i_var );
				// abort rest of this case
				break;
			}
			case DivvvOp:
			case MulvvOp:
			case PowvvOp:
			case ZmulvvOp:
			if( previous == 0 )
			{	//
				size_pair = record_vv(
					play                ,
					random_itr          ,
					new_var             ,
					i_op                ,
					rec
				);
				new_op[i_op]  = addr_t( size_pair.i_op );
				new_var[i_op] = addr_t( size_pair.i_var );
			}
			break;
			// ---------------------------------------------------
			// Conditional expression operators
			case CExpOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 6, 1);
			new_arg[0] = arg[0];
			new_arg[1] = arg[1];
			mask = 1;
			for(size_t i = 2; i < 6; i++)
			{	if( arg[1] & mask )
				{	new_arg[i] = new_var[ random_itr.var2op(arg[i]) ];
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
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutOp(op);
			//
			// The new addresses for left and right are used during
			// fill in the arguments for the CSkip operations. This does not
			// affect max_left_right which is used during this sweep.
			if( conditional_skip )
			{	CPPAD_ASSERT_UNKNOWN( cexp_next < num_cexp );
				CPPAD_ASSERT_UNKNOWN( cexp_info[cexp_next].i_op == i_op );
				cskip_new[ cexp_next ].left  = new_arg[2];
				cskip_new[ cexp_next ].right = new_arg[3];
				++cexp_next;
			}
			break;
			// ---------------------------------------------------
			// Operations with no arguments and no results
			case EndOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 0, 0);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(op);
			break;
			// ---------------------------------------------------
			// Comparison operations: two arguments and no results
			case LepvOp:
			case LtpvOp:
			case EqpvOp:
			case NepvOp:
			CPPAD_ASSERT_UNKNOWN( compare_op );
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			if( previous == 0 )
			{	new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
				new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
				rec->PutArg(new_arg[0], new_arg[1]);
				new_op[i_op] = addr_t( rec->num_op_rec() );
				rec->PutOp(op);
			}
			break;
			//
			case LevpOp:
			case LtvpOp:
			CPPAD_ASSERT_UNKNOWN( compare_op );
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			if( previous == 0 )
			{	new_arg[0] = new_var[ random_itr.var2op(arg[0]) ];
				new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
				rec->PutArg(new_arg[0], new_arg[1]);
				new_op[i_op] = addr_t( rec->num_op_rec() );
				rec->PutOp(op);
			}
			break;
			//
			case LevvOp:
			case LtvvOp:
			case EqvvOp:
			case NevvOp:
			CPPAD_ASSERT_UNKNOWN( compare_op );
			CPPAD_ASSERT_NARG_NRES(op, 2, 0);
			if( previous == 0 )
			{	new_arg[0] = new_var[ random_itr.var2op(arg[0]) ];
				new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
				rec->PutArg(new_arg[0], new_arg[1]);
				new_op[i_op] = addr_t( rec->num_op_rec() );
				rec->PutOp(op);
			}
			break;

			// ---------------------------------------------------
			// Operations with no arguments and one result
			case InvOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutOp(op);
			break;

			// ---------------------------------------------------
			// Unary operators, argument a parameter, one result
			case ParOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 1, 1);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0] ) );
			rec->PutArg( new_arg[0] );
			//
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutOp(op);
			break;

			// ---------------------------------------------------
			// print forward operator
			case PriOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 5, 0);
			// arg[0]
			new_arg[0] = arg[0];
			//
			// arg[1]
			if( arg[0] & 1 )
			{	new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
				CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			}
			else
			{	new_arg[1] = rec->PutPar( play->GetPar( arg[1] ) );
			}
			//
			// arg[3]
			if( arg[0] & 2 )
			{	new_arg[3] = new_var[ random_itr.var2op(arg[3]) ];
				CPPAD_ASSERT_UNKNOWN( size_t(new_arg[3]) < num_var );
			}
			else
			{	new_arg[3] = rec->PutPar( play->GetPar( arg[3] ) );
			}
			new_arg[2] = rec->PutTxt( play->GetTxt(arg[2]) );
			new_arg[4] = rec->PutTxt( play->GetTxt(arg[4]) );
			//
			rec->PutArg(
				new_arg[0] ,
				new_arg[1] ,
				new_arg[2] ,
				new_arg[3] ,
				new_arg[4]
			);
			// new operator
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			// no new variable
			rec->PutOp(op);
			break;

			// ---------------------------------------------------
			// VecAD operators

			// Load using a parameter index
			case LdpOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = arg[1];
			CPPAD_ASSERT_UNKNOWN(
				size_t( std::numeric_limits<addr_t>::max() ) >= rec->num_load_op_rec()
			);
			new_arg[2] = addr_t( rec->num_load_op_rec() );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutLoadOp(op);
			break;

			// Load using a variable index
			case LdvOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 3, 1);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
			CPPAD_ASSERT_UNKNOWN(
				size_t( std::numeric_limits<addr_t>::max() ) >= rec->num_load_op_rec()
			);
			new_arg[2] = addr_t( rec->num_load_op_rec() );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutLoadOp(op);
			break;

			// Store a parameter using a parameter index
			case StppOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
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
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(op);
			break;

			// Store a parameter using a variable index
			case StvpOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
			new_arg[2] = rec->PutPar( play->GetPar(arg[2]) );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(op);
			break;

			// Store a variable using a parameter index
			case StpvOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = rec->PutPar( play->GetPar(arg[1]) );
			new_arg[2] = new_var[ random_itr.var2op(arg[2]) ];
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[2]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(op);
			break;

			// Store a variable using a variable index
			case StvvOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 3, 0);
			new_arg[0] = new_vecad_ind[ arg[0] ];
			new_arg[1] = new_var[ random_itr.var2op(arg[1]) ];
			new_arg[2] = new_var[ random_itr.var2op(arg[2]) ];
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < num_vecad_ind );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[1]) < num_var );
			CPPAD_ASSERT_UNKNOWN( size_t(new_arg[2]) < num_var );
			rec->PutArg(
				new_arg[0],
				new_arg[1],
				new_arg[2]
			);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(op);
			break;

			// -----------------------------------------------------------
			// user atomic function call operators

			case UserOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 4, 0);
			// user_old, user_n, user_m
			rec->PutArg(arg[0], arg[1], arg[2], arg[3]);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(UserOp);
			break;

			case UsrapOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 1, 0);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
			rec->PutArg(new_arg[0]);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(UsrapOp);
			break;

			case UsravOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 1, 0);
			new_arg[0] = new_var[ random_itr.var2op(arg[0]) ];
			if( size_t(new_arg[0]) < num_var )
			{	rec->PutArg(new_arg[0]);
				new_op[i_op] = addr_t( rec->num_op_rec() );
				rec->PutOp(UsravOp);
			}
			else
			{	// This argument does not affect the result and
				// has been optimized out so use nan in its place.
				new_arg[0] = rec->PutPar( base_nan );
				rec->PutArg(new_arg[0]);
				new_op[i_op] = addr_t( rec->num_op_rec() );
				rec->PutOp(UsrapOp);
			}
			break;

			case UsrrpOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 1, 0);
			new_arg[0] = rec->PutPar( play->GetPar(arg[0]) );
			rec->PutArg(new_arg[0]);
			new_op[i_op] = addr_t( rec->num_op_rec() );
			rec->PutOp(UsrrpOp);
			break;

			case UsrrvOp:
			CPPAD_ASSERT_UNKNOWN( previous == 0 );
			CPPAD_ASSERT_NARG_NRES(op, 0, 1);
			new_op[i_op]  = addr_t( rec->num_op_rec() );
			new_var[i_op] = rec->PutOp(UsrrvOp);
			break;
			// ---------------------------------------------------

			// all cases should be handled above
			default:
			CPPAD_ASSERT_UNKNOWN(false);

		}
	}
	// modify the dependent variable vector to new indices
	for(size_t i = 0; i < dep_taddr.size(); i++ )
	{	dep_taddr[i] = new_var[ random_itr.var2op(dep_taddr[i]) ];
		CPPAD_ASSERT_UNKNOWN( size_t(dep_taddr[i]) < num_var );
	}

# ifndef NDEBUG
	for(i_op = 0; i_op < num_op; i_op++)
	{	random_itr.op_info(i_op, op, arg, i_var);
		if( NumRes(op) > 0 )
			CPPAD_ASSERT_UNKNOWN(
				size_t(new_op[i_op]) < rec->num_op_rec()
			);
	}
# endif
	// make sure that all the conditional expressions have been
	// checked to see if they are still present
	CPPAD_ASSERT_UNKNOWN( cskip_order_next == num_cexp );
	// fill in the arguments for the CSkip operations
	for(size_t i = 0; i < num_cexp; i++)
	{	// if cskip_new[i].i_arg == 0, this conditional expression was skipped
		if( cskip_new[i].i_arg > 0 )
		{	struct_cexp_info info = cexp_info[i];
			size_t n_true  = skip_op_true.number_elements(i);
			size_t n_false = skip_op_false.number_elements(i);
			size_t i_arg   = cskip_new[i].i_arg;
			size_t left    = cskip_new[i].left;
			size_t right   = cskip_new[i].right;
			rec->ReplaceArg(i_arg++, info.cop   );
			rec->ReplaceArg(i_arg++, info.flag  );
			rec->ReplaceArg(i_arg++, left  );
			rec->ReplaceArg(i_arg++, right );
			rec->ReplaceArg(i_arg++, n_true     );
			rec->ReplaceArg(i_arg++, n_false    );
			sparse_list::const_iterator itr_true(skip_op_true, i);
			while( *itr_true != skip_op_true.end() )
			{	i_op = *itr_true;
				// op_usage[i_op] == usage_t(yes_usage)
				CPPAD_ASSERT_UNKNOWN( new_op[i_op] != 0 );
				rec->ReplaceArg(i_arg++, new_op[i_op] );
				//
				++itr_true;
			}
			sparse_list::const_iterator itr_false(skip_op_false, i);
			while( *itr_false != skip_op_false.end() )
			{	i_op   = *itr_false;
				// op_usage[i_op] == usage_t(yes_usage)
				CPPAD_ASSERT_UNKNOWN( new_op[i_op] != 0 );
				rec->ReplaceArg(i_arg++, new_op[i_op] );
				//
				++itr_false;
			}
			rec->ReplaceArg(i_arg++, n_true + n_false);
# ifndef NDEBUG
			size_t n_arg   = 7 + n_true + n_false;
			CPPAD_ASSERT_UNKNOWN( cskip_new[i].i_arg + n_arg == i_arg );
# endif
		}
	}
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
