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

$head Purpose$$
For fixed positive integers $latex r$$, $latex m$$, $latex c$$,
construct and atomic operation that computes the matrix product
$latex L \times R$$ for any $latex L \in \B{R}^{r \times m}$$ and
$latex R \in \B{R}^{m \times c}$$.


$nospell

$head Start Class Definition$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <Eigen/Core>


/* %$$
$head Public$$

$subhead Types$$
$srccode%cpp% */
namespace { // BEGIN_EMPTY_NAMESPACE

template <class Base>
class atomic_eigen_mat_mul : public CppAD::atomic_base<Base> {
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
	atomic_eigen_mat_mul(
		// number of rows in left operand
		const size_t nr_left  ,
		// number of rows in left and columns in right operand
		const size_t n_middle   ,
		// number of columns in right operand
		const size_t nc_right
	) :
	CppAD::atomic_base<Base>(
		"atom_eigen_mat_mul"                             ,
		CppAD::atomic_base<Base>::set_sparsity_enum
	) ,
	nr_left_(  nr_left  )                   ,
	n_middle_(   n_middle   )                   ,
	nc_right_( nc_right )                   ,
	nx_( (nr_left + nc_right) * n_middle_ ) ,
	ny_( nr_left * nc_right )
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
		assert( cols( left ) == n_middle_ );
		assert( rows( right ) == n_middle_ );
		assert( cols( right ) == nc_right_ );
		//
		size_t n_left = nr_left_ * n_middle_;
		size_t n_right = n_middle_ * nc_right_;
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
	// number of of columns in left operand and rows in right operand
	const size_t n_middle_;
	// number of columns in right operand and result
	const size_t nc_right_;
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
		size_t n_left   = nr_left_ * n_middle_;
		size_t n_right  = n_middle_ * nc_right_;
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
			{	f_left_[k].resize(nr_left_, n_middle_);
				f_right_[k].resize(n_middle_, nc_right_);
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
		for(size_t k = 0; k < n_order; k++)
		{	// result[k] = sum_ell left[ell] * right[k-ell]
			f_result_[k] = matrix::Zero(nr_left_, nc_right_);
			for(size_t ell = 0; ell <= k; ell++)
				f_result_[k] += f_left_[ell] * f_right_[k-ell];
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
		for(size_t i = 0; i < nr_left_; i++)
		{	for(size_t j = 0; j < nc_right_; j++)
			{	bool var = false;
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	// left information
					size_t index   = i * n_middle_ + ell;
					bool var_left  = vx[index];
					bool nz_left   = var_left | (f_left_[0](i, ell) != zero);
					// right information
					index          = n_left + ell * nc_right_ + j;
					bool var_right = vx[index];
					bool nz_right  = var_right | (f_right_[0](ell, j) != zero);
					// effect of result
					var |= var_left & nz_right;
					var |= nz_left  & var_right;
				}
				size_t index = i * nc_right_ + j;
				vy[index]    = var;
			}
		}
		return true;
	}
/* %$$
$subhead reverse$$
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
		//
		size_t n_left   = nr_left_ * n_middle_;
		size_t n_right  = n_middle_ * nc_right_;
		size_t n_result = nr_left_ * nc_right_;
		assert( n_left + n_right == nx_ );
		assert( n_result == ny_ );
		// -------------------------------------------------------------------
		// make sure f_left_, f_right_ are large enough
		assert( f_left_.size() == f_right_.size() );
		assert( f_left_.size() == f_result_.size() );
		// must have previous run forward with order >= n_order
		assert( f_left_.size() >= n_order );
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
			{	r_left_[k].resize(nr_left_, n_middle_);
				r_right_[k].resize(n_middle_, nc_right_);
				r_result_[k].resize(nr_left_, nc_right_);
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
		// unpack result_ from py
		for(size_t k = 0; k < n_order; k++)
		{	for(size_t i = 0; i < n_result; i++)
				r_result_[k].data()[i] = py[ i * n_order + k ];
		}
		// -------------------------------------------------------------------
		// initialize r_left_ and r_right_ as zero
		for(size_t k = 0; k < n_order; k++)
		{	r_left_[k]   = matrix::Zero(nr_left_, n_middle_);
			r_right_[k]  = matrix::Zero(n_middle_, nc_right_);
		}
		// -------------------------------------------------------------------
		// matrix reverse mode calculation
		for(size_t k1 = n_order; k1 > 0; k1--)
		{	size_t k = k1 - 1;
			for(size_t ell = 0; ell <= k; ell++)
			{	// nr x nm       = nr x nc      * nc * nm
				r_left_[ell]    += r_result_[k] * f_right_[k-ell].transpose();
				// nm x nc       = nm x nr * nr * nc
				r_right_[k-ell] += f_left_[ell].transpose() * r_result_[k];
			}
		}
		// -------------------------------------------------------------------
		// pack r_left and r_right int px
		for(size_t k = 0; k < n_order; k++)
		{	// pack left values for this order
			for(size_t i = 0; i < n_left; i++)
				px[ i * n_order + k ] = r_left_[k].data()[i];
			//
			// pack right values for this order
			for(size_t i = 0; i < n_right; i++)
				px[ (i + n_left) * n_order + k] = r_right_[k].data()[i];
		}
		//
		return true;
	}
/* %$$
$subhead for_sparse_jac$$
$srccode%cpp% */
	// forward Jacobian sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		// number of columns in the matrix R
		size_t                                       q ,
		// sparsity pattern for the matrix R
		const CppAD::vector< std::set<size_t> >&     r ,
		// sparsity pattern for the matrix S = f'(x) * R
		CppAD::vector< std::set<size_t> >&           s )
	{	assert( nx_ == r.size() );
		assert( ny_ == s.size() );
		//
		size_t n_left = nr_left_ * n_middle_;
		for(size_t i = 0; i < nr_left_; i++)
		{	for(size_t j = 0; j < nc_right_; j++)
			{	// pack index for entry (i, j) in result
				size_t i_result = i * nc_right_ + j;
				s[i_result].clear();
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	// pack index for entry (i, ell) in left
					size_t i_left  = i * n_middle_ + ell;
					// pack index for entry (ell, j) in right
					size_t i_right = n_left + ell * nc_right_ + j;
					//
					s[i_result] = CppAD::set_union(s[i_result], r[i_left] );
					s[i_result] = CppAD::set_union(s[i_result], r[i_right] );
				}
			}
		}
		return true;
	}
/* %$$
$subhead rev_sparse_jac$$
$srccode%cpp% */
	// reverse Jacobian sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		// number of columns in the matrix R^T
		size_t                                      q  ,
		// sparsity pattern for the matrix R^T
		const CppAD::vector< std::set<size_t> >&    rt ,
		// sparsoity pattern for the matrix S^T = f'(x)^T * R^T
		CppAD::vector< std::set<size_t> >&          st )
	{	assert( nx_ == st.size() );
		assert( ny_ == rt.size() );

		// initialize S^T as empty
		for(size_t i = 0; i < nx_; i++)
			st[i].clear();

		// sparsity for S(x)^T = f'(x)^T * R^T
		size_t n_left = nr_left_ * n_middle_;
		for(size_t i = 0; i < nr_left_; i++)
		{	for(size_t j = 0; j < nc_right_; j++)
			{	// pack index for entry (i, j) in result
				size_t i_result = i * nc_right_ + j;
				st[i_result].clear();
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	// pack index for entry (i, ell) in left
					size_t i_left  = i * n_middle_ + ell;
					// pack index for entry (ell, j) in right
					size_t i_right = n_left + ell * nc_right_ + j;
					//
					st[i_left]  = CppAD::set_union(st[i_left],  rt[i_result]);
					st[i_right] = CppAD::set_union(st[i_right], rt[i_result]);
				}
			}
		}
		return true;
	}
/* %$$
$subhead for_sparse_hes$$
$srccode%cpp% */
	virtual bool for_sparse_hes(
		// which components of x are variables for this call
		const CppAD::vector<bool>&                   vx,
		// sparsity pattern for the diagonal of R
		const CppAD::vector<bool>&                   r ,
		// sparsity pattern for the vector S
		const CppAD::vector<bool>&                   s ,
		// sparsity patternfor the Hessian H(x)
		CppAD::vector< std::set<size_t> >&           h )
	{	assert( vx.size() == nx_ );
		assert( r.size()  == nx_ );
		assert( s.size()  == ny_ );
		assert( h.size()  == nx_ );
		//
		// initilize h as empty
		for(size_t i = 0; i < nx_; i++)
			h[i].clear();
		//
		size_t n_left = nr_left_ * n_middle_;
		for(size_t i = 0; i < nr_left_; i++)
		{	for(size_t j = 0; j < nc_right_; j++)
			{	// pack index for entry (i, j) in result
				size_t i_result = i * nc_right_ + j;
				if( s[i_result] )
				{	for(size_t ell = 0; ell < n_middle_; ell++)
					{	// pack index for entry (i, ell) in left
						size_t i_left  = i * n_middle_ + ell;
						// pack index for entry (ell, j) in right
						size_t i_right = n_left + ell * nc_right_ + j;
						if( r[i_left] & r[i_right] )
						{	h[i_left].insert(i_right);
							h[i_right].insert(i_left);
						}
					}
				}
			}
		}
		return true;
	}
/* %$$
$subhead rev_sparse_hes$$
$srccode%cpp% */
	// reverse Hessian sparsity routine called by CppAD
	virtual bool rev_sparse_hes(
		// which components of x are variables for this call
		const CppAD::vector<bool>&                   vx,
		// sparsity pattern for S(x) = g'[f(x)]
		const CppAD::vector<bool>&                   s ,
		// sparsity pattern for d/dx g[f(x)] = S(x) * f'(x)
		CppAD::vector<bool>&                         t ,
		// number of columns in R, U(x), and V(x)
		size_t                                       q ,
		// sparsity pattern for R
		const CppAD::vector< std::set<size_t> >&     r ,
		// sparsity pattern for U(x) = g^{(2)} [ f(x) ] * f'(x) * R
		const CppAD::vector< std::set<size_t> >&     u ,
		// sparsity pattern for
		// V(x) = f'(x)^T * U(x) + sum_{i=0}^{m-1} S_i(x) f_i^{(2)} (x) * R
		CppAD::vector< std::set<size_t> >&           v )
	{	assert( vx.size() == nx_ );
		assert( s.size()  == ny_ );
		assert( t.size()  == nx_ );
		assert( r.size()  == nx_ );
		assert( v.size()  == nx_ );
		//
		// initilaize return sparsity patterns as false
		for(size_t j = 0; j < nx_; j++)
		{	t[j] = false;
			v[j].clear();
		}
		//
		size_t n_left = nr_left_ * n_middle_;
		for(size_t i = 0; i < nr_left_; i++)
		{	for(size_t j = 0; j < nc_right_; j++)
			{	// pack index for entry (i, j) in result
				size_t i_result = i * nc_right_ + j;
				for(size_t ell = 0; ell < n_middle_; ell++)
				{	// pack index for entry (i, ell) in left
					size_t i_left  = i * n_middle_ + ell;
					// pack index for entry (ell, j) in right
					size_t i_right = n_left + ell * nc_right_ + j;
					//
					// back propagate T(x) = S(x) * f'(x).
					t[i_left]  |= bool( s[i_result] );
					t[i_right] |= bool( s[i_result] );
					//
					// V(x) = f'(x)^T * U(x) +  sum_i S_i(x) * f_i''(x) * R
					// U(x)   = g''[ f(x) ] * f'(x) * R
					// S_i(x) = g_i'[ f(x) ]
					//
					// back propagate f'(x)^T * U(x)
					v[i_left]  = CppAD::set_union(v[i_left],  u[i_result] );
					v[i_right] = CppAD::set_union(v[i_right], u[i_result] );
					//
					// back propagate S_i(x) * f_i''(x) * R
					// (here is where we use vx to check for cross terms)
					if( s[i_result] & vx[i_left] & vx[i_right] )
					{	v[i_left]  = CppAD::set_union(v[i_left],  r[i_right] );
						v[i_right] = CppAD::set_union(v[i_right], r[i_left]  );
					}
				}
			}
		}
		return true;
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
