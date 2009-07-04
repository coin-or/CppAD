/* $Id$ */
# ifndef CPPAD_COND_OP_INCLUDED
# define CPPAD_COND_OP_INCLUDED
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
\file cond_op.hpp
Forward and reverse mode for conditional expressions z = CondExpRel(v, w, x, y).
*/

/*!
Compute forward mode Taylor coefficients for op = CExpOp.

The C++ source code corresponding to this operation is
\verbatim
	z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
is the order of the Taylor coefficient of z that we are  computing.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param arg
\n
\a arg[0]
is static cast to size_t from the enum type
\verbatim
	enum CompareOp {CompareLt, CompareLe, CompareEq, CompareGe, CompareGt}
\endverbatim
for this operation.
\n
\n 
\a arg[1] & 1 
\n
If this expression is true, y_0 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 2
\n
if this expression is true, y_1 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 4
\n
if this expression is true, y_2 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 8
\n
if this expression is true, y_3 is a variable, otherwise it is a parameter.
\n
\n
\a arg[2+j ] for j = 0, 1, 2, 3 
\n
is the index corresponding to the operand y_j.

\param num_par
is the lenght of the \a parameter vector.
This value is only used for checking assumptions mentioned below
(and is not used at all when NDEBUG is defined).

\param parameter
For j = 0, 1, 2, 3, if y_j is a parameter then
\a parameter [ \a arg[2+j] ] is its value.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input:
For j = 0, 1, 2, 3 and k = 0 , ... , \a d,
if y_j is a variable then
\a taylor [ \a arg[2+j] * nc_taylor + k ]
is the k-th order Taylor coefficient corresponding to y_j.
\n
\b Input: \a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d - 1
is the k-th order Taylor coefficient corresponding to z.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + \a d ] 
is the \a d-th order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is a binary operator:
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li  arg[0] <= static_cast<size_t>( CompareGt )
\li arg[1] != 0 (not all of y_j are parameters)
\li For j = 0, 1 ,2 ,3, if y_j is a variable, \a arg[2+j] < \a i_z
\li For j = 0, 1, 2, 3, if y_j is a parameter, \a arg[2+j] < \a num_par

*/
template <class Base>
inline void forward_cond_op(
	size_t         d           ,
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      )
{	Base y_0, y_1, y_2, y_3;
	Base zero(0);
	Base* z;

	CPPAD_ASSERT_UNKNOWN( arg[0] <= static_cast<size_t> (CompareGt) );
	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

	if( arg[1] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );
		y_0 = taylor[ arg[2] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );
		y_0 = parameter[ arg[2] ];
	}
	if( arg[1] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < i_z );
		y_1 = taylor[ arg[3] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < num_par );
		y_1 = parameter[ arg[3] ];
	}
# if CPPAD_USE_FORWARD0SWEEP
	CPPAD_ASSERT_UNKNOWN( d > 0 );
# else
	if( d == 0 )
	{	if( arg[1] & 4 )
		{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
			y_2 = taylor[ arg[4] * nc_taylor + 0 ];
		}
		else
		{	CPPAD_ASSERT_UNKNOWN( arg[4] < num_par );
			y_2 = parameter[ arg[4] ];
		}
		if( arg[1] & 8 )
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
			y_3 = taylor[ arg[5] * nc_taylor + 0 ];
		}
		else
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < num_par );
			y_3 = parameter[ arg[5] ];
		}
	}
	else
# endif
	{	if( arg[1] & 4 )
		{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
			y_2 = taylor[ arg[4] * nc_taylor + d];
		}
		else	y_2 = zero;
		if( arg[1] & 8 )
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
			y_3 = taylor[ arg[5] * nc_taylor + d];
		}
		else	y_3 = zero;
	}
	z = taylor + i_z * nc_taylor;
	z[d] = CondExpOp(
		CompareOp( arg[0] ),
		y_0,
		y_1,
		y_2,
		y_3
	);
	return;
}

/*!
Compute zero order forward mode Taylor coefficients for op = CExpOp.

The C++ source code corresponding to this operation is
\verbatim
	z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param arg
\n
\a arg[0]
is static cast to size_t from the enum type
\verbatim
	enum CompareOp {CompareLt, CompareLe, CompareEq, CompareGe, CompareGt}
\endverbatim
for this operation.
\n
\n 
\a arg[1] & 1 
\n
If this expression is true, y_0 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 2
\n
if this expression is true, y_1 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 4
\n
if this expression is true, y_2 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 8
\n
if this expression is true, y_3 is a variable, otherwise it is a parameter.
\n
\n
\a arg[2+j ] for j = 0, 1, 2, 3 
\n
is the index corresponding to the operand y_j.

\param num_par
is the lenght of the \a parameter vector.
This value is only used for checking assumptions mentioned below
(and is not used at all when NDEBUG is defined).

\param parameter
For j = 0, 1, 2, 3, if y_j is a parameter then
\a parameter [ \a arg[2+j] ] is its value.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input:
For j = 0, 1, 2, 3,
if y_j is a variable then
\a taylor [ \a arg[2+j] * nc_taylor + 0 ]
is the zero order Taylor coefficient corresponding to y_j.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is a binary operator:
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li  arg[0] <= static_cast<size_t>( CompareGt )
\li arg[1] != 0 (not all of y_j are parameters)
\li For j = 0, 1 ,2 ,3, if y_j is a variable, \a arg[2+j] < \a i_z
\li For j = 0, 1, 2, 3, if y_j is a parameter, \a arg[2+j] < \a num_par

*/
template <class Base>
inline void forward_cond_op_0(
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      )
{	Base y_0, y_1, y_2, y_3;
	Base* z;

	CPPAD_ASSERT_UNKNOWN( arg[0] <= static_cast<size_t> (CompareGt) );
	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

	if( arg[1] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );
		y_0 = taylor[ arg[2] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );
		y_0 = parameter[ arg[2] ];
	}
	if( arg[1] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < i_z );
		y_1 = taylor[ arg[3] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < num_par );
		y_1 = parameter[ arg[3] ];
	}
	if( arg[1] & 4 )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
		y_2 = taylor[ arg[4] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < num_par );
		y_2 = parameter[ arg[4] ];
	}
	if( arg[1] & 8 )
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
		y_3 = taylor[ arg[5] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < num_par );
		y_3 = parameter[ arg[5] ];
	}
	z = taylor + i_z * nc_taylor;
	z[0] = CondExpOp(
		CompareOp( arg[0] ),
		y_0,
		y_1,
		y_2,
		y_3
	);
	return;
}

/*!
Compute reverse mode Taylor coefficients for op = CExpOp.

The C++ source code corresponding to this operation is
\verbatim
	z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim

This routine is given the partial derivatives of a function 
G( z , y , x , w , ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( y , x , w , u , ... ) = G[ z(y) , y , x , w , u , ... ]
\endverbatim
where y above represents y_0, y_1, y_2, y_3.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
is the order of the Taylor coefficient of z that we are  computing.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param arg
\n
\a arg[0]
is static cast to size_t from the enum type
\verbatim
	enum CompareOp {CompareLt, CompareLe, CompareEq, CompareGe, CompareGt}
\endverbatim
for this operation.
\n
\n 
\a arg[1] & 1 
\n
If this expression is true, y_0 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 2
\n
if this expression is true, y_1 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 4
\n
if this expression is true, y_2 is a variable, otherwise it is a parameter.
\n
\n
\a arg[1] & 8
\n
if this expression is true, y_3 is a variable, otherwise it is a parameter.
\n
\n
\a arg[2+j ] for j = 0, 1, 2, 3 
\n
is the index corresponding to the operand y_j.

\param num_par
is the lenght of the \a parameter vector.
This value is only used for checking assumptions mentioned below
(and is not used at all when NDEBUG is defined).

\param parameter
For j = 0, 1, 2, 3, if y_j is a parameter then
\a parameter [ \a arg[2+j] ] is its value.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\b Input:
For j = 0, 1, 2, 3 and k = 0 , ... , \a d,
if y_j is a variable then
\a taylor [ \a arg[2+j] * nc_taylor + k ]
is the k-th order Taylor coefficient corresponding to y_j.
\n
\b Input: \a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to z.

\param nc_partial
number of columns in the matrix containing the Taylor coefficients.

\param partial
\b Input:
For j = 0, 1, 2, 3 and k = 0 , ... , \a d,
if y_j is a variable then
\a partial [ \a arg[2+j] * nc_partial + k ]
is the partial derivative of G( z , y , x , w , u , ... )
with respect to the k-th order Taylor coefficient corresponding to y_j.
\n
\b Input: \a partial [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , y , x , w , u , ... )
with respect to the k-th order Taylor coefficient corresponding to z.
\n
\b Output:
For j = 0, 1, 2, 3 and k = 0 , ... , \a d,
if y_j is a variable then
\a partial [ \a arg[2+j] * nc_partial + k ]
is the partial derivative of H( y , x , w , u , ... )
with respect to the k-th order Taylor coefficient corresponding to y_j.

\par Checked Assertions where op is a binary operator:
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li  arg[0] <= static_cast<size_t>( CompareGt )
\li arg[1] != 0 (not all of y_j are parameters)
\li For j = 0, 1 ,2 ,3, if y_j is a variable, \a arg[2+j] < \a i_z
\li For j = 0, 1, 2, 3, if y_j is a parameter, \a arg[2+j] < \a num_par

*/
template <class Base>
inline void reverse_cond_op(
	size_t         d           ,
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	const Base*    taylor      ,
	size_t         nc_partial  ,
	Base*          partial     )
{	Base y_0, y_1;
	Base zero(0);
	Base* pz;
	Base* py_2;
	Base* py_3;

	CPPAD_ASSERT_UNKNOWN( arg[0] <= static_cast<size_t> (CompareGt) );
	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

	pz = partial + i_z * nc_partial + 0;
	if( arg[1] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );
		y_0 = taylor[ arg[2] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );
		y_0 = parameter[ arg[2] ];
	}
	if( arg[1] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < i_z );
		y_1 = taylor[ arg[3] * nc_taylor + 0 ];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < num_par );
		y_1 = parameter[ arg[3] ];
	}
	if( arg[1] & 4 )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
		py_2 = partial + arg[4] * nc_partial;
		size_t j = d + 1;
		while(j--)
		{	py_2[j] += CondExpOp(
				CompareOp( arg[0] ),
				y_0,
				y_1,
				pz[j],
				zero
			);
		}
	}
	if( arg[1] & 8 )
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
		py_3 = partial + arg[5] * nc_partial;
		size_t j = d + 1;
		while(j--)
		{	py_3[j] += CondExpOp(
				CompareOp( arg[0] ),
				y_0,
				y_1,
				zero,
				pz[j]
			);
		}
	}
	return;
}

CPPAD_END_NAMESPACE
# endif
