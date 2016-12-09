// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_MATCH_OP_HPP
# define CPPAD_LOCAL_OPTIMIZE_MATCH_OP_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/optimize/hash_code.hpp>
/*!
\file match_op.hpp
Check if current operator matches a previous operator.
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Search for a previous operator that matches the current one.

\param var2op
mapping from variable index to operator index.

\param op_info
mapping from operator index to operator information

\param current
is the index of the current operator.

\li
This must be a unary or binary
operator; hence, NumArg( op_info[current].op ) is one or two.
There is one exception, NumRes( ErfOp ) == 3, but arg[0]
is the only true arguments (the others are always the same).

\li
This must not be a VecAD load or store operation; i.e.,
LtpvOp, LtvpOp, LtvvOp, StppOp, StpvOp, StvpOp, StvvOp.
It also must not be an independent variable operator InvOp.

\param hash_table_op
is a vector with size CPPAD_HASH_TABLE_SIZE
that maps a hash code to the corresponding
variable index in the operation sequence.
All the values in this table are less than current.

\param code [out]
The input value of code does not matter.
The output value of code is the hash code corresponding to
the current operation in the new operation sequence.

\return
We refer to the return value as pevious.
If pevious == 0, no match was found.
If previous != 0,
it is a pevious operator that can be used in place of current.
In addition op_info[previous].previous == 0.
*/

inline size_t match_op(
	const vector<size_t>&                              var2op         ,
	const vector<struct_op_info>&                      op_info        ,
	size_t                                             current        ,
	const vector<size_t>&                              hash_table_op  ,
	unsigned short&                                    code           )
{	// current operator
	OpCode        op         = op_info[current].op;
	const addr_t* arg        = op_info[current].arg;
	//
	// which arguments are variable
	size_t num_arg = NumArg(op);
	//
	bool   variable[2];
	variable[0] = false;
	variable[1] = false;
	//
	// If i-th argument to current operator has a previous operator,
	// this is the i-th argument for previous operator.
	// Otherwise, it is the i-th argument for the current operator
	// (if a previous variable exists)
	addr_t arg_match[2];
	switch(op)
	{	//
		case ErfOp:
		case AbsOp:
		case AcosOp:
		case AcoshOp:
		case AsinOp:
		case AsinhOp:
		case AtanOp:
		case AtanhOp:
		case CosOp:
		case CoshOp:
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
		{	arg_match[0] = arg[0];
			size_t previous = op_info[ var2op[arg[0]] ].previous;
			if( previous != 0 )
			{	CPPAD_ASSERT_UNKNOWN( op_info[previous].previous == 0 );
				arg_match[0] = op_info[previous].i_var;
			}
			num_arg = 1;
			code = optimize_hash_code(op, num_arg, arg_match);
			//
			// candidate previous for current operator
			size_t candidate  = hash_table_op[code];
			CPPAD_ASSERT_UNKNOWN( candidate < current );
			CPPAD_ASSERT_UNKNOWN( op_info[candidate].previous == 0 );
			//
			if( (candidate == 0) | (op != op_info[candidate].op) )
				return 0;
			//
			// check for a match
			bool match;
			previous = op_info[ var2op[op_info[candidate].arg[0]] ].previous;
			if( previous == 0 )
				match = arg_match[0] == op_info[candidate].arg[0];
			else
			{	CPPAD_ASSERT_UNKNOWN( op_info[previous].previous == 0 );
				match = arg_match[0] == addr_t( op_info[previous].i_var );
			}
			if( match )
				return candidate;
			return 0;
		}
		break;


		case AddpvOp:
		case DisOp:
		case DivpvOp:
		case EqpvOp:
		case LepvOp:
		case LtpvOp:
		case MulpvOp:
		case NepvOp:
		case PowpvOp:
		case SubpvOp:
		case ZmulpvOp:
		CPPAD_ASSERT_UNKNOWN( num_arg == 2 );
		variable[1] = true;
		break;

		case DivvpOp:
		case LevpOp:
		case LtvpOp:
		case PowvpOp:
		case SubvpOp:
		case ZmulvpOp:
		CPPAD_ASSERT_UNKNOWN( num_arg == 2 );
		variable[0] = true;
		break;

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
		CPPAD_ASSERT_UNKNOWN( num_arg == 2 );
		variable[0] = true;
		variable[1] = true;
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(false);
	}
	for(size_t j = 0; j < num_arg; ++j)
	{	arg_match[j] = arg[j];
		if( variable[j] )
		{	size_t previous = op_info[ var2op[arg[j]] ].previous;
			if( previous != 0 )
			{	CPPAD_ASSERT_UNKNOWN( op_info[previous].previous == 0 );
				//
				arg_match[j] = op_info[previous].i_var;
			}
		}
	}
	code = optimize_hash_code(op, num_arg, arg_match);
	//
	// candidate previous for current operator
	size_t  candidate  = hash_table_op[code];
	CPPAD_ASSERT_UNKNOWN( candidate < current );
	CPPAD_ASSERT_UNKNOWN( op_info[candidate].previous == 0 );
	//
	// check for a match
	bool match = candidate != 0;
	match     &= op == op_info[candidate].op;
	if( match )
	{	for(size_t j = 0; j < num_arg; j++)
		{	if( variable[j] )
			{	size_t previous =
					op_info[ var2op[op_info[candidate].arg[j]] ].previous;
				if( previous != 0 )
				{	CPPAD_ASSERT_UNKNOWN( op_info[previous].previous == 0 );
					//
					match &= arg_match[j] == addr_t( op_info[previous].i_var );
				}
				else
					match &= arg_match[j] == op_info[candidate].arg[j];
			}
		}
		if( match )
			return candidate;
	}

	// special case where operator is commutative
	if( (op == AddvvOp) | (op == MulvvOp ) )
	{	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
		std::swap( arg_match[0], arg_match[1] );
		//
		code      = optimize_hash_code(op, num_arg, arg_match);
		candidate  = hash_table_op[code];
		CPPAD_ASSERT_UNKNOWN( candidate < current );
		CPPAD_ASSERT_UNKNOWN( op_info[candidate].previous == 0 );
		//
		match  = candidate != 0;
		match &= op == op_info[candidate].op;
		if( match )
		{	for(size_t j = 0; j < num_arg; j++)
			{	CPPAD_ASSERT_UNKNOWN( variable[j] )
				size_t previous =
					op_info[ var2op[op_info[candidate].arg[j]] ].previous;
				if( previous != 0 )
				{	CPPAD_ASSERT_UNKNOWN( op_info[previous].previous == 0 );
					//
					match &= arg_match[j] == addr_t( op_info[previous].i_var );
				}
				else
					match &= arg_match[j] == op_info[candidate].arg[j];
			}
			if( match )
				return candidate;
		}
	}
	// special op code used for no match
	return 0;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
