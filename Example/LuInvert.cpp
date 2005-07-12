// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT

/*
$begin LuInvert.cpp$$
$spell
	Geq
	Cpp
	Lu
$$

$section LuInvert: Example and Test$$

$index example, LuInvert$$
$index test, LuInvert$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/LuInvert.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/


// BEGIN PROGRAM
# include <cstdlib>               // for rand function
# include <CppAD/LuInvert.h>      // for CppAD::LuInvert
# include <CppAD/NearEqual.h>     // for CppAD::NearEqual
# include <CppAD/CppAD_vector.h>  // for CppAD::vector

bool LuInvert(void)
{	bool  ok = true;

# ifndef _MSC_VER
	using std::rand;
	using std::srand;
# endif

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
				X[i * m + k], B[i * m + k], 1e-10, 1e-10
			);
		}
	}
	return ok;
}

// END PROGRAM
