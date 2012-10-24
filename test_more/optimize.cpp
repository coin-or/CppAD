/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <limits>
# include <cppad/cppad.hpp>

namespace {

	// -------------------------------------------------------------------
	// Test the reverse dependency analysis optimization
	template <class Vector>
	void depend_fun
	(const Vector& x, Vector& y, size_t& original, size_t& opt)
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

	bool depend_one(void)
	{	// Test all except for VecAD operations
		bool ok = true;
		using CppAD::AD;
		size_t original;
		size_t opt;
		size_t i, j;
	
		// domain space vector
		size_t n  = 6;
		CppAD::vector< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = 1. / double(j + 1); 
	
		// declare independent variables and start tape recording
		CppAD::Independent(X);
	
		// range space vector 
		size_t m = n;
		CppAD::vector< AD<double> > Y(m);
		depend_fun(X, Y, original, opt);
	
		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 
	
		CppAD::vector<double> x(n), y(m), check(m);
		for(j = 0; j < n; j++)
			x[j] = Value(X[j]);
		y = F.Forward(0, x);
		depend_fun(x, check, original, opt);
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

	bool depend_two(void)
	{	// Test VecAD operations
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 2;
		CppAD::vector< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j); 

		// range space vector
		size_t m = 3;
		CppAD::vector< AD<double> > Y(m);

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
	
		CppAD::vector<double> x(n), y(m);
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
	bool depend_three(void)
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
	// ===================================================================
	// Test duplicate operation analysis

	template <class Vector>
	void duplicate_fun
	(const Vector& x, Vector& y, size_t& original, size_t& opt)
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
		y[0] = a1 * a2;
		y[1] = b1 * b2;
		y[2] = c1 * c2;
		y[3] = d1 * d2;
		y[4] = e1 * e2;
		y[5] = f1 * f2;
		y[6] = g1 * g2;
		original += 7;
		opt      += 7;

		return;
	}
	bool duplicate_one(void)
	{
		bool ok = true;
		using CppAD::AD;
		size_t original;
		size_t opt;
		size_t i, j;
	
		// domain space vector
		size_t n  = 7;
		CppAD::vector< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = 1. / double(j + 1); 
	
		// declare independent variables and start tape recording
		CppAD::Independent(X);
	
		// range space vector 
		size_t m = n;
		CppAD::vector< AD<double> > Y(m);
		duplicate_fun(X, Y, original, opt);
	
		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 
	
		CppAD::vector<double> x(n), y(m), check(m);
		for(j = 0; j < n; j++)
			x[j] = Value(X[j]);
		y = F.Forward(0, x);
		duplicate_fun(x, check, original, opt);
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
	bool duplicate_two(void)
	{	// test that duplicate expression removal is relative to
		// new and not just old argument indices.
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 1;
		CppAD::vector< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j + 2); 

		// range space vector
		size_t m = 1;
		CppAD::vector< AD<double> > Y(m);

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
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// check number of variables in original function
		ok &= (F.size_var() ==  1 + n + m + 4 ); 
	
		CppAD::vector<double> x(n), y(m);
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
	bool duplicate_three(void)
	{	// test that duplicate expression removal is relative to
		// new and not just old argument indices (commutative case).
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 1;
		CppAD::vector< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j + 2); 

		// range space vector
		size_t m = 1;
		CppAD::vector< AD<double> > Y(m);

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
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// check number of variables in original function
		ok &= (F.size_var() ==  1 + n + m + 4 ); 
	
		CppAD::vector<double> x(n), y(m);
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
	bool duplicate_four(void)
	{	// Check that unary expression matching not only checks hash code,
		// and operator, but also operand (old bug that has been fixed).
		bool ok = true;
		using CppAD::AD;
		size_t j;

		// domain space vector
		size_t n  = 1;
		CppAD::vector< AD<double> > X(n);
		X[0] = 1.;

		// range space vector
		size_t m = 1;
		CppAD::vector< AD<double> > Y(m);

		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// check a huge number of same operation with different operands 
		size_t n_operations = std::min(
			size_t(CPPAD_HASH_TABLE_SIZE) + 5,
			size_t(std::numeric_limits<CPPAD_TAPE_ADDR_TYPE>::max()) - 5
		);
		Y[0] = X[0];
		for(j = 0; j < n_operations; j++)
			Y[0] = abs(Y[0]);

		// create f: X -> Y and stop tape recording
		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// check number of variables in original function
		ok &= (F.size_var() ==  1 + n + n_operations ); 
	
		CppAD::vector<double> x(n), y(m);
		x[0] = 1.;

		y   = F.Forward(0, x);
		ok &= ( y[0] == Value( Y[0] ) );

		F.optimize();

		// check same number of variables in optimized version
		ok &= (F.size_var() == 1 + n + n_operations ); 

		y   = F.Forward(0, x);
		ok &= ( y[0] == Value( Y[0] ) );

		return ok;
	}
	// ====================================================================
	bool cummulative_sum(void)
	{	// test conversion of a sequence of additions and subtraction
		// to a cummulative summation sequence.
		bool ok = true;
		using CppAD::AD;
		size_t i, j;

		// domain space vector
		size_t n  = 7;
		CppAD::vector< AD<double> > X(n);
		for(j = 0; j < n; j++)
			X[j] = double(j + 2); 

		size_t n_original = 1 + n;
		size_t n_optimize = 1 + n;

		// range space vector
		size_t m = 2;
		CppAD::vector< AD<double> > Y(m);

		// declare independent variables and start tape recording
		CppAD::Independent(X);

		// Operations inside of optimize_cadd
		Y[0] = 5. + (X[0] + X[1]) + (X[1] - X[2]) // Addvv, Subvv
		     + (X[2] - 1.) + (2. - X[3])   // Subvp, Subpv
		     + (X[4] + 3.) + (4. + X[5]);  // Addpv, Addpv (no Addvp)
		n_original += 12;
		n_optimize += 1;


		// Operations inside of optimize_csub
		Y[1] = 5. - (X[1] + X[2]) - (X[2] - X[3]) // Addvv, Subvv
		     - (X[3] - 1.) - (2. - X[4])   // Subvp, Subpv
		     - (X[5] + 3.) - (4. + X[6]);  // Addpv, Addpv (no Addvp)
		n_original += 12;
		n_optimize += 1;

		CppAD::ADFun<double> F;
		F.Dependent(X, Y); 

		// check number of variables in original function
		ok &= (F.size_var() ==  n_original ); 
	
		CppAD::vector<double> x(n), y(m);
		for(j = 0; j < n; j++)
			x[j] = double(j + 2);

		y   = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= ( y[i] == Value( Y[i] ) );

		F.optimize();

		// check number of variables  in optimized version
		ok &= (F.size_var() == n_optimize ); 

		y   = F.Forward(0, x);
		for(i = 0; i < m; i++)
			ok &= ( y[i] == Value( Y[i] ) );

		return ok;
	}
	// -------------------------------------------------------------------
	bool forward_csum(void)
	{	bool ok = true;
	
		using namespace CppAD;
	
		// independent variable vector 
		CppAD::vector< AD<double> > X(2);
		X[0] = 0.; 
		X[1] = 1.;
		Independent(X);
	
		// compute sum of elements in X
		CppAD::vector< AD<double> > Y(1);
		Y[0] = X[0] + X[0] + X[1];
	
		// create function object F : X -> Y
		ADFun<double> F(X, Y);
	
		// now optimize the operation sequence
		F.optimize();
	
		// use zero order to evaluate F[ (3, 4) ] 
		CppAD::vector<double>  x0( F.Domain() );
		CppAD::vector<double>  y0( F.Range() );
		x0[0]    = 3.;
		x0[1]    = 4.;
		y0       = F.Forward(0, x0);
		ok      &= NearEqual(y0[0] , x0[0]+x0[0]+x0[1], 1e-10, 1e-10);
	
		// evaluate derivative of F in X[0] direction
		CppAD::vector<double> x1( F.Domain() );
		CppAD::vector<double> y1( F.Range() );
		x1[0]    = 1.;
		x1[1]    = 0.;
		y1       = F.Forward(1, x1);
		ok      &= NearEqual(y1[0] , x1[0]+x1[0]+x1[1], 1e-10, 1e-10);
	
		// evaluate second derivative of F in X[0] direction
		CppAD::vector<double> x2( F.Domain() );
		CppAD::vector<double> y2( F.Range() );
		x2[0]       = 0.;
		x2[1]       = 0.;
		y2          = F.Forward(2, x2);
		double F_00 = 2. * y2[0];
		ok         &= NearEqual(F_00, 0., 1e-10, 1e-10);
	
		return ok;
	}
	// -------------------------------------------------------------------
	bool reverse_csum(void)
	{	bool ok = true;
	
		using namespace CppAD;
	
		// independent variable vector 
		CppAD::vector< AD<double> > X(2);
		X[0] = 0.; 
		X[1] = 1.;
		Independent(X);
	
		// compute sum of elements in X
		CppAD::vector< AD<double> > Y(1);
		Y[0] = X[0] - (X[0] - X[1]);
	
		// create function object F : X -> Y
		ADFun<double> F(X, Y);
	
		// now optimize the operation sequence
		F.optimize();
	
		// use zero order to evaluate F[ (3, 4) ] 
		CppAD::vector<double>  x0( F.Domain() );
		CppAD::vector<double>  y0( F.Range() );
		x0[0]    = 3.;
		x0[1]    = 4.;
		y0       = F.Forward(0, x0);
		ok      &= NearEqual(y0[0] , x0[0]-x0[0]+x0[1], 1e-10, 1e-10);
	
		// evaluate derivative of F 
		CppAD::vector<double> dF( F.Domain() );
		CppAD::vector<double> w( F.Range() );
		w[0]    = 1.;
		dF      = F.Reverse(1, w);
		ok     &= NearEqual(dF[0] , 0., 1e-10, 1e-10);
		ok     &= NearEqual(dF[1] , 1., 1e-10, 1e-10);
	
		return ok;
	}
	bool forward_sparse_jacobian_csum()
	{	bool ok = true;
		using namespace CppAD;
	
		// dimension of the domain space
		size_t n = 3; 
	
		// dimension of the range space
		size_t m = 2;
	
		// independent variable vector 
		CppAD::vector< AD<double> > X(n);
		X[0] = 2.; 
		X[1] = 3.;
		Independent(X);
	
		// dependent variable vector
		CppAD::vector< AD<double> > Y(m);
	
		// check results vector
		CppAD::vector< bool >       Check(m * n);
	
		// initialize index into Y
		size_t index = 0;
	
		// Y[0] 
		Y[index]             = X[0] + X[1] + 5.;
		Check[index * n + 0] = true;
		Check[index * n + 1] = true;
		Check[index * n + 2] = false;
		index++;
	
		// Y[1] 
		Y[index]             = Y[0] - (X[1] + X[2]);
		Check[index * n + 0] = true;
		Check[index * n + 1] = true;
		Check[index * n + 2] = true;
		index++;
	
		// check final index
		assert( index == m );
	
		// create function object F : X -> Y
		ADFun<double> F(X, Y);
		F.optimize();
	
		// ---------------------------------------------------------
		// dependency matrix for the identity function 
		CppAD::vector< std::set<size_t> > Sx(n);
		size_t i, j;
		for(i = 0; i < n; i++)
		{	assert( Sx[i].empty() );
			Sx[i].insert(i);
		}
	
		// evaluate the dependency matrix for F(x)
		CppAD::vector< std::set<size_t> > Sy(m);
		Sy = F.ForSparseJac(n, Sx);
	
		// check values
		bool found;
		for(i = 0; i < m; i++)
		{	for(j = 0; j < n; j++)
			{	found = Sy[i].find(j) != Sy[i].end();
				ok &= (found == Check[i * n + j]);
			}
		}	
	
		return ok;
	}
	bool reverse_sparse_jacobian_csum()
	{	bool ok = true;
		using namespace CppAD;
	
		// dimension of the domain space
		size_t n = 3; 
	
		// dimension of the range space
		size_t m = 2;
	
		// independent variable vector 
		CppAD::vector< AD<double> > X(n);
		X[0] = 2.; 
		X[1] = 3.;
		Independent(X);
	
		// dependent variable vector
		CppAD::vector< AD<double> > Y(m);
	
		// check results vector
		CppAD::vector< bool >       Check(m * n);
	
		// initialize index into Y
		size_t index = 0;
	
		// Y[0] 
		Y[index]             = X[0] + X[1] + 5.;
		Check[index * n + 0] = true;
		Check[index * n + 1] = true;
		Check[index * n + 2] = false;
		index++;
	
		// Y[1] 
		Y[index]             = Y[0] - (X[1] + X[2]);
		Check[index * n + 0] = true;
		Check[index * n + 1] = true;
		Check[index * n + 2] = true;
		index++;
	
		// check final index
		assert( index == m );
	
		// create function object F : X -> Y
		ADFun<double> F(X, Y);
		F.optimize();
	
		// ----------------------------------------------------------
		// dependency matrix for the identity function
		CppAD::vector< bool > Py(m * m);
		size_t i, j;
		for(i = 0; i < m; i++)
		{	for(j = 0; j < m; j++)
				Py[ i * m + j ] = (i == j);
		}
	
		// evaluate the dependency matrix for F(x)
		CppAD::vector< bool > Px(m * n);
		Px = F.RevSparseJac(m, Py);
	
		// check values
		for(i = 0; i < m; i++)
		{	for(j = 0; j < n; j++)
				ok &= (Px[i * n + j] == Check[i * n + j]);
		}	
	
		return ok;
	}
	bool reverse_sparse_hessian_csum(void)
	{	bool ok = true;
		using CppAD::AD;
		size_t i, j;
	
		size_t n = 2;
		CppAD::vector< AD<double> > X(n); 
		X[0] = 1.;
		X[1] = 2.;
		CppAD::Independent(X);
	
		size_t m = 1;
		CppAD::vector< AD<double> > Y(m);
		Y[0] = (2. + X[0] + X[1] + 3.) * X[0];
	
		CppAD::vector<bool> check(n * n);
		check[0 * n + 0] = true;  // partial w.r.t. x[0], x[0]
		check[0 * n + 1] = true;  //                x[0], x[1]
		check[1 * n + 0] = true;  //                x[1], x[0]
		check[1 * n + 1] = false; //                x[1], x[1]
	
		// create function object F : X -> Y
		CppAD::ADFun<double> F(X, Y);
		F.optimize();
	
		// sparsity pattern for the identity function U(x) = x
		CppAD::vector<bool> Px(n * n);
		for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				Px[ i * n + j ] = (i == j);
	
		// compute sparsity pattern for Jacobian of F(U(x))
		CppAD::vector<bool> P_jac(m * n);
		P_jac = F.ForSparseJac(n, Px);
	
		// compute sparsity pattern for Hessian of F_k ( U(x) ) 
		CppAD::vector<bool> Py(m);
		CppAD::vector<bool> Pxx(n * n);
		Py[0] = true;
		Pxx = F.RevSparseHes(n, Py);
		// check values
		for(i = 0; i < n * n; i++)
			ok &= (Pxx[i] == check[i]);
	
		return ok;
	}
	// check that CondExp properly detects dependencies
	bool cond_exp_depend(void)
	{	bool ok = true;
		using CppAD::AD;

		AD<double> zero(0.), one(1.), two(2.), three(3.);
	
		size_t n = 4;
		CppAD::vector< AD<double> > X(n); 
		X[0] = zero;
		X[1] = one;
		X[2] = two;
		X[3] = three;
		CppAD::Independent(X);
	
		size_t m = 4;
		CppAD::vector< AD<double> > Y(m);
		Y[0] = CondExpLt(X[0] + .5,  one,  two, three);
		Y[1] = CondExpLt(zero, X[1] + .5,  two, three);
		Y[2] = CondExpLt(zero,  one, X[2] + .5, three);
		Y[3] = CondExpLt(zero,  one,  two,  X[3] + .5);

		CppAD::ADFun<double> f(X, Y);
		f.optimize();

		CppAD::vector<double> x(n), y(m);
		size_t i;
		for(i = 0; i < n; i++)
			x[i] = double(n - i);
		y    = f.Forward(0, x);

		if( x[0] + .5 < 1. )
			ok  &= y[0] == 2.;
		else	ok  &= y[0] == 3.;
		if( 0. < x[1] + .5 )
			ok  &= y[1] == 2.;
		else	ok  &= y[1] == 3.;
		ok  &= y[2] == x[2] + .5;;
		ok  &= y[3] == 2.;

		return ok;
	}
	// -------------------------------------------------------------------
	void my_union(
		std::set<size_t>&         result  ,
		const std::set<size_t>&   left    ,
		const std::set<size_t>&   right   )
	{	std::set<size_t> temp;
		std::set_union(
			left.begin()              ,
			left.end()                ,
			right.begin()             ,
			right.end()               ,
			std::inserter(temp, temp.begin())
		);
		result.swap(temp);
	}

	bool user_atomic_forward(
		size_t                         id ,
		size_t                          k , 
		size_t                          n ,
		size_t                          m ,
		const CppAD::vector<bool>&     vx ,
		CppAD::vector<bool>&           vy ,
		const CppAD::vector<double>&   tx , 
		CppAD::vector<double>&         ty )
	{	assert(n == 3 && m == 2);
		if( k > 0 ) 
			return false;

		// y[0] = x[0] + x[1]
		ty[0] = tx[0] + tx[1];

		// y[1] = x[1] + x[2]
		ty[1] = tx[1] + tx[2];
		
		if( vy.size() > 0 )
		{	vy[0] = (vx[0] | vx[1]);
			vy[1] = (vx[1] | vx[2]);
		}
		return true; 
	}

	bool user_atomic_reverse(
		size_t                         id ,
		size_t                          k , 
		size_t                          n , 
		size_t                          m , 
		const CppAD::vector<double>&   tx , 
		const CppAD::vector<double>&   ty ,
		CppAD::vector<double>&         px ,
		const CppAD::vector<double>&   py )
	{	return false; }

	bool user_atomic_for_jac_sparse(
		size_t                                  id ,
		size_t                                   n ,
		size_t                                   m ,
		size_t                                   q ,
		const CppAD::vector< std::set<size_t> >& r ,
		CppAD::vector< std::set<size_t>  >&      s )
	{	return false; }

	bool user_atomic_rev_jac_sparse(
		size_t                                  id ,
		size_t                                   n ,
		size_t                                   m ,
		size_t                                   q ,
		CppAD::vector< std::set<size_t> >&       r ,
		const CppAD::vector< std::set<size_t> >& s )
	{	assert(n == 3 && m == 2);
		r[0].clear();
		r[1].clear();
		r[2].clear();
		// y[0] = x[0] + x[1]
		my_union(r[0], r[0], s[0]);
		my_union(r[1], r[1], s[0]);
		// y[1] = x[1] + x[2]
		my_union(r[1], r[1], s[1]);
		my_union(r[2], r[2], s[1]);

		return true; 
	}

	bool user_atomic_rev_hes_sparse(
		size_t                                  id ,
		size_t                                   n ,
		size_t                                   m ,
		size_t                                   q ,
		const CppAD::vector< std::set<size_t> >& r ,
		const CppAD::vector<bool>&               s ,
		CppAD::vector<bool>&                     t ,
		const CppAD::vector< std::set<size_t> >& u ,
		CppAD::vector< std::set<size_t> >&       v )
	{	return false; }

	CPPAD_USER_ATOMIC(
		my_user_atomic             ,
		CppAD::vector              ,
		double                     ,
		user_atomic_forward        ,
		user_atomic_reverse        ,
		user_atomic_for_jac_sparse ,
		user_atomic_rev_jac_sparse ,
		user_atomic_rev_hes_sparse 
	)

	bool user_atomic_test(void)
	{	bool ok = true;

		using CppAD::AD;
		size_t j;
		size_t n = 3;
		size_t m = 2;
		CppAD::vector< AD<double> > ax(n), ay(m), az(m);
		for(j = 0; j < n; j++)
			ax[j] = AD<double>(j + 1);
		CppAD::Independent(ax);

		size_t id = 0;
		// first call should stay in the tape
		my_user_atomic(id++, ax, ay);
		// second call will not get used
		my_user_atomic(id++, ax, az);
		// create function
		CppAD::ADFun<double> g(ax, ay);
		// should have 1 + n + m + m varaibles
		ok &= g.size_var() == (1 + n + m + m);
		g.optimize();
		// should have 1 + n + m varaibles
		ok &= g.size_var() == (1 + n + m);

		// now test that the optimized function gives same results
		CppAD::vector<double> x(n), y(m);
		for(j = 0; j < n; j++)
			x[j] = (j + 1) * (j + 1);
		y = g.Forward(0, x);
		// y[0] = x[0] + x[1]
		ok &= (y[0] == x[0] + x[1]);
		// y[1] = x[1] + x[2]
		ok &= (y[0] == x[0] + x[1]);

		return ok;
	}
	bool not_identically_equal(void)
	{	bool ok = true;
		using CppAD::AD;

		// independent variable vector
		size_t n = 5;
		CppAD::vector< AD<double> > ax(n);
		size_t j;
		for(j = 0; j < n; j++)
			ax[j] = 1. / 3.;
		CppAD::Independent(ax);
	
		// dependent variable vector
		size_t m = 1;
		CppAD::vector< AD<double> > ay(m);
		ay[0]       = 0.;
		for(j = 0; j < n; j++)
		{	if( j % 2 == 0 )
				ay[0] += ax[j];
			else	ay[0] -= ax[j];
		}
		CppAD::ADFun<double> f(ax, ay);
	
		// Used to fail assert in optimize that forward mode results
		// are identically equal
		f.optimize();
	
		return ok;
	}
}

bool optimize(void)
{	bool ok = true;
	// check reverse dependency analysis optimization
	ok     &= depend_one();
	ok     &= depend_two();
	ok     &= depend_three();
	// check removal of duplicate expressions
	ok     &= duplicate_one();
	ok     &= duplicate_two();
	ok     &= duplicate_three();
	ok     &= duplicate_four();
	// convert sequence of additions to cummulative summation
	ok     &= cummulative_sum();
	ok     &= forward_csum();
	ok     &= reverse_csum();
	ok     &= forward_sparse_jacobian_csum();
	ok     &= reverse_sparse_jacobian_csum();
	ok     &= reverse_sparse_hessian_csum();
	// check that CondExp properly detects dependencies
	ok     &= cond_exp_depend();
	// check user_atomic functions
	ok     &= user_atomic_test();
	// case where results are not identically equal
	ok     &= not_identically_equal();

	CppAD::user_atomic<double>::clear();
	return ok;
}
