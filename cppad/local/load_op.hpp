/* $Id$ */
# ifndef CPPAD_LOAD_OP_INCLUDED
# define CPPAD_LOAD_OP_INCLUDED
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
\file load_op.hpp
Setting a VecAD element so that it corresponds to an AD variable address.
*/
/*!
Zero order forward mode implementation of op = LdpOp

The C++ source code corresponding to this operation is
\verbatim
	z = x[a]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 
In the documentation below, we use the notation
\verbatim
	i_x_a = combined[ arg[0] + arg[1] ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
\a arg[0]
is the offset of this VecAD vector relative to the beginning 
of the \a combined VecAD array.
\n
\n 
\a arg[1] 
\n
Is this index of this VecAD element with in this VecAD vector.
\n
\n
\a arg[2]
\b Input: The input value of \a arg[2] does not matter.
\n
\b Output: 
If x[a] is a parameter, \a arg[2] is set to zero 
(which is not a valid variable index).
If x[a] is a variable, 
\a arg[2] is set to the variable index corresponding to x[a]; i.e.  i_x_a.

\param num_par
is the number of parameters in \a parameter.

\param parameter
\b Input: If x[a] is a parameter, \a parameter [ i_x_a ] is its value.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input: if x[a] is a variable, \a taylor[ i_x_a * nc_taylor + 0 ]
is the zero order Taylor coefficient for x[a]
\n
\b Output: \a taylor[ i_z * nc_taylor + 0 ]
is the zero order Taylor coefficient for the variable z.

\param nc_combined
is the total number of elements in the combined VecAD array.

\param variable
If \a variable [ \a arg[0] + \a arg[1] ] is true,
x[a] is a variable.  Otherwise it is a parameter.

\param combined
\b Input: \a combined[ \a arg[0] - 1 ] 
is the number of elements in the VecAD vector containing this element.
\n
\b Input: 
if x[a] is a variable,
\verbatim
	i_x_a = combined[ arg[0] + arg[1] ] 
\endverbatim
is its index in the Taylor coefficient array \a taylor.
Otherwise, i_x_a is its index in parameter array \a parameter.

\par Checked Assertions where op is a binary operator:
\li NumArg(LdpOp) == 3
\li NumRes(LdpOp) == 1
\li 0 <  \a \a arg[0]
\li \a arg[0] + \a arg[1] < nc_combined
\li \a arg[1] < combined[ \a arg[0] - 1 ]
\li if x[a] is a parameter, i_x_a < num_par
\li if x[a] is a variable, i_x_a < i_z
*/

template <class Base>
inline void forward_load_p_op_0(
	size_t         i_z         ,
	size_t*        arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	const bool*    variable    ,
	const size_t*  combined    )
{

	size_t combined_index = arg[0] + arg[1];
	size_t i_x_a          = combined[ combined_index ];	
	CPPAD_ASSERT_UNKNOWN( NumArg(LdpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + arg[1] < nc_combined );
	CPPAD_ASSERT_UNKNOWN( arg[1] < combined[ arg[0] - 1 ] );

	Base* z  = taylor + i_z * nc_taylor;

	if( variable[ combined_index ] )
	{	CPPAD_ASSERT_UNKNOWN( i_x_a < i_z );
		Base* x_a = taylor + i_x_a * nc_taylor;
		arg[2]    = i_x_a;
		z[0]      = x_a[0];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( i_x_a < num_par );
		Base x_a  = parameter[i_x_a];
		arg[2]    = 0;
		z[0]      = x_a;
	}
}

/*!
Forward mode, except for zero order, for op = LdpOp

The C++ source code corresponding to this operation is
\verbatim
	z = x[a]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
is the order of the Taylor coefficient that we are computing.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\a arg[2]
If x[a] is a parameter, \a arg[2] is zero 
(which is not a valid variable index).
If x[a] is a variable, 
\a arg[2] is the variable index corresponding to x[a].

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input: if x[a] is a variable, \a taylor[ \a arg[2] * nc_taylor + d ]
is the d-order Taylor coefficient corresponding to x[a].
\n
\b Output: \a taylor[ i_z * nc_taylor + d ]
is the d-order Taylor coefficient for the variable z.

\par Checked Assertions 
\li NumArg(LdpOp) == 3
\li NumRes(LdpOp) == 1
\li 0 < d < nc_taylor
\li arg[2] < i_z
*/
template <class Base>
inline void forward_load_p_op(
	size_t         d           ,
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         nc_taylor   ,
	Base*          taylor      )
{

	CPPAD_ASSERT_UNKNOWN( NumArg(LdpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( d > 0 )
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );

	Base* z      = taylor + i_z * nc_taylor;
	if( arg[2] > 0 )
	{	Base* x_a = taylor + arg[2] * nc_taylor;
		z[d]      = x_a[d];
	}
	else	z[d]      = Base(0);
}

/*!
Reverse mode, except for zero order, for op = LdpOp

The C++ source code corresponding to this operation is
\verbatim
	z = x[a]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 

This routine is given the partial derivatives of a function 
G(z , x[a] , w , u ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( x[a] , w , u , ... ) = G[ z( x[a] ) , x[a] , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
highest order the Taylor coefficient that we are computing the partial
derivative with respect to.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\a arg[2]
If x[a] is a parameter, \a arg[2] is zero 
(which is not a valid variable index).
If x[a] is a variable, 
\a arg[2] is the variable index corresponding to x[a].

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients
(not used).

\param taylor
matrix of Taylor coefficients (not used).

\param nc_partial
number of colums in the matrix containing all the partial derivatives
(not used if \a arg[2] is zero).

\param partial
If \a arg[0] is zero, x[a] is a parameter
and no values need to be modified; i.e., \a partial is not used.
Otherwise, x[a] is a variable and:
\n
\b Input: \a partial [ \a arg[2] * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z( x[a] ) , x[a], w , u , ... ) 
with respect to the k-th order Taylor coefficient for x[a].
\n
\b Input: \a partial [ \a i_z * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z( x[a] ) , x[a] , w , u , ... ) 
with respect to the k-th order Taylor coefficient for z.
\n
\b Output: \a partial [ \a arg[2] * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative of H( x[a] , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.

\par Checked Assertions 
\li NumArg(LdpOp) == 3
\li NumRes(LdpOp) == 1
\li 0 <= d < nc_partial
\li arg[2] < i_z
*/
template <class Base>
inline void reverse_load_p_op(
	size_t         d           ,
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         nc_taylor   ,
	const Base*    taylor      ,
	size_t         nc_partial  ,
	Base*          partial     )
{

	CPPAD_ASSERT_UNKNOWN( NumArg(LdpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 <= d )
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );

	if( arg[2] > 0 )
	{
		Base* pz   = partial + i_z    * nc_partial;
		Base* px_a = partial + arg[2] * nc_partial;
		size_t j = d + 1;
		while(j--)
			px_a[j]   += pz[j];
	}
}
CPPAD_END_NAMESPACE
# endif
