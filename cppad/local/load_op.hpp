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
	z = v[x]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 
In the documentation below, we use the notation
\verbatim
	i_v_x = combined[ arg[0] + arg[1] ]
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
If v[x] is a parameter, \a arg[2] is set to zero 
(which is not a valid variable index).
If v[x] is a variable, 
\a arg[2] is set to the variable index corresponding to v[x]; i.e.  i_v_x.

\param num_par
is the number of parameters in \a parameter.

\param parameter
\b Input: If v[x] is a parameter, \a parameter [ i_v_x ] is its value.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input: if v[x] is a variable, \a taylor[ i_v_x * nc_taylor + 0 ]
is the zero order Taylor coefficient for v[x]
\n
\b Output: \a taylor[ i_z * nc_taylor + 0 ]
is the zero order Taylor coefficient for the variable z.

\param nc_combined
is the total number of elements in the combined VecAD array.

\param variable
If \a variable [ \a arg[0] + \a arg[1] ] is true,
v[x] is a variable.  Otherwise it is a parameter.

\param combined
\b Input: \a combined[ \a arg[0] - 1 ] 
is the number of elements in the VecAD vector containing this element.
\n
\b Input: 
if v[x] is a variable,
\verbatim
	i_v_x = combined[ arg[0] + arg[1] ] 
\endverbatim
is its index in the Taylor coefficient array \a taylor.
Otherwise, i_v_x is its index in parameter array \a parameter.

\par Checked Assertions where op is a binary operator:
\li NumArg(LdpOp) == 3
\li NumRes(LdpOp) == 1
\li 0 <  \a \a arg[0]
\li \a arg[0] + \a arg[1] < nc_combined
\li \a arg[1] < combined[ \a arg[0] - 1 ]
\li if v[x] is a parameter, i_v_x < num_par
\li if v[x] is a variable, i_v_x < i_z
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
	size_t i_v_x          = combined[ combined_index ];	
	CPPAD_ASSERT_UNKNOWN( NumArg(LdpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + arg[1] < nc_combined );
	CPPAD_ASSERT_UNKNOWN( arg[1] < combined[ arg[0] - 1 ] );

	Base* z  = taylor + i_z * nc_taylor;

	if( variable[ combined_index ] )
	{	CPPAD_ASSERT_UNKNOWN( i_v_x < i_z );
		Base* v_x = taylor + i_v_x * nc_taylor;
		arg[2]    = i_v_x;
		z[0]      = v_x[0];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( i_v_x < num_par );
		Base v_x  = parameter[i_v_x];
		arg[2]    = 0;
		z[0]      = v_x;
	}
}

/*!
Forward mode, except for zero order, for op = LdpOp

The C++ source code corresponding to this operation is
\verbatim
	z = v[x]
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
If v[x] is a parameter, \a arg[2] is zero 
(which is not a valid variable index).
If v[x] is a variable, 
\a arg[2] is the variable index corresponding to v[x].

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input: if v[x] is a variable, \a taylor[ \a arg[2] * nc_taylor + d ]
is the d-order Taylor coefficient corresponding to v[x].
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
	{	Base* v_x = taylor + arg[2] * nc_taylor;
		z[d]      = v_x[d];
	}
	else	z[d]      = Base(0);
}
CPPAD_END_NAMESPACE
# endif
