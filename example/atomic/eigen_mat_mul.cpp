// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_eigen_mat_mul.cpp$$
$spell
	mul
	Eigen
$$

$section  Atomic Eigen Matrix Multiply: Example and Test$$

$head Under Construction$$
This example is under construction. So far only forward mode has been
implemented.

$children%
	cppad/example/eigen_mat_mul.hpp
%$$

$head Class Definition$$
This example uses the file $cref atomic_eigen_mat_mul.hpp$$
which defines matrix multiply as a $cref atomic_base$$ operation.

$nospell

$head Use Atomic Function$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/example/eigen_mat_mul.hpp>

namespace {
	typedef double            scalar;
	typedef CppAD::AD<scalar> ad_scalar;
	typedef typename atomic_eigen_mat_mul<scalar>::matrix     matrix;
	typedef typename atomic_eigen_mat_mul<scalar>::ad_matrix  ad_matrix;

	bool near_equal(const matrix& x, const matrix& y)
	{	scalar eps = 10. * std::numeric_limits<scalar>::epsilon();
		assert( x.rows() == y.rows() );
		assert( x.cols() == y.cols() );
		bool ok = true;
		for(size_t i = 0; i < size_t(x.rows()); i++)
		{	for(size_t j = 0; j < size_t(x.cols()); j++)
			{	ok &= CppAD::NearEqual( x(i, j), y(i, j), eps, eps );
			}
		}
		return ok;
	}
}

bool eigen_mat_mul(void)
{	bool ok = true;
	//
/* %$$
$subhead Constructor$$
$srccode%cpp% */
	// -------------------------------------------------------------------
	// object that multiplies  at 3x2 matrix times a 2x1 matrix
	size_t nrow_left  = 3;
	size_t n_middle   = 2;
	size_t ncol_right = 1;
	atomic_eigen_mat_mul<scalar> mat_mul(nrow_left, n_middle, ncol_right);
	//
	// left matrix
	matrix    left(nrow_left, n_middle);
	ad_matrix ad_left(nrow_left, n_middle);
	for(size_t i = 0; i < nrow_left; i++)
	{	for(size_t j = 0; j < n_middle; j++)
		{	// any value will do here
			ad_left(i, j) = left(i, j) = scalar( i * n_middle + j );
		}
	}
	//
	// right matrix
	matrix    right(n_middle, ncol_right);
	ad_matrix ad_right(n_middle, ncol_right);
	for(size_t i = 0; i < n_middle; i++)
	{	for(size_t j = 0; j < ncol_right; j++)
		{	// any value will do here
			ad_right(i, j) = right(i, j) = scalar( i * ncol_right + j );
		}
	}
	//
	// number of entries in left and right matrices
	size_t nx = (nrow_left + ncol_right) * n_middle;
	//
	// number of entires in the result matrix
	size_t ny = nrow_left * ncol_right;
	//
	// packed version of left and right matrix
	CPPAD_TESTVECTOR(ad_scalar) ad_x(nx);
	mat_mul.pack(ad_x, ad_left, ad_right);
	//
	// declare packed version of (left,right) to be the independent variables
	CppAD::Independent(ad_x);
	//
	// multiply the matrices
	CPPAD_TESTVECTOR(ad_scalar) ad_y(ny);
	mat_mul(ad_x, ad_y);
	//
	// declare packed version of left * right to be dependent variables
	CppAD::ADFun<scalar> f(ad_x, ad_y);
	//
	// compute zero order forward mode
	CPPAD_TESTVECTOR(scalar) x(nx), y(ny);
	mat_mul.pack(x, left, right);
	y = f.Forward(0, x);
	matrix result(nrow_left, ncol_right);
	mat_mul.unpack(y, result);
	//
	// check zero order forward mode
	matrix check = left * right;
	ok &= near_equal(result, check);
	//
	return ok;
}
/* %$$
$$ $comment end nospell$$
$end
*/
