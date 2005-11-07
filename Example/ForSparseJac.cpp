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

/*
$begin ForSparseJac.cpp$$
$spell
	Jacobian
	Jac
	Dep
	Cpp
$$

$section Forward Mode Jacobian Dependency: Example and Test$$
$index ForSparseJac$$
$index example, forward Jacobian sparsity$$
$index example, Jacobian  forward sparsity$$
$index test, forward Jacobian sparsity$$
$index test, Jacobian forward sparsity$$

$code
$verbatim%Example/ForSparseJac.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <vector>
# include <valarray>


namespace { // Begin empty namespace
template <typename VectorBool> // vector class, elements of type bool
bool ForSparseJacCases(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 2; 

	// dimension of the range space
	size_t m = 3;

	// independent variable vector 
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;
	Independent(X);

	// compute product of elements in X
	CppADvector< AD<double> > Y(m);
	Y[0] = X[0];
	Y[1] = X[0] * X[1];
	Y[2] = X[1];

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// sparsity pattern for the identity function U(x) = x
	VectorBool Px(n * n);
	size_t i, j;
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			Px[ i * n + j ] = false;
		Px[ i * n + i ] = true;
	}

	// sparsity pattern for F(X(x))
	VectorBool Py(m * n);
	Py = F.ForSparseJac(n, Px);

	// check values
	ok &= (Py[ 0 * n + 0 ] == true);  // Y[0] does     depend on X[0]
	ok &= (Py[ 0 * n + 1 ] == false); // Y[0] does not depend on X[1]
	ok &= (Py[ 1 * n + 0 ] == true);  // Y[1] does     depend on X[0]
	ok &= (Py[ 1 * n + 1 ] == true);  // Y[1] does     depend on X[1]
	ok &= (Py[ 2 * n + 0 ] == false); // Y[2] does not depend on X[0]
	ok &= (Py[ 2 * n + 1 ] == true);  // Y[2] does     depend on X[1]

	return ok;
}
} // End empty namespace

bool ForSparseJac(void)
{	bool ok = true;

	ok &= ForSparseJacCases< CppAD::vectorBool     >();
	ok &= ForSparseJacCases< CppAD::vector  <bool> >();
	ok &= ForSparseJacCases< std::vector    <bool> >(); 
	ok &= ForSparseJacCases< std::valarray  <bool> >(); 

	return ok;
}


// END PROGRAM
