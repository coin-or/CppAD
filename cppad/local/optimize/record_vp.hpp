// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_RECORD_VP_HPP
# define CPPAD_LOCAL_OPTIMIZE_RECORD_VP_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file record_vp.hpp
Record an operation of the form (variable op parameter).
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {


/*!
Record an operation of the form (variable op parameter).

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

\param rec
is the object that will record the operations.

\param op
is the operator that we are recording which must be one of the following:
DivvpOp, PowvpOp, SubvpOp, ZmulvpOp.

\param arg
is the vector of arguments for this operator.

\return
the result operation and variable index corresponding to the current
operation in the new operation sequence.
*/
template <class Base>
struct_size_pair record_vp(
	const CppAD::vector<struct struct_old_variable>&   tape           ,
	size_t                                             current        ,
	size_t                                             npar           ,
	const Base*                                        par            ,
	recorder<Base>*                                    rec            ,
	OpCode                                             op             ,
	const addr_t*                                      arg            )
{
# ifndef NDEBUG
	switch(op)
	{	case DivvpOp:
		case PowvpOp:
		case SubvpOp:
		case ZmulvpOp:
		break;

		default:
		CPPAD_ASSERT_UNKNOWN(false);
	}
# endif
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < current );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < npar    );
	addr_t new_arg[2];
	new_arg[0]   = tape[ arg[0] ].new_var;
	new_arg[1]   = rec->PutPar( par[arg[1]] );
	rec->PutArg( new_arg[0], new_arg[1] );

	struct_size_pair ret;
	ret.i_op  = rec->num_op_rec();
	ret.i_var = rec->PutOp(op);
	CPPAD_ASSERT_UNKNOWN( size_t(new_arg[0]) < ret.i_var );
	return ret;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
