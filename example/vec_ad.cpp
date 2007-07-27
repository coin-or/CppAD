/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin VecAD.cpp$$
$spell
	Vec
	Cpp
	cstddef
$$

$section AD Vectors that Record Index Operations: Example and Test$$

$index VecAD$$
$index example, VecAD$$
$index test, VecAD$$

$code
$verbatim%example/vec_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cassert>

namespace {
	// return the vector x that solves the following linear system 
	//	a[0] * x[0] + a[1] * x[1] = b[0]
	//	a[2] * x[0] + a[3] * x[1] = b[1]
	// in a way that will record pivot operations on the AD<double> tape
	typedef CPPAD_TEST_VECTOR< CppAD::AD<double> > Vector;
	Vector Solve(const Vector &a , const Vector &b)
	{	using namespace CppAD;
		assert(a.size() == 4 && b.size() == 2);	

		// copy the vector b into the VecAD object B
		VecAD<double> B(2); 
		AD<double>    u;
		for(u = 0; u < 2; u += 1.)
			B[u] = b[ Integer(u) ];

		// copy the matrix a into the VecAD object A
		VecAD<double> A(4); 
		for(u = 0; u < 4; u += 1.)
			A[u] = a [ Integer(u) ];

		// tape AD operation sequence that determines the row of A
		// with maximum absolute element in column zero
		AD<double> zero(0), one(1);
		AD<double> rmax = CondExpGt(abs(a[0]), abs(a[2]), zero, one);

		// divide row rmax by A(rmax, 0)
		A[rmax * 2 + 1]  = A[rmax * 2 + 1] / A[rmax * 2 + 0];
		B[rmax]          = B[rmax]         / A[rmax * 2 + 0];
		A[rmax * 2 + 0]  = one;

		// subtract A(other,0) times row rmax from other row
		AD<double> other   = one - rmax;
		A[other * 2 + 1]   = A[other * 2 + 1]
		                   - A[other * 2 + 0] * A[rmax * 2 + 1];
		B[other]           = B[other]
		                   - A[other * 2 + 0] * B[rmax];
		A[other * 2 + 0] = zero;

		// back substitute to compute the solution vector x
		CPPAD_TEST_VECTOR< AD<double> > x(2);
		size_t iother = Integer(other);
		size_t imax   = Integer(rmax);
		x[iother]     = B[other] / A[other * 2 + 1];
		x[imax ]      = (B[rmax] - A[rmax * 2 + other] * x[iother])
		              / A[rmax * 2 + 0];

		return x;
	}
}

bool VecAD(void)
{	bool ok = true;
	
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 4;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 2.; X[1] = 0.;  // 2 * identity matrix (rmax in Solve will be 0)
	X[2] = 0.; X[3] = 2.; 

	// declare independent variables and start tape recording
	CppAD::Independent(X);

	// define the vector b
	CPPAD_TEST_VECTOR< AD<double> > B(2);
	B[0] = 0.;
	B[1] = 1.;

	// range space vector solves X * Y = b
	size_t m = 2;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y = Solve(X, B);

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y); 

	// check value 
	ok &= NearEqual(Y[0] , B[0] / X[0],  1e-10 , 1e-10);
	ok &= NearEqual(Y[1] , B[1] / X[3],  1e-10 , 1e-10);

	// forward computation of partials w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.; dx[1] = 0.;
	dx[2] = 0.; dx[3] = 0.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], - B[0] / (X[0] * X[0]) , 1e-10, 1e-10);
	ok   &= NearEqual(dy[1],                     0. , 1e-10, 1e-10);

	// compute the solution for a new x matrix such that pivioting
	// on the original rmax row would divide by zero
	CPPAD_TEST_VECTOR<double> x(n);  
	CPPAD_TEST_VECTOR<double> y(m);
	x[0] = 0.; x[1] = 2.;
	x[2] = 2.; x[3] = 0.;
	y    = f.Forward(0, x);
	ok &= NearEqual(y[0] , B[1] / x[2],  1e-10 , 1e-10);
	ok &= NearEqual(y[1] , B[0] / x[1],  1e-10 , 1e-10);
	
	// forward computation of partials w.r.t. x[1]
	dx[0] = 0.; dx[1] = 1.;
	dx[2] = 0.; dx[3] = 0.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0],                     0. , 1e-10, 1e-10);
	ok   &= NearEqual(dy[1], - B[0] / (x[1] * x[1]) , 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
