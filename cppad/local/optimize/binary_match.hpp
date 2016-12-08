// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_BINARY_MATCH_HPP
# define CPPAD_LOCAL_OPTIMIZE_BINARY_MATCH_HPP
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
\file binary_match.hpp
Check if a binary operation is a match for a previous operation.
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Check if a binary operation is a match for a previous operation.
A previous identical operation can be used inplace of the current operation.

\param var2op
mapping from old variable index to old operator index.

\param op_info
mapping from old index to operator index to operator information

\param old2new
mapping from old operator index to information about the new recording.

\param current
is the index in the old operation sequence for
the variable corresponding to the result for the current operator.
We use the notataion i_op = var2op[current].
It follows that  NumRes( op_info[i_op].op ) > 0.
If 0 < j_op < i_op, either op_info[j_op].csum_connected,
op_info[j_op].usage = 0, or old2new[j_op].new_var != 0.

\param npar
is the number of parameters corresponding to the old operation sequence.

\param par
is a vector of length npar containing the parameters
the old operation sequence; i.e.,
given a parameter index i < npar, the corresponding parameter value is par[i].

\param hash_table_var
is a vector with size CPPAD_HASH_TABLE_SIZE
that maps a hash code to the corresponding
variable index in the old operation sequence.
All the values in this table are less than current.

\param code [out]
The input value of code does not matter.
The output value of code is the hash code corresponding to
this operation in the new operation sequence.

\return
If the return value is zero, no identical operation was found.
If the return value is greater than zero,
it is the old operation sequence index of a variable,
that comes before current, and can be used to replace the current variable.

\par Restrictions:
The binary operator must be an addition, subtraction, multiplication, division
or power operator.
NumArg( op_info[i_op].op ) == 2, NumRes( op_info[i_op].op ) == 1.
*/

template <class Base>
inline addr_t binary_match(
	const vector<size_t>&                              var2op         ,
	const vector<struct_op_info>&                      op_info        ,
	const CppAD::vector<struct struct_old2new>&        old2new        ,
	size_t                                             current        ,
	size_t                                             npar           ,
	const Base*                                        par            ,
	const CppAD::vector<size_t>&                       hash_table_var ,
	unsigned short&                                    code           )
{	// current operator
	size_t        i_op       = var2op[current];
	OpCode        op         = op_info[i_op].op;
	const addr_t* arg        = op_info[i_op].arg;
	addr_t        new_arg[2];
	bool          parameter[2];

	// initialize return value
	addr_t  match_var = 0;

	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) >  0 );
	switch(op)
	{	// index op variable
		case DisOp:
		// parameter not defined for this case
		CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < current );
		new_arg[0]   = arg[0];
		new_arg[1]   = old2new[ var2op[arg[1]] ].new_var;
		break;

		// parameter op variable ----------------------------------
		case AddpvOp:
		case MulpvOp:
		case DivpvOp:
		case PowpvOp:
		case SubpvOp:
		case ZmulpvOp:
		// arg[0]
		parameter[0] = true;
		new_arg[0]   = arg[0];
		CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < npar );
		// arg[1]
		parameter[1] = false;
		new_arg[1]   = old2new[ var2op[arg[1]] ].new_var;
		CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < current );
		break;

		// variable op parameter -----------------------------------
		case DivvpOp:
		case PowvpOp:
		case SubvpOp:
		case ZmulvpOp:
		// arg[0]
		parameter[0] = false;
		new_arg[0]   = old2new[ var2op[arg[0]] ].new_var;
		CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < current );
		// arg[1]
		parameter[1] = true;
		new_arg[1]   = arg[1];
		CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < npar );
		break;

		// variable op variable -----------------------------------
		case AddvvOp:
		case MulvvOp:
		case DivvvOp:
		case PowvvOp:
		case SubvvOp:
		case ZmulvvOp:
		// arg[0]
		parameter[0] = false;
		new_arg[0]   = old2new[ var2op[arg[0]] ].new_var;
		CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < current );
		// arg[1]
		parameter[1] = false;
		new_arg[1]   = old2new[ var2op[arg[1]] ].new_var;
		CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < current );
		break;

		// must be one of the cases above
		default:
		CPPAD_ASSERT_UNKNOWN(false);
	}
	code           = optimize_hash_code(op, new_arg);
	size_t  i_var  = hash_table_var[code];
	CPPAD_ASSERT_UNKNOWN( i_var < current );
	if( i_var != 0 && op == op_info[var2op[i_var]].op )
	{	bool match = true;
		if( op == DisOp )
		{	match   &= new_arg[0] == op_info[var2op[i_var]].arg[0];
			size_t k = op_info[var2op[i_var]].arg[1];
			match   &= new_arg[1] == old2new[var2op[k]].new_var;
		}
		else
		{	for(size_t j = 0; j < 2; j++)
			{	size_t k = op_info[var2op[i_var]].arg[j];
				if( parameter[j] )
				{	CPPAD_ASSERT_UNKNOWN( k < npar );
					match &= IdenticalEqualPar(
						par[ arg[j] ], par[k]
					);
				}
				else
				{	CPPAD_ASSERT_UNKNOWN( k < i_var );
					match &= (new_arg[j] == old2new[var2op[k]].new_var);
				}
			}
		}
		if( match )
			match_var = i_var;
	}
	if( (match_var > 0) | ( (op != AddvvOp) & (op != MulvvOp ) ) )
		return match_var;

	// check for match with argument order switched ----------------------
	CPPAD_ASSERT_UNKNOWN( op == AddvvOp || op == MulvvOp );
	std::swap(new_arg[0], new_arg[1]);
	unsigned short code_switch = local_hash_code(
		op                  ,
		new_arg             ,
		npar                ,
		par
	);
	i_var  = hash_table_var[code_switch];
	CPPAD_ASSERT_UNKNOWN( i_var < current );
	if( i_var != 0 && op == op_info[var2op[i_var]].op )
	{	bool match = true;
		size_t j;
		for(j = 0; j < 2; j++)
		{	size_t k = op_info[var2op[i_var]].arg[j];
			CPPAD_ASSERT_UNKNOWN( k < i_var );
			match &= (new_arg[j] == old2new[var2op[k]].new_var);
		}
		if( match )
			match_var = i_var;
	}
	return match_var;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
