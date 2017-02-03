/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


# include <cppad/cppad.hpp>
# include <vector>
# include <valarray>

// ============================================================================
// old tests
// ============================================================================

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

namespace { // BEGIN empty namespace

bool old_one()
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 3;

	// dimension of the range space
	size_t m = (4 + 11 + 1) * 3 + 4;

	// independent variable vector
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = .1;
	X[1] = .2;
	X[2] = .3;
	Independent(X);

	// dependent variable vector
	CPPAD_TESTVECTOR(AD<double>) Y(m);

	// check results vector
	CPPAD_TESTVECTOR( bool )       Check(m * n);

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
	Check[index * n + 1] = true;
	Check[index * n + 2] = false;
	index++;
	Y[index] = CondExpLt(X[0], X[1], AD<double>(3.), X[1]);
	Check[index * n + 0] = false;
	Check[index * n + 1] = true;
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

	// --------------------------------------------------------
	// dependency matrix for the identity function U(y) = y
	CPPAD_TESTVECTOR( bool ) Py(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			Py[ i * m + j ] = false;
		Py[ i * m + i ] = true;
	}

	// evaluate the dependency matrix for F(x)
	CPPAD_TESTVECTOR( bool ) Px(m * n);
	Px = F.RevSparseJac(m, Py);

	// check values
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Px[i * n + j] == Check[i * n + j]);
	}
	// --------------------------------------------------------
	// dependency matrix for the identity function U(y) = y
	CPPAD_TESTVECTOR(std::set<size_t>) Sy(m);
	for(i = 0; i < m; i++)
	{	assert( Sy[i].empty() );
		Sy[i].insert(i);
	}

	// evaluate the dependency matrix for U(F(x))
	CPPAD_TESTVECTOR(std::set<size_t>) Sx(m);
	Sx = F.RevSparseJac(m, Sy);

	// check values
	std::set<size_t>::iterator itr;
	bool found;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	found = Sx[i].find(j) != Sx[i].end();
			ok    &= (found == Check[i * n + j]);
		}
	}

	return ok;
}

bool old_two()
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 3;

	// dimension of the range space
	size_t m = 3;

	// inialize the vector as zero
	CppAD::VecAD<double> Z(n - 1);
	size_t k;
	for(k = 0; k < n-1; k++)
		Z[k] = 0.;

	// independent variable vector
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 0.;
	X[1] = 1.;
	X[2] = 2.;
	Independent(X);

	// VecAD vector is going to depend on X[1] and X[2]
	Z[ X[0] ] = X[1];
	Z[ X[1] ] = X[2];

	// dependent variable vector
	CPPAD_TESTVECTOR(AD<double>) Y(m);

	// check results vector
	CPPAD_TESTVECTOR( bool )       Check(m * n);

	// initialize index into Y
	size_t index = 0;

	// First component only depends on X[0];
	Y[index]             = X[0];
	Check[index * n + 0] = true;
	Check[index * n + 1] = false;
	Check[index * n + 2] = false;
	index++;

	// Second component depends on the vector Z
	AD<double> zero(0);
	Y[index]             = Z[zero]; // Load by a parameter
	Check[index * n + 0] = false;
	Check[index * n + 1] = true;
	Check[index * n + 2] = true;
	index++;

	// Third component depends on the vector Z
	Y[index]             = Z[ X[0] ]; // Load by a variable
	Check[index * n + 0] = false;
	Check[index * n + 1] = true;
	Check[index * n + 2] = true;
	index++;

	// check final index
	assert( index == m );

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// dependency matrix for the identity function S(y) = y
	CPPAD_TESTVECTOR( bool ) Py(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			Py[ i * m + j ] = false;
		Py[ i * m + i ] = true;
	}

	// evaluate the dependency matrix for S [ F(x) ]
	CPPAD_TESTVECTOR( bool ) Px(m * n);
	Px = F.RevSparseJac(m, Py);

	// check values
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Px[i * n + j] == Check[i * n + j]);
	}
	// --------------------------------------------------------
	// dependency matrix for the identity function U(y) = y
	CPPAD_TESTVECTOR(std::set<size_t>) Sy(m);
	for(i = 0; i < m; i++)
	{	assert( Sy[i].empty() );
		Sy[i].insert(i);
	}

	// evaluate the dependency matrix for U(F(x))
	CPPAD_TESTVECTOR(std::set<size_t>) Sx(m);
	Sx = F.RevSparseJac(m, Sy);

	// check values
	std::set<size_t>::iterator itr;
	bool found;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	found = Sx[i].find(j) != Sx[i].end();
			ok    &= (found == Check[i * n + j]);
		}
	}

	return ok;
}

bool old_three()
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 2;

	// dimension of the range space
	size_t m = 3;

	// independent variable vector
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 2.;
	X[1] = 3.;
	Independent(X);

	// dependent variable vector
	CPPAD_TESTVECTOR(AD<double>) Y(m);

	// check results vector
	CPPAD_TESTVECTOR( bool )       Check(m * n);

	// initialize index into Y
	size_t index = 0;

	// Y[0] only depends on X[0];
	Y[index]             = pow(X[0], 2.);
	Check[index * n + 0] = true;
	Check[index * n + 1] = false;
	index++;

	// Y[1] depends on X[1]
	Y[index]             = pow(2., X[1]);
	Check[index * n + 0] = false;
	Check[index * n + 1] = true;
	index++;

	// Y[2] depends on X[0] and X[1]
	Y[index]             = pow(X[0], X[1]);
	Check[index * n + 0] = true;
	Check[index * n + 1] = true;
	index++;

	// check final index
	assert( index == m );

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// -----------------------------------------------------------------
	// dependency matrix for the identity function
	CPPAD_TESTVECTOR( bool ) Py(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			Py[ i * m + j ] = (i == j);
	}

	// evaluate the dependency matrix for F(x)
	CPPAD_TESTVECTOR( bool ) Px(m * n);
	Px = F.RevSparseJac(m, Py);

	// check values
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Px[i * n + j] == Check[i * n + j]);
	}

	// ---------------------------------------------------------
	// dependency matrix for the identity function
	CPPAD_TESTVECTOR(std::set<size_t>) Sy(m);
	for(i = 0; i < m; i++)
	{	assert( Sy[i].empty() );
		Sy[i].insert(i);
	}

	// evaluate the dependency matrix for F(x)
	CPPAD_TESTVECTOR(std::set<size_t>) Sx(m);
	Sx = F.RevSparseJac(m, Sy);

	// check values
	bool found;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	found = Sx[i].find(j) != Sx[i].end();
			ok &= (found == Check[i * n + j]);
		}
	}

	return ok;
}


// case where s is not identity matrix
bool old_four()
{
	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 2;

	// dimension of the range space
	size_t m = n;

	// independent and variable vectors
	CPPAD_TESTVECTOR(AD<double>) ax(n), ay(m);
	ax[0] = 2.;
	ax[1] = 3.;
	Independent(ax);
	ay[0] = ax[1];
	ay[1] = ax[0];

	// create function object F : x -> y
	ADFun<double> F(ax, ay);

	// evaluate the dependency matrix for F(x)
	size_t q = 1;
	CPPAD_TESTVECTOR( bool ) s(q * m), r(q * n);
	s[0] = true;
	s[1] = false;
	r = F.RevSparseJac(q, s);

	ok &= size_t( r.size() ) == q * n;
	ok &= r[0] == false;
	ok &= r[1] == true;

	return ok;
}

bool old_five()
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 2;

	// dimension of the range space
	size_t m = 3;

	// independent variable vector
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 2.;
	X[1] = 3.;
	Independent(X);

	// dependent variable vector
	CPPAD_TESTVECTOR(AD<double>) Y(m);

	// check results vector
	CPPAD_TESTVECTOR( bool )       Check(m * n);

	// initialize index into Y
	size_t index = 0;

	// Y[0] only depends on X[0];
	Y[index]             = pow(X[0], 2.);
	Check[index * n + 0] = true;
	Check[index * n + 1] = false;
	index++;

	// Y[1] depends on X[1]
	Y[index]             = pow(2., X[1]);
	Check[index * n + 0] = false;
	Check[index * n + 1] = true;
	index++;

	// Y[2] depends on X[0] and X[1]
	Y[index]             = pow(X[0], X[1]);
	Check[index * n + 0] = true;
	Check[index * n + 1] = true;
	index++;

	// check final index
	assert( index == m );

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// -----------------------------------------------------------------
	// dependency matrix for the identity function
	CPPAD_TESTVECTOR( bool ) Py(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			Py[ i * m + j ] = (i == j);
	}

	// evaluate the dependency matrix for F(x)
	bool transpose = true;
	CPPAD_TESTVECTOR( bool ) Px(n * m);
	Px = F.RevSparseJac(m, Py, transpose);

	// check values
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Px[j * m + i] == Check[i * n + j]);
	}

	// ---------------------------------------------------------
	// dependency matrix for the identity function
	CPPAD_TESTVECTOR(std::set<size_t>) Sy(m);
	for(i = 0; i < m; i++)
	{	assert( Sy[i].empty() );
		Sy[i].insert(i);
	}

	// evaluate the dependency matrix for F(x)
	CPPAD_TESTVECTOR(std::set<size_t>) Sx(n);
	Sx = F.RevSparseJac(m, Sy, transpose);

	// check values
	bool found;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	found = Sx[j].find(i) != Sx[j].end();
			ok &= (found == Check[i * n + j]);
		}
	}

	return ok;
}
} // END empty namespace
// ============================================================================
// Deprecated examples
// ============================================================================
# include <cppad/cppad.hpp>
namespace { // -------------------------------------------------------------
// define the template function BoolCases<Vector>
template <typename Vector>  // vector class, elements of type bool
bool BoolCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.;
	ax[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = ax[0];
	ay[1] = ax[0] * ax[1];
	ay[2] = ax[1];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// sparsity pattern for the identity matrix
	Vector r(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			r[ i * m + j ] = (i == j);
	}

	// sparsity pattern for F'(x)
	Vector s(m * n);
	s = f.RevSparseJac(m, r);

	// check values
	ok &= (s[ 0 * n + 0 ] == true);  // y[0] does     depend on x[0]
	ok &= (s[ 0 * n + 1 ] == false); // y[0] does not depend on x[1]
	ok &= (s[ 1 * n + 0 ] == true);  // y[1] does     depend on x[0]
	ok &= (s[ 1 * n + 1 ] == true);  // y[1] does     depend on x[1]
	ok &= (s[ 2 * n + 0 ] == false); // y[2] does not depend on x[0]
	ok &= (s[ 2 * n + 1 ] == true);  // y[2] does     depend on x[1]

	// sparsity pattern for F'(x)^T, note R is the identity, so R^T = R
	bool transpose = true;
	Vector st(n * m);
	st = f.RevSparseJac(m, r, transpose);

	// check values
	ok &= (st[ 0 * m + 0 ] == true);  // y[0] does     depend on x[0]
	ok &= (st[ 1 * m + 0 ] == false); // y[0] does not depend on x[1]
	ok &= (st[ 0 * m + 1 ] == true);  // y[1] does     depend on x[0]
	ok &= (st[ 1 * m + 1 ] == true);  // y[1] does     depend on x[1]
	ok &= (st[ 0 * m + 2 ] == false); // y[2] does not depend on x[0]
	ok &= (st[ 1 * m + 2 ] == true);  // y[2] does     depend on x[1]

	return ok;
}
// define the template function SetCases<Vector>
template <typename Vector>  // vector class, elements of type std::set<size_t>
bool SetCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.;
	ax[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = ax[0];
	ay[1] = ax[0] * ax[1];
	ay[2] = ax[1];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// sparsity pattern for the identity matrix
	Vector r(m);
	size_t i;
	for(i = 0; i < m; i++)
	{	assert( r[i].empty() );
		r[i].insert(i);
	}

	// sparsity pattern for F'(x)
	Vector s(m);
	s = f.RevSparseJac(m, r);

	// check values
	bool found;

	// y[0] does     depend on x[0]
	found = s[0].find(0) != s[0].end();  ok &= (found == true);
	// y[0] does not depend on x[1]
	found = s[0].find(1) != s[0].end();  ok &= (found == false);
	// y[1] does     depend on x[0]
	found = s[1].find(0) != s[1].end();  ok &= (found == true);
	// y[1] does     depend on x[1]
	found = s[1].find(1) != s[1].end();  ok &= (found == true);
	// y[2] does not depend on x[0]
	found = s[2].find(0) != s[2].end();  ok &= (found == false);
	// y[2] does     depend on x[1]
	found = s[2].find(1) != s[2].end();  ok &= (found == true);

	// sparsity pattern for F'(x)^T
	bool transpose = true;
	Vector st(n);
	st = f.RevSparseJac(m, r, transpose);

	// y[0] does     depend on x[0]
	found = st[0].find(0) != st[0].end();  ok &= (found == true);
	// y[0] does not depend on x[1]
	found = st[1].find(0) != st[1].end();  ok &= (found == false);
	// y[1] does     depend on x[0]
	found = st[0].find(1) != st[0].end();  ok &= (found == true);
	// y[1] does     depend on x[1]
	found = st[1].find(1) != st[1].end();  ok &= (found == true);
	// y[2] does not depend on x[0]
	found = st[0].find(2) != st[0].end();  ok &= (found == false);
	// y[2] does     depend on x[1]
	found = st[1].find(2) != st[1].end();  ok &= (found == true);

	return ok;
}
} // End empty namespace

# include <vector>
# include <valarray>

bool rev_sparse_jac(void)
{	bool ok = true;

	ok &= old_one();
	ok &= old_two();
	ok &= old_three();
	ok &= old_four();
	ok &= old_five();

	// Run with Vector equal to four different cases
	// all of which are Simple Vectors with elements of type bool.
	ok &= BoolCases< CppAD::vectorBool     >();
	ok &= BoolCases< CppAD::vector  <bool> >();
	ok &= BoolCases< std::vector    <bool> >();
	ok &= BoolCases< std::valarray  <bool> >();


	// Run with Vector equal to two different cases both of which are
	// Simple Vectors with elements of type std::set<size_t>
	typedef std::set<size_t> set;
	ok &= SetCases< CppAD::vector  <set> >();
	ok &= SetCases< std::vector    <set> >();

	// Do not use valarray because its element access in the const case
	// returns a copy instead of a reference
	// ok &= SetCases< std::valarray  <set> >();

	return ok;
}
