/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin HesLagrangian.cpp$$
$spell
	Cpp
	HesLagrangian
$$

$section Hessian of Lagrangian: Example and Test$$

$index HesLagrangian$$
$index example, Hessian of Lagrangian$$
$index test, Hessian of Lagrangian$$

$code
$verbatim%Example/HesLagrangian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cassert>

namespace {
	CppAD::AD<double> Lagragian(
		const CppADvector< CppAD::AD<double> > &x  ,
		const CppADvector< CppAD::AD<double> > &y  ,  
		const CppAD::AD<double>                &z  )
	{	using CppAD::AD;
	
		assert( x.size() == 3 );
		assert( y.size() == 2 );
	
		// compute objective function
		AD<double> f = x[0] * x[0];
	
		// compute constraint functions
		CppADvector< AD<double> > h(2);
		h[0] = 1. + 2.*x[1] + 3.*x[2];
		h[1] = log( x[0] * x[2] );
	
		// compute the Lagragian
		AD<double> L = y[0] * h[0] + y[1] * h[1] + z * f;
	
		return L;
	
	}
}

bool HesLagrangian()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 3;
	CppADvector< AD<double> >  X(n);
	X[0] = 0.5;
	X[1] = 1000.;
	X[2] = 1.0;

	// Lagragian multipliers
	CppADvector< AD<double> > Y(2);
	Y[0] = 1.;
	Y[1] = 1.;
	AD<double> Z = 1.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CppADvector< AD<double> >  L(m);
	L[0] = Lagragian(X, Y, Z);

	// create F: X -> L and stop tape recording
	CppAD::ADFun<double> F(X, L);

	// independent variable vector
	CppADvector<double> x(n);
	x[0] = 0.5;
	x[1] = 1000.;
	x[2] = 1.0;

	// second derivative of L[0] 
	CppADvector<double> hes( n * n );
	hes = F.Hessian(x, 0);
	/*
	L  =    z * x[0] * x[0]
           + y[0] * (1 + 2*x[1] + 3*x[2])
           + y[1] * log( x[0] * x[2] )

	L_0 = 2 * z * x[0] + y[1] / x[0]
	L_1 = y[0] * 2 
	L_2 = y[0] * 3 + y[1] / x[2] 
	*/
	// L_00 = 2 * z - y[1] / ( x[0] * x[0] )
	double check = 2. * Value(Z) - Value(Y[1]) / (x[0] * x[0]);
	ok &= NearEqual(hes[0 * n + 0], check, 1e-10, 1e-10); 

	// L_01 = L_10 = 0
	check = 0.;
	ok &= NearEqual(hes[0 * n + 1], check, 1e-10, 1e-10);
	ok &= NearEqual(hes[1 * n + 0], check, 1e-10, 1e-10);

	// L_02 = L_20 = 0
	ok &= NearEqual(hes[0 * n + 2], check, 1e-10, 1e-10);
	ok &= NearEqual(hes[2 * n + 0], check, 1e-10, 1e-10);

	// L_11 = 0
	ok &= NearEqual(hes[1 * n + 1], check, 1e-10, 1e-10);

	// L_12 = L_21 = 0
	ok &= NearEqual(hes[1 * n + 2], check, 1e-10, 1e-10);
	ok &= NearEqual(hes[2 * n + 1], check, 1e-10, 1e-10);

	// L_22 = - y[1] / (x[2] * x[2])
	check = - Value(Y[1]) / (x[2] * x[2]);
	ok &= NearEqual(hes[2 * n + 2], check, 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
