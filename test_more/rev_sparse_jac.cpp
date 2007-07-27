/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


# include <cppad/cppad.hpp>
# include <vector>
# include <valarray>

# define CheckOp(Op)                   \
	Y[index] = X[0] Op 2.;         \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = false;  \
	Check[index * n + 2] = false;  \
	index++;                       \
	Y[index] = X[0] Op X[1];       \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = true;   \
	Check[index * n + 2] = false;  \
	index++;                       \
	Y[index] = 3.   Op X[1];       \
	Check[index * n + 0] = false;  \
	Check[index * n + 1] = true;   \
	Check[index * n + 2] = false;  \
	index++;

# define CheckUnaryFun(Fun)            \
	Y[index] = Fun(X[0]);          \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = false;  \
	Check[index * n + 2] = false;  \
	index++;                       \
	Y[index] = Fun(X[0] + X[1]);   \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = true;   \
	Check[index * n + 2] = false;  \
	index++;                       \
	Y[index] = Fun(X[1]);          \
	Check[index * n + 0] = false;  \
	Check[index * n + 1] = true;   \
	Check[index * n + 2] = false;  \
	index++;
 

# define CheckBinaryFun(Fun)           \
	Y[index] = Fun( X[0] , 2.);    \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = false;  \
	Check[index * n + 2] = false;  \
	index++;                       \
	Y[index] = Fun( X[0] , X[1]);  \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = true;   \
	Check[index * n + 2] = false;  \
	index++;                       \
	Y[index] = Fun( 3.   , X[1]);  \
	Check[index * n + 0] = false;  \
	Check[index * n + 1] = true;   \
	Check[index * n + 2] = false;  \
	index++;


bool RevSparseJac(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 3; 

	// dimension of the range space
	size_t m = (4 + 11 + 1) * 3 + 4;

	// independent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = .1; 
	X[1] = .2;
	X[2] = .3;
	Independent(X);

	// dependent variable vector
	CPPAD_TEST_VECTOR< AD<double> > Y(m);

	// check results vector
	CPPAD_TEST_VECTOR< bool >       Check(m * n);

	// initialize index into Y
	size_t index = 0;

	// 4 binary operators
	CheckOp(+);
	CheckOp(-);
	CheckOp(*);
	CheckOp(/);

	// 11 unary functions
	CheckUnaryFun(abs);
	CheckUnaryFun(acos);
	CheckUnaryFun(asin);
	CheckUnaryFun(atan);
	CheckUnaryFun(cos);
	CheckUnaryFun(cosh);
	CheckUnaryFun(exp);
	CheckUnaryFun(log);
	CheckUnaryFun(sin);
	CheckUnaryFun(sinh);
	CheckUnaryFun(sqrt);

	// 1 binary function
	CheckBinaryFun(pow);

	// conditional expression
	Y[index] = CondExpLt(X[0], X[1], X[0], AD<double>(2.));
	Check[index * n + 0] = true;
	Check[index * n + 1] = false;
	Check[index * n + 2] = false;
	index++;
	Y[index] = CondExpLt(X[0], X[1], X[0], X[1]);
	Check[index * n + 0] = true;
	Check[index * n + 1] = false;
	Check[index * n + 2] = false;
	index++;
	Y[index] = CondExpLt(X[0], X[1], AD<double>(3.), X[1]);
	Check[index * n + 0] = false;
	Check[index * n + 1] = false;
	Check[index * n + 2] = false;
	index++;

	// non-trival composition
	Y[index] = Y[0] + Y[1] + X[2];
	Check[index * n + 0] = true;
	Check[index * n + 1] = true;
	Check[index * n + 2] = true;
	index++;

	// check final index
	assert( index == m );


	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// dependency matrix for the identity function U(y) = y
	CPPAD_TEST_VECTOR< bool > Py(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			Py[ i * m + j ] = false;
		Py[ i * m + i ] = true;
	}

	// evaluate the dependency matrix for U(F(x))
	CPPAD_TEST_VECTOR< bool > Px(m * n);
	Px = F.RevSparseJac(m, Py);

	// check values
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Px[i * n + j] == Check[i * n + j]);
	}	

	return ok;
}
