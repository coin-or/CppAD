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
$begin atomic_eigen_cholesky.cpp$$
$spell
	Eigen
	Cholesky
$$

$section  Atomic Eigen Cholesky Factorization: Example and Test$$

$head Description$$
The $cref ADFun$$ function object $icode f$$ for this example is
$latex \[
f(x) = \R{det}
\left( \begin{array}{cc}
	x_0   & x_1 \\
	x_1   & x_2
\end{array} \right)
\] $$
where the matrix is positive definite; i.e.,
$latex x_0 > 0$$, $latex x_2 > 0$$ and
$latex f(x) = x_0 x_2 - x_1 x_1 > 0$$.

$childtable%omh/theory/cholesky.omh
	%cppad/example/eigen_cholesky.hpp
%$$

$head Use Atomic Function$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/example/eigen_cholesky.hpp>


bool eigen_cholesky(void)
{
	typedef double scalar;
	typedef typename atomic_eigen_cholesky<scalar>::ad_scalar ad_scalar;
	typedef typename atomic_eigen_cholesky<scalar>::ad_matrix ad_matrix;
	//
	bool ok    = true;
	scalar eps = 10. * std::numeric_limits<scalar>::epsilon();
	using CppAD::NearEqual;
	//
/* %$$
$subhead Constructor$$
$srccode%cpp% */
	// -------------------------------------------------------------------
	// object that computes cholesky factor of a matrix
	atomic_eigen_cholesky<scalar> cholesky;
	// -------------------------------------------------------------------
	// declare independent variable vector x
	size_t n = 3;
	CPPAD_TESTVECTOR(ad_scalar) ad_x(n);
	ad_x[0] = 2.0;
	ad_x[1] = 0.5;
	ad_x[2] = 3.0;
	CppAD::Independent(ad_x);
	// -------------------------------------------------------------------
	// A = [ x[0]  x[1] ]
	//     [ x[1]  x[2] ]
	size_t nr  = 2;
	ad_matrix ad_A(nr, nr);
	ad_A(0, 0) = ad_x[0];
	ad_A(1, 0) = ad_x[1];
	ad_A(0, 1) = ad_x[1];
	ad_A(1, 1) = ad_x[2];
	// -------------------------------------------------------------------
	// use atomic operation to L such that A = L * L^T
	ad_matrix ad_L = cholesky.op(ad_A);
	// compute the determinant of A using its Cholesky factor
	ad_scalar ad_det_L = ad_L(0, 0) * ad_L(1, 1);
	ad_scalar ad_det_A = ad_det_L * ad_det_L;
	// -------------------------------------------------------------------
	// declare the dependent variable vector y
	size_t m = 1;
	CPPAD_TESTVECTOR(ad_scalar) ad_y(m);
	ad_y[0] = ad_det_A;
	CppAD::ADFun<scalar> f(ad_x, ad_y);
	// -------------------------------------------------------------------
	// check zero order forward mode
	CPPAD_TESTVECTOR(scalar) x(n), y(m);
	x[0] = 2.0;
	x[1] = 0.5;
	x[2] = 4.0;
	y   = f.Forward(0, x);
	scalar check = x[0] * x[2] - x[1] * x[1];
	ok &= NearEqual(y[0], check, eps, eps);
	// -------------------------------------------------------------------
	// check first order forward mode
	CPPAD_TESTVECTOR(scalar) x1(n), y1(m);
	x1[0] = 1.0;
	x1[1] = 0.0;
	x1[2] = 0.0;
	y1    = f.Forward(1, x1);
	check = x[2];
	ok   &= NearEqual(y1[0], check, eps, eps);
	//
	x1[0] = 0.0;
	x1[1] = 0.0;
	x1[2] = 1.0;
	y1    = f.Forward(1, x1);
	check = x[0];
	ok   &= NearEqual(y1[0], check, eps, eps);
	//
	x1[0] = 0.0;
	x1[1] = 1.0;
	x1[2] = 0.0;
	y1    = f.Forward(1, x1);
	check = - 2.0 * x[1];
	ok   &= NearEqual(y1[0], check, eps, eps);
	// -------------------------------------------------------------------
	// check second order forward mode
	CPPAD_TESTVECTOR(scalar) x2(n), y2(m);
	x2[0] = 0.0;
	x2[1] = 0.0;
	x2[2] = 0.0;
	y2    = f.Forward(2, x2);
	check = - 1.0; // (1/2) * second partial w.r.t x[1]
	ok   &= NearEqual(y2[0], check, eps, eps);
	// -------------------------------------------------------------------
	return ok;
}
/* %$$
$end
*/
