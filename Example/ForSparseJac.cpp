/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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

/*
$begin ForSparseJac.cpp$$
$spell
	Jacobian
	Jac
	Dep
	Cpp
$$

$section Forward Mode Jacobian Sparsity: Example and Test$$

$index ForSparseJac$$
$index example, sparsity forward$$
$index test, sparsity forward$$
$index sparsity, forward example$$

$code
$verbatim%Example/ForSparseJac.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
namespace { // -------------------------------------------------------------
// define the template function ForSparseJacCases<Vector> in empty namespace
template <typename Vector> 
bool ForSparseJacCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2; 
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 3;
	CppADvector< AD<double> > Y(m);
	Y[0] = X[0];
	Y[1] = X[0] * X[1];
	Y[2] = X[1];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// sparsity pattern for the identity matrix
	Vector r(n * n);
	size_t i, j;
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			r[ i * n + j ] = false;
		r[ i * n + i ] = true;
	}

	// sparsity pattern for F'(x)
	Vector s(m * n);
	s = f.ForSparseJac(n, r);

	// check values
	ok &= (s[ 0 * n + 0 ] == true);  // Y[0] does     depend on X[0]
	ok &= (s[ 0 * n + 1 ] == false); // Y[0] does not depend on X[1]
	ok &= (s[ 1 * n + 0 ] == true);  // Y[1] does     depend on X[0]
	ok &= (s[ 1 * n + 1 ] == true);  // Y[1] does     depend on X[1]
	ok &= (s[ 2 * n + 0 ] == false); // Y[2] does not depend on X[0]
	ok &= (s[ 2 * n + 1 ] == true);  // Y[2] does     depend on X[1]

	return ok;
}
} // End empty namespace
# include <vector>
# include <valarray>
bool ForSparseJac(void)
{	bool ok = true;
	// Run with Vector equal to four different cases
	// all of which are Simple Vectors with elements of type bool.
	ok &= ForSparseJacCases< CppAD::vectorBool     >();
	ok &= ForSparseJacCases< CppAD::vector  <bool> >();
	ok &= ForSparseJacCases< std::vector    <bool> >(); 
	ok &= ForSparseJacCases< std::valarray  <bool> >(); 

	return ok;
}

// END PROGRAM
