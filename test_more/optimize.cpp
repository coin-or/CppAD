/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

namespace {

	template <class Vector>
	void fun(const Vector& x, Vector& y, size_t& original, size_t& opt)
	{	typedef typename Vector::value_type Scalar;
		Scalar a;
		Scalar one(1), two(2), three(3), four(4);
		original = 0;
		opt      = 0;

		// unary operator where operand is arg[0]
		a = CppAD::abs(x[0]); 
		if( a < 1. )
			y[0] = sin(x[0]);
		else	y[0] = cos(x[0]); 
		original += 3;
		opt      += 2;

		// binary operator where left operand is a variable
		// and right operand is a parameter
		a = x[1] + 2.;
		if( a < 3. )
			y[1] = x[1] * 3.;
		else	y[1] = x[1] / 2.;
		original += 2;
		opt      += 1;

		// binary operator where left operand is a parameter
		// and right operation is a variable
		a = 2. - x[2];
		if( a < 4. )
			y[2] = 3. / x[2];
		else	y[2] = 4. + x[2];
		original += 2;
		opt      += 1;

		// binary operator where both operands are variables
		a = x[3] - x[2];
		if( a < 4. )
			y[3] = x[3] / x[2];
		else	y[3] = x[3] + x[2];
		original += 2;
		opt      += 1;

		// a conditional expression that will be optimized out
		a = CppAD::CondExpLt(x[0], x[1], x[2], x[3]);
		if( a < 5. )
			y[4] = CppAD::CondExpLt(x[4], one, two, three);
		else	y[4] = CppAD::CondExpLt(x[4], two, three, four);
		original += 2;
		opt      += 1;

		// Make sure that a parameter dependent variable
		// is not optimized out of the operation sequence.
		// In addition, we do not use the argument x[5], to
		// make sure it is not optimized out.
		y[5] = 1.;
		original += 1;
		opt      += 1;

		return;
	}

	bool CaseOne(void)
	{	// Test all except for VecAD operations
		bool ok = true;
		using CppAD::AD;
		size_t original;
		size_t opt;
		size_t i, j;
	
		// domain space vector
		size_t n  = 6;
		CPPAD_TEST_VECTOR< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = 1. / double(j + 1); 
	
		// declare independent variables and start tape recording
		CppAD::Independent(X);
	
		// range space vector 
		size_t m = n;
		CPPAD_TEST_VECTOR< AD<double> > Y(m);
		fun(X, Y, original, opt);
	
		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 
	
		CPPAD_TEST_VECTOR<double> x(n), y(m), check(m);
		for(j = 0; j < n; j++)
			x[j] = Value(X[j]);
		y = F.Forward(0, x);
		fun(x, check, original, opt);
		for(i = 0; i < m; i++)
			ok &= (y[i] == check[i]);
	
		// Check size before optimization
		ok &= F.size_var() == (n + 1 + original);
	
		// Optimize the operation sequence
		F.optimize();
	
		// Check size after optimization
		ok &= F.size_var() == (n + 1 + opt);
	
		// check result now
		// (should have already been checked if NDEBUG not defined)
		y = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= (y[i] == check[i]);
	
		return ok;
	}

	bool CaseTwo(void)
	{	// Test VecAD operations
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 2;
		CPPAD_TEST_VECTOR< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j); 

		// range space vector
		size_t m = 3;
		CPPAD_TEST_VECTOR< AD<double> > Y(m);

		CppAD::VecAD<double> U(n);
		CppAD::VecAD<double> V(m);
		for(j = 0; j < n; j++)
			U[j] = 0;
		for(i = 0; i < m; i++)
			V[i] = 0;
	
		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// make U a variable
		U[ X[0] ] = X[1];

		// make V a variable
		V[ X[0] ] = X[1];

		// Y only depend on V (and not on U)
		for(i = 0; i < m; i++)
		{	AD<double> I(i);
			Y[i] = V[I];
		}
	
		// create f: X -> Y and stop tape recording
		// Y[ X[0] ] = X[1] and other components of Y are zero. 
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// Check number of VecAD vectors plus number of VecAD elements
		ok &= (F.size_VecAD() == 2 + n + m); 
	
		CPPAD_TEST_VECTOR<double> x(n), y(m);
		for(j = 0; j < n; j++)
			x[j] = double(j);

		y = F.Forward(0, x);
		for(i = 0; i < m; i++)
		{	if( i != static_cast<size_t>(x[0]) )
				ok &= (y[i] == 0.);
			else	ok &= (y[i] == x[1]);
		}

		F.optimize();

		// Check number of VecAD vectors plus number of VecAD elements
		ok &= (F.size_VecAD() == 1 + m); 
		y = F.Forward(0, x);
		for(i = 0; i < m; i++)
		{	if( i != static_cast<size_t>(x[0]) )
				ok &= (y[i] == 0.);
			else	ok &= (y[i] == x[1]);
		}
		
		return ok;
	}
	bool CaseThree(void)
	{	// Power function is a special case for optimize
		bool ok = true;
		using CppAD::AD;
		using CppAD::vector;

		size_t n = 3;
		size_t j;
	
		vector< AD<double> >    X(n), Y(n);
		vector<double>          x(n), y(n); 
	
		for(j = 0; j < n; j++)
			X[j] = x[j] = double(j+1);
	
		CppAD::Independent(X);
	                
		Y[0] = pow(X[0], 2.0);
		Y[1] = pow(2.0, X[1]);
		Y[2] = pow(X[0], X[1]);
	
		CppAD::ADFun<double> F(X, Y);
		F.optimize();
		y = F.Forward(0, x);

		// Use identically equal because the result of the operations
		// have been stored as double and gaurd bits have been dropped.
		// (This may not be true for some compiler in the future).
		for(j = 0; j < n; j++)
			ok &= ( y[j] == Value(Y[j]) );
	
		return ok;
	}
}

bool optimize(void)
{	bool ok = true;
	ok     &= CaseOne();
	ok     &= CaseTwo();
	ok     &= CaseThree();

	return ok;
}
