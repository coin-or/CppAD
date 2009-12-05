/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <limits>
# include <cppad/cppad.hpp>

namespace {

	// -------------------------------------------------------------------
	// Test the reverse dependency analysis optimization
	template <class Vector>
	void FunOne(const Vector& x, Vector& y, size_t& original, size_t& opt)
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

		// this conditional expression that will be optimized out
		a = CppAD::CondExpLt(x[0], x[1], x[2], x[3]);
		// 1 of the following 2 conditional expressions will be kept
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
		FunOne(X, Y, original, opt);
	
		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 
	
		CPPAD_TEST_VECTOR<double> x(n), y(m), check(m);
		for(j = 0; j < n; j++)
			x[j] = Value(X[j]);
		y = F.Forward(0, x);
		FunOne(x, check, original, opt);
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

		CppAD::VecAD<double> U(m);
		CppAD::VecAD<double> V(n);
		for(i = 0; i < m; i++)
			U[i] = 0;
		for(j = 0; j < n; j++)
			V[j] = 0;
	
		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// first vecad vector that is a variable
		U[ X[0] ] = X[1];

		// second vecad vector that is a variable
		V[ X[0] ] = X[1];

		// Make dependency for vecad vectors different that for
		// variables because original code used worng dependency info.
		// Y does not depend on the first variable in the tape; i.e.
		// the one corresponding to the BeginOp. So make it depend
		// on the first vecad vector in the tape.
		for(i = 0; i < m; i++)
		{	AD<double> I(i);
			Y[i] = U[I];
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
			X[j] = x[j] = double(j+2);
	
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

		// check reverse mode derivative
		vector<double>   w(n), dw(n); 
		w[0] = 0.;
		w[1] = 0.;
		w[2] = 1.;
		dw = F.Reverse(1, w);

		double eps = 20. * std::numeric_limits<double>::epsilon();
		double check = x[1] * pow( x[0], x[1] - 1. );
		ok &= CppAD::NearEqual( dw[0], check, eps, eps );

		check = log( x[0] ) * pow( x[0], x[1] );
		ok &= CppAD::NearEqual( dw[1], check, eps, eps );

		check = 0.;
		ok &= CppAD::NearEqual( dw[2], check, eps, eps );
	
		return ok;
	}
	// -------------------------------------------------------------------
	// Test duplicate operation analysis

	template <class Vector>
	void fun_one(const Vector& x, Vector& y, size_t& original, size_t& opt)
	{	typedef typename Vector::value_type Scalar;
		original = 0;
		opt      = 0;

		// unary operator where operand is arg[0] and one result
		Scalar a1 = CppAD::exp(x[0]); 
		original += 1;
		opt      += 1;

		// unary operator where operand is arg[0] and two results
		Scalar b1 = CppAD::sin(x[1]);
		original += 2;
		opt      += 2;

		// non-commutative binary operator where left is a variable
		// and right is a parameter
		Scalar c1 = x[2] - 3.;
		original += 1;
		opt      += 1;

		// non-commutative binary operator where left is a parameter
		// and right is a variable
		Scalar d1 = 3. / x[3];
		original += 1;
		opt      += 1;

		// non-commutative binary operator where left is a variable
		// and right is a variable
		Scalar e1 = pow(x[3], x[4]);
		original += 3;
		opt      += 3;

		// commutative binary operator where  left is a variable
		// and right is a parameter
		Scalar f1 = x[5] * 5.;
		original += 1;
		opt      += 1;

		// commutative binary operator where  left is a variable
		// and right is a variable
		Scalar g1 = x[5] + x[6];
		original += 1;
		opt      += 1;

		// duplicate variables
		Scalar a2 = CppAD::exp(x[0]);
		Scalar b2 = CppAD::sin(x[1]);  // counts for 2 variables
		Scalar c2 = x[2] - 3.;
		Scalar d2 = 3. / x[3];
		Scalar e2 = pow(x[3], x[4]);   // counts for 3 variables
		Scalar f2 = 5. * x[5];
		Scalar g2 = x[6] + x[5];
		original += 10;

		// result vector
		y[0] = a1 + a2;
		y[1] = b1 + b2;
		y[2] = c1 + c2;
		y[3] = d1 + d2;
		y[4] = e1 + e2;
		y[5] = f1 + f2;
		y[6] = g1 + g2;
		original += 7;
		opt      += 7;

		return;
	}
	bool case_one(void)
	{
		bool ok = true;
		using CppAD::AD;
		size_t original;
		size_t opt;
		size_t i, j;
	
		// domain space vector
		size_t n  = 7;
		CPPAD_TEST_VECTOR< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = 1. / double(j + 1); 
	
		// declare independent variables and start tape recording
		CppAD::Independent(X);
	
		// range space vector 
		size_t m = n;
		CPPAD_TEST_VECTOR< AD<double> > Y(m);
		fun_one(X, Y, original, opt);
	
		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 
	
		CPPAD_TEST_VECTOR<double> x(n), y(m), check(m);
		for(j = 0; j < n; j++)
			x[j] = Value(X[j]);
		y = F.Forward(0, x);
		fun_one(x, check, original, opt);
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
	// -------------------------------------------------------------------
	bool case_two(void)
	{	// test that duplicate expression removal is relative to
		// new and not just old argument indices.
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 1;
		CPPAD_TEST_VECTOR< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j + 2); 

		// range space vector
		size_t m = 1;
		CPPAD_TEST_VECTOR< AD<double> > Y(m);

		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// create a new variable
		AD<double> A1 = X[0] - 2.;

		// create a duplicate variable
		AD<double> A2 = X[0] - 2.;

		// create a new variable using first version of duplicate
		AD<double> B1 = A1 / 2.;

		// create a duplicate that can only be dectected using new
		// argument indices
		AD<double> B2 = A2 / 2.; 

		// Make a new variable for result 
		// and make it depend on all the variables
		Y[0] = B1 + B2;

		// create f: X -> Y and stop tape recording
		// Y[ X[0] ] = X[1] and other components of Y are zero. 
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// check number of variables in original function
		ok &= (F.size_var() ==  1 + n + m + 4 ); 
	
		CPPAD_TEST_VECTOR<double> x(n), y(m);
		for(j = 0; j < n; j++)
			x[j] = double(j + 2);

		y   = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= ( y[i] == Value( Y[i] ) );

		F.optimize();

		// check number of variables  in optimized version
		ok &= (F.size_var() == 1 + n + m + 2 ); 

		y   = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= ( y[i] == Value( Y[i] ) );

		return ok;
	}
	// -------------------------------------------------------------------
	bool case_three(void)
	{	// test that duplicate expression removal is relative to
		// new and not just old argument indices (commutative case).
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 1;
		CPPAD_TEST_VECTOR< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j + 2); 

		// range space vector
		size_t m = 1;
		CPPAD_TEST_VECTOR< AD<double> > Y(m);

		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// create a new variable
		AD<double> A1 = X[0] + 2.;

		// create a duplicate variable
		AD<double> A2 = 2. + X[0];

		// create a new variable using first version of duplicate
		AD<double> B1 = A1 * 2.;

		// create a duplicate that can only be dectected using new
		// argument indices
		AD<double> B2 = 2. * A2; 

		// Make a new variable for result 
		// and make it depend on all the variables
		Y[0] = B1 + B2;

		// create f: X -> Y and stop tape recording
		// Y[ X[0] ] = X[1] and other components of Y are zero. 
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// check number of variables in original function
		ok &= (F.size_var() ==  1 + n + m + 4 ); 
	
		CPPAD_TEST_VECTOR<double> x(n), y(m);
		for(j = 0; j < n; j++)
			x[j] = double(j + 2);

		y   = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= ( y[i] == Value( Y[i] ) );

		F.optimize();

		// check number of variables  in optimized version
		ok &= (F.size_var() == 1 + n + m + 2 ); 

		y   = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= ( y[i] == Value( Y[i] ) );

		return ok;
	}
}

bool optimize(void)
{	bool ok = true;
	// check reverse dependency analysis optimization
	ok     &= CaseOne();
	ok     &= CaseTwo();
	ok     &= CaseThree();
	// check removal of duplicate expressions
	ok     &= case_one();
	ok     &= case_two();
	ok     &= case_three();
	return ok;
}
