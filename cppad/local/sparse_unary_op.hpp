/* $Id$ */
# ifndef CPPAD_SPARSE_UNARY_OP_INCLUDED
# define CPPAD_SPARSE_UNARY_OP_INCLUDED
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
\file sparse_unary_op.hpp
Forward and reverse mode sparsity patterns for unary operators.
*/


/*!
Forward mode Jacobian sparsity pattern for all unary operators. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = fun(x)
\endverbatim
where fun is a C++ unary function, or it has the form
\verbatim
	z = x op p
\endverbatim
where op is a C++ binary unary operator and p is a parameter.

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param i_z
variable index corresponding to the result for this operation; 
i.e. the from node index in sparsity corresponding to z. 

\param i_x
variable index corresponding to the argument for this operator;
i.e. the from node index in sparsity corresponding to x.


\param sparsity
\b Input: The from node with index \a arg[0] in \a sparsity
contains the sparsity bit pattern for x.
This identifies which of the independent variables the variable x
depends on. 
\n
\n
\b Output: The from node with index \a i_z in \a sparsity
contains the sparsity bit pattern for z.
This identifies which of the independent variables the variable z
depends on. 
\n

\par Checked Assertions:
\li \a i_x < \a i_z 
*/

template <class Pack>
inline void forward_sparse_jacobian_unary_op(
	size_t            i_z           ,
	size_t            i_x           ,
	connection<Pack>& sparsity      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );

	sparsity.assignment(i_z, i_x);
}	

/*!
Reverse mode Jacobian sparsity pattern for all unary operators. 

The C++ source code corresponding to a unary operation has the form
\verbatim
	z = fun(x)
\endverbatim
where fun is a C++ unary function, or it has the form
\verbatim
	z = x op p
\endverbatim
where op is a C++ bianry operator and p is a parameter.

This routine is given the sparsity patterns
for a function G(z, y, ... )
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

\param nc_sparsity
number of packed values corresponding to each variable; i.e.,
the number of columns in the sparsity pattern matrix.

\param sparsity
\b Input: \a sparsity [ \a i_z * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable z. 
\n
\b Input: \a sparsity [ \a i_x * \a nc_sparsity + j ]
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for G with respect to the variable x. 
\n
\b Output: \a sparsity [ \a i_x * \a nc_sparsity + j ] 
for j = 0 , ... , \a nc_sparsity - 1 
is the sparsity bit pattern for H with respect to the variable x.

\par Checked Assertions:
\li \a i_x < \a i_z 
*/

template <class Pack>
inline void reverse_sparse_jacobian_unary_op(
	size_t     i_z           ,
	size_t     i_x           ,
	size_t     nc_sparsity   ,
	Pack*      sparsity      )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );

	Pack* z  = sparsity + i_z * nc_sparsity;
	Pack* x  = sparsity + i_x * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		x[j] |= z[j];
	return;
}	

/*!
Reverse mode Hessian sparsity pattern for linear unary operators.

The C++ source code corresponding to this operation is
\verbatim
        z = fun(x)
\endverbatim
where fun is a linear functions; e.g. abs, or
\verbatim
	z = x op p
\endverbatim
where op is a C++ binary operator and p is a parameter.

\copydetails reverse_sparse_hessian_unary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_linear_unary_op(
	size_t      i_z           ,
	size_t      i_x           ,
	Pack*       jac_reverse   ,
	size_t      nc_sparsity   ,
	const Pack* jac_forward   ,
	Pack*       hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );

	Pack* z_hes  = hes_sparsity + i_z * nc_sparsity;
	Pack* x_hes  = hes_sparsity + i_x * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		x_hes[j] |= z_hes[j];

	jac_reverse[i_x] |= jac_reverse[i_z];
	return;
}

/*!
Reverse mode Hessian sparsity pattern for non-linear unary operators.

The C++ source code corresponding to this operation is
\verbatim
        z = fun(x)
\endverbatim
where fun is a non-linear functions; e.g. sin. or
\verbatim
	z = p / x
\endverbatim
where p is a parameter.


\copydetails reverse_sparse_hessian_unary_op
*/
template <class Pack>
inline void reverse_sparse_hessian_nonlinear_unary_op(
	size_t      i_z           ,
	size_t      i_x           ,
	Pack*       jac_reverse   ,
	size_t      nc_sparsity   ,
	const Pack* jac_forward   ,
	Pack*       hes_sparsity  )
{	
	// check assumptions
	CPPAD_ASSERT_UNKNOWN( i_x < i_z );

	const Pack* z_hes  = hes_sparsity + i_z * nc_sparsity;
	const Pack* x_for  = jac_forward  + i_x * nc_sparsity;
	Pack* x_hes        = hes_sparsity + i_x * nc_sparsity;
	size_t j = nc_sparsity;
	while(j--)
		x_hes[j] |= z_hes[j] | (jac_reverse[i_z] & x_for[j]);

	jac_reverse[i_x] |= jac_reverse[i_z];
	return;
}

CPPAD_END_NAMESPACE
# endif
