/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin reverse_any.cpp$$
$spell
	Taylor
$$

$section Reverse Mode General Case: Example and Test$$

$index general, reverse example$$
$index reverse, general example$$
$index example, general reverse$$
$index test, general reverse$$

$index composition, example$$
$index example, composition$$
$index test, composition$$ 

$head Purpose$$
Break a derivative computation into pieces and only store values at the 
interface of the pieces.
In actual applications, there may be many functions, but 
for this example there are only two.
The functions 
$latex F : \R^2 \rightarrow \R^2$$ 
and
$latex G : \R^2 \rightarrow \R^2$$ 
defined by
$latex \[
	F(x) = \left( \begin{array}{c} x_0 x_1   \\ x_1 - x_0 \end{array} \right) 
	\; , \;
	G(y) = \left( \begin{array}{c} y_0 - y_1 \\ y_1  y_0   \end{array} \right) 
\] $$
Another difference is that in actual applications,
the memory corresponding to function objects not currently being used
is sometimes returned to the system (see $cref/checkpoint.cpp/$$).

$head Processing Steps$$
We apply reverse mode to compute the derivative of
$latex H : \R^2 \rightarrow \R$$
is defined by
$latex \[
\begin{array}{rcl}
	H(x) 
	& = & G_0 [ F(x) ] + G_1 [ F(x)  ] 
	\\
	& = & x_0 x_1 - ( x_1 - x_0 ) + x_0 x_1 ( x_1 - x_0 ) 
	\\
	& = & x_0 x_1 ( 1 - x_0 + x_1 ) - x_1 + x_0
\end{array}
\] $$ 
Given the zero and first order Taylor coefficients 
$latex x^{(0)} $$ and $latex x^{(1)}$$,
we use $latex X(t)$$, $latex Y(t)$$ and $latex Z(t)$$
for the corresponding functions; i.e.,
$latex \[
\begin{array}{rcl}
	X(t) & = & x^{(0)} + x^{(1)} t 
	\\
	Y(t) & = & F[X(t)] = y^{(0)} + y^{(1)} t  + O(t^2)
	\\
	Z(t) & = & G \{ F [ X(t) ] \} = z^{(0)} + z^{(1)} t  + O(t^2)
	\\
	h^{(0)} & = & z^{(0)}_0 + z^{(0)}_1
	\\
	h^{(1)} & = & z^{(1)}_0 + z^{(1)}_1
\end{array}
\] $$
Here are the processing steps:
$list number$$
Use forward mode on $latex F(x)$$ to compute 
$latex y^{(0)}$$ and $latex y^{(1)}$$ 
$lnext
Use forward mode on $latex G(y)$$ to compute 
$latex z^{(0)}$$ and $latex z^{(1)}$$ 
$lnext
Use reverse mode on $latex G(y)$$ to compute the derivative of 
$latex h^{(k)}$$ with respect to
$latex y^{(0)}$$ and $latex y^{(1)}$$.
$lnext
Use reverse mode on $latex F(x)$$ to compute the derivative of
$latex h^{(k)}$$ with respect to
$latex x^{(0)}$$ and $latex x^{(1)}$$.
$lend
This uses the following relations for $latex k = 0 , 1$$:
$latex \[
\begin{array}{rcl}
	\partial_{x(0)} h^{(k)} [ x^{(0)} , x^{(1)} ]
	& = &
	\partial_{y(0)} h^{(k)} [ y^{(0)} , y^{(1)} ]
	\partial_{x(0)} y^{(0)} [ x^{(0)} , x^{(1)} ]
	\\
	& + &
	\partial_{y(1)} h^{(k)} [ y^{(0)} , y^{(1)} ]
	\partial_{x(0)} y^{(1)} [ x^{(0)} , x^{(1)} ]
	\\
	\partial_{x(1)} h^{(k)} [ x^{(0)} , x^{(1)} ]
	& = &
	\partial_{y(0)} h^{(k)} [ y^{(0)} , y^{(1)} ]
	\partial_{x(1)} y^{(0)} [ x^{(0)} , x^{(1)} ]
	\\
	& + &
	\partial_{y(1)} h^{(k)} [ y^{(0)} , y^{(1)} ]
	\partial_{x(1)} y^{(1)} [ x^{(0)} , x^{(1)} ]
\end{array}
\] $$
where $latex \partial_{x(0)}$$ denotes the partial with respect
to $latex x^{(0)}$$.

$code
$verbatim%example/reverse_any.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$


$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

namespace {
	template <class Vector>
	Vector F(const Vector& x)
	{	Vector y(2);
		y[0] = x[0] * x[1];
		y[1] = x[1] - x[0];
		return y;
	}
	template <class Vector>
	Vector G(const Vector& y)
	{	Vector z(2);
		z[0] = y[0] - y[1];
		z[1] = y[1] * y[0];
		return z;
	}
}

bool reverse_any(void)
{	bool ok = true;
     double eps = 10. * std::numeric_limits<double>::epsilon();

	using CppAD::AD;
	using CppAD::NearEqual;
	CppAD::ADFun<double> f, g;

	// Record the function F(x)
	size_t n    = 2;
	CPPAD_TEST_VECTOR< AD<double> > X(n), Y(n);
	X[0] = X[1] = 0.;
	CppAD::Independent(X);
	Y = F(X);
	f.Dependent(X, Y);

	// Record the function G(x)
	CPPAD_TEST_VECTOR< AD<double> > Z(n);
	Y[0] = Y[1] = 0.;
	CppAD::Independent(Y);
	Z = G(Y);
	g.Dependent(Y, Z);

	// argument and function values
	CPPAD_TEST_VECTOR<double> x0(n), y0(n), z0(n);
	x0[0] = 1.;
	x0[1] = 2.;
	y0    = f.Forward(0, x0);
	z0    = g.Forward(0, y0);

	// check function value
	double check = x0[0] * x0[1] * (1. - x0[0] + x0[1]) - x0[1] + x0[0];
	double h0    = z0[0] + z0[1];
	ok          &= NearEqual(h0, check, eps, eps);

	// first order Taylor coefficients
	CPPAD_TEST_VECTOR<double> x1(n), y1(n), z1(n);
	x1[0] = 3.;
	x1[1] = 4.;
	y1    = f.Forward(1, x1);
	z1    = g.Forward(1, y1);

	// check first order Taylor coefficients
	check     = x0[0] * x0[1] * (- x1[0] + x1[1]) - x1[1] + x1[0];
	check    += x1[0] * x0[1] * (1. - x0[0] + x0[1]);
	check    += x0[0] * x1[1] * (1. - x0[0] + x0[1]);
	double h1 = z1[0] + z1[1];
	ok       &= NearEqual(h1, check, eps, eps);

	// ----------------------------------------------------------------
	// dw^0 (y) = \partial_y^0 h^0 (y)
	// dw^1 (y) = \partial_y^1 h^0 (y)
	size_t p = 2;
	CPPAD_TEST_VECTOR<double> w(n*p), dw(n*p);
	w[0*p+0] = 1.; // coefficient for z^0_0
	w[1*p+0] = 1.; // coefficient for z^0_1
	w[0*p+1] = 0.; // coefficient for z^1_0
	w[1*p+1] = 0.; // coefficient for z^1_1 
	dw       = g.Reverse(p, w);

	// dv^0 = dw^0 * \partial_x^0 y^0 (x) + dw^1 * \partial_x^0 y^1 (x)  
	// dv^1 = dw^0 * \partial_x^1 y^0 (x) + dw^1 * \partial_x^1 y^1 (x)  
	CPPAD_TEST_VECTOR<double> dv(n*p);
	dv   = f.Reverse(p, dw); 

	// check partial of h^0 w.r.t x^0_0
	check  = x0[1] * (1. - x0[0] + x0[1]) + 1.;
	check -= x0[0] * x0[1];
	ok    &= NearEqual(dv[0*p+0], check, eps, eps);

	// check partial of h^0 w.r.t x^0_1
	check  = x0[0] * (1. - x0[0] + x0[1]) - 1.;
	check += x0[0] * x0[1];
	ok    &= NearEqual(dv[1*p+0], check, eps, eps);

	// check partial of h^0 w.r.t x^1_0 and x^1_1
	check  = 0.;
	ok    &= NearEqual(dv[0*p+1], check, eps, eps);
	ok    &= NearEqual(dv[1*p+1], check, eps, eps);

	// ----------------------------------------------------------------
	// dw^0 (y) = \partial_y^0 h^1 (y)
	// dw^1 (y) = \partial_y^1 h^1 (y)
	w[0*p+0] = 0.; // coefficient for z^0_0
	w[1*p+0] = 0.; // coefficient for z^0_1
	w[0*p+1] = 1.; // coefficient for z^1_0
	w[1*p+1] = 1.; // coefficient for z^1_1 
	dw       = g.Reverse(p, w);

	// dv^0 = dw^0 * \partial_x^0 y^0 (x) + dw^1 * \partial_x^0 y^1 (x)  
	// dv^1 = dw^0 * \partial_x^1 y^0 (x) + dw^1 * \partial_x^1 y^1 (x)  
	dv   = f.Reverse(p, dw); 

	// check partial of h^1 w.r.t x^0_0
	check  = x0[1] * (- x1[0] + x1[1]);
	check -= x1[0] * x0[1];
	check += x1[1] * (1. - x0[0] + x0[1]) - x0[0] * x1[1];
	ok    &= NearEqual(dv[0*p+0], check, eps, eps);

	// check partial of h^1 w.r.t x^0_1
	check  = x0[0] * (- x1[0] + x1[1]);
	check += x1[0] * (1. - x0[0] + x0[1]) + x1[0] * x0[1]; 
	check += x0[0] * x1[1];
	ok    &= NearEqual(dv[1*p+0], check, eps, eps);

	// check partial of h^1 w.r.t x^1_0
	// (by reverse mode identity is equal to partial h^0 w.r.t. x^0_0)
	check  = 1. - x0[0] * x0[1];
	check += x0[1] * (1. - x0[0] + x0[1]);
	ok    &= NearEqual(dv[0*p+1], check, eps, eps);

	// check partial of h^1 w.r.t x^1_1
	// (by reverse mode identity is equal to partial h^0 w.r.t. x^0_1)
	check  = x0[0] * x0[1] - 1.;
	check += x0[0] * (1. - x0[0] + x0[1]);
	ok    &= NearEqual(dv[1*p+1], check, eps, eps);

	return ok;
}

// END PROGRAM
