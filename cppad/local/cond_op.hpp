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
Forward, reverse, and sparse operations for conditional expressions.
*/

/*!
Compute forward mode Taylor coefficients for op = CExpOp.

\copydetails conditional_exp_op

\param d
is the order of the Taylor coefficient of z that we are computing.

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

	CPPAD_ASSERT_UNKNOWN( arg[0] < static_cast<size_t> (CompareNe) );
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

\copydetails conditional_exp_op

\param taylor
\b Input:
For j = 0, 1, 2, 3,
if y_j is a variable then
\a taylor [ \a arg[2+j] * nc_taylor + 0 ]
is the zero order Taylor coefficient corresponding to y_j.
\n
\b Output: \a taylor [ \a i_z * \a nc_taylor + 0 ] 
is the zero order Taylor coefficient corresponding to z. 
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

	CPPAD_ASSERT_UNKNOWN( arg[0] < static_cast<size_t> (CompareNe) );
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

\copydetails conditional_exp_op

This routine is given the partial derivatives of a function 
G( z , y , x , w , ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( y , x , w , u , ... ) = G[ z(y) , y , x , w , u , ... ]
\endverbatim
where y above represents y_0, y_1, y_2, y_3.

\param d
is the order of the Taylor coefficient of z that we are  computing.

\param taylor
\b Input:
For j = 0, 1, 2, 3 and k = 0 , ... , \a d,
if y_j is a variable then
\a taylor [ \a arg[2+j] * nc_taylor + k ]
is the k-th order Taylor coefficient corresponding to y_j.
\n
\a taylor [ \a i_z * \a nc_taylor + k ] 
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

	CPPAD_ASSERT_UNKNOWN( arg[0] < static_cast<size_t> (CompareNe) );
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

/*!
Compute forward Jacobian sparsity patterns for op = CExpOp.

\copydetails sparse_conditional_exp_op

\param sparsity
\b Input:
if y_2 is a variable,
for k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a arg[4] * nc_sparsity + k ]
is the sparsity bit pattern corresponding to y_2.
\n
\b Input:
if y_3 is a variable,
for k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a arg[5] * nc_sparsity + k ]
is the sparsity bit pattern corresponding to y_3.
\n
\b Output: 
for k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a i_z * \a nc_sparsity + k ] 
is the sparsity bit pattern corresponding to z.
*/
template <class Pack>
inline void forward_sparse_jacobian_cond_op(
	size_t         i_z           ,
	const size_t*  arg           , 
	size_t         num_par       ,
	size_t         nc_sparsity   ,
	Pack*          sparsity      )
{	// value with all it's bits false
	static Pack zero(0);
	
	Pack* y_2 = CPPAD_NULL;
	Pack* y_3 = CPPAD_NULL;	
	Pack* z   = sparsity + i_z * nc_sparsity;

	CPPAD_ASSERT_UNKNOWN( arg[0] < static_cast<size_t> (CompareNe) );
	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

# ifndef NDEBUG
	if( arg[1] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );
	}
	if( arg[1] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < i_z );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < num_par );
	}
# endif
	size_t k = nc_sparsity;
	if( arg[1] & 4 )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
		y_2 = sparsity + arg[4] * nc_sparsity;
		if( arg[1] & 8 )
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
			y_3 = sparsity + arg[5] * nc_sparsity;
			while(k--)
				z[k] = y_2[k] | y_3[k];
		}
		else
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < num_par );
			while(k--)
				z[k] = y_2[k];
		}
	}	
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < num_par );
		if( arg[1] & 8 )
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
			y_3 = sparsity + arg[5] * nc_sparsity;
			while(k--)
				z[k] = y_3[k];
		}
		else
		{	CPPAD_ASSERT_UNKNOWN( arg[5] < num_par );
			while(k--)
				z[k] = zero;
		}
	}
	return;
}

/*!
Compute reverse Jacobian sparsity patterns for op = CExpOp.

This routine is given the sparsity patterns
for a function G(z, y, x, ... )
and it uses them to compute the sparsity patterns for 
\verbatim
	H( y, x, w , u , ... ) = G[ z(x,y) , y , x , w , u , ... ]
\endverbatim
where y represents the combination of y_0, y_1, y_2, and y_3.

\copydetails sparse_conditional_exp_op

\param sparsity
If y_2 is a variable,
for k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a arg[4] * nc_sparsity + k ]
is the sparsity bit pattern corresponding to y_2.
On input, this pattern corresponds to the function G.
On ouput, it corresponds to the function H.
\n
\n
If y_3 is a variable,
for k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a arg[5] * nc_sparsity + k ]
is the sparsity bit pattern corresponding to y_3.
On input, this pattern corresponds to the function G.
On ouput, it corresponds to the function H.
\n
\n
For k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a i_z * nc_sparsity + k ]
is the sparsity bit pattern corresponding to z.
On input and output, this pattern corresponds to the function G.
*/
template <class Pack>
inline void reverse_sparse_jacobian_cond_op(
	size_t         i_z           ,
	const size_t*  arg           , 
	size_t         num_par       ,
	size_t         nc_sparsity   ,
	Pack*          sparsity      )
{	
	Pack* z   = sparsity + i_z * nc_sparsity;

	CPPAD_ASSERT_UNKNOWN( arg[0] < static_cast<size_t> (CompareNe) );
	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

# ifndef NDEBUG
	if( arg[1] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );
	}
	if( arg[1] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < i_z );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < num_par );
	}
	if( ! ( arg[1] & 4 ) )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < num_par );
	}
	if( ! ( arg[1] & 8 ) )
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < num_par );
	}
# endif
	size_t k;
	if( arg[1] & 4 )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
		Pack* y_2 = sparsity + arg[4] * nc_sparsity;
		k = nc_sparsity;
		while(k--)
			y_2[k] |= z[k];
	}
	if( arg[1] & 8 )
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
		Pack* y_3 = sparsity + arg[5] * nc_sparsity;
		k = nc_sparsity;
		while(k--)
			y_3[k] |= z[k];
	}
	return;
}

/*!
Compute reverse Hessian sparsity patterns for op = CExpOp.

This routine is given the sparsity patterns
for a function G(z, y, x, ... )
and it uses them to compute the sparsity patterns for 
\verbatim
	H( y, x, w , u , ... ) = G[ z(x,y) , y , x , w , u , ... ]
\endverbatim
where y represents the combination of y_0, y_1, y_2, and y_3.

\copydetails sparse_conditional_exp_op

\param z_jac
is all true (ones complement of 0) if the scalar valued 
function we are computing the Hessian sparsity for 
has a non-zero partial with respect to the variable z
(actually may have a non-zero partial with respect to z).
Otherwise it zero.

\param hes_sparsity
For k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a i_z * nc_sparsity + k ]
is the sparsity bit pattern corresponding to z.
On input and output, this pattern corresponds to the function G.
\n
\n
For k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a arg[4] * nc_sparsity + k ]
is the sparsity bit pattern corresponding to y_2.
On input, this pattern corresponds to the function G.
On output, this pattern corresponds to the function H.
\n
\n
For k = 0 , ... , nc_sparsity-1,
\a sparsity [ \a arg[5] * nc_sparsity + k ]
is the sparsity bit pattern corresponding to y_3.
On input, this pattern corresponds to the function G.
On output, this pattern corresponds to the function H.
*/
template <class Pack>
inline void reverse_sparse_hessian_cond_op(
	size_t         i_z           ,
	const size_t*  arg           , 
	size_t         num_par       ,
	Pack           z_jac         ,
	size_t         nc_sparsity   ,
	Pack*          hes_sparsity  )
{	

	CPPAD_ASSERT_UNKNOWN( arg[0] < static_cast<size_t> (CompareNe) );
	CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
	CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

	Pack* z_hes   = hes_sparsity + i_z * nc_sparsity;

# ifndef NDEBUG
	if( arg[1] & 1 )
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );
	}
	if( arg[1] & 2 )
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < i_z );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( arg[3] < num_par );
	}
	if( ! ( arg[1] & 4 ) )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < num_par );
	}
	if( ! ( arg[1] & 8 ) )
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < num_par );
	}
# endif
	size_t k;
	if( arg[1] & 4 )
	{	CPPAD_ASSERT_UNKNOWN( arg[4] < i_z );
		Pack* y_2_hes = hes_sparsity + arg[4] * nc_sparsity;
		k = nc_sparsity;
		while(k--)
			y_2_hes[k] |= z_hes[k];
	}
	if( arg[1] & 8 )
	{	CPPAD_ASSERT_UNKNOWN( arg[5] < i_z );
		Pack* y_3_hes = hes_sparsity + arg[5] * nc_sparsity;
		k = nc_sparsity;
		while(k--)
			y_3_hes[k] |= z_hes[k];
	}
	return;
}

CPPAD_END_NAMESPACE
# endif
