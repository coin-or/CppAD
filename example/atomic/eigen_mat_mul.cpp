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

$head Description$$
The $cref ADFun$$ function object $icode f$$ for this example is
$latex \[
f(x) =
\left( \begin{array}{cc}
	0 & 0 \\
	1 & 2 \\
	2 & 4
\end{array} \right)
\left( \begin{array}{c}
	x_0 \\
	x_1
\end{array} \right)
=
\left( \begin{array}{c}
	0 \\
	x_0 + 2 x_1 \\
	2 x_0 + 4 x_1 )
\end{array} \right)
\] $$

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

}

bool eigen_mat_mul(void)
{	bool ok    = true;
	scalar eps = 10. * std::numeric_limits<scalar>::epsilon();
	using CppAD::NearEqual;
	//
/* %$$
$subhead Constructor$$
$srccode%cpp% */
	// -------------------------------------------------------------------
	// object that multiplies a 3x2 matrix times a 2x1 matrix
	size_t nr_left  = 3;
	size_t n_middle   = 2;
	size_t nc_right = 1;
	atomic_eigen_mat_mul<scalar> mat_mul(nr_left, n_middle, nc_right);
	// -------------------------------------------------------------------
	//        [ 0  0 ]
	// left = [ 1  1 ]
	//        [ 2  2 ]
	ad_matrix ad_left(nr_left, n_middle);
	for(size_t i = 0; i < nr_left; i++)
	{	for(size_t j = 0; j < n_middle; j++)
			ad_left(i, j) = scalar( (j + 1) * i );
	}
	// -------------------------------------------------------------------
	// declare independent variable vector x
	size_t n = 2;
	CPPAD_TESTVECTOR(ad_scalar) ad_x(n);
	for(size_t j = 0; j < n; j++)
		ad_x[j] = ad_scalar(j);
	CppAD::Independent(ad_x);
	// -------------------------------------------------------------------
	// right = [ x[0] , x[1] ]^T
	ad_matrix ad_right(n_middle, nc_right);
	for(size_t i = 0; i < n_middle; i++)
	{	for(size_t j = 0; j < nc_right; j++)
			ad_right(i, j) = ad_x[i];
	}
	// -------------------------------------------------------------------
	// use atomic operation to multiply left * right
	ad_matrix ad_result = matrix_multiply(mat_mul, ad_left, ad_right);
	// -------------------------------------------------------------------
	// check that first component of result is a parameter
	// and the other components are varaibles.
	ok &= Parameter( ad_result(0, 0) );
	ok &= Variable(  ad_result(1, 0) );
	ok &= Variable(  ad_result(2, 0) );
	// -------------------------------------------------------------------
	// declare the dependent variable vector y
	size_t m = 3;
	CPPAD_TESTVECTOR(ad_scalar) ad_y(m);
	for(size_t i = 0; i < m; i++)
		ad_y[i] = ad_result(i, 0);
	CppAD::ADFun<scalar> f(ad_x, ad_y);
	// -------------------------------------------------------------------
	// check zero order forward mode
	CPPAD_TESTVECTOR(scalar) x(n), y(m);
	for(size_t i = 0; i < n; i++)
		x[i] = scalar(i + 2);
	y   = f.Forward(0, x);
	ok &= NearEqual(y[0], 0.0,                     eps, eps);
	ok &= NearEqual(y[1], x[0] + 2.0 * x[1],       eps, eps);
	ok &= NearEqual(y[2], 2.0 * x[0] + 4.0 * x[1], eps, eps);
	// -------------------------------------------------------------------
	// check first order forward mode
	CPPAD_TESTVECTOR(scalar) x1(n), y1(m);
	x1[0] = 1.0;
	x1[1] = 0.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], 0.0, eps, eps);
	ok   &= NearEqual(y1[1], 1.0, eps, eps);
	ok   &= NearEqual(y1[2], 2.0, eps, eps);
	x1[0] = 0.0;
	x1[1] = 1.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], 0.0, eps, eps);
	ok   &= NearEqual(y1[1], 2.0, eps, eps);
	ok   &= NearEqual(y1[2], 4.0, eps, eps);
	// -------------------------------------------------------------------
	// check first order reverse mode
	CPPAD_TESTVECTOR(scalar) w(m), dw(n);
	w[0]  = 0.0;
	w[1]  = 1.0;
	w[2]  = 0.0;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 1.0, eps, eps);
	ok   &= NearEqual(dw[1], 2.0, eps, eps);
	w[0]  = 0.0;
	w[1]  = 0.0;
	w[2]  = 1.0;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 2.0, eps, eps);
	ok   &= NearEqual(dw[1], 4.0, eps, eps);
	return ok;
}
/* %$$
$$ $comment end nospell$$
$end
*/
