/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Comprehensive test built on 08/07 for new user interface to CondExp 
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool CondExp_pvvv(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	CPPAD_TEST_VECTOR< AD<double> > X(3);
	X[0]     = 0.;
	X[1]     = 1.;
	X[2]     = 2.;
	Independent(X);

	// parameter value
	AD<double> one = 1.; 

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Y(5);

	// CondExp(parameter, variable, variable, variable)
	Y[0] = CondExpLt(one, X[0], X[1], X[2]);
	Y[1] = CondExpLe(one, X[0], X[1], X[2]);
	Y[2] = CondExpEq(one, X[0], X[1], X[2]);
	Y[3] = CondExpGe(one, X[0], X[1], X[2]);
	Y[4] = CondExpGt(one, X[0], X[1], X[2]);

	// create f: X -> Y 
	ADFun<double> f(X, Y);

	// vectors for function values
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	// vectors for derivative values
	CPPAD_TEST_VECTOR<double> dv( f.Domain() );
	CPPAD_TEST_VECTOR<double> dw( f.Range() );

	// check original function values
	ok &= Y[0] == X[2];
	ok &= Y[1] == X[2];
	ok &= Y[2] == X[2];
	ok &= Y[3] == X[1];
	ok &= Y[4] == X[1];

	// function values 
	v[0] = 2.;
	v[1] = 1.;
	v[2] = 0.;
	w    = f.Forward(0, v);
	ok &= ( w[0] == v[1] );
	ok &= ( w[1] == v[1] );
	ok &= ( w[2] == v[2] );
	ok &= ( w[3] == v[2] );
	ok &= ( w[4] == v[2] );

	// forward mode derivative values
	dv[0] = 1.;
	dv[1] = 2.;
	dv[2] = 3.;
	dw    = f.Forward(1, dv);
	ok   &= (dw[0] == dv[1] );
	ok   &= (dw[1] == dv[1] );
	ok   &= (dw[2] == dv[2] );
	ok   &= (dw[3] == dv[2] );
	ok   &= (dw[4] == dv[2] );

	// reverse mode derivative values
	dw[0] = 1.;
	dw[1] = 2.;
	dw[2] = 3.;
	dw[3] = 4.;
	dw[4] = 5.;
	dv    = f.Reverse(1, dw);
	ok   &= (dv[0] == 0.);
	ok   &= (dv[1] == dw[0] + dw[1] );
	ok   &= (dv[2] == dw[2] + dw[3] + dw[4] );
	
	return ok;
}
bool CondExp_vpvv(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	CPPAD_TEST_VECTOR< AD<double> > X(3);
	X[0]     = 0.;
	X[1]     = 1.;
	X[2]     = 2.;
	Independent(X);

	// parameter value
	AD<double> one = 1.; 

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Y(5);

	// CondExp(variable, parameter, variable, variable)
	Y[0] = CondExpLt(X[0], one, X[1], X[2]);
	Y[1] = CondExpLe(X[0], one, X[1], X[2]);
	Y[2] = CondExpEq(X[0], one, X[1], X[2]);
	Y[3] = CondExpGe(X[0], one, X[1], X[2]);
	Y[4] = CondExpGt(X[0], one, X[1], X[2]);

	// create f: X -> Y 
	ADFun<double> f(X, Y);

	// vectors for function values
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	// vectors for derivative values
	CPPAD_TEST_VECTOR<double> dv( f.Domain() );
	CPPAD_TEST_VECTOR<double> dw( f.Range() );

	// check original function values
	ok &= Y[0] == X[1];
	ok &= Y[1] == X[1];
	ok &= Y[2] == X[2];
	ok &= Y[3] == X[2];
	ok &= Y[4] == X[2];

	// function values 
	v[0] = 2.;
	v[1] = 1.;
	v[2] = 0.;
	w    = f.Forward(0, v);
	ok &= ( w[0] == v[2] );
	ok &= ( w[1] == v[2] );
	ok &= ( w[2] == v[2] );
	ok &= ( w[3] == v[1] );
	ok &= ( w[4] == v[1] );

	// forward mode derivative values
	dv[0] = 1.;
	dv[1] = 2.;
	dv[2] = 3.;
	dw    = f.Forward(1, dv);
	ok   &= (dw[0] == dv[2] );
	ok   &= (dw[1] == dv[2] );
	ok   &= (dw[2] == dv[2] );
	ok   &= (dw[3] == dv[1] );
	ok   &= (dw[4] == dv[1] );

	// reverse mode derivative values
	dw[0] = 1.;
	dw[1] = 2.;
	dw[2] = 3.;
	dw[3] = 4.;
	dw[4] = 5.;
	dv    = f.Reverse(1, dw);
	ok   &= (dv[0] == 0.);
	ok   &= (dv[1] == dw[3] + dw[4] );
	ok   &= (dv[2] == dw[0] + dw[1] + dw[2] );
	
	return ok;
}
bool CondExp_vvpv(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	CPPAD_TEST_VECTOR< AD<double> > X(3);
	X[0]     = 0.;
	X[1]     = 1.;
	X[2]     = 2.;
	Independent(X);

	// parameter value
	AD<double> three = 3.; 

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Y(5);

	// CondExp(variable, variable, parameter, variable)
	Y[0] = CondExpLt(X[0], X[1], three, X[2]);
	Y[1] = CondExpLe(X[0], X[1], three, X[2]);
	Y[2] = CondExpEq(X[0], X[1], three, X[2]);
	Y[3] = CondExpGe(X[0], X[1], three, X[2]);
	Y[4] = CondExpGt(X[0], X[1], three, X[2]);

	// create f: X -> Y 
	ADFun<double> f(X, Y);

	// vectors for function values
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	// vectors for derivative values
	CPPAD_TEST_VECTOR<double> dv( f.Domain() );
	CPPAD_TEST_VECTOR<double> dw( f.Range() );

	// check original function values
	ok &= Y[0] == three;
	ok &= Y[1] == three;
	ok &= Y[2] == X[2];
	ok &= Y[3] == X[2];
	ok &= Y[4] == X[2];

	// function values 
	v[0] = 2.;
	v[1] = 1.;
	v[2] = 0.;
	w    = f.Forward(0, v);
	ok &= ( w[0] == v[2] );
	ok &= ( w[1] == v[2] );
	ok &= ( w[2] == v[2] );
	ok &= ( w[3] == three );
	ok &= ( w[4] == three );

	// forward mode derivative values
	dv[0] = 1.;
	dv[1] = 2.;
	dv[2] = 3.;
	dw    = f.Forward(1, dv);
	ok   &= (dw[0] == dv[2] );
	ok   &= (dw[1] == dv[2] );
	ok   &= (dw[2] == dv[2] );
	ok   &= (dw[3] == 0.    );
	ok   &= (dw[4] == 0.    );

	// reverse mode derivative values
	dw[0] = 1.;
	dw[1] = 2.;
	dw[2] = 3.;
	dw[3] = 4.;
	dw[4] = 5.;
	dv    = f.Reverse(1, dw);
	ok   &= (dv[0] == 0.);
	ok   &= (dv[1] == 0.);
	ok   &= (dv[2] == dw[0] + dw[1] + dw[2] );
	
	return ok;
}
bool CondExp_vvvp(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	CPPAD_TEST_VECTOR< AD<double> > X(3);
	X[0]     = 0.;
	X[1]     = 1.;
	X[2]     = 2.;
	Independent(X);

	// parameter value
	AD<double> three = 3.; 

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Y(5);

	// CondExp(variable, variable, variable, parameter)
	Y[0] = CondExpLt(X[0], X[1], X[2], three);
	Y[1] = CondExpLe(X[0], X[1], X[2], three);
	Y[2] = CondExpEq(X[0], X[1], X[2], three);
	Y[3] = CondExpGe(X[0], X[1], X[2], three);
	Y[4] = CondExpGt(X[0], X[1], X[2], three);

	// create f: X -> Y 
	ADFun<double> f(X, Y);

	// vectors for function values
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	// vectors for derivative values
	CPPAD_TEST_VECTOR<double> dv( f.Domain() );
	CPPAD_TEST_VECTOR<double> dw( f.Range() );

	// check original function values
	ok &= Y[0] == X[2];
	ok &= Y[1] == X[2];
	ok &= Y[2] == three;
	ok &= Y[3] == three;
	ok &= Y[4] == three;

	// function values 
	v[0] = 2.;
	v[1] = 1.;
	v[2] = 0.;
	w    = f.Forward(0, v);
	ok &= ( w[0] == three );
	ok &= ( w[1] == three );
	ok &= ( w[2] == three );
	ok &= ( w[3] == v[2]  );
	ok &= ( w[4] == v[2]  );

	// forward mode derivative values
	dv[0] = 1.;
	dv[1] = 2.;
	dv[2] = 3.;
	dw    = f.Forward(1, dv);
	ok   &= (dw[0] == 0.    );
	ok   &= (dw[1] == 0.    );
	ok   &= (dw[2] == 0.    );
	ok   &= (dw[3] == dv[2] );
	ok   &= (dw[4] == dv[2] );

	// reverse mode derivative values
	dw[0] = 1.;
	dw[1] = 2.;
	dw[2] = 3.;
	dw[3] = 4.;
	dw[4] = 5.;
	dv    = f.Reverse(1, dw);
	ok   &= (dv[0] == 0.);
	ok   &= (dv[1] == 0.);
	ok   &= (dv[2] == dw[3] + dw[4] );
	
	return ok;
}
bool CondExp(void)
{	bool ok  = true;
	ok      &= CondExp_pvvv();
	ok      &= CondExp_vpvv();
	ok      &= CondExp_vvpv();
	ok      &= CondExp_vvvp();
	return ok;
}
// END PROGRAM
