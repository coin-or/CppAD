/* $Id$ */
# ifndef CPPAD_UNARY_OP_INCLUDED
# define CPPAD_UNARY_OP_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


/*!
\file unary_op.hpp
*/


/*!
Prototype for forward mode unary operator, never called.

\tparam Base
is the base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param p
order of the Taylor coefficient that we are computing.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor corresponding to z. 

\param arg
arg[0] is the variable index corresponding to the argument for this operator;
i.e. the row index in taylor corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: taylor[ arg[0] * nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to y for j = 0 , ... , p. 
\n
\b Input: taylor[ i_z * nc_taylor + j ] is the j-th order Taylor coefficient 
corresponding to z for j = 0 , ... , p-1. 
\n
\b Output: taylor[ i_z * nc_taylor + p ] is the p-th order Taylor coefficient 
corresponding to z. 

\par Checked Assertions where op is the unary operator:
\li NumInd(op) == 1
\li NumVar(op) == 1
\li arg[0] < i_z 
\li p < nc_taylor
*/
template <class Base>
inline void forward_unary_op(
	size_t p           ,
	size_t i_z         ,
	const size_t *arg  ,
	size_t nc_taylor   , 
	Base   *taylor     )
{
	// this routine is never called, it is only used for documentaiton
	// of unary operators.
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for zero order forward mode unary operator, never called. 

\tparam Base
is the base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor corresponding to z. 

\param arg
arg[0] is the variable index corresponding to the argument for this operator;
i.e. the row index in taylor corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: taylor[ arg[0] * nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to y. 
\n
\b Output: taylor[ i_z * nc_taylor + 0 ] is the zero order Taylor coefficient 
corresponding to z. 

\par Checked Assertions where op is the unary operator:
\li NumInd(op) == 1
\li NumVar(op) == 1
\li arg[0] < i_z 
\li p < nc_taylor
*/
template <class Base>
inline void forward_unary_op_0(
	size_t i_z         ,
	const size_t *arg  ,
	size_t nc_taylor   , 
	Base   *taylor     )
{
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for reverse mode unary operator, never called.

This routine is given the partial derivatives of a function G(z, y, ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H(y, x, ... ) = G[ z(y) , y , x ... ]
\endverbatim

\param p
order of the partial derivative that we are computing 

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in taylor to z. 

\param arg
arg[0] is the variable index corresponding to the argument for this operation;
i.e. the row index in taylor corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
taylor[ arg[0] * nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to y for j = 0 , ... , p-1. 
\n
taylor[ i_z * nc_taylor + j ] is the j-th order Taylor coefficient 
corresponding to z for j = 0 , ... , p-1. 


\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: partial[ arg[0] * nc_taylor + j ] 
is the j-th order partial derivative of G(z , y , x , ... ) with respect to y
for j = 0 , ... , p. 
\n
\b Input: partial[ i_z * nc_taylor + j ] 
is the j-th order partial derivative of G(z , y , x , ... ) with respect to z.
for j = 0 , ... , p. 
\n
\b Output: partial[ arg[0] * nc_taylor + j ]
is the j-th order partial derivative of H(y , x , ... ) with respect to y
for j = 0 , ... , p.


\par Checked Assumptions:
\li NumInd(op) == 1
\li NumVar(op) == 1
\li arg[0] < i_z 
\li p < nc_taylor
\li p < nc_partial
*/
template <class Base>
inline void reverse_unary_op(
	size_t p            ,
	size_t i_z          ,
	const size_t *arg   ,
	size_t nc_taylor    , 
	const Base  *taylor ,
	size_t nc_partial   ,
	Base   *partial     )
{
	CPPAD_ASSERT_UNKNOWN( false );
}
