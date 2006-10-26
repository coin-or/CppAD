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

$comment ! NOTE the title states that this example is used two places !$$
$section Hessian of Lagrangian and  ADFun Default Constructor: Example and Test$$

$index HesLagrangian$$
$index example, Hessian of Lagrangian$$
$index test, Hessian of Lagrangian$$
$index example, ADFun default constructor$$
$index test, ADFun default constructor$$

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
		const CppAD::vector< CppAD::AD<double> > &xyz )
	{	using CppAD::AD;
	
		assert( xyz.size() == 6 );
		AD<double> x0 = xyz[0];
		AD<double> x1 = xyz[1];
		AD<double> x2 = xyz[2];
		AD<double> y0 = xyz[3];
		AD<double> y1 = xyz[4];
		AD<double> z  = xyz[5];
	
		// compute objective function
		AD<double> f = x0 * x0;
	
		// compute constraint functions
		AD<double> h0 = 1. + 2.*x1 + 3.*x2;
		AD<double> h1 = log( x0 * x2 );
	
		// compute the Lagragian
		AD<double> L = y0 * h0 + y1 * h1 + z * f;
	
		return L;
	
	}
}

bool HesLagrangian()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::vector;

	// double values corresponding to XYZ vector
	double x0, x1, x2, y0, y1, z;

	// domain space vector
	size_t n = 3;
	vector< AD<double> >  XYZ(n);
	XYZ[0] = x0 = 0.5;
	XYZ[1] = x1 = 1000.;
	XYZ[2] = x2 = 1.0;

	// declare X as independent variable vector and start recording
	CppAD::Independent(XYZ);

	// add the Lagragian multipliers to XYZ
	// (note that this modifies the vector XYZ)
	XYZ.push_back(y0);
	XYZ.push_back(y1);
	XYZ.push_back(z);

	// range space vector
	size_t m = 1;
	vector< AD<double> >  L(m);
	L[0] = Lagragian(XYZ);

	// create F: X -> L and stop tape recording
	// We cannot use the ADFun sequence constructor because XYZ has
	// changed between the call to Independent and here.
	CppAD::ADFun<double> F;
	F.Dependent(L);

	// independent variable vector
	vector<double> x(n);
	x[0] = x0;
	x[1] = x1;
	x[2] = x2;

	// second derivative of L[0] 
	vector<double> hes( n * n );
	hes = F.Hessian(x, 0);
	/*
	L  =    z * x0 * x0 + y0 * (1 + 2*x1 + 3*x2) + y1 * log( x0 * x2 )

	L_0 = 2 * z * x0 + y1 / x0
	L_1 = y0 * 2 
	L_2 = y0 * 3 + y1 / x2 
	*/
	// L_00 = 2 * z - y1 / ( x0 * x0 )
	double check = 2. * z - y1 / (x0 * x0);
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

	// L_22 = - y1 / (x2 * x2)
	check = - y1 / (x2 * x2);
	ok &= NearEqual(hes[2 * n + 2], check, 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
