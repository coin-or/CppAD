/* $Id$ */
# ifndef CPPAD_FORWARD_INCLUDED
# define CPPAD_FORWARD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// documened after Forward but included here so easy to see
# include <cppad/local/capacity_order.hpp>
# include <cppad/local/num_skip.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\{
\file forward.hpp
User interface to forward mode computations
*/

/*!
Arbitrary order, one direction, forward mode Taylor coefficieints.

\tparam Base
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type AD<Base>.

\tparam Vector
is a Simple Vector class with eleements of type Base.

\param q
is the hightest order for this forward mode computation; i.e., 
after this calculation there will be <code>q+1</code>
Taylor coefficients per variable.

\param xq
contains Taylor coefficients for the independent variables.
The size of xq must either be n or <code>(q+1)*n</code>,
We define <code>p = q + 1 - xq.size()/n</code>. 
For <code>j = 0 , ... , n-1</code>,
<code>k = p, ... , q</code>, are
<code>xq[ (q+1-p)*j + k - p ]</code>
is the k-th order coefficient for the j-th independent variable.

\param s
Is the stream where output corresponding to PriOp operations will written.

\return
contains Taylor coefficients for the independent variables.
The size of the return value y is <code>m*(q+1-p)</code>.
For <code>i = 0, ... , m-1</code>,
<code>k = p, ..., q</code>,
<code>y[(q+1-p)*i + (k-p)]</code> 
is the k-th order coefficient for the i-th dependent variable.

\par taylor_
The Taylor coefficients up to order p-1 are inputs
and the coefficents from order p through q are outputs.
Let <code>N = num_var_tape_</code>, and
<code>C = cap_order_taylor_</code>.
Note that for
<code>i = 1 , ..., N-1</code>,
<code>k = 0 , ..., q</code>,
<code>taylor_[ C*i + k ]</code>
is the k-th order cofficent,
for the i-th varaible on the tape.
(The first independent variable has index one on the tape 
and there is no variable with index zero.)
*/

template <typename Base>
template <typename Vector>
Vector ADFun<Base>::Forward(
	size_t q                    , 
	const Vector& xq            , 
	std::ostream& s             )
{	// temporary indices
	size_t i, j, k;

	// number of independent variables
	size_t n = ind_taddr_.size();

	// number of dependent variables
	size_t m = dep_taddr_.size();

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, Vector>();


	CPPAD_ASSERT_KNOWN(
		size_t(xq.size()) == n || size_t(xq.size()) == n*(q+1),
		"Forward(q, xq): xq.size() is not equal n or n*(q+1)"
	);

	// lowest order we are computing
	size_t p = q + 1 - size_t(xq.size()) / n;
	CPPAD_ASSERT_UNKNOWN( p == 0 || p == q );
	CPPAD_ASSERT_KNOWN(
		q <= num_order_taylor_ || p == 0,
		"Forward(q, xq): Number of Taylor coefficient orders stored in this"
		" ADFun\nis less than q and xq.size() != n*(q+1)."
	);  
	// does taylor_ need more orders or different number of directions
	if( cap_order_taylor_ <= q )
	{	if( p == 0 )
		{	// no need to copy old values during capacity_order
			num_order_taylor_ = 0;
		}
		size_t c = std::max(q + 1, cap_order_taylor_);
		capacity_order(c);
	}
	CPPAD_ASSERT_UNKNOWN( cap_order_taylor_ > q );

	// short hand notation for order capacity
	size_t C = cap_order_taylor_;

	// set Taylor coefficients for independent variables
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < num_var_tape_  );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == InvOp );

		if( p ==  q )
			taylor_[ C * ind_taddr_[j] + q] = xq[j];
		else
		{	for(k = 0; k <= q; k++)
				taylor_[ C * ind_taddr_[j] + k] = xq[ (q+1)*j + k];
		}
	}

	// evaluate the derivatives
	CPPAD_ASSERT_UNKNOWN( cskip_op_.size() == play_.num_op_rec() );
	CPPAD_ASSERT_UNKNOWN( load_op_.size()  == play_.num_load_op_rec() );
	if( q == 0 )
	{
		compare_change_ = forward0sweep(s, true,
			n, num_var_tape_, &play_, C, 
			taylor_.data(), cskip_op_.data(), load_op_
		);
	}
	else if( p == 0 )
	{	compare_change_ = forward1sweep(s, true, p, q, 
			n, num_var_tape_, &play_, C, 
			taylor_.data(), cskip_op_.data(), load_op_
		);
	}
	else
	{	forward1sweep(s, true, p, q, 
			n, num_var_tape_, &play_, C, 
			taylor_.data(), cskip_op_.data(), load_op_
		);
	}

	// return Taylor coefficients for dependent variables
	Vector yq;
	if( p == q )
	{	yq.resize(m);
		for(i = 0; i < m; i++)
		{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_  );
			yq[i] = taylor_[ C * dep_taddr_[i] + q];
		}
	}
	else
	{	yq.resize(m * (q+1) );
		for(i = 0; i < m; i++)	
		{	for(k = 0; k <= q; k++)
				yq[ (q+1) * i + k] = 
					taylor_[ C * dep_taddr_[i] + k ]; 
		}
	}
# ifndef NDEBUG
	if( check_for_nan_ )
	{	bool ok = true;
		if( p == 0 )
		{	for(i = 0; i < m; i++)
			{	// Visual Studio 2012, CppAD required in front of isnan ?
				ok &= ! CppAD::isnan( yq[ (q+1) * i + 0 ] );
			}
		} 
		CPPAD_ASSERT_KNOWN(ok,
			"yq = f.Forward(q, xq): has a zero order Taylor coefficient "
			"with the value nan."
		);  
		if( 0 < q )
		{	for(i = 0; i < m; i++)
			{	for(k = p; k <= q; k++)
				{	// Studio 2012, CppAD required in front of isnan ?
					ok &= ! CppAD::isnan( yq[ (q+1-p)*i + k-p ] );
				}
			}
		}
		CPPAD_ASSERT_KNOWN(ok,
		"yq = f.Forward(q, xq): has a non-zero order Taylor coefficient\n"
		"with the value nan (but zero order coefficients are not nan)."
		);
	}
# endif

	// now we have q + 1  taylor_ coefficient orders per variable
	num_order_taylor_ = q + 1;

	return yq;
}


} // END_CPPAD_NAMESPACE
# endif
