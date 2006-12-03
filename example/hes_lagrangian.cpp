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
$verbatim%example/hes_lagrangian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
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
		AD<double> g0 = 1. + 2.*x1 + 3.*x2;
		AD<double> g1 = log( x0 * x2 );
		// compute the Lagragian
		AD<double> L = y0 * g0 + y1 * g1 + z * f;
	
		return L;
	
	}
	CppAD::vector< CppAD::AD<double> > fg(
		const CppAD::vector< CppAD::AD<double> > &x )
	{	using CppAD::AD;
		using CppAD::vector;
		assert( x.size() == 3 );

		vector< AD<double> > fg(3);
		fg[0] = x[0] * x[0];
		fg[1] = 1. + 2. * x[1] + 3. * x[2];
		fg[2] = log( x[0] * x[2] );

		return fg;
	}
	bool CheckHessian(
	CppAD::vector<double> H , 
	double x0, double x1, double x2, double y0, double y1, double z )
	{	using CppAD::NearEqual;
		bool ok  = true;
		size_t n = 3;
		assert( H.size() == n * n );
		/*
		L   =    z*x0*x0 + y0*(1 + 2*x1 + 3*x2) + y1*log(x0*x2)

		L_0 = 2 * z * x0 + y1 / x0
		L_1 = y0 * 2 
		L_2 = y0 * 3 + y1 / x2 
		*/
		// L_00 = 2 * z - y1 / ( x0 * x0 )
		double check = 2. * z - y1 / (x0 * x0);
		ok &= NearEqual(H[0 * n + 0], check, 1e-10, 1e-10); 
		// L_01 = L_10 = 0
		ok &= NearEqual(H[0 * n + 1], 0., 1e-10, 1e-10);
		ok &= NearEqual(H[1 * n + 0], 0., 1e-10, 1e-10);
		// L_02 = L_20 = 0
		ok &= NearEqual(H[0 * n + 2], 0., 1e-10, 1e-10);
		ok &= NearEqual(H[2 * n + 0], 0., 1e-10, 1e-10);
		// L_11 = 0
		ok &= NearEqual(H[1 * n + 1], 0., 1e-10, 1e-10);
		// L_12 = L_21 = 0
		ok &= NearEqual(H[1 * n + 2], 0., 1e-10, 1e-10);
		ok &= NearEqual(H[2 * n + 1], 0., 1e-10, 1e-10);
		// L_22 = - y1 / (x2 * x2)
		check = - y1 / (x2 * x2);
		ok &= NearEqual(H[2 * n + 2], check, 1e-10, 1e-10);

		return ok;
	}
	bool UseL()
	{	using CppAD::AD;
		using CppAD::vector;

		// double values corresponding to XYZ vector
		double x0(.5), x1(1e3), x2(1), y0(2.), y1(3.), z(4.);

		// domain space vector
		size_t n = 3;
		vector< AD<double> >  XYZ(n);
		XYZ[0] = x0;
		XYZ[1] = x1;
		XYZ[2] = x2;

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

		// create K: X -> L and stop tape recording
		// We cannot use the ADFun sequence constructor because XYZ has
		// changed between the call to Independent and here.
		CppAD::ADFun<double> K;
		K.Dependent(L);

		// Operation sequence corresponding to K does depends on 
		// value of y0, y1, and z. Must redo calculations above when 
		// y0, y1, or z changes.

		// declare independent variable vector and Hessian
		vector<double> x(n);
		vector<double> H( n * n );

		// point at which we are computing the Hessian
		// (must redo calculations below each time x changes)
		x[0] = x0;
		x[1] = x1;
		x[2] = x2;
		H = K.Hessian(x, 0);

		// check this Hessian calculation
		return CheckHessian(H, x0, x1, x2, y0, y1, z); 
	}
	bool Usefg()
	{	using CppAD::AD;
		using CppAD::vector;

		// parameters defining problem
		double x0(.5), x1(1e3), x2(1), y0(2.), y1(3.), z(4.);

		// domain space vector
		size_t n = 3;
		vector< AD<double> >  X(n);
		X[0] = x0;
		X[1] = x1;
		X[2] = x2;

		// declare X as independent variable vector and start recording
		CppAD::Independent(X);

		// range space vector
		size_t m = 3;
		vector< AD<double> >  FG(m);
		FG = fg(X);

		// create K: X -> FG and stop tape recording
		CppAD::ADFun<double> K;
		K.Dependent(FG);

		// Operation sequence corresponding to K does not depend on 
		// value of x0, x1, x2, y0, y1, or z. 

		// forward and reverse mode arguments and results 
		vector<double> x(n);
		vector<double> H( n * n );
		vector<double>  dx(n);
		vector<double>   w(m);
		vector<double>  dw(2*n);

		// compute Hessian at this value of x
		// (must redo calculations below each time x changes)
		x[0] = x0;
		x[1] = x1;
		x[2] = x2;
		K.Forward(0, x);

		// set weights to Lagrange multiplier values
		// (must redo calculations below each time y0, y1, or z changes)
		w[0] = z;
		w[1] = y0;
		w[2] = y1;

		// initialize dx as zero
		size_t i, j;
		for(i = 0; i < n; i++)
			dx[i] = 0.;
		// loop over components of x
		for(i = 0; i < n; i++)
		{	dx[i] = 1.;             // dx is i-th elementary vector
			K.Forward(1, dx);       // partial w.r.t dx
			dw = K.Reverse(2, w);   // deritavtive of partial
			for(j = 0; j < n; j++)
				H[ i * n + j ] = dw[ j * 2 + 1 ];
			dx[i] = 0.;             // dx is zero vector
		}

		// check this Hessian calculation
		return CheckHessian(H, x0, x1, x2, y0, y1, z); 
	}
}

bool HesLagrangian(void)
{	bool ok = true;

	// UseL is simpler, but must retape every time that y of z changes
	ok     &= UseL();

	// Usefg does not need to retape unless operation sequence changes
	ok     &= Usefg();
	return ok;
}

// END PROGRAM
