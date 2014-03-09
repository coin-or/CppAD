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
# include <cppad/local/cap_taylor.hpp>
# include <cppad/local/num_skip.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup forward_hpp forward.hpp
\{
\file forward.hpp
User interface to forward mode computations
*/

/*!
Compute arbitrary order forward mode Taylor coefficieints.

\tparam Base
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type \c AD<Base>.

\tparam Vector
is a Simple Vector class with eleements of type \c Base.

\param q
is the hightest order for this forward mode computation; i.e., 
after this calculation there will be <code>q+1</code>
Taylor coefficients per variables.

\param x_q
contains Taylor coefficients for the independent variables.
The size of \a x_q must either be \c n or <code>n*(q+1)</code>,
We define <code>p = q + 1 - x_q.size() / n</code>. 
The Taylor coefficients of order k, for
k = p, ... , q are calculated.

\param s
Is the stream where output corresponding to \c PriOp operations will written.
*/

template <typename Base>
template <typename Vector>
Vector ADFun<Base>::Forward(
	size_t q                    , 
	const Vector& x_q           , 
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
		size_t(x_q.size()) == n || size_t(x_q.size()) == n*(q+1),
		"Forward: x_q.size() is not equal n or n*(q+1)"
	);
	size_t n_order = size_t(x_q.size()) / n;
	CPPAD_ASSERT_KNOWN(
		q <= taylor_per_var_ || n_order == q + 1,
		"The number of Taylor coefficient currently stored in this ADFun\n"
		"is less than q and x_q.size() != n*(q+1)."
	);  

	// check if the taylor_ matrix needs more columns
	if( taylor_col_dim_ <= q )
		capacity_taylor(q + 1);
	CPPAD_ASSERT_UNKNOWN( taylor_col_dim_ > q );

	// set the q-th order taylor_ coefficients for independent variables
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < total_num_var_ );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == InvOp );

		// It is also variable taddr for j-th independent variable
		if( n_order ==  1 )
			taylor_[ind_taddr_[j] * taylor_col_dim_ + q] = x_q[j];
		else for(k = 0; k < n_order; k++)
			taylor_[ind_taddr_[j] * taylor_col_dim_ + k] = 
				x_q[j * n_order + k];
	}

	// evaluate the derivatives
	CPPAD_ASSERT_UNKNOWN( cskip_op_.size() == play_.num_op_rec() );
	CPPAD_ASSERT_UNKNOWN( load_op_.size()   == play_.num_load_op_rec() );
	size_t p = (q + 1) - n_order;
	if( q == 0 )
	{
# if CPPAD_USE_FORWARD0SWEEP
		compare_change_ = forward0sweep(s, true,
			n, total_num_var_, &play_, taylor_col_dim_, taylor_.data(),
			cskip_op_.data(), load_op_
		);
# else
		compare_change_ = forward_sweep(s, true, p,
			q, n, total_num_var_, &play_, taylor_col_dim_, taylor_.data(),
			cskip_op_.data(), load_op_
		);
# endif
	}
	else if( p == 0 )
	{	compare_change_ = forward_sweep(s, true, p,
			q, n, total_num_var_, &play_, taylor_col_dim_, taylor_.data(),
			cskip_op_.data(), load_op_
		);
	}
	else
	{	forward_sweep(s, true, p,
			q, n, total_num_var_, &play_, taylor_col_dim_, taylor_.data(),
			cskip_op_.data(), load_op_
		);
	}

	// return Taylor coefficients for dependent variables
	Vector y_q;
	if( n_order == 1 )
	{	y_q.resize(m);
		for(i = 0; i < m; i++)
		{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );
			y_q[i] = taylor_[dep_taddr_[i] * taylor_col_dim_ + q];
		}
	}
	else
	{	y_q.resize(m * n_order );
		for(i = 0; i < m; i++)	
		{	for(k = 0; k < n_order; k++)
				y_q[ i * n_order + k] = 
					taylor_[ dep_taddr_[i] * taylor_col_dim_ + k ]; 
		}
	}
# ifndef NDEBUG
	if( check_for_nan_ )
	{	bool ok = true;
		if( q == 0 && n_order == 1 )
			ok = ! hasnan(y_q);
		else if( n_order != 1 )
		{	for(i = 0; i < m; i++)
			// on MS Visual Studio 2012, CppAD required in front of isnan ?
			ok &= ! CppAD::isnan( y_q[ i * n_order + 0 ] );
		} 
		CPPAD_ASSERT_KNOWN(ok,
			"y_q = f.Forward(q, x): has a zero order Taylor coefficient "
			"with the value nan."
		);  
		if( q != 0 && n_order == 1 )
			ok = ! hasnan(y_q);
		else if( n_order != 1 )
		{	for(i = 0; i < m; i++)
			{	for(k = 1; k < n_order; k++)
					// Studio 2012, CppAD required in front of isnan ?
					ok &= ! CppAD::isnan( y_q[ i * n_order + k ] );
			}
		}
		CPPAD_ASSERT_KNOWN(ok,
		"y_q = f.Forward(q, x): has a non-zero order Taylor coefficient\n"
		"with the value nan (but zero order coefficients are not nan)."
		);
	}
# endif


	// now we have q + 1  taylor_ coefficients per variable
	taylor_per_var_ = q + 1;

	return y_q;
}

/*! \} */
} // END_CPPAD_NAMESPACE
# endif
