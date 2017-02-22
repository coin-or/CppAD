/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin lu_invert.cpp$$
$spell
	Geq
	Cpp
	Lu
$$

$section LuInvert: Example and Test$$
$mindex LuInvert$$


$code
$srcfile%example/utility/lu_invert.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/


// BEGIN C++
# include <cstdlib>               // for rand function
# include <cppad/utility/lu_invert.hpp>      // for CppAD::LuInvert
# include <cppad/utility/near_equal.hpp>     // for CppAD::NearEqual
# include <cppad/utility/vector.hpp>  // for CppAD::vector

bool LuInvert(void)
{	bool  ok = true;

# ifndef _MSC_VER
	using std::rand;
	using std::srand;
# endif
	double eps200 = 200.0 * std::numeric_limits<double>::epsilon();

	size_t  n = 7;                        // number rows in A
	size_t  m = 3;                        // number columns in B
	double  rand_max = double(RAND_MAX);  // maximum rand value
	double  sum;                          // element of L * U
	size_t  i, j, k;                      // temporary indices

	// dimension matrices
	CppAD::vector<double>
		A(n*n), X(n*m), B(n*m), LU(n*n), L(n*n), U(n*n);

	// seed the random number generator
	srand(123);

	// pivot vectors
	CppAD::vector<size_t> ip(n);
	CppAD::vector<size_t> jp(n);

	// set pivot vectors
	for(i = 0; i < n; i++)
	{	ip[i] = (i + 2) % n;      // ip = 2 , 3, ... , n-1, 0, 1
		jp[i] = (n + 2 - i) % n;  // jp = 2 , 1, n-1, n-2, ... , 3
	}

	// chose L, a random lower triangular matrix
	for(i = 0; i < n; i++)
	{	for(j = 0; j <= i; j++)
			L [i * n + j]  = rand() / rand_max;
		for(j = i+1; j < n; j++)
			L [i * n + j]  = 0.;
	}
	// chose U, a random upper triangular matrix with ones on diagonal
	for(i = 0; i < n; i++)
	{	for(j = 0; j < i; j++)
			U [i * n + j]  = 0.;
		U[ i * n + i ] = 1.;
		for(j = i+1; j < n; j++)
			U [i * n + j]  = rand() / rand_max;
	}
	// chose X, a random matrix
	for(i = 0; i < n; i++)
	{	for(k = 0; k < m; k++)
			X[i * m + k] = rand() / rand_max;
	}
	// set LU to a permuted combination of both L and U
	for(i = 0; i < n; i++)
	{	for(j = 0; j <= i; j++)
			LU [ ip[i] * n + jp[j] ]  = L[i * n + j];
		for(j = i+1; j < n; j++)
			LU [ ip[i] * n + jp[j] ]  = U[i * n + j];
	}
	// set A to a permuted version of L * U
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
		{	// compute (i,j) entry in permuted matrix
			sum = 0.;
			for(k = 0; k < n; k++)
				sum += L[i * n + k] * U[k * n + j];
			A[ ip[i] * n + jp[j] ] = sum;
		}
	}
	// set B to A * X
	for(i = 0; i < n; i++)
	{	for(k = 0; k < m; k++)
		{	// compute (i,k) entry of B
			sum = 0.;
			for(j = 0; j < n; j++)
				sum += A[i * n + j] * X[j * m + k];
			B[i * m + k] = sum;
		}
	}
	// solve for X
	CppAD::LuInvert(ip, jp, LU, B);

	// check result
	for(i = 0; i < n; i++)
	{	for(k = 0; k < m; k++)
		{	ok &= CppAD::NearEqual(
				X[i * m + k], B[i * m + k], eps200, eps200
			);
		}
	}
	return ok;
}

// END C++
