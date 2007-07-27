/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old examples now only used for validation testing
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>


namespace { // Begin empty namespace

bool VecADTestOne(void)
{	bool ok = true;

	using namespace CppAD;
	using CppAD::sin;
	using CppAD::cos;


	size_t      n = 3;
	AD<double>  N(n);

	AD<double>  a;
	size_t      i;

	// create the array
	CppAD::VecAD<double> V(n);

	// check assignment from double (while not taping)
	for(i = 0; i < n; i++)
		V[i] = double(n - i);

	// check assignment from an AD<double> (while not taping)
	for(i = 0; i < n; i++)
		V[i] = 2. * V[i];

	// check array values (while not taping)
	for(i = 0; i < n; i++)
		ok &= ( V[i] == 2. * double(n - i) ); 

	// independent variable 
	CPPAD_TEST_VECTOR< AD<double> > X(1);
	X[0] = double(n - 1);
	Independent(X);

	// check assignment from double during taping
	a = -1.;
	for(i = 0; i < n; i++)
	{	a += 1.;
		V[a] = double(n - i);
	}

	// check assignment from AD<double> during taping
	a = -1.;
	for(i = 0; i < n; i++)
	{	a += 1.;
		V[a] = sin( X[0] ) * V[a];
	}

	// dependent variable
	CPPAD_TEST_VECTOR< AD<double> > Z(1);
	Z[0] = V[ X[0] ];

	// create f: X -> Z
	ADFun<double> f(X, Z);
	CPPAD_TEST_VECTOR<double>  x( f.Domain() );
	CPPAD_TEST_VECTOR<double> dx( f.Domain() );
	CPPAD_TEST_VECTOR<double>  z( f.Range() );
	CPPAD_TEST_VECTOR<double> dz( f.Range() );

	double vx;
	for(i = 0; i < n; i++)
	{	// check that the indexing operation was taped
		x[0] = double(i);
		z    = f.Forward(0, x);
		vx   = double(n - i);
		ok  &= NearEqual(z[0], sin(x[0]) * vx, 1e-10, 1e-10); 

		// note that derivative of v[x] w.r.t. x is zero
		dx[0] = 1.;
		dz    = f.Forward(1, dx);
		ok   &= NearEqual(dz[0], cos(x[0]) * vx, 1e-10, 1e-10); 

		// reverse mode calculation of same value
		dz[0] = 1.;
		dx    = f.Reverse(1, dz);
		ok   &= NearEqual(dx[0], cos(x[0]) * vx, 1e-10, 1e-10); 
	}


	return ok;
}

// create the discrete function AD<double> Floor(const AD<double> &X) 
double Floor(const double &x)
{	return std::floor(x); }	
CPPAD_DISCRETE_FUNCTION(double, Floor)

bool VecADTestTwo(void)
{	bool ok = true;
	using namespace CppAD;
	
	double pi    = 4. * CppAD::atan(1.);
	size_t nx    = 10;                       // number of x grid point
	double xLow  = 0;                        // minimum value for x
	double xUp   = 2 * pi;                   // maximum value for x
	double xStep = (xUp - xLow) / (nx - 1);  // step size in x
	double xCur;                             // current value for x

	// fill in the data vector on a uniform grid
	VecAD<double> Data(nx);
	size_t i;
	for(i = 0; i < nx; i++)
	{	xCur = xLow + double(i) * xStep; 
		// use size_t indexing of Data while not taping
		Data[i] = CppAD::sin(xCur); 
	}

	// declare independent variable 
	CPPAD_TEST_VECTOR< AD<double> > X(1);
	X[0] = 2.;
	Independent(X);

	// compute index corresponding to current value of X[0]
	AD<double> I = X[0] / xStep;
	AD<double> Ifloor = Floor(I);

	// make sure Ifloor >= 0  (during taping)
	AD<double> Zero(0);
	Ifloor = CondExpLt(Ifloor, Zero, Zero, Ifloor);

	// make sure Ifloor <= nx - 2 (during taping)
	AD<double> Nxminus2(nx - 2);
	Ifloor = CondExpGt(Ifloor, Nxminus2, Nxminus2, Ifloor);

	// Iceil is Ifloor + 1
	AD<double> Iceil  = Ifloor + 1.;

	// linear interpolate Data
	CPPAD_TEST_VECTOR< AD<double> > Y(1);
	Y[0] = Data[Ifloor] + (I - Ifloor) * (Data[Iceil] - Data[Ifloor]);

	// create f: X -> Y that linearly interpolates the data vector
	ADFun<double> f(X, Y);

	// evaluate the linear interpolant at the mid point for first interval
	CPPAD_TEST_VECTOR<double>  x(1);
	CPPAD_TEST_VECTOR<double>  y(1);
	x[0] = xStep / 2.;
	y    = f.Forward(0, x);
	ok  &= NearEqual(y[0], (Data[0] + Data[1])/2., 1e-10, 1e-10);

	// evalute the derivative with respect to x
	CPPAD_TEST_VECTOR<double> dx(1);
	CPPAD_TEST_VECTOR<double> dy(1);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], (Data[1] - Data[0]) / xStep, 1e-10, 1e-10);

	return ok;
}

} // END empty namespace

bool VecAD(void)
{	bool ok = true;
	ok &= VecADTestOne();
	ok &= VecADTestTwo(); 
	return ok;
}
