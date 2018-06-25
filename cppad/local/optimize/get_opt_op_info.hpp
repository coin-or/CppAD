# ifndef CPPAD_LOCAL_OPTIMIZE_GET_OPT_OP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_GET_OPT_OP_INFO_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file get_opt_op_info.hpp
Create operator information tables
*/

# include <cppad/local/optimize/match_op.hpp>
# include <cppad/local/optimize/cexp_info.hpp>
# include <cppad/local/optimize/usage.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/*!
Get variable to operator map and operator basic operator information

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type
\a Base.

\param conditional_skip
If conditional_skip this is true, the conditional expression information
cexp_info will be calculated.
This may be time intensive and may not have much benefit in the optimized
recording.

\param compare_op
if this is true, arguments are considered used if they appear in compare
operators. This is a side effect because compare operators have boolean
results (and the result is not in the tape; i.e. NumRes(op) is zero
for these operators. (This is an example of a side effect.)

\param print_for_op
if this is true, arguments are considered used if they appear in
print forward operators; i.e., PriOp.
This is also a side effect; i.e. NumRes(PriOp) is zero.

\param play
This is the old operation sequence.

\param random_itr
This is a random iterator for the old operation sequence.

\param dep_taddr
is a vector of variable indices for the dependent variables.

\param cexp2op
The input size of this vector must be zero.
Upon retun it has size equal to the number of conditional expressions,
CExpOp operators. The value $icode%cexp2op[%j%]%$$ is the operator
index corresponding to the $th j$$ operator.

\param cexp_set
This is a vector of sets that is empty on input.
If conditional_skip is false, cexp_usage is not modified.
Otherwise, set[i] is a set of elements for the i-th operator.
Suppose that e is an element of set[i], j = e / 2, k = e % 2.
If the comparision for the j-th conditional expression is equal to bool(k),
the i-th operator can be skipped (is not used by any of the results).
Note the the j indexs the CExpOp operators in the operation sequence.

\param cexp_info
The input size of this vector must be zero.
If conditional_skip is false, cexp_info is not changed.
Otherwise,
upon return cexp_info has size equal to the number of conditional expressions
in the operation sequence; i.e., the number of CExpOp operators.
The value cexp_info[j] is the information corresponding to the j-th
conditional expression in the operation sequence.
This vector is in the same order as the operation sequence; i.e.
if j1 > j2, cexp_info[j1].i_op > cexp_info[j2].i_op.
Note that skip_op_true and skip_op_false could be part of this structure,
but then we would allocate and deallocate two vectors for each conditonal
expression in the operation sequence.

\param skip_op_true
This vector of sets is empty on input.
Upon return, the j-th set is the operators that are not used when
comparison result for cexp_info[j] is true.
Note that UsrapOp, UsravOp, UsrrpOp, and UsrrvOp, are not in this
set and should be skipped when the corresponding UserOp are skipped.

\param skip_op_false
This vector of sets is empty on input.
Upon return, the j-th set is the operators that are not used when
comparison result for cexp_info[j] is false.
Note that UsrapOp, UsravOp, UsrrpOp, and UsrrvOp, are not in this
set and should be skipped when the corresponding UserOp are skipped.

\param vecad_used
The input size of this vector must be zero.
Upon retun it has size equal to the number of VecAD vectors
in the operations sequences; i.e., play->num_vecad_vec_rec().
The VecAD vectors are indexed in the order that thier indices apprear
in the one large play->GetVecInd that holds all the VecAD vectors.

\param op_previous
The input size of this vector must be zero.
Upon return it has size equal to the number of operators
in the operation sequence; i.e., num_op = play->nun_var_rec().
If op_previous[i] == 0, no replacement was found for the i-th operator.
If op_previous[i] != 0, op_usage[ op_previous[i] ] == usage_t(yes_usage).


\param op_usage
The input size of this vector must be zero.
Upon return it has size equal to the number of operators
in the operation sequence; i.e., num_op = play->nun_var_rec().
The value op_usage[i]
have been set to the usage for
the i-th operator in the operation sequence.
*/

template <class Addr, class Base>
void get_opt_op_info(
	bool                                        conditional_skip    ,
	bool                                        compare_op          ,
	bool                                        print_for_op        ,
	const player<Base>*                         play                ,
	const play::const_random_iterator<Addr>&    random_itr          ,
	const vector<size_t>&                       dep_taddr           ,
	const pod_vector<addr_t>&                   cexp2op             ,
	sparse_list&                                cexp_set            ,
	vector<struct_cexp_info>&                   cexp_info           ,
	sparse_list&                                skip_op_true        ,
	sparse_list&                                skip_op_false       ,
	pod_vector<bool>&                           vecad_used          ,
	pod_vector<addr_t>&                         op_previous         ,
	pod_vector<usage_t>&                        op_usage            )
{
	CPPAD_ASSERT_UNKNOWN( cexp_info.size() == 0 );
	CPPAD_ASSERT_UNKNOWN( op_previous.size() == 0 );

	// number of operators in the tape
	const size_t num_op = play->num_op_rec();
	op_usage.resize( num_op );
	op_previous.resize( num_op );
	CPPAD_ASSERT_UNKNOWN( op_usage.size() == num_op );
	//
	// number of conditional expressions in the tape
	size_t num_cexp_op = cexp2op.size();
	//
	// initialize mapping from variable index to operator index
	CPPAD_ASSERT_UNKNOWN(
		size_t( std::numeric_limits<addr_t>::max() ) >= num_op
	);
	// -----------------------------------------------------------------------
	// information about current operator
	OpCode        op;     // operator
	const addr_t* arg;    // arguments
	size_t        i_op;   // operator index
	size_t        i_var;  // variable index of first result
	// ----------------------------------------------------------------------
	// compute op_previous
	// ----------------------------------------------------------------------
	sparse_list  hash_table_op;
	hash_table_op.resize(CPPAD_HASH_TABLE_SIZE, num_op);
	//
	pod_vector<bool> match_work;
	for(i_op = 0; i_op < num_op; ++i_op)
	{	op_previous[i_op] = 0;

		if( op_usage[i_op] == usage_t(yes_usage) )
		switch( play->GetOp(i_op) )
		{
			case NumberOp:
			CPPAD_ASSERT_UNKNOWN(false);
			break;

			case BeginOp:
			case CExpOp:
			case CSkipOp:
			case CSumOp:
			case EndOp:
			case InvOp:
			case LdpOp:
			case LdvOp:
			case ParOp:
			case PriOp:
			case StppOp:
			case StpvOp:
			case StvpOp:
			case StvvOp:
			case UserOp:
			case UsrapOp:
			case UsravOp:
			case UsrrpOp:
			case UsrrvOp:
			// these operators never match pevious operators
			break;

			case AbsOp:
			case AcosOp:
			case AcoshOp:
			case AddpvOp:
			case AddvvOp:
			case AsinOp:
			case AsinhOp:
			case AtanOp:
			case AtanhOp:
			case CosOp:
			case CoshOp:
			case DisOp:
			case DivpvOp:
			case DivvpOp:
			case DivvvOp:
			case EqpvOp:
			case EqvvOp:
			case ErfOp:
			case ExpOp:
			case Expm1Op:
			case LepvOp:
			case LevpOp:
			case LevvOp:
			case LogOp:
			case Log1pOp:
			case LtpvOp:
			case LtvpOp:
			case LtvvOp:
			case MulpvOp:
			case MulvvOp:
			case NepvOp:
			case NevvOp:
			case PowpvOp:
			case PowvpOp:
			case PowvvOp:
			case SignOp:
			case SinOp:
			case SinhOp:
			case SqrtOp:
			case SubpvOp:
			case SubvpOp:
			case SubvvOp:
			case TanOp:
			case TanhOp:
			case ZmulpvOp:
			case ZmulvpOp:
			case ZmulvvOp:
			// check for a previous match
			match_op(random_itr, op_previous, i_op, hash_table_op, match_work);
			if( op_previous[i_op] != 0 )
			{	// like a unary operator that assigns i_op equal to previous.
				size_t previous = op_previous[i_op];
				bool sum_op = false;
				CPPAD_ASSERT_UNKNOWN( previous < i_op );
				increase_arg_usage(
					play, sum_op, i_op, previous, op_usage, cexp_set
				);
			}
			break;
		}
	}
	// ----------------------------------------------------------------------
	// compute cexp_info
	// ----------------------------------------------------------------------
	if( cexp_set.n_set() == 0 )
		return;
	//
	// initialize information for each conditional expression
	cexp_info.resize(num_cexp_op);
	skip_op_true.resize(num_cexp_op, num_op);
	skip_op_false.resize(num_cexp_op, num_op);
	//
	for(size_t i = 0; i < num_cexp_op; i++)
	{	CPPAD_ASSERT_UNKNOWN(
			op_previous[i] == 0 || op_usage[i] == usage_t(yes_usage)
		);
		i_op            = cexp2op[i];
		random_itr.op_info(i_op, op, arg, i_var);
		CPPAD_ASSERT_UNKNOWN( op == CExpOp );
		//
		struct_cexp_info info;
		info.i_op       = i_op;
		info.cop        = CompareOp( arg[0] );
		info.flag       = arg[1];
		info.left       = arg[2];
		info.right      = arg[3];
		//
		// max_left_right
		size_t index    = 0;
		if( arg[1] & 1 )
			index = std::max(index, info.left);
		if( arg[1] & 2 )
			index = std::max(index, info.right);
		CPPAD_ASSERT_UNKNOWN( index > 0 );
		info.max_left_right = index;
		//
		cexp_info[i] = info;
	};
	// Determine which operators can be conditionally skipped
	i_op = 0;
	while(i_op < num_op)
	{	size_t j_op = i_op;
		bool keep = op_usage[i_op] != usage_t(no_usage);
		keep     &= op_usage[i_op] != usage_t(csum_usage);
		keep     &= op_previous[i_op] == 0;
		if( keep )
		{	sparse_list_const_iterator itr(cexp_set, i_op);
			if( *itr != cexp_set.end() )
			{	if( play->GetOp(i_op) == UserOp )
				{	// i_op is the first operations in this user atomic call.
					// Find the last operation in this call.
					++j_op;
					while( play->GetOp(j_op) != UserOp )
					{	switch( play->GetOp(j_op) )
						{	case UsrapOp:
							case UsravOp:
							case UsrrpOp:
							case UsrrvOp:
							break;

							default:
							CPPAD_ASSERT_UNKNOWN(false);
						}
						++j_op;
					}
				}
			}
			while( *itr != cexp_set.end() )
			{	size_t element = *itr;
				size_t index   = element / 2;
				bool   compare = bool( element % 2 );
				if( compare == false )
				{	// cexp_info[index].skip_op_false.push_back(i_op);
					skip_op_false.add_element(index, i_op);
					if( j_op != i_op )
					{	// cexp_info[index].skip_op_false.push_back(j_op);
						skip_op_false.add_element(index, j_op);
					}
				}
				else
				{	// cexp_info[index].skip_op_true.push_back(i_op);
					skip_op_true.add_element(index, i_op);
					if( j_op != i_op )
					{	// cexp_info[index].skip_op_true.push_back(j_op);
						skip_op_true.add_element(index, j_op);
					}
				}
				++itr;
			}
		}
		CPPAD_ASSERT_UNKNOWN( i_op <= j_op );
		i_op += (1 + j_op) - i_op;
	}
	return;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
