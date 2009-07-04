/* $Id$ */
# ifndef CPPAD_PROTOTYPE_OP_INCLUDED
# define CPPAD_PROTOTYPE_OP_INCLUDED
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
\file prototype_op.hpp
Documentation for generic cases (these generic cases are never used).
*/

// ==================== Unary operators with one result ====================


/*!
Prototype for forward mode unary operator with one result (not used).

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param j
order of the Taylor coefficient that we are computing.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param i_x
variable index corresponding to the argument for this operator;
i.e. the row index in \a taylor corresponding to x.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_x * \a nc_taylor + k ] 
for k = 0 , ... , \a j
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input: \a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a j - 1
is the k-th order Taylor coefficient corresponding to z.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is a unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 1
\li \a i_x < \a i_z 
\li \a j < \a nc_taylor
*/
template <class Base>
inline void forward_unary1_op(
	size_t j           ,
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for zero order forward mode unary operator with one result (not used). 
\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param i_x
variable index corresponding to the argument for this operator;
i.e. the row index in \a taylor corresponding to x.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_x * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to x. 
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is the unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 1
\li \a i_x < \a i_z 
\li \a j < \a nc_taylor
*/
template <class Base>
inline void forward_unary1_op_0(
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for reverse mode unary operator with one result (not used).

This routine is given the partial derivatives of a function 
G(z , x , w, u ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( x , w , u , ... ) = G[ z(x) , x , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param d
highest order Taylor coefficient that
we are computing the partial derivatives with respect to.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor to z. 

\param i_x
variable index corresponding to the argument for this operation;
i.e. the row index in \a taylor corresponding to x.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\a taylor [ \a i_x * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to x.
\n
\a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to z.

\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: \a partial [ \a i_x * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Input: \a partial [ \a i_z * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for z.
\n
\b Output: \a partial [ \a i_x * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative of H( x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Output: \a partial [ \a i_z * \a nc_partial + k ]
for k = 0 , ... , \a d 
may be used as work space; i.e., may change in an unspecified manner.


\par Checked Assumptions where op is a unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 1
\li \a i_x < \a i_z 
\li \a d < \a nc_taylor
\li \a d < \a nc_partial
*/
template <class Base>
inline void reverse_unary1_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t      i_x          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

// ==================== Unary operators with two results ====================

/*!
Prototype for forward mode unary operator with two results (not used).

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param j
order of the Taylor coefficients that we are computing.

\param i_z
variable index corresponding to the first result for this operation; 
i.e. the row index in \a taylor corresponding to z. 
The auxillary result is called y has index \a i_z + 1.

\param i_x
variable index corresponding to the argument for this operator;
i.e. the row index in \a taylor corresponding to x.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_x * \a nc_taylor + k ] 
for k = 0 , ... , \a j
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input: \a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a j - 1
is the k-th order Taylor coefficient corresponding to z.
\n
\b Input: \a taylor [ ( \a i_z + 1) * \a nc_taylor + k ] 
for k = 0 , ... , \a j - 1
is the k-th order Taylor coefficient corresponding to the auxillary result y.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to z. 
\n
\b Output: \a taylor [ ( \a i_z + 1 ) * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to 
the autillary result y.

\par Checked Assertions where op is a unary operator with two results:
\li NumArg(op) == 1
\li NumRes(op) == 2
\li \a i_x < \a i_z 
\li \a j < \a nc_taylor
*/
template <class Base>
inline void forward_unary2_op(
	size_t j           ,
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for zero order forward mode unary operator with two results (not used). 
\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 
The auxillary result is called y and has index \a i_z + 1.

\param i_x
variable index corresponding to the argument for this operator;
i.e. the row index in \a taylor corresponding to x.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_x * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to x. 
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z. 
\n
\b Output: \a taylor [ ( \a i_z + 1 ) * \a nc_taylor + j ] 
is the j-th order Taylor coefficient corresponding to 
the autillary result y. 

\par Checked Assertions where op is the unary operator with two result:
\li NumArg(op) == 1
\li NumRes(op) == 2
\li \a i_x < \a i_z 
\li \a j < \a nc_taylor
*/
template <class Base>
inline void forward_unary2_op_0(
	size_t i_z         ,
	size_t i_x         ,
	size_t nc_taylor   , 
	Base*  taylor      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for reverse mode unary operator with two results (not used).

This routine is given the partial derivatives of a function 
G( z , y , x , w , ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( x , w , u , ... ) = G[ z(x) , y(x), x , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param d
highest order Taylor coefficient that
we are computing the partial derivatives with respect to.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor to z. 
The auxillary result is called y and has index \a i_z + 1.

\param i_x
variable index corresponding to the argument for this operation;
i.e. the row index in \a taylor corresponding to x.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\a taylor [ \a i_x * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to x.
\n
\a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to z.
\n
\a taylor [ ( \a i_z + 1) * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to 
the auxillary variable y.

\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: \a partial [ \a i_x * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of 
G( z , y , x , w , u , ... ) 
with respect to the k-th order Taylor coefficient for x.
\n
\b Input: \a partial [ \a i_z * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for z.
\n
\b Input: \a partial [ ( \a i_z + 1) * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for the auxillary variable y.
\n
\b Output: \a partial [ \a i_x * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative of H( x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Output: \a partial [ \a i_z * \a nc_partial + k ]
for k = 0 , ... , \a d 
may be used as work space; i.e., may change in an unspecified manner.
\n
\b Output: \a partial [ ( \a i_z + 1) * \a nc_partial + k ]
for k = 0 , ... , \a d 
may be used as work space; i.e., may change in an unspecified manner.


\par Checked Assumptions where op is a unary operator with one result:
\li NumArg(op) == 1
\li NumRes(op) == 2
\li \a i_x < \a i_z 
\li \a d < \a nc_taylor
\li \a d < \a nc_partial
*/
template <class Base>
inline void reverse_unary2_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t      i_x          ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
// =================== Binary operators with one result ====================

/*!
Prototype forward mode x op y (not used)

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
order of the Taylor coefficient that we are computing.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param arg
\a arg[0]
index corresponding to the left operand for this operator;
i.e. the index corresponding to x.
\n
\a arg[1]
index corresponding to the right operand for this operator;
i.e. the index corresponding to y.

\param parameter
\b Input: If x is a parameter, \a parameter [ \a arg[0] ] 
is the value corresponding to x.
\n
\b Input: If y is a parameter, \a parameter [ \a arg[1] ] 
is the value corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: If x is a variable, \a taylor [ \a arg[0] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input: If y is a variable, \a taylor [ \a arg[1] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to y.
\n
\b Input: \a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d - 1
is the k-th order Taylor coefficient corresponding to z.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + \a d ] 
is the \a d-th order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is a binary operator:
\li NumArg(op) == 2
\li NumRes(op) == 1
\li If x is a variable, \a arg[0] < \a i_z 
\li If y is a variable, \a arg[1] < \a i_z 
\li \a d < \a nc_taylor
*/
template <class Base>
inline void forward_binary_op(
	size_t         d         ,
	size_t        i_z        ,
	const size_t* arg        ,
	const Base*   parameter  ,
	size_t        nc_taylor  , 
	Base*         taylor     )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}


/*!
Prototype zero order forward mode x op y (not used)

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param arg
\a arg[0]
index corresponding to the left operand for this operator;
i.e. the index corresponding to x.
\n
\a arg[1]
index corresponding to the right operand for this operator;
i.e. the index corresponding to y.

\param parameter
\b Input: If x is a parameter, \a parameter [ \a arg[0] ] 
is the value corresponding to x.
\n
\b Input: If y is a parameter, \a parameter [ \a arg[1] ] 
is the value corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: If x is a variable, \a taylor [ \a arg[0] * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to x.
\n
\b Input: If y is a variable, \a taylor [ \a arg[1] * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to y.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z. 

\par Checked Assertions where op is a binary operator:
\li NumArg(op) == 2
\li NumRes(op) == 1
\li If x is a variable, \a arg[0] < \a i_z 
\li If y is a variable, \a arg[1] < \a i_z 
*/
template <class Base>
inline void forward_binary_op_0(
	size_t        i_z         ,
	const size_t* arg         ,
	const Base*   parameter   ,
	size_t        nc_taylor   , 
	Base*         taylor      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

/*!
Prototype for reverse mode binary operator x op y (not used).

This routine is given the partial derivatives of a function 
G( z , y , x , w , ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( y , x , w , u , ... ) = G[ z(x , y) , y , x , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param d
highest order Taylor coefficient that
we are computing the partial derivatives with respect to.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in \a taylor corresponding to z. 

\param arg
\a arg[0]
index corresponding to the left operand for this operator;
i.e. the index corresponding to x.
\n
\a arg[1]
index corresponding to the right operand for this operator;
i.e. the index corresponding to y.

\param parameter
\b Input: If x is a parameter, \a parameter [ \a arg[0] ] 
is the value corresponding to x.
\n
\b Input: If y is a parameter, \a parameter [ \a arg[1] ] 
is the value corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a i_z * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to z.
\n
\b Input: If x is a variable, \a taylor [ \a arg[0] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input: If y is a variable, \a taylor [ \a arg[1] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to y.
\n

\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: \a partial [ \a i_z * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of 
G( z , y , x , w , u , ... ) 
with respect to the k-th order Taylor coefficient for z.
\n
\b Input: If x is a variable, \a partial [ \a arg[0] * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Input: If y is a variable, \a partial [ \a arg[1] * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for the auxillary variable y.
\n
\b Output: If x is a variable, \a partial [ \a arg[0] * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative of H( y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Output: If y is a variable, \a partial [ \a arg[1] * \a nc_partial + k ]
for k = 0 , ... , \a d 
is the partial derivative of H( y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for y.

\par Checked Assumptions where op is a binary operator with one result:
\li NumArg(op) == 2
\li NumRes(op) == 1
\li \a If x is a variable, arg[0] < \a i_z 
\li \a If y is a variable, arg[1] < \a i_z 
\li \a d < \a nc_taylor
\li \a d < \a nc_partial
*/
template <class Base>
inline void reverse_binary_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t*     arg          ,
	const Base* parameter    ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
// ======================= Pow Function ===================================
/*!
Prototype for forward mode z = pow(x, y) (not used).

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param d
order of the Taylor coefficient that we are computing.

\param i_z
variable index corresponding to the first result for this operation; 
i.e. the row index in \a taylor corresponding to z. 
Note that there are three results for this operation,
below they are referred to as z_0, z_1, z_2 and correspond to
\verbatim
	z_0 = log(x)
	z_1 = z0 * y
	z_2 = exp(z1)
\endverbatim
It follows that the last result is equal to z; i.e., z = z_2 = pow(x, y). 

\param arg
\a arg[0]
index corresponding to the left operand for this operator;
i.e. the index corresponding to x.
\n
\a arg[1]
index corresponding to the right operand for this operator;
i.e. the index corresponding to y.

\param parameter
\b Input: If x is a parameter, \a parameter [ \a arg[0] ] 
is the value corresponding to x.
\n
\b Input: If y is a parameter, \a parameter [ \a arg[1] ] 
is the value corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: If x is a variable, \a taylor [ \a arg[0] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input: If y is a variable, \a taylor [ \a arg[1] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to y.
\n
\b Input: \a taylor [ \a (i_z + j) * \a nc_taylor + k ] 
for j = 0, 1, 2 , and for k = 0 , ... , \a d - 1
is the k-th order Taylor coefficient corresponding to z_j.
\n
\b Output: \a taylor [ \a (i_z + j) * \a nc_taylor + \a d ] 
is the \a d-th order Taylor coefficient corresponding to z_j.

\par Checked Assertions where op is a binary operator:
\li NumArg(op) == 2
\li NumRes(op) == 3
\li If x is a variable, \a arg[0] < \a i_z 
\li If y is a variable, \a arg[1] < \a i_z 
\li \a d < \a nc_taylor
*/
template <class Base>
inline void forward_pow_op(
	size_t        d          ,
	size_t        i_z        ,
	const size_t* arg        ,
	const Base*   parameter  ,
	size_t        nc_taylor  , 
	Base*         taylor     )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
/*!
Prototype for zero order forward mode z = pow(x, y) (not used).

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param i_z
variable index corresponding to the first result for this operation; 
i.e. the row index in \a taylor corresponding to z. 
Note that there are three results for this operation,
below they are referred to as z_0, z_1, z_2 and correspond to
\verbatim
	z_0 = log(x)
	z_1 = z0 * y
	z_2 = exp(z1)
\endverbatim
It follows that the last result is equal to z; i.e., z = z_2 = pow(x, y). 

\param arg
\a arg[0]
index corresponding to the left operand for this operator;
i.e. the index corresponding to x.
\n
\a arg[1]
index corresponding to the right operand for this operator;
i.e. the index corresponding to y.

\param parameter
\b Input: If x is a parameter, \a parameter [ \a arg[0] ] 
is the value corresponding to x.
\n
\b Input: If y is a parameter, \a parameter [ \a arg[1] ] 
is the value corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: If x is a variable, \a taylor [ \a arg[0] * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to x.
\n
\b Input: If y is a variable, \a taylor [ \a arg[1] * \a nc_taylor + 0 ] 
is the k-th order Taylor coefficient corresponding to y.
\n
\b Output: \a taylor [ \a (i_z + j) * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z_j.

\par Checked Assertions where op is a binary operator:
\li NumArg(op) == 2
\li NumRes(op) == 3
\li If x is a variable, \a arg[0] < \a i_z 
\li If y is a variable, \a arg[1] < \a i_z 
*/
template <class Base>
inline void forward_pow_op_0(
	size_t        i_z        ,
	const size_t* arg        ,
	const Base*   parameter  ,
	size_t        nc_taylor  , 
	Base*         taylor     )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
/*!
Prototype for reverse mode z = pow(x, y) (not used).

This routine is given the partial derivatives of a function 
G( z , y , x , w , ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( y , x , w , u , ... ) = G[ pow(x , y) , y , x , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base .

\param d
highest order Taylor coefficient that
we are computing the partial derivatives with respect to.

\param i_z
variable index corresponding to the first result for this operation; 
i.e. the row index in \a taylor corresponding to z. 
Note that there are three results for this operation,
below they are referred to as z_0, z_1, z_2 and correspond to
\verbatim
	z_0 = log(x)
	z_1 = z0 * y
	z_2 = exp(z1)
\endverbatim
It follows that the last result is equal to z; i.e., z = z_2 = pow(x, y). 

\param arg
\a arg[0]
index corresponding to the left operand for this operator;
i.e. the index corresponding to x.
\n
\a arg[1]
index corresponding to the right operand for this operator;
i.e. the index corresponding to y.

\param parameter
\b Input: If x is a parameter, \a parameter [ \a arg[0] ] 
is the value corresponding to x.
\n
\b Input: If y is a parameter, \a parameter [ \a arg[1] ] 
is the value corresponding to y.

\param nc_taylor
number of colums in the matrix containing all the Taylor coefficients.

\param taylor
\b Input: \a taylor [ \a (i_z + j) * \a nc_taylor + k ] 
for j = 0, 1, 2 and k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to z_j.
\n
\b Input: If x is a variable, \a taylor [ \a arg[0] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to x.
\n
\b Input: If y is a variable, \a taylor [ \a arg[1] * \a nc_taylor + k ] 
for k = 0 , ... , \a d
is the k-th order Taylor coefficient corresponding to y.
\n

\param nc_partial
number of colums in the matrix containing all the partial derivatives.

\param partial
\b Input: \a partial [ \a (i_z + j) * \a nc_partial + k ] 
for j = 0, 1, 2, and k = 0 , ... , \a d
is the partial derivative of 
G( z , y , x , w , u , ... ) 
with respect to the k-th order Taylor coefficient for z_j.
\n
\b Input: If x is a variable, \a partial [ \a arg[0] * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Input: If y is a variable, \a partial [ \a arg[1] * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G( z , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for the auxillary variable y.
\n
\b Output: If x is a variable, \a partial [ \a arg[0] * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative of H( y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for x.
\n
\b Output: If y is a variable, \a partial [ \a arg[1] * \a nc_partial + k ]
for k = 0 , ... , \a d 
is the partial derivative of H( y , x , w , u , ... ) with respect to 
the k-th order Taylor coefficient for y.

\par Checked Assumptions where op is a binary operator with one result:
\li NumArg(op) == 2
\li NumRes(op) == 3
\li \a If x is a variable, arg[0] < \a i_z 
\li \a If y is a variable, arg[1] < \a i_z 
\li \a d < \a nc_taylor
\li \a d < \a nc_partial
*/
template <class Base>
inline void reverse_pow_op(
	size_t      d            ,
	size_t      i_z          ,
	size_t*     arg          ,
	const Base* parameter    ,
	size_t      nc_taylor    , 
	const Base* taylor       ,
	size_t      nc_partial   ,
	Base*       partial      )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
// ==================== Sparsity Calculations ==============================
/*!
Prototype for reverse mode Hessian sparsity, unary operators with one result (not used). 

This routine is given the forward mode Jacobian sparsity patterns for x.
It is also given the reverse mode dependence of G on z.
In addition, it is given the revese mode Hessian sparsity
for the quanity of interest G(z , y , ... )
and it uses them to compute the sparsity patterns for 
\verbatim
	H( x , w , u , ... ) = G[ z(x) , x , w , u , ... ]
\endverbatim

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in sparsity corresponding to z. 

\param i_x
variable index corresponding to the argument for this operator;
i.e. the row index in sparsity corresponding to x.

\param jac_z
is all zero (ones) if the Jacobian of G with respect to z is 
zero (non-zero).

\param nc_sparsity
number of packed values corresponding to each sparsity pattern; i.e.,
the number of columns in the sparsity pattern matrices.

\param jac_sparsity
for k = 0 , ... , \a nc_sparsity - 1,
jac_sparsitx[ \a i_x * \a nc_sparsity + k ]
is the forward mode Jacobian sparsity pattern for the variable x. 

\param hes_sparsity
\b Input: hes_sparsitx[ \a i_z * \a nc_sparsity + k ]
for k = 0 , ... , \a nc_sparsity -1 
is the Hessian sparsity pattern for the fucntion G
where one of the partials derivative is with respect to z.
\n
\b Input: hes_sparsitx[ \a i_x * \a nc_sparsity + k ]
for k = 0 , ... , \a nc_sparsity -1 
is the Hessian sparsity pattern for the fucntion G
where one of the partials derivative is with respect to y.
\n
\b Output: hes_sparsitx[ \a i_x * \a nc_sparsity + k ]
for k = 0 , ... , \a nc_sparsity -1 
is the Hessian sparsity pattern for the fucntion H
where one of the partials derivative is with respect to y.

\par Checked Assertions:
\li \a i_x < \a i_z 
*/
template <class Pack>
inline void reverse_sparse_hessian_unary_op(
	size_t      i_z           ,
	size_t      i_x           ,
	Pack        jac_z         ,
	size_t      nc_sparsity   ,
	const Pack* jac_sparsity  ,
	Pack*       hes_sparsity  )
{	
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}

CPPAD_END_NAMESPACE
# endif
