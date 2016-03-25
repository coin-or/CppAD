// $Id$
# ifndef CPPAD_EXAMPLE_EIGEN_MAT_DIV_HPP
# define CPPAD_EXAMPLE_EIGEN_MAT_DIV_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_eigen_mat_div.hpp$$
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
class atomic_eigen_mat_div : public CppAD::atomic_base<Base> {
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
	atomic_eigen_mat_div(
		// number of rows in left operand
		const size_t nr_left  ,
		// number of columns in right operand
		const size_t nc_right
	) :
	CppAD::atomic_base<Base>(
		"atom_eigen_mat_div"                             ,
		CppAD::atomic_base<Base>::set_sparsity_enum
	) ,
	nr_left_(  nr_left  )                   ,
	nc_right_( nc_right )                   ,
	nx_( (nr_left + nc_right) * nr_left )   ,
	ny_( nr_left * nc_right )               ,
	f_sum_(nr_left, nc_right)
	{ }
/* %$$
$subhead Pack$$
$srccode%cpp% */
	template <class Matrix, class Vector>
	void pack(
		Vector&        packed  ,
		const Matrix&  left    ,
		const Matrix&  right   )
	{	assert( packed.size() == nx_      );
		assert( rows( left ) == nr_left_ );
		assert( cols( left ) == nr_left_ );
		assert( rows( right ) == nr_left_ );
		assert( cols( right ) == nc_right_ );
		//
		size_t n_left = nr_left_ * nr_left_;
		size_t n_right = nr_left_ * nc_right_;
		assert( n_left + n_right == nx_ );
		//
		for(size_t i = 0; i < n_left; i++)
			packed[i] = left.data()[i];
		for(size_t i = 0; i < n_right; i++)
			packed[ i + n_left ] = right.data()[i];
		//
		return;
	}
/* %$$
$subhead Unpack$$
$srccode%cpp% */
	template <class Matrix, class Vector>
	void unpack(
		const Vector&   packed  ,
		Matrix&         result  )
	{	assert( packed.size() == ny_      );
		assert( rows( result ) == nr_left_ );
		assert( cols( result ) == nc_right_ );
		//
		size_t n_result = nr_left_ * nc_right_;
		assert( n_result == ny_ );
		//
		for(size_t i = 0; i < n_result; i++)
			result.data()[i] = packed[ i ];
		//
		return;
	}
/* %$$
$head Private$$

$subhead Variables$$
$srccode%cpp% */
private:
	// -------------------------------------------------------------
	// number of of rows in left operand and result
	const size_t nr_left_;
	// number of columns in right operand and result
	const size_t nc_right_;
	// size of the domain space
	const size_t nx_;
	// size of the range space
	const size_t ny_;
	// -------------------------------------------------------------
	// one forward mode vector of matrices for left, right, and result
	CppAD::vector<matrix> f_left_, f_right_, f_result_;
	// matrix used for forward mode summation
	matrix f_sum_;
	// one reverse mode vector of matrices for left, right, and result
	CppAD::vector<matrix> r_left_, r_right_, r_result_;
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
		assert( vx.size() == 0 || ny_ == vy.size() );
		assert( nx_ * n_order == tx.size() );
		assert( ny_ * n_order == ty.size() );
		//
		size_t n_left   = nr_left_ * nr_left_;
		size_t n_right  = nr_left_ * nc_right_;
		size_t n_result = nr_left_ * nc_right_;
		assert( n_left + n_right == nx_ );
		assert( n_result == ny_ );
		//
		// -------------------------------------------------------------------
		// make sure f_left_, f_right_, and f_result_ are large enough
		assert( f_left_.size() == f_right_.size() );
		assert( f_left_.size() == f_result_.size() );
		if( f_left_.size() < n_order )
		{	f_left_.resize(n_order);
			f_right_.resize(n_order);
			f_result_.resize(n_order);
			//
			for(size_t k = 0; k < n_order; k++)
			{	f_left_[k].resize(nr_left_, nr_left_);
				f_right_[k].resize(nr_left_, nc_right_);
				f_result_[k].resize(nr_left_, nc_right_);
			}
		}
		// -------------------------------------------------------------------
		// unpack tx into f_left and f_right
		for(size_t k = 0; k < n_order; k++)
		{	// unpack left values for this order
			for(size_t i = 0; i < n_left; i++)
				f_left_[k].data()[i] = tx[ i * n_order + k ];
			//
			// unpack right values for this order
			for(size_t i = 0; i < n_right; i++)
				f_right_[k].data()[i] = tx[ (i + n_left) * n_order + k ];
		}
		// -------------------------------------------------------------------
		// result for each order
		//
		// compute LU factorixation of left_[0]
		Eigen::FullPivLU<matrix> lu_left( f_left_[0] );
		for(size_t k = 0; k < n_order; k++)
		{	// initialize sum
			f_sum_ = f_right_[k];
			// compute sum
			for(size_t ell = 0; ell < k; ell++)
				f_sum_ -= f_left_[k-ell] * f_result_[ell];
			// solve left_[0] * result_[k] = sum
			f_result_[k] = lu_left.solve(f_sum_);
		}
		// -------------------------------------------------------------------
		// pack result_ into ty
		for(size_t k = 0; k < n_order; k++)
		{	for(size_t i = 0; i < n_result; i++)
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
		for(size_t j = 0; j < nc_right_; j++)
		{	for(size_t i = 0; i < nr_left_; i++)
			{	// initialize vy as false
				size_t index = i * nc_right_ + j;
				vy[index]    = false;
			}
			for(size_t i = 0; i < nr_left_; i++)
			{	for(size_t ell = 0; ell < nr_left_; ell++)
				{	// left information
					size_t index   = i * nr_left_ + ell;
					bool var_left  = vx[index];
					bool nz_left   = var_left | (f_left_[0](i, ell) != zero);
					// right information
					index          = n_left + ell * nc_right_ + j;
					bool var_right = vx[index];
					bool nz_right  = var_right | (f_right_[0](ell, j) != zero);
					// result information
					index = i * nc_right_ + j;
					vy[index] |= var_left & nz_right;
					vy[index] |= nz_left  & var_right;
				}
			}
		}
		return true;
	}
/* %$$
$head End Class Definition$$
$srccode%cpp% */
}; // End of atomic_eigen_mat_div class

}  // END_EMPTY_NAMESPACE
/* %$$
$$ $comment end nospell$$
$end
*/


# endif
