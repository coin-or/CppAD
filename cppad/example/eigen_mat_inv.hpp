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
$$

$section Atomic Eigen Matrix Divide Class$$

$head Purpose$$
For fixed positive integers $latex r$$, $latex c$$,
construct and atomic operation that solve the matrix equation
$latex L \times X = R$$ for $latex X \in \B{R}^{r \times c}$$
for any $latex L \in \B{R}^{r \times r}$$ and
$latex R \in \B{R}^{r \times c}$$.

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
	CppAD::vector<matrix> r_arg, r_result_;
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
			for(size_t ell = 0; ell < k; ell++)
				f_sum_ -= f_arg_[k-ell] * f_result_[ell];
			// solve arg_[0] * result_[k] = sum
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
		{	for(size_t i = 0; i < nr_; i++)
			{	// initialize vy as false
				size_t index = i * nr_ + j;
				vy[index]    = false;
			}
			for(size_t i = 0; i < nr_; i++)
			{	for(size_t ell = 0; ell < nr_; ell++)
				{	// arg information
					size_t index   = i * nr_ + ell;
					bool var_arg  = vx[index];
					// identity information
					bool nz_eye   = ell == j;
					// result information
					index = i * nr_ + j;
					vy[index] |= var_arg & nz_eye;
				}
			}
		}
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
