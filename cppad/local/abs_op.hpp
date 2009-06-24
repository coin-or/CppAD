/* $Id$ */
# ifndef CPPAD_ABS_OP_INCLUDED
# define CPPAD_ABS_OP_INCLUDED
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
\file abs_op.hpp
*/


/*!
Forward mode Taylor coefficient for result of op = AbsOp.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(y)
\endverbatim

\copydetails forward_unary_op
*/
template <class Base>
inline void forward_abs_op(
	size_t p           ,
	size_t i_z         ,
	const size_t *arg  ,
	size_t nc_taylor   , 
	Base   *taylor     )
{
	static Base zero(0);

	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );

	// Taylor coefficients corresponding to argument
	Base *y = taylor + arg[0] * nc_taylor;

	// Taylor coefficients corresponding to result
	Base *z = taylor + i_z * nc_taylor;

	// order that decides positive, negative or zero
	size_t k;
	k = 0;
	while( (k < p) & (y[k] == zero) )
		k++; 

	if( GreaterThanZero(y[k]) )
		z[p]  = y[p];
	else if( LessThanZero(y[k]) )
		z[p] = -y[p]; 
	else	z[p] = zero;
}

/*!
Zero order forward mode Taylor coefficient for result of op = AbsOp.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(y)
\endverbatim

\copydetails forward_unary_op_0
*/
template <class Base>
inline void forward_abs_op_0(
	size_t i_z         ,
	const size_t *arg  ,
	size_t nc_taylor   , 
	Base   *taylor     )
{

	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( 0 < nc_taylor );

	// Taylor coefficients corresponding to argument
	Base y0 = *(taylor + arg[0] * nc_taylor);

	// Taylor coefficients corresponding to result
	Base *z = taylor + i_z * nc_taylor;

	if( LessThanZero(y0) )
		z[0]  = - y0;
	else	z[0] = y0; 
}
/*!
Reverse mode partial derivatives for result of op = AbsOp.

The C++ source code corresponding to this operation is
\verbatim
	z = abs(y)
\endverbatim

\copydetails reverse_unary_op
*/

template <class Base>
inline void reverse_abs_op(
	size_t p            ,
	size_t i_z          ,
	const size_t *arg   ,
	size_t nc_taylor    , 
	const Base  *taylor ,
	size_t nc_partial   ,
	Base   *partial     )
{	size_t j, k;	
	static Base zero(0);


	// check assumptions
	CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( p < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( p < nc_partial );

	// Taylor coefficients and partials corresponding to argument
	const Base *y  = taylor  + arg[0] * nc_taylor;
	Base *py       = partial + arg[0] * nc_partial;

	// Taylor coefficients and partials corresponding to result
	Base *pz       = partial +    i_z * nc_partial;

	// order that decides positive, negative or zero
	k = 0;
	while( (k < p) & (y[k] == zero) )
		k++; 

	if( GreaterThanZero(y[k]) )
	{	// partial of z w.r.t y is +1
		for(j = k; j <= p; j++)
			py[j] += pz[j];
	}
	else if( LessThanZero(y[k]) )
	{	// partial of z w.r.t y is -1
		for(j = k; j <= p; j++)
			py[j] -= pz[j];
	}
}

CPPAD_END_NAMESPACE
# endif
