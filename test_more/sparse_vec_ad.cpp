/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


# include <cppad/cppad.hpp>

bool SparseVecAD(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 3; 

	size_t i, j;

	// independent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double>(j); 
	Independent(X);

	// dependent variable vector
	CPPAD_TEST_VECTOR< AD<double> > Y(n);

	// check results vector
	CPPAD_TEST_VECTOR< bool >  Check(n * n);

	// VecAD equal to X
	VecAD<double> Z(n);
	AD<double> J;
	for(j = 0; j < n; j++)
	{	J    = AD<double>(j);
		Z[J] = X[j];
	}

	// compute dependent variables values
	AD<double> P = 1;
	J = AD<double>(0);
	for(j = 0; j < n; j++)
	{	P    = P * Z[J];
		Y[j] = P;
		for(i = 0; i < n; i++)
			Check[ i * n + j ] = (j <= i);
		J = J + X[1];  // X[1] is equal to one
	}
	
	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// dependency matrix for the identity function W(x) = x
	CPPAD_TEST_VECTOR< bool > Identity(n * n);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			Identity[ i * n + j ] = false;
		Identity[ i * n + i ] = true;
	}

	// evaluate the dependency matrix for F(Identity(x))
	CPPAD_TEST_VECTOR< bool > Py(n * n);
	Py = F.ForSparseJac(n, Identity);

	// check values
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Py[i * n + j] == Check[i * n + j]);
	}	

	// evaluate the dependency matrix for Identity(F(x))
	CPPAD_TEST_VECTOR< bool > Px(n * n);
	Px = F.RevSparseJac(n, Identity);

	// check values
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Px[i * n + j] == Check[i * n + j]);
	}	

	return ok;
}
