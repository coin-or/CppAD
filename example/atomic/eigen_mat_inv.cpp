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
$begin atomic_eigen_mat_inv.cpp$$
$spell
	mul
	Eigen
$$

$section  Atomic Eigen Matrix Division: Example and Test$$

$head Description$$
The $cref ADFun$$ function object $icode f$$ for this example is
$latex \[
f(x) =
\left( \begin{array}{cc}
	x_0   & 0 \\
	0   & x_1
\end{array} \right)^{-1}
\left( \begin{array}{c}
	0   \\
	x_2
\end{array} \right)
=
\left( \begin{array}{c}
	0 \\
	x_2 / x_1 )
\end{array} \right)
\] $$

$children%
	cppad/example/eigen_mat_inv.hpp
%$$

$head Class Definition$$
This example uses the file $cref atomic_eigen_mat_inv.hpp$$
which defines matrix multiply as a $cref atomic_base$$ operation.

$nospell

$head Use Atomic Function$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/example/eigen_mat_inv.hpp>
# include <cppad/example/eigen_mat_mul.hpp>

namespace {
	typedef double            scalar;
	typedef CppAD::AD<scalar> ad_scalar;
	typedef typename atomic_eigen_mat_inv<scalar>::matrix     matrix;
	typedef typename atomic_eigen_mat_inv<scalar>::ad_matrix  ad_matrix;

	// use atomic operation to multiply two AD matrices
	ad_matrix matrix_multiply(
		atomic_eigen_mat_mul<scalar>& mat_mul ,
		const ad_matrix&              left    ,
		const ad_matrix&              right   )
	{	size_t nr_left   = size_t( left.rows() );
		size_t n_middle    = size_t( left.cols() );
		size_t nc_right  = size_t ( right.cols() );
		assert( size_t( right.rows() ) == n_middle );

		// packed version of left and right
		size_t nx = (nr_left + nc_right) * n_middle;
		CPPAD_TESTVECTOR(ad_scalar) packed_arg(nx);
		mat_mul.pack(packed_arg, left, right);

		// packed version of result = left * right
		size_t ny = nr_left * nc_right;
		CPPAD_TESTVECTOR(ad_scalar) packed_result(ny);
		mat_mul(packed_arg, packed_result);

		// result matrix
		ad_matrix result(nr_left, nc_right);
		mat_mul.unpack(packed_result, result);

		return result;
	}

	// use atomic operation to multiply two AD matrices
	ad_matrix matrix_inverse(
		atomic_eigen_mat_inv<scalar>& mat_inv ,
		const ad_matrix&              arg     )
	{	size_t nr   = size_t( arg.rows() );
		assert( size_t( arg.cols()  ) == nr );

		// packed version of arg
		size_t nx = nr * nr;
		CPPAD_TESTVECTOR(ad_scalar) packed_arg(nx);
		mat_inv.pack(packed_arg, arg);

		// packed version of result = left^{-1}
		CPPAD_TESTVECTOR(ad_scalar) packed_result(nx);
		mat_inv(packed_arg, packed_result);

		// result matrix
		ad_matrix result(nr, nr);
		mat_inv.unpack(packed_result, result);

		return result;
	}

}

bool eigen_mat_inv(void)
{	bool ok    = true;
	scalar eps = 10. * std::numeric_limits<scalar>::epsilon();
	using CppAD::NearEqual;
	//
/* %$$
$subhead Constructor$$
$srccode%cpp% */
	// -------------------------------------------------------------------
	// object that multiplies a 2x2 matrix times a 2x1 matrix
	size_t nr_left  = 2;
	size_t n_middle = 2;
	size_t nc_right = 1;
	atomic_eigen_mat_mul<scalar> mat_mul(nr_left, n_middle, nc_right);
	// -------------------------------------------------------------------
	// object that computes invers of a 2x2 matrix
	size_t nr  = 2;
	atomic_eigen_mat_inv<scalar> mat_inv(nr);
	// -------------------------------------------------------------------
	// declare independent variable vector x
	size_t n = 3;
	CPPAD_TESTVECTOR(ad_scalar) ad_x(n);
	for(size_t j = 0; j < n; j++)
		ad_x[j] = ad_scalar(j);
	CppAD::Independent(ad_x);
	// -------------------------------------------------------------------
	// left = [ x[0]  0    ]
	//        [ 0     x[1] ]
	ad_matrix ad_left(nr_left, nr_left);
	ad_left(0, 0) = ad_x[0];
	ad_left(0, 1) = ad_scalar(0.0);
	ad_left(1, 0) = ad_scalar(0.0);
	ad_left(1, 1) = ad_x[1];
	// -------------------------------------------------------------------
	// right = [ 0 , x[2] ]^T
	ad_matrix ad_right(nr_left, nc_right);
	ad_right(0, 0) = ad_scalar(0.0);
	ad_right(1, 0) = ad_x[2];
	// -------------------------------------------------------------------
	// use atomic operation to compute left^{-1}
	ad_matrix ad_left_inv = matrix_inverse(mat_inv, ad_left);
	// use atomic operation to multiply left^{-1} * right
	ad_matrix ad_result   = matrix_multiply(mat_mul, ad_left_inv, ad_right);
	// -------------------------------------------------------------------
	// check that first component of result is a parameter
	// and the second component is a varaible.
	ok &= Parameter( ad_result(0, 0) );
	ok &= Variable(  ad_result(1, 0) );
	// -------------------------------------------------------------------
	// declare the dependent variable vector y
	size_t m = 2;
	CPPAD_TESTVECTOR(ad_scalar) ad_y(2);
	for(size_t i = 0; i < m; i++)
		ad_y[i] = ad_result(i, 0);
	CppAD::ADFun<scalar> f(ad_x, ad_y);
	// -------------------------------------------------------------------
	// check zero order forward mode
	CPPAD_TESTVECTOR(scalar) x(n), y(m);
	for(size_t i = 0; i < n; i++)
		x[i] = scalar(i + 2);
	y   = f.Forward(0, x);
	ok &= NearEqual(y[0], 0.0,          eps, eps);
	ok &= NearEqual(y[1], x[2] / x[1],  eps, eps);
	// -------------------------------------------------------------------
	// check first order forward mode
	CPPAD_TESTVECTOR(scalar) x1(n), y1(m);
	x1[0] = 1.0;
	x1[1] = 0.0;
	x1[2] = 0.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], 0.0,        eps, eps);
	ok   &= NearEqual(y1[1], 0.0,        eps, eps);
	x1[0] = 0.0;
	x1[1] = 0.0;
	x1[2] = 1.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], 0.0,        eps, eps);
	ok   &= NearEqual(y1[1], 1.0 / x[1], eps, eps);
	x1[0] = 0.0;
	x1[1] = 1.0;
	x1[2] = 0.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], 0.0,                  eps, eps);
	ok   &= NearEqual(y1[1], - x[2] / (x[1]*x[1]), eps, eps);
	// -------------------------------------------------------------------
	// check second order forward mode
	CPPAD_TESTVECTOR(scalar) x2(n), y2(m);
	x2[0] = 0.0;
	x2[1] = 0.0;
	x2[2] = 0.0;
	scalar  partial = 2.0 * x[2] / (x[1] * x[1] * x[1] );
	y2    = f.Forward(2, x2);
	ok   &= NearEqual(y2[0], 0.0,           eps, eps);
	ok   &= NearEqual(y2[1], partial / 2.0, eps, eps);
	// -------------------------------------------------------------------
	return ok;
}
/* %$$
$$ $comment end nospell$$
$end
*/
