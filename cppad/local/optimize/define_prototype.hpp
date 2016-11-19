// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_DEFINE_PROTOTYPE_HPP
# define CPPAD_LOCAL_OPTIMIZE_DEFINE_PROTOTYPE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/core/cppad_assert.hpp>

/*!
\file define_prototype.hpp
Shared documentation for optimization helper functions.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Shared documentation for optimization helper functions (not called).

<!-- define prototype -->
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
Note that tape[i].arg[j] < i <= current.

\param npar
is the number of parameters corresponding to the old operation sequence.

\param par
is a vector of length npar containing the parameters
the old operation sequence; i.e.,
given a parameter index i < npar, the corresponding parameter value is par[i].
<!-- end prototype -->
*/
template <class Base>
void prototype(
	const CppAD::vector<struct struct_old_variable>&   tape           ,
	size_t                                             current        ,
	size_t                                             npar           ,
	const Base*                                        par            )
{	CPPAD_ASSERT_UNKNOWN(false); }

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
