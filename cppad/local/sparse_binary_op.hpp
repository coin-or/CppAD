/* $Id$ */
# ifndef CPPAD_SPARSE_BINARY_OP_INCLUDED
# define CPPAD_SPARSE_BINARY_OP_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/connection.hpp>
CPPAD_BEGIN_NAMESPACE

/*!
\file sparse_binary_op.hpp
Forward and reverse mode sparsity patterns for binary operators.
*/


/*!
Forward mode Jacobian sparsity pattern for all binary operators. 

The C++ source code corresponding to a binary operation has the form
\verbatim
	z = fun(x, y)
\endverbatim
where fun is a C++ binary function and both x and y are variables, 
or it has the form
\verbatim
	z = x op y
\endverbatim
where op is a C++ binary unary operator and both x and y are variables.

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the from node index in sparsity corresponding to z. 

\param arg
\a arg[0]
variable index corresponding to the left operand for this operator;
i.e. the from node index in sparsity corresponding to x.
\n
\n arg[1]
variable index corresponding to the right operand for this operator;
i.e. the from node index in sparsity corresponding to y.

\param sparsity
\b Input: The from node with index \a arg[0] in \a sparsity
contains the sparsity bit pattern for x.
This identifies which of the independent variables the variable x
depends on. 
\n
\n
\b Input: The from node with index \a arg[1] in \a sparsity
contains the sparsity bit pattern for y.
This identifies which of the independent variables the variable y
depends on. 
\n
\n
\b Output: The from node with index \a i_z in \a sparsity
contains the sparsity bit pattern for z.
This identifies which of the independent variables the variable z
depends on. 

\par Checked Assertions:
\li \a arg[0] < \a i_z 
\li \a arg[1] < \a i_z 
*/


template <class Pack>
inline void forward_sparse_jacobian_binary_op(
	size_t            i_z           ,
	const size_t*     arg           ,
	connection<Pack>& sparsity      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	sparsity.binary_union(i_z, arg[0], arg[1]);

	return;
}	

/*!
Reverse mode Jacobian sparsity pattern for all binary operators. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = fun(x, y)
\endverbatim
where fun is a C++ unary function and x and y are variables, 
or it has the form
\verbatim
	z = x op y
\endverbatim
where op is a C++ bianry operator and x and y are variables.

This routine is given the sparsity patterns
for a function G(z, y, x, ... )
and it uses them to compute the sparsity patterns for 
\verbatim
	H( y, x, w , u , ... ) = G[ z(x,y) , y , x , w , u , ... ]
\endverbatim

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the row index in sparsity corresponding to z. 

\param arg
\a arg[0]
variable index corresponding to the left operand for this operator;
i.e. the row index in sparsity corresponding to x.
\n
\n arg[1]
variable index corresponding to the right operand for this operator;
i.e. the row index in sparsity corresponding to y.

\param nc_sparsity
number of packed values corresponding to each variable; i.e.,
the number of columns in the sparsity pattern matrix.

\param sparsity
\b Input: \a sparsity [ \a i_z * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable z. 
\n
\b Input: \a sparsity [ \a arg[0] * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable x. 
\n
\b Input: \a sparsity [ \a arg[1] * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable y. 
\n
\b Output: \a sparsity [ \a arg[0] * \a nc_sparsity + j ] 
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for H with respect to the variable x.
\n
\b Output: \a sparsity [ \a arg[1] * \a nc_sparsity + j ] 
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for H with respect to the variable y.

\par Checked Assertions:
\li \a arg[0] < \a i_z 
\li \a arg[1] < \a i_z 
*/
template <class Pack>
inline void reverse_sparse_jacobian_binary_op(
	size_t            i_z           ,
	const size_t*     arg           ,
	size_t            nc_sparsity   ,
	Pack*             sparsity      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	Pack* z  = sparsity + i_z * nc_sparsity;
	Pack* x  = sparsity + arg[0] * nc_sparsity;
	Pack* y  = sparsity + arg[1] * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
	{	x[j] |= z[j];
		y[j] |= z[j];
	}
	return;
}	

/*!
Reverse mode Hessian sparsity pattern for add and subtract operators. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = x op y
\endverbatim
where op is + or - and x, y are variables.

\copydetails reverse_sparse_hessian_binary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_addsub_op(
	size_t            i_z           ,
	const size_t*     arg           ,
	Pack*             jac_reverse   ,
	size_t            nc_sparsity   ,
	const Pack*       jac_forward   ,
	Pack*             hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	Pack* z_hes  = hes_sparsity + i_z * nc_sparsity;
	Pack* x_hes  = hes_sparsity + arg[0] * nc_sparsity;
	Pack* y_hes  = hes_sparsity + arg[1] * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
	{	x_hes[j] |= z_hes[j];
		y_hes[j] |= z_hes[j];
	}

	jac_reverse[arg[0]] |= jac_reverse[i_z];
	jac_reverse[arg[1]] |= jac_reverse[i_z];
	return;
}	

/*!
Reverse mode Hessian sparsity pattern for multiplication operator. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = x * y
\endverbatim
where x and y are variables.

\copydetails reverse_sparse_hessian_binary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_mul_op(
	size_t            i_z           ,
	const size_t*     arg           ,
	Pack*             jac_reverse         ,
	size_t            nc_sparsity   ,
	const Pack*       jac_forward   ,
	Pack*             hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	const Pack* x_for  = jac_forward  + arg[0] * nc_sparsity;
	const Pack* y_for  = jac_forward  + arg[1] * nc_sparsity;

	const Pack* z_hes  = hes_sparsity + i_z * nc_sparsity;
	Pack* x_hes  = hes_sparsity + arg[0] * nc_sparsity;
	Pack* y_hes  = hes_sparsity + arg[1] * nc_sparsity;

	size_t j = nc_sparsity;
	while(j--)
	{	x_hes[j] |= z_hes[j] | (jac_reverse[i_z] & y_for[j]);
		y_hes[j] |= z_hes[j] | (jac_reverse[i_z] & x_for[j]);
	}

	jac_reverse[arg[0]] |= jac_reverse[i_z];
	jac_reverse[arg[1]] |= jac_reverse[i_z];
	return;
}	

/*!
Reverse mode Hessian sparsity pattern for division operator. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = x / y
\endverbatim
where x and y are variables.

\copydetails reverse_sparse_hessian_binary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_div_op(
	size_t            i_z           ,
	const size_t*     arg           ,
	Pack*             jac_reverse         ,
	size_t            nc_sparsity   ,
	const Pack*       jac_forward   ,
	Pack*             hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	const Pack* x_for  = jac_forward  + arg[0] * nc_sparsity;
	const Pack* y_for  = jac_forward  + arg[1] * nc_sparsity;

	const Pack* z_hes  = hes_sparsity + i_z * nc_sparsity;
	Pack* x_hes  = hes_sparsity + arg[0] * nc_sparsity;
	Pack* y_hes  = hes_sparsity + arg[1] * nc_sparsity;

	size_t j = nc_sparsity;
	while(j--)
	{	x_hes[j] |= z_hes[j] | (jac_reverse[i_z] & y_for[j]);
		y_hes[j] |= z_hes[j] | ( jac_reverse[i_z] & (x_for[j] | y_for[j]) );
	}

	jac_reverse[arg[0]] |= jac_reverse[i_z];
	jac_reverse[arg[1]] |= jac_reverse[i_z];
	return;
}	

/*!
Reverse mode Hessian sparsity pattern for power function. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = pow(x, y)
\endverbatim
where x and y are variables.

\copydetails reverse_sparse_hessian_binary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_pow_op(
	size_t            i_z           ,
	const size_t*     arg           ,
	Pack*             jac_reverse         ,
	size_t            nc_sparsity   ,
	const Pack*       jac_forward   ,
	Pack*             hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( arg[0] < i_z );
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );

	const Pack* x_for  = jac_forward  + arg[0] * nc_sparsity;
	const Pack* y_for  = jac_forward  + arg[1] * nc_sparsity;

	const Pack* z_hes  = hes_sparsity + i_z * nc_sparsity;
	Pack* x_hes  = hes_sparsity + arg[0] * nc_sparsity;
	Pack* y_hes  = hes_sparsity + arg[1] * nc_sparsity;

	size_t j = nc_sparsity;
	while(j--)
	{	x_hes[j] |= z_hes[j] | 
			( jac_reverse[i_z] & (x_for[j] | y_for[j]) );
		y_hes[j] |= z_hes[j] | 
			( jac_reverse[i_z] & (x_for[j] | y_for[j]) );
	}

	// I cannot think of a case where this is necessary, but it including
	// it makes it like the other cases.
	jac_reverse[arg[0]] |= jac_reverse[i_z];
	jac_reverse[arg[1]] |= jac_reverse[i_z];
	return;
}	

CPPAD_END_NAMESPACE
# endif
