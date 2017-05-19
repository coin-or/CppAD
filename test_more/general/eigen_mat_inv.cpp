/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>
# include <cppad/example/eigen_mat_inv.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE

typedef double                                            scalar;
typedef CppAD::AD<scalar>                                 ad_scalar;
typedef typename atomic_eigen_mat_inv<scalar>::ad_matrix  ad_matrix;

scalar eps = 10. * std::numeric_limits<scalar>::epsilon();
using CppAD::NearEqual;
// --------------------------------------------------------------------------
/*
Test atomic_eigen_mat_inv using a non-symetric matrix

f(x) = [ x[0]   -1  ]^{-1}
       [ 2     x[1] ]

     = [ x[1]  1    ] / (x[0] * x[1] + 2)
       [ -2    x[0] ]

y[0] = x[1] / (x[0] * x[1] + 2)
y[1] = 1.0  / (x[0] * x[1] + 2)
y[2] = -2.0 / (x[0] * x[1] + 2)
y[3] = x[0] / (x[0] * x[1] + 2)
*/
bool non_symmetric(void)
{
	bool ok    = true;
	// -------------------------------------------------------------------
	// object that computes inverse of a 2x2 matrix
	atomic_eigen_mat_inv<scalar> mat_inv;
	// -------------------------------------------------------------------
	// declare independent variable vector x
	size_t n = 2;
	CPPAD_TESTVECTOR(ad_scalar) ad_x(n);
	for(size_t j = 0; j < n; j++)
		ad_x[j] = ad_scalar(j);
	CppAD::Independent(ad_x);
	// -------------------------------------------------------------------
	// arg = [ x[0]  -1   ]
	//       [ 2     x[1] ]
	size_t nr  = 2;
	ad_matrix ad_arg(nr, nr);
	ad_arg(0, 0) = ad_x[0];
	ad_arg(0, 1) = ad_scalar(-1.0);
	ad_arg(1, 0) = ad_scalar(2.0);
	ad_arg(1, 1) = ad_x[1];
	// -------------------------------------------------------------------
	// use atomic operation to compute arg^{-1}
	ad_matrix ad_result = mat_inv.op(ad_arg);
	// -------------------------------------------------------------------
	// declare the dependent variable vector y
	size_t m = 4;
	CPPAD_TESTVECTOR(ad_scalar) ad_y(4);
	for(size_t i = 0; i < nr; i++)
		for(size_t j = 0; j < nr; j++)
			ad_y[ i * nr + j ] = ad_result(i, j);
	/* Used to test hand calculated derivaives
	CppAD::AD<scalar> ad_dinv = 1.0 / (ad_x[0] * ad_x[1] + 2.0);
	ad_y[0] = ad_x[1] * ad_dinv;
	ad_y[1] = 1.0  * ad_dinv;
	ad_y[2] = -2.0 * ad_dinv;
	ad_y[3] = ad_x[0] * ad_dinv;
	*/
	CppAD::ADFun<scalar> f(ad_x, ad_y);
	// -------------------------------------------------------------------
	// check zero order forward mode
	CPPAD_TESTVECTOR(scalar) x(n), y(m);
	for(size_t i = 0; i < n; i++)
		x[i] = scalar(i + 2);
	scalar dinv = 1.0 / (x[0] * x[1] + 2.0);
	y          = f.Forward(0, x);
	ok        &= NearEqual(y[0], x[1] * dinv,  eps, eps);
	ok        &= NearEqual(y[1], 1.0  * dinv,  eps, eps);
	ok        &= NearEqual(y[2], -2.0 * dinv,  eps, eps);
	ok        &= NearEqual(y[3], x[0] * dinv,  eps, eps);
	// -------------------------------------------------------------------
	// check first order forward mode
	CPPAD_TESTVECTOR(scalar) x1(n), y1(m);
	scalar dinv_x0 = - x[1] * dinv * dinv;
	x1[0] = 1.0;
	x1[1] = 0.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], x[1] * dinv_x0,        eps, eps);
	ok   &= NearEqual(y1[1], 1.0  * dinv_x0,        eps, eps);
	ok   &= NearEqual(y1[2], -2.0 * dinv_x0,        eps, eps);
	ok   &= NearEqual(y1[3], dinv + x[0] * dinv_x0, eps, eps);
	//
	scalar dinv_x1 = - x[0] * dinv * dinv;
	x1[0] = 0.0;
	x1[1] = 1.0;
	y1    = f.Forward(1, x1);
	ok   &= NearEqual(y1[0], dinv + x[1] * dinv_x1, eps, eps);
	ok   &= NearEqual(y1[1], 1.0  * dinv_x1,        eps, eps);
	ok   &= NearEqual(y1[2], -2.0 * dinv_x1,        eps, eps);
	ok   &= NearEqual(y1[3], x[0] * dinv_x1,        eps, eps);
	// -------------------------------------------------------------------
	// check second order forward mode
	CPPAD_TESTVECTOR(scalar) x2(n), y2(m);
	scalar dinv_x1_x1 = 2.0 * x[0] * x[0] * dinv * dinv * dinv;
	x2[0] = 0.0;
	x2[1] = 0.0;
	y2    = f.Forward(2, x2);
	ok   &= NearEqual(2.0*y2[0], 2.0*dinv_x1 + x[1]*dinv_x1_x1, eps, eps);
	ok   &= NearEqual(2.0*y2[1], 1.0  * dinv_x1_x1,             eps, eps);
	ok   &= NearEqual(2.0*y2[2], -2.0 * dinv_x1_x1,             eps, eps);
	ok   &= NearEqual(2.0*y2[3], x[0] * dinv_x1_x1,             eps, eps);
	// -------------------------------------------------------------------
	// check first order reverse
	CPPAD_TESTVECTOR(scalar) w(m), d1w(n);
	for(size_t i = 0; i < m; i++)
		w[i] = 0.0;
	w[0] = 1.0;
	d1w  = f.Reverse(1, w);
	ok  &= NearEqual(d1w[0], x[1] * dinv_x0,        eps, eps);
	ok  &= NearEqual(d1w[1], dinv + x[1] * dinv_x1, eps, eps);
	w[0] = 0.0;
	w[1] = 1.0;
	d1w  = f.Reverse(1, w);
	ok  &= NearEqual(d1w[0], 1.0 * dinv_x0,         eps, eps);
	ok  &= NearEqual(d1w[1], 1.0 * dinv_x1,         eps, eps);
	w[1] = 0.0;
	w[2] = 1.0;
	d1w  = f.Reverse(1, w);
	ok  &= NearEqual(d1w[0], -2.0 * dinv_x0,        eps, eps);
	ok  &= NearEqual(d1w[1], -2.0 * dinv_x1,        eps, eps);
	w[2] = 0.0;
	w[3] = 1.0;
	d1w  = f.Reverse(1, w);
	ok  &= NearEqual(d1w[0], dinv + x[0] * dinv_x0, eps, eps);
	ok  &= NearEqual(d1w[1], x[0] * dinv_x1,        eps, eps);
	// -------------------------------------------------------------------
	// check second order reverse
	CPPAD_TESTVECTOR(scalar) d2w(2 * n);
	// dinv_x1 = - x[0] * dinv * dinv;
	scalar dinv_x1_x0 = 2.0 * x[0] * x[1] * dinv * dinv * dinv - dinv * dinv;
	d2w  = f.Reverse(2, w);
	// partial f_3 w.r.t x_0
	ok  &= NearEqual(d2w[0 * 2 + 0], dinv + x[0] * dinv_x0, eps, eps);
	// partial f_3 w.r.t x_1
	ok  &= NearEqual(d2w[1 * 2 + 0], x[0] * dinv_x1,              eps, eps);
	// partial f_3 w.r.t. x_1, x_0
	ok  &= NearEqual(d2w[0 * 2 + 1], dinv_x1 + x[0] * dinv_x1_x0, eps, eps);
	// partial f_3 w.r.t. x_1, x_1
	ok  &= NearEqual(d2w[1 * 2 + 1], x[0] * dinv_x1_x1,           eps, eps);
	// -------------------------------------------------------------------
	return ok;
}

} // END_EMPTY_NAMESPACE

bool eigen_mat_inv(void)
{	bool ok = true;
	ok     &= non_symmetric();
	return ok;
}
