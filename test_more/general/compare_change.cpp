/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old CompareChange examples and tests, now just used for validation testing
*/

# include <cppad/cppad.hpp>

namespace {
	// ----------------------------------------------------------------------
	bool CompareChange_one(void)
	{	bool ok = true;

		using namespace CppAD;

		// ------------------------------- < ----------------------------

		// create independent variables
		CPPAD_TESTVECTOR(AD<double>) X(2);
		X[0] = 3.;
		X[1] = 4.;
		Independent(X);

		// create dependent variables
		CPPAD_TESTVECTOR(AD<double>) Y(6);

		// CondExp would never require retaping
		if( X[0] < X[1] )      // True variable < variable
			Y[0] = X[0];
		else	Y[0] = X[1];
		if( X[1] < X[0] )      // False variable < variable
			Y[1] = X[0];
		else	Y[1] = X[1];
		if( 3.5  < X[1] )      // True parameter < variable
			Y[2] = X[0];
		else	Y[2] = X[1];
		if( 3.5  < X[0] )      // False parameter < variable
			Y[3] = X[0];
		else	Y[3] = X[1];
		if( X[0] < 4.   )      // True variable < parameter
			Y[4] = X[0];
		else	Y[4] = X[1];
		if( X[1] < 4.   )      // False variable < parameter
			Y[5] = X[0];
		else	Y[5] = X[1];

		// f : X -> Y
		ADFun<double> *f;
		f = new ADFun<double>(X, Y);

		// new argument value
		CPPAD_TESTVECTOR(double) x( X.size() );
		x[0] = 4.;
		x[1] = 3.;

		// evaluate the function at new argument
		CPPAD_TESTVECTOR(double) y( Y.size() );
		y = f->Forward(0, x);

		// check results
		ok &= (y[0] == x[0]);           // this is what the was taped
		ok &= (y[1] == x[1]);
		ok &= (y[2] == x[0]);
		ok &= (y[3] == x[1]);
		ok &= (y[4] == x[0]);
		ok &= (y[5] == x[1]);
		ok &= (f->CompareChange() == 6); // all comparisions have changed

		// done with this function
		delete f;

		// ------------------------------- > ----------------------------
		// create independent variables
		Independent(X);

		if( X[0] > X[1] )      // False variable > variable
			Y[0] = X[0];
		else	Y[0] = X[1];
		if( X[1] > X[0] )      // True variable > variable
			Y[1] = X[0];
		else	Y[1] = X[1];
		if( 3.5  > X[1] )      // False parameter > variable
			Y[2] = X[0];
		else	Y[2] = X[1];
		if( 3.5  > X[0] )      // True parameter > variable
			Y[3] = X[0];
		else	Y[3] = X[1];
		if( X[0] > 3.   )      // False variable > parameter
			Y[4] = X[0];
		else	Y[4] = X[1];
		if( X[1] > 3.   )      // True variable > parameter
			Y[5] = X[0];
		else	Y[5] = X[1];

		// f : X -> Y
		f = new ADFun<double> (X, Y);

		// evaluate the function at new argument
		y = f->Forward(0, x);

		// check results
		ok &= (y[0] == x[1]);           // this is what the was taped
		ok &= (y[1] == x[0]);
		ok &= (y[2] == x[1]);
		ok &= (y[3] == x[0]);
		ok &= (y[4] == x[1]);
		ok &= (y[5] == x[0]);
		ok &= (f->CompareChange() == 6); // all comparisions have changed

		// done with this function
		delete f;

		// ------------------------------- <= ----------------------------
		// create independent variables
		Independent(X);

		if( X[0] <= X[1] )      // True variable <= variable
			Y[0] = X[0];
		else	Y[0] = X[1];
		if( X[1] <= X[0] )      // False variable <= variable
			Y[1] = X[0];
		else	Y[1] = X[1];
		if( 4.  <= X[1] )       // True parameter <= variable
			Y[2] = X[0];
		else	Y[2] = X[1];
		if( 4.  <= X[0] )       // False parameter <= variable
			Y[3] = X[0];
		else	Y[3] = X[1];
		if( X[0] <= 3.5   )     // True variable <= parameter
			Y[4] = X[0];
		else	Y[4] = X[1];
		if( X[1] <= 3.5  )      // False variable <= parameter
			Y[5] = X[0];
		else	Y[5] = X[1];

		// f : X -> Y
		f = new ADFun<double> (X, Y);

		// evaluate the function at new argument
		y = f->Forward(0, x);

		// check results
		ok &= (y[0] == x[0]);           // this is what the was taped
		ok &= (y[1] == x[1]);
		ok &= (y[2] == x[0]);
		ok &= (y[3] == x[1]);
		ok &= (y[4] == x[0]);
		ok &= (y[5] == x[1]);
		ok &= (f->CompareChange() == 6); // all comparisions have changed

		// done with this function
		delete f;


		// ------------------------------- >= ----------------------------
		// create independent variables
		Independent(X);

		if( X[0] >= X[1] )      // False variable >= variable
			Y[0] = X[0];
		else	Y[0] = X[1];
		if( X[1] >= X[0] )      // True variable >= variable
			Y[1] = X[0];
		else	Y[1] = X[1];
		if( 3.5  >= X[1] )      // False parameter >= variable
			Y[2] = X[0];
		else	Y[2] = X[1];
		if( 3.5  >= X[0] )      // True parameter >= variable
			Y[3] = X[0];
		else	Y[3] = X[1];
		if( X[0] >= 4.   )      // False variable >= parameter
			Y[4] = X[0];
		else	Y[4] = X[1];
		if( X[1] >= 4.   )      // True variable >= parameter
			Y[5] = X[0];
		else	Y[5] = X[1];

		// f : X -> Y
		f = new ADFun<double> (X, Y);

		// evaluate the function at new argument
		y = f->Forward(0, x);

		// check results
		ok &= (y[0] == x[1]);           // this is what the was taped
		ok &= (y[1] == x[0]);
		ok &= (y[2] == x[1]);
		ok &= (y[3] == x[0]);
		ok &= (y[4] == x[1]);
		ok &= (y[5] == x[0]);
		ok &= (f->CompareChange() == 6); // all comparisions have changed

		// done with this function
		delete f;

		// ------------------------------- == ----------------------------
		// create independent variables
		Independent(X);

		if( X[0] == X[1] )      // False variable == variable
			Y[0] = X[0];
		else	Y[0] = X[1];
		if( X[0] == X[0] )      // True variable == variable
			Y[1] = X[0];
		else	Y[1] = X[1];
		if( 3.  == X[1] )       // False parameter == variable
			Y[2] = X[0];
		else	Y[2] = X[1];
		if( 3.  == X[0] )       // True parameter == variable
			Y[3] = X[0];
		else	Y[3] = X[1];
		if( X[0] == 4.   )      // False variable == parameter
			Y[4] = X[0];
		else	Y[4] = X[1];
		if( X[1] == 4.   )      // True variable == parameter
			Y[5] = X[0];
		else	Y[5] = X[1];

		// f : X -> Y
		f = new ADFun<double> (X, Y);

		// evaluate the function at new argument
		y = f->Forward(0, x);

		// check results
		ok &= (y[0] == x[1]);           // this is what the was taped
		ok &= (y[1] == x[0]);
		ok &= (y[2] == x[1]);
		ok &= (y[3] == x[0]);
		ok &= (y[4] == x[1]);
		ok &= (y[5] == x[0]);
		// the first two comparisions do not change
		ok &= (f->CompareChange() == 4);

		// done with this function
		delete f;

		// ------------------------------- != ----------------------------
		// create independent variables
		Independent(X);

		if( X[0] != X[1] )      // True variable != variable
			Y[0] = X[0];
		else	Y[0] = X[1];
		if( X[0] != X[0] )      // False variable != variable
			Y[1] = X[0];
		else	Y[1] = X[1];
		if( 3.  != X[1] )       // True parameter != variable
			Y[2] = X[0];
		else	Y[2] = X[1];
		if( 3.  != X[0] )       // False parameter != variable
			Y[3] = X[0];
		else	Y[3] = X[1];
		if( X[0] != 4.   )      // True variable != parameter
			Y[4] = X[0];
		else	Y[4] = X[1];
		if( X[1] != 4.   )      // False variable != parameter
			Y[5] = X[0];
		else	Y[5] = X[1];

		// f : X -> Y
		f = new ADFun<double> (X, Y);

		// evaluate the function at new argument
		y = f->Forward(0, x);

		// check results
		ok &= (y[0] == x[0]);           // this is what the was taped
		ok &= (y[1] == x[1]);
		ok &= (y[2] == x[0]);
		ok &= (y[3] == x[1]);
		ok &= (y[4] == x[0]);
		ok &= (y[5] == x[1]);
		// the first two comparisions do not change
		ok &= (f->CompareChange() == 4);

		// done with this function
		delete f;

		return ok;
	}
	// ----------------------------------------------------------------------
	template <typename Type>
	Type Minimum(const Type &x, const Type &y)
	{	// Use a comparision to compute the min(x, y)
		// (note that CondExp would never require retaping).
		if( x < y )
			return x;
		return y;
	}

	bool CompareChange_two(void)
	{	bool ok = true;

		using CppAD::AD;
		using CppAD::ADFun;
		using CppAD::Independent;

		// domain space vector
		size_t n = 2;
		CPPAD_TESTVECTOR(AD<double>) X(n);
		X[0] = 3.;
		X[1] = 4.;

		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// range space vector
		size_t m = 1;
		CPPAD_TESTVECTOR(AD<double>) Y(m);
		Y[0] = Minimum(X[0], X[1]);

		// create f: x -> y and stop tape recording
		ADFun<double> f(X, Y);

		// evaluate zero mode Forward where conditional has the same result
		// note that f.CompareChange is not defined when NDEBUG is true
		CPPAD_TESTVECTOR(double) x(n);
		CPPAD_TESTVECTOR(double) y(m);
		x[0] = 3.5;
		x[1] = 4.;
		y    = f.Forward(0, x);
		ok  &= (y[0] == x[0]);
		ok  &= (y[0] == Minimum(x[0], x[1]));
		ok  &= (f.CompareChange() == 0);

		// evaluate zero mode Forward where conditional has different result
		x[0] = 4.;
		x[1] = 3.;
		y    = f.Forward(0, x);
		ok  &= (y[0] == x[0]);
		ok  &= (y[0] != Minimum(x[0], x[1]));
		ok  &= (f.CompareChange() == 1);

		// re-tape to obtain the new AD operation sequence
		X[0] = 4.;
		X[1] = 3.;
		Independent(X);
		Y[0] = Minimum(X[0], X[1]);

		// stop tape and store result in f
		f.Dependent(Y);

		// evaluate the function at new argument values
		y    = f.Forward(0, x);
		ok  &= (y[0] == x[1]);
		ok  &= (y[0] == Minimum(x[0], x[1]));
		ok  &= (f.CompareChange() == 0);

		return ok;
	}
}

bool compare_change(void)
{	bool ok  = true;
	ok &= CompareChange_one();
	ok &= CompareChange_two();
	return ok;
}

// END C++
