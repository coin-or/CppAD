// $Id$
# ifndef CPPAD_EXAMPLE_EIGEN_MAT_INV_HPP
# define CPPAD_EXAMPLE_EIGEN_MAT_INV_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_eigen_mat_inv.hpp$$
$spell
	Eigen
	Taylor
$$

$section Atomic Eigen Matrix Inversion Class$$

$head Purpose$$
For fixed positive integer $latex p$$,
construct and atomic operation that solve the computes the matrix inverse
$latex R = A^{-1}$$
for any invertible $latex A \in \B{R}^{p \times p}$$.

$head Theory$$

$head Forward$$
For $latex k = 0 , \ldots$$, the $th k$$ order Taylor coefficient is given by
$latex \[
	I_k = \sum_{\ell=0}^k A_\ell R_{k-\ell}
\] $$
$latex \[
	R_k = A_0^{-1} \left( I_k - \sum_{\ell=1}^k A_\ell R_{k-\ell} \right)
\] $$
where $latex I_k$$ is the $th k$$ order Taylor coefficient for the
identity matrix. i.e., $latex I_k$$ is the identity matrix if $latex k = 0$$
and is the zero matrix if $latex k \neq 0$$.

$head Reverse$$
We use $latex \bar{R}_k$$ for the partial of the scalar final result
with respect to $latex R_k$$.
Note that $latex R_0 = A_0^{-1}$$.
The back-propagation algorithm that eliminates $latex R_k$$,
for $latex k > 0$$, is
for $latex \ell = 1 , \ldots , k$$
$latex \[
\bar{A}_\ell = \bar{A}_\ell - R_0 \bar{R}_k R_{k-\ell}^\R{T}
\] $$
$latex \[
\bar{R}_{k-\ell} = \bar{R}_{k-\ell} - R_0 A_\ell^\R{T} \bar{R}_k
\] $$
$latex \[
\bar{R}_0  = \bar{R}_0 + \bar{R}_k
	\left( I_k - \sum_{\ell=1}^k A_\ell R_{k-\ell} \right)^\R{T}
\] $$
$latex \[
\bar{R}_0  = \bar{R}_0 + \bar{R}_k ( A_0 R_k )^\R{T}
\] $$
The back-propagation algorithm that eliminates $latex R_0$$ is
$latex \[
	\bar{A}_0 = \bar{A}_0 - R_0^\R{T} \bar{R}_0 R_0^\R{T}
\]$$

$nospell

$head Start Class Definition$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <Eigen/Core>
# include <Eigen/LU>



/* %$$
$head Public$$

$subhead Types$$
$srccode%cpp% */
namespace { // BEGIN_EMPTY_NAMESPACE

template <class Base>
class atomic_eigen_mat_inv : public CppAD::atomic_base<Base> {
public:
	// -----------------------------------------------------------
	// type of elements during calculation of derivatives
	typedef Base              scalar;
	// type of elements during taping
	typedef CppAD::AD<scalar> ad_scalar;
	// type of matrix during calculation of derivatives
	typedef Eigen::Matrix<
		scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>     matrix;
	// type of matrix during taping
	typedef Eigen::Matrix<
		ad_scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor > ad_matrix;
/* %$$
$subhead Constructor$$
$srccode%cpp% */
	// constructor
	atomic_eigen_mat_inv(
		// number of rows and columns in argument
		const size_t nr
	) :
	CppAD::atomic_base<Base>(
		"atom_eigen_mat_inv"                             ,
		CppAD::atomic_base<Base>::set_sparsity_enum
	) ,
	nr_(  nr  )       ,
	nx_( nr * nr )    ,
	f_sum_( nr, nr )
	{ }
/* %$$
$subhead op$$
$srccode%cpp% */
	// use atomic operation to invert an AD matrix
	ad_matrix op(const ad_matrix& arg)
	{	assert( nr_ == size_t( arg.rows() ) );
		assert( nr_ == size_t( arg.cols() ) );

		// -------------------------------------------------------------------
		// packed version of arg
		CPPAD_TESTVECTOR(ad_scalar) packed_arg(nx_);
		for(size_t i = 0; i < nx_; i++)
			packed_arg[i] = arg.data()[i];

		// -------------------------------------------------------------------
		// packed version of result = arg^{-1}
		CPPAD_TESTVECTOR(ad_scalar) packed_result(nx_);
		(*this)(packed_arg, packed_result);

		// -------------------------------------------------------------------
		// unpack result matrix
		ad_matrix result(nr_, nr_);
		for(size_t i = 0; i < nx_; i++)
			result.data()[i] = packed_result[ i ];

		return result;
	}
/* %$$
$head Private$$

$subhead Variables$$
$srccode%cpp% */
private:
	// -------------------------------------------------------------
	// number of of rows in argument and result
	const size_t nr_;
	// size of the domain (and range) space
	const size_t nx_;
	// -------------------------------------------------------------
	// one forward mode vector of matrices for argument and result
	CppAD::vector<matrix> f_arg_, f_result_;
	// matrix used for forward mode summation
	matrix f_sum_;
	// one reverse mode vector of matrices for argument and result
	CppAD::vector<matrix> r_arg_, r_result_;
	// -------------------------------------------------------------
/* %$$
$subhead rows$$
$srccode%cpp% */
	// convert from int to size_t
	static size_t rows(const matrix& x)
	{	return size_t( x.rows() ); }
	static size_t rows(const ad_matrix& x)
	{	return size_t( x.rows() ); }
/* %$$
$subhead cols$$
$srccode%cpp% */
	// convert from int to size_t
	static size_t cols(const matrix& x)
	{	return size_t( x.cols() ); }
	static size_t cols(const ad_matrix& x)
	{	return size_t( x.cols() ); }
/* %$$
$subhead forward$$
$srccode%cpp% */
	// forward mode routine called by CppAD
	virtual bool forward(
		// lowest order Taylor coefficient we are evaluating
		size_t                          p ,
		// highest order Taylor coefficient we are evaluating
		size_t                          q ,
		// which components of x are variables
		const CppAD::vector<bool>&      vx ,
		// which components of y are variables
		CppAD::vector<bool>&            vy ,
		// tx [ j * (q+1) + k ] is x_j^k
		const CppAD::vector<scalar>&    tx ,
		// ty [ i * (q+1) + k ] is y_i^k
		CppAD::vector<scalar>&          ty
	)
	{	size_t n_order = q + 1;
		assert( vx.size() == 0 || nx_ == vx.size() );
		assert( vx.size() == 0 || nx_ == vy.size() );
		assert( nx_ * n_order == tx.size() );
		assert( nx_ * n_order == ty.size() );
		//
		// -------------------------------------------------------------------
		// make sure f_arg_ and f_result_ are large enough
		assert( f_arg_.size() == f_result_.size() );
		if( f_arg_.size() < n_order )
		{	f_arg_.resize(n_order);
			f_result_.resize(n_order);
			//
			for(size_t k = 0; k < n_order; k++)
			{	f_arg_[k].resize(nr_, nr_);
				f_result_[k].resize(nr_, nr_);
			}
		}
		// -------------------------------------------------------------------
		// unpack tx into f_arg_
		for(size_t k = 0; k < n_order; k++)
		{	// unpack arg values for this order
			for(size_t i = 0; i < nx_; i++)
				f_arg_[k].data()[i] = tx[ i * n_order + k ];
		}
		// -------------------------------------------------------------------
		// result for each order
		//
		// compute LU factorixation of arg_[0]
		Eigen::FullPivLU<matrix> lu_arg( f_arg_[0] );
		for(size_t k = 0; k < n_order; k++)
		{	// initialize sum
			if( k == 0 )
				f_sum_ = matrix::Identity(nr_, nr_);
			else
				f_sum_ = matrix::Zero(nr_, nr_);
			// compute sum
			for(size_t ell = 1; ell <= k; ell++)
				f_sum_ -= f_arg_[ell] * f_result_[k-ell];
			// solve arg_[0] * result_[k] = sum
			// result_[k] = arg_[0]^{-1} * sum
			f_result_[k] = lu_arg.solve(f_sum_);
		}
		// -------------------------------------------------------------------
		// pack result_ into ty
		for(size_t k = 0; k < n_order; k++)
		{	for(size_t i = 0; i < nx_; i++)
				ty[ i * n_order + k ] = f_result_[k].data()[i];
		}

		// check if we are computing vy
		if( vx.size() == 0 )
			return true;
		// ------------------------------------------------------------------
		// compute variable information for y; i.e., vy
		// (note that the constant zero times a variable is a constant)
		scalar zero(0.0);
		assert( n_order == 1 );
		for(size_t j = 0; j < nr_; j++)
		{	// only row j of this column of the identity is non-zero
			// initialize which elements of column j of result are variables
			for(size_t i = 0; i < nr_; i++)
			{	// initialize vy as false
				size_t index = i * nr_ + j;
				vy[index]    = false;
			}
			// determine if any elements in row j of argument are variables
			bool row_var = false;
			for(size_t ell = 0; ell < nr_; ell++)
			{	// arg information
				size_t index  = j * nr_ + ell;
				row_var      |= vx[index];
			}
			if( row_var )
			{	for(size_t ell = 0; ell < nr_; ell++)
				{	// arg information
					size_t index = j * nr_ + ell;
					bool not_zero = f_arg_[0](j, ell) != scalar(0.0);
					bool var      = vx[index];
					if( not_zero | var )
					{	// result information
						index = ell * nr_ + j;
						vy[index] = true;
					}
				}
			}
		}
		return true;
	}
/* %$$
$subhead reverse$$
$srccode%cpp% */
	// reverse mode routine called by CppAD
	virtual bool reverse(
		// highest order Taylor coefficient that we are computing derivative of
		size_t                     q ,
		// forward mode Taylor coefficients for x variables
		const CppAD::vector<double>&     tx ,
		// forward mode Taylor coefficients for y variables
		const CppAD::vector<double>&     ty ,
		// upon return, derivative of G[ F[ {x_j^k} ] ] w.r.t {x_j^k}
		CppAD::vector<double>&           px ,
		// derivative of G[ {y_i^k} ] w.r.t. {y_i^k}
		const CppAD::vector<double>&     py
	)
	{	size_t n_order = q + 1;
		assert( nx_           == nr_ * nr_ );
		assert( nx_ * n_order == tx.size() );
		assert( nx_ * n_order == ty.size() );
		assert( px.size()     == tx.size() );
		assert( py.size()     == ty.size() );
		//
		// -------------------------------------------------------------------
		// make sure f_arg_ is large enough
		assert( f_arg_.size() == f_result_.size() );
		// must have previous run forward with order >= n_order
		assert( f_arg_.size() >= n_order );
		// -------------------------------------------------------------------
		// make sure r_arg_, r_result_ are large enough
		assert( r_arg_.size() == r_result_.size() );
		if( r_arg_.size() < n_order )
		{	r_arg_.resize(n_order);
			r_result_.resize(n_order);
			//
			for(size_t k = 0; k < n_order; k++)
			{	r_arg_[k].resize(nr_, nr_);
				r_result_[k].resize(nr_, nr_);
			}
		}
		// -------------------------------------------------------------------
		// unpack tx into f_arg_
		for(size_t k = 0; k < n_order; k++)
		{	// unpack arg values for this order
			for(size_t i = 0; i < nx_; i++)
				f_arg_[k].data()[i] = tx[ i * n_order + k ];
		}
		// -------------------------------------------------------------------
		// unpack py into r_result_
		for(size_t k = 0; k < n_order; k++)
		{	for(size_t i = 0; i < nx_; i++)
				r_result_[k].data()[i] = py[ i * n_order + k ];
		}
		// -------------------------------------------------------------------
		// initialize r_arg_ as zero
		for(size_t k = 0; k < n_order; k++)
			r_arg_[k]   = matrix::Zero(nr_, nr_);
		// -------------------------------------------------------------------
		// matrix reverse mode calculation
		//
		for(size_t k1 = n_order; k1 > 1; k1--)
		{	size_t k = k1 - 1;
			for(size_t ell = 1; ell <= k; ell++)
			{	r_arg_[ell]    -=
					f_result_[0] * r_result_[k] * f_result_[k-ell].transpose();
				//
				r_result_[k-ell] -=
					f_result_[0] * f_arg_[ell].transpose() * r_result_[k];
			}
			r_result_[0] +=
			r_result_[k] * f_result_[k].transpose() * f_arg_[0].transpose();
		}
		r_arg_[0] -=
		f_result_[0].transpose() * r_result_[0] * f_result_[0].transpose();
		// -------------------------------------------------------------------
		// pack r_arg into px
		for(size_t k = 0; k < n_order; k++)
		{	for(size_t i = 0; i < nx_; i++)
				px[ i * n_order + k ] = r_arg_[k].data()[i];
		}
		//
		return true;
	}
/* %$$
$head End Class Definition$$
$srccode%cpp% */
}; // End of atomic_eigen_mat_inv class

}  // END_EMPTY_NAMESPACE
/* %$$
$$ $comment end nospell$$
$end
*/


# endif
