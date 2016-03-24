// $Id$
# ifndef CPPAD_EXAMPLE_EIGEN_MAT_MUL_HPP
# define CPPAD_EXAMPLE_EIGEN_MAT_MUL_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_eigen_mat_mul.hpp$$
$spell
	Eigen
$$

$section Atomic Eigen Matrix Multiply Class$$

$nospell

$head Start Class Definition$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <Eigen/Core>


/* %$$
$head Publice Types$$
$srccode%cpp% */
namespace { // BEGIN_EMPTY_NAMESPACE
//
template <class Base>
class atomic_eigen_mat_mul : public CppAD::atomic_base<Base> {
public:
	// -----------------------------------------------------------
	// type of elements during calculation of derivatives
	typedef Base              scalar;
	// type of elements during taping
	typedef CppAD::AD<scalar> ad_scalar;
	// type of matrix during calculation of derivatives
	typedef Eigen::Matrix<scalar, Eigen::Dynamic, Eigen::Dynamic>    matrix;
	// type of matrix during taping
	typedef Eigen::Matrix<ad_scalar, Eigen::Dynamic, Eigen::Dynamic> ad_matrix;
/* %$$
$head Public Constructor$$
$srccode%cpp% */
	// constructor
	atomic_eigen_mat_mul(
		// number of rows in left operand
		const size_t nrow_left  ,
		// number of rows in left and columns in right operand
		const size_t n_middle   ,
		// number of columns in right operand
		const size_t ncol_right
	) :
	CppAD::atomic_base<Base>(
		"atom_eigen_mat_mul"                             ,
		CppAD::atomic_base<Base>::set_sparsity_enum
	) ,
	nrow_left_(  nrow_left  )                   ,
	n_middle_(   n_middle   )                   ,
	ncol_right_( ncol_right )                   ,
	nx_( (nrow_left + ncol_right) * n_middle_ ) ,
	ny_( nrow_left * ncol_right )
	{ }
/* %$$
$head Public Pack$$
$srccode%cpp% */
	template <class Matrix, class Vector>
	void pack(
		Vector&        packed  ,
		const Matrix&  left    ,
		const Matrix&  right   )
	{	assert( packed.size() == nx_      );
		assert( rows( left ) == nrow_left_ );
		assert( cols( left ) == n_middle_ );
		assert( rows( right ) == n_middle_ );
		assert( cols( right ) == ncol_right_ );
		//
		size_t index = 0;
		for(size_t i = 0; i < nrow_left_; i++)
		{	for(size_t j = 0; j < n_middle_; j++)
			{	packed[ index ] = left(i, j);
				++index;
			}
		}
		for(size_t i = 0; i < n_middle_; i++)
		{	for(size_t j = 0; j < ncol_right_; j++)
			{	packed[ index ] = right(i, j);
				++index;
			}
		}
		assert( index == nx_ );
		return;
	}
/* %$$
$head Public Unpack$$
$srccode%cpp% */
	template <class Matrix, class Vector>
	void unpack(
		const Vector&   packed  ,
		Matrix&         result  )
	{	assert( packed.size() == ny_      );
		assert( rows( result ) == nrow_left_ );
		assert( cols( result ) == ncol_right_ );
		//
		size_t index = 0;
		for(size_t i = 0; i < nrow_left_; i++)
		{	for(size_t j = 0; j < ncol_right_; j++)
			{	result(i, j) = packed[ index ];
				++index;
			}
		}
		assert( index == ny_ );
		return;
	}
/* %$$
$head Private Variables$$
$srccode%cpp% */
private:
	// -------------------------------------------------------------
	// number of of rows in left operand and result
	const size_t nrow_left_;
	// number of of columns in left operand and rows in right operand
	const size_t n_middle_;
	// number of columns in right operand and result
	const size_t ncol_right_;
	// size of the domain space
	const size_t nx_;
	// size of the range space
	const size_t ny_;
	// -------------------------------------------------------------
	// one forward mode vector of matrices for left, right, and result
	CppAD::vector<matrix> f_left_, f_right_, f_result_;
	// one reverse mode vector of matrices for left, right, and result
	CppAD::vector<matrix> r_left_, r_right_, r_result_;
	// -------------------------------------------------------------
/* %$$
$head Private rows$$
$srccode%cpp% */
	// convert from int to size_t
	static size_t rows(const matrix& x)
	{	return size_t( x.rows() ); }
	static size_t rows(const ad_matrix& x)
	{	return size_t( x.rows() ); }
/* %$$
$head Private cols$$
$srccode%cpp% */
	// convert from int to size_t
	static size_t cols(const matrix& x)
	{	return size_t( x.cols() ); }
	static size_t cols(const ad_matrix& x)
	{	return size_t( x.cols() ); }
/* %$$
$head Private forward$$
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
			{	f_left_[k].resize(nrow_left_, n_middle_);
				f_right_[k].resize(n_middle_, ncol_right_);
				f_result_[k].resize(nrow_left_, ncol_right_);
			}
		}

		// -------------------------------------------------------------------
		// unpack tx into f_left and f_right
		for(size_t k = 0; k < n_order; k++)
		{	// unpack left values for this order
			assert( rows( f_left_[k] ) == nrow_left_ );
			assert( cols( f_left_[k] ) == n_middle_ );
			size_t index = 0;
			for(size_t i = 0; i < nrow_left_; i++)
			{	for(size_t j = 0; j < n_middle_; j++)
				{	f_left_[k](i, j) = tx[ index * n_order + k ];
					++index;
				}
			}
			// unpack right values for this order
			assert( rows( f_right_[k] ) == n_middle_ );
			assert( cols( f_right_[k] ) == ncol_right_ );
			for(size_t i = 0; i < n_middle_; i++)
			{	for(size_t j = 0; j < ncol_right_; j++)
				{	f_right_[k](i, j) = tx[ index * n_order + k ];
					++index;
				}
			}
			assert( index == nx_ );
		}

		// -------------------------------------------------------------------
		// result for each order
		for(size_t k = 0; k < n_order; k++)
		{	f_result_[k] = matrix::Zero(nrow_left_, ncol_right_);
			for(size_t ell = 0; ell <= k; ell++)
			{	f_result_[k] += f_left_[ell] * f_right_[k-ell];
			}
		}

		// -------------------------------------------------------------------
		// pack result_ into ty
		for(size_t k = 0; k < n_order; k++)
		{	assert( rows( f_result_[k] ) == nrow_left_ );
			assert( cols( f_result_[k] ) == ncol_right_ );
			size_t index = 0;
			for(size_t i = 0; i < nrow_left_; i++)
			{	for(size_t j = 0; j < ncol_right_; j++)
				{	ty[ index * n_order + k ] = f_result_[k](i, j);
					++index;
				}
			}
			assert( index == ny_ );
		}

		// check if we are compute vy
		if( vx.size() == 0 )
			return true;

		// compute variable information for y; i.e., vy
		scalar zero(0.0);
		assert( n_order == 1 );
		for(size_t i = 0; i < nrow_left_; i++)
		{	for(size_t j = 0; j < ncol_right_; j++)
			{	bool var = false;
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	size_t index   = i * n_middle_ + ell;
					bool var_left  = vx[index];
					bool nz_left   = var_left | (f_left_[0](i, ell) != zero);
					index          = nrow_left_ * n_middle_;
					index         += ell * ncol_right_ + j;
					bool var_right = vx[index];
					bool nz_right  = var_right | (f_right_[0](ell, j) != zero);
					var |= var_left & nz_right;
					var |= nz_left  & var_right;
				}
				size_t index = i * ncol_right_ + j;
				vy[index]    = var;
			}
		}
		return true;
	}
/* %$$
$head reverse$$
$srccode%cpp% */
	// reverse mode routine called by CppAD
	virtual bool reverse(
		// highest order Taylor coefficient that we are computing deritive of
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
		assert( nx_ * n_order == tx.size() );
		assert( ny_ * n_order == ty.size() );
		assert( px.size() == tx.size() );
		assert( py.size() == ty.size() );
		// -------------------------------------------------------------------
		// unpack tx into f_left and f_right
		assert( f_left_.size() >= n_order );
		assert( f_right_.size() >= n_order );
		for(size_t k = 0; k < n_order; k++)
		{	// unpack left values for this order
			assert( rows( f_left_[k] ) == nrow_left_ );
			assert( cols( f_left_[k] ) == n_middle_ );
			size_t index = 0;
			for(size_t i = 0; i < nrow_left_; i++)
			{	for(size_t j = 0; j < n_middle_; j++)
				{	f_left_[k](i, j) = tx[ index * n_order + k ];
					++index;
				}
			}
			// unpack right values for this order
			assert( rows( f_right_[k] ) == n_middle_ );
			assert( cols( f_right_[k] ) == ncol_right_ );
			for(size_t i = 0; i < n_middle_; i++)
			{	for(size_t j = 0; j < ncol_right_; j++)
				{	f_right_[k](i, j) = tx[ index * n_order + k ];
					++index;
				}
			}
			assert( index == nx_ );
		}
		// -------------------------------------------------------------------
		// make sure r_left_, r_right_, and r_result_ are large enough
		assert( r_left_.size() == r_right_.size() );
		assert( r_left_.size() == r_result_.size() );
		if( r_left_.size() < n_order )
		{	r_left_.resize(n_order);
			r_right_.resize(n_order);
			r_result_.resize(n_order);
			//
			for(size_t k = 0; k < n_order; k++)
			{	r_left_[k].resize(nrow_left_, n_middle_);
				r_right_[k].resize(n_middle_, ncol_right_);
				r_result_[k].resize(nrow_left_, ncol_right_);
			}
		}
		// -------------------------------------------------------------------
		// unpack result_ from py
		for(size_t k = 0; k < n_order; k++)
		{	assert( rows( r_result_[k] ) == nrow_left_ );
			assert( cols( r_result_[k] ) == ncol_right_ );
			size_t index = 0;
			for(size_t i = 0; i < nrow_left_; i++)
			{	for(size_t j = 0; j < ncol_right_; j++)
				{	r_result_[k](i, j) = py[ index * n_order + k ];
					++index;
				}
			}
			assert( index == ny_ );
		}
		// -------------------------------------------------------------------
		// initialize r_left_ and r_right_ as zero
		for(size_t k = 0; k < n_order; k++)
		{	for(size_t i = 0; i < nrow_left_; i++)
				for(size_t j = 0; j < n_middle_; j++)
					r_left_[k](i, j) = scalar(0.0);
			for(size_t i = 0; i < n_middle_; i++)
				for(size_t j = 0; j < ncol_right_; j++)
					r_right_[k](i, j) = scalar(0.0);
		}
		// -------------------------------------------------------------------
		// matrix reverse mode calculation
		for(size_t k1 = n_order +1; k1 > 0; k1--)
		{	size_t k = k1 -1;
			for(size_t ell = 0; ell <= k; ell++)
			{
				r_left_[ell]    += r_result_[k] * f_right_[k-ell].transpose();
				r_right_[k-ell] += f_left_[ell].transpose() * r_result_[k];
			}
		}
		// not yet implemented
		return false;
	}
/* %$$
$head End Class Definition$$
$srccode%cpp% */
}; // End of atomic_eigen_mat_mul class

}  // END_EMPTY_NAMESPACE
/* %$$
$$ $comment end nospell$$
$end
*/


# endif
