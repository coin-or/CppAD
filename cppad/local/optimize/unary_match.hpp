// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_UNARY_MATCH_HPP
# define CPPAD_LOCAL_OPTIMIZE_UNARY_MATCH_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file unary_match.hpp
Check if a unary operation is identical to a previous operation.
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {

/*!
Check if a unary operation is a match for a previous operation.

A previous identical operation can be used inplace of the current operation.

<!-- replace prototype -->
\param tape
is a vector that maps a variable index, in the old operation sequence,
to the corresponding struct_old_variable information.
Note that the old variable index must be greater than or equal zero and
less than tape.size().

\param current
is the index in the old operation sequence for
the variable corresponding to the result for the current operator.
It follows that current < tape.size() and NumRes( tape[current].op ) > 0.
Suppose i <= current, j < NumArg( tape[i] ), and k = tape[i].arg[j],
It is assumed that tape[i].arg[j] is connected to the dependent variables
and tape[k].new_var has been set to the corresponding variable.
Note that tape[i].arg[j] < i <= current and
tape[k].new_var <= k < current.

\param npar
is the number of parameters corresponding to the old operation sequence.

\param par
is a vector of length npar containing the parameters
the old operation sequence; i.e.,
given a parameter index i < npar, the corresponding parameter value is par[i].
<!-- end prototype -->

\param hash_table_var
is a vector with size CPPAD_HASH_TABLE_SIZE
that maps a hash code to the corresponding
variable index in the old operation sequence.
All the values in this table are be less than current.

\param code
The input value of code does not matter.
The output value of code is the hash code corresponding to
this operation in the new operation sequence.

\return
If the return value is zero, no identical operation was found.
If the return value is greater than zero,
it is the old operation sequence index of a variable,
that comes before current, and can be used to replace the current variable.

\par Assumption:
NumArg( tape[current].op ) == 1 or tape[current].op == ErfOp.
NumRes( tape[current].op ) > 0.
*/
template <class Base>
addr_t unary_match(
	const CppAD::vector<struct struct_old_variable>&   tape           ,
	size_t                                             current        ,
	size_t                                             npar           ,
	const Base*                                        par            ,
	const CppAD::vector<size_t>&                       hash_table_var ,
	unsigned short&                                    code           )
{	// pointer to argument for the current operation
	const addr_t* arg = tape[current].arg;
	// operatgor for the current operation
	OpCode        op  = tape[current].op;
	// new a
	addr_t new_arg[1];

	// ErfOp has three arguments, but the second and third are always the
	// parameters 0 and 2 / sqrt(pi) respectively.
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 || op == ErfOp);
	CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0  );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < current );
	//
	new_arg[0] = tape[arg[0]].new_var;
	CPPAD_ASSERT_UNKNOWN( new_arg[0] <= arg[0] );
	code = local_hash_code(
		op                  ,
		new_arg             ,
		npar                ,
		par
	);
	size_t  i_var  = hash_table_var[code];
	CPPAD_ASSERT_UNKNOWN( i_var < current );
	if( op == tape[i_var].op )
	{	size_t k = tape[i_var].arg[0];
		CPPAD_ASSERT_UNKNOWN( k < i_var );
		if (new_arg[0] == tape[k].new_var )
			return i_var;
	}
	return 0;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
