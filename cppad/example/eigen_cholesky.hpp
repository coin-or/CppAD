// $Id$
# ifndef CPPAD_EXAMPLE_EIGEN_CHOLESKY_HPP
# define CPPAD_EXAMPLE_EIGEN_CHOLESKY_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_eigen_cholesky.hpp$$
$spell
	Eigen
	Taylor
	Cholesky
	op
$$

$section Atomic Eigen Cholesky Factorization Class$$

$head Under Construction$$

$head Purpose$$
Construct an atomic operation that computes a lower triangular matrix
$latex L $$ such that $latex L L^\R{T} = A$$
for any positive integer $latex p$$
and symmetric positive definite matrix $latex A \in \B{R}^{p \times p}$$.

$head Start Class Definition$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <Eigen/Dense>


/* %$$
$head Public$$

$subhead Types$$
$srccode%cpp% */
namespace { // BEGIN_EMPTY_NAMESPACE

template <class Base>
class atomic_eigen_cholesky : public CppAD::atomic_base<Base> {
public:
	// -----------------------------------------------------------
	// type of elements during calculation of derivatives
	typedef Base              scalar;
	// type of elements during taping
	typedef CppAD::AD<scalar> ad_scalar;
	//
	// type of matrix during calculation of derivatives
	typedef Eigen::Matrix<
		scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>        matrix;
	// type of matrix during taping
	typedef Eigen::Matrix<
		ad_scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor > ad_matrix;
	//
	// lower triangular scalar matrix
	typedef Eigen::TriangularView<matrix, Eigen::Lower>             lower_view;
/* %$$
$subhead Constructor$$
$srccode%cpp% */
	// constructor
	atomic_eigen_cholesky(void) : CppAD::atomic_base<Base>(
		"atom_eigen_cholesky"                             ,
		CppAD::atomic_base<Base>::set_sparsity_enum
	)
	{ }
/* %$$
$subhead op$$
$srccode%cpp% */
	// use atomic operation to invert an AD matrix
	ad_matrix op(const ad_matrix& arg)
	{	size_t nr = size_t( arg.rows() );
		size_t ny = ( (nr + 1 ) * nr ) / 2;
		size_t nx = 1 + ny;
		assert( nr == size_t( arg.cols() ) );
		// -------------------------------------------------------------------
		// packed version of arg
		CPPAD_TESTVECTOR(ad_scalar) packed_arg(nx);
		size_t index = 0;
		packed_arg[index++] = ad_scalar( nr );
		// lower triangle of symmetric matrix A
		for(size_t i = 0; i < nr; i++)
		{	for(size_t j = 0; j <= i; j++)
				packed_arg[index++] = arg(i, j);
		}
		assert( index == nx );
		// -------------------------------------------------------------------
		// packed version of result = arg^{-1}.
		// This is an atomic_base function call that CppAD uses to
		// store the atomic operation on the tape.
		CPPAD_TESTVECTOR(ad_scalar) packed_result(ny);
		(*this)(packed_arg, packed_result);
		// -------------------------------------------------------------------
		// unpack result matrix L
		ad_matrix result = ad_matrix::Zero(nr, nr);
		index = 0;
		for(size_t i = 0; i < nr; i++)
		{	for(size_t j = 0; j <= i; j++)
				result(i, j) = packed_result[index++];
		}
		return result;
	}
	/* %$$
$head Private$$

$subhead Variables$$
$srccode%cpp% */
private:
	// -------------------------------------------------------------
	// one forward mode vector of matrices for argument and result
	CppAD::vector<matrix> f_arg_, f_result_;
	// one reverse mode vector of matrices for argument and result
	CppAD::vector<matrix> r_arg_, r_result_;
	// -------------------------------------------------------------
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
		size_t nr      = size_t( CppAD::Integer( tx[ 0 * n_order + 0 ] ) );
		size_t ny      = ((nr + 1) * nr) / 2;
		size_t nx      = 1 + ny;
		assert( vx.size() == 0 || nx == vx.size() );
		assert( vx.size() == 0 || ny == vy.size() );
		assert( nx * n_order == tx.size() );
		assert( ny * n_order == ty.size() );
		//
		// -------------------------------------------------------------------
		// make sure f_arg_ and f_result_ are large enough
		assert( f_arg_.size() == f_result_.size() );
		if( f_arg_.size() < n_order )
		{	f_arg_.resize(n_order);
			f_result_.resize(n_order);
			//
			for(size_t k = 0; k < n_order; k++)
			{	f_arg_[k].resize(nr, nr);
				f_result_[k].resize(nr, nr);
			}
		}
		// -------------------------------------------------------------------
		// unpack tx into f_arg_
		for(size_t k = 0; k < n_order; k++)
		{	size_t index = 1;
			// unpack arg values for this order
			for(size_t i = 0; i < nr; i++)
			{	for(size_t j = 0; j <= i; j++)
				{	f_arg_[k](i, j) = tx[ index * n_order + k ];
					f_arg_[k](j, i) = f_arg_[k](i, j);
					index++;
				}
			}
		}
		// -------------------------------------------------------------------
		// result for each order
		// (we could avoid recalculting f_result_[k] for k=0,...,p-1)
		//
		Eigen::LLT<matrix> cholesky(f_arg_[0]);
		f_result_[0]   = cholesky.matrixL();
		lower_view L_0 = f_result_[0].template triangularView<Eigen::Lower>();
		for(size_t k = 1; k < n_order; k++)
		{	// initialize sum as A_k
			matrix f_sum = f_arg_[k];
			// compute A_k - B_k
			for(size_t ell = 1; ell < k; ell++)
				f_sum -= f_result_[ell] * f_result_[k-ell].transpose();
			// compute L_0^{-1} * (A_k - B_k) * L_0^{-T}
			matrix temp = L_0.template solve<Eigen::OnTheLeft>(f_sum);
			temp   = L_0.transpose().template solve<Eigen::OnTheRight>(temp);
			// divide the diagonal by 2
			for(size_t i = 0; i < nr; i++)
				temp(i, i) /= scalar(2.0);
			// L_k = L_0 * low[ L_0^{-1} * (A_k - B_k) * L_0^{-T} ]
			lower_view view = temp.template triangularView<Eigen::Lower>();
			f_result_[k] = f_result_[0] * view;
		}
		// -------------------------------------------------------------------
		// pack result_ into ty
		for(size_t k = 0; k < n_order; k++)
		{	size_t index = 0;
			for(size_t i = 0; i < nr; i++)
			{	for(size_t j = 0; j <= i; j++)
				{	ty[ index * n_order + k ] = f_result_[k](i, j);
					index++;
				}
			}
		}
		// -------------------------------------------------------------------
		// check if we are computing vy
		if( vx.size() == 0 )
			return true;
		// ------------------------------------------------------------------
		// This is a very dumb algorithm that over estimates which
		// elements of the inverse are variables (which is not efficient).
		bool var = false;
		for(size_t i = 0; i < ny; i++)
			var |= vx[1 + i];
		for(size_t i = 0; i < ny; i++)
			vy[i] = var;
		//
		return true;
	}
/* %$$
$head End Class Definition$$
$srccode%cpp% */
}; // End of atomic_eigen_cholesky class

}  // END_EMPTY_NAMESPACE
/* %$$
$end
*/


# endif
