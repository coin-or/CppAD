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
$begin RevSparseJac.cpp$$
$spell
	Jacobian
	Jac
	Dep
	Cpp
$$

$section Reverse Mode Jacobian Sparsity: Example and Test$$
$index RevSparseJac$$
$index example, reverse Jacobian sparsity$$
$index example, Jacobian  reverse sparsity$$
$index test, reverse Jacobian sparsity$$
$index test, Jacobian reverse sparsity$$

$code
$verbatim%Example/RevSparseJac.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <vector>
# include <valarray>


namespace { // Begin empty namespace
template <typename VectorBool> // vector class, elements of type bool
bool RevSparseJacCases(void)
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

	// sparsity pattern for the identity function U(y) = y
	VectorBool Py(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			Py[ i * m + j ] = false;
		Py[ i * m + i ] = true;
	}

	// sparsity pattern for U(F(x))
	VectorBool Px(m * n);
	Px = F.RevSparseJac(m, Py);

	// check values
	ok &= (Px[ 0 * n + 0 ] == true);  // Y[0] does     depend on X[0]
	ok &= (Px[ 0 * n + 1 ] == false); // Y[0] does not depend on X[1]
	ok &= (Px[ 1 * n + 0 ] == true);  // Y[1] does     depend on X[0]
	ok &= (Px[ 1 * n + 1 ] == true);  // Y[1] does     depend on X[1]
	ok &= (Px[ 2 * n + 0 ] == false); // Y[2] does not depend on X[0]
	ok &= (Px[ 2 * n + 1 ] == true);  // Y[2] does     depend on X[1]

	return ok;
}
} // End empty namespace

bool RevSparseJac(void)
{	bool ok = true;

	ok &= RevSparseJacCases< CppAD::vectorBool     >();
	ok &= RevSparseJacCases< CppAD::vector  <bool> >();
	ok &= RevSparseJacCases< std::vector    <bool> >(); 
	ok &= RevSparseJacCases< std::valarray  <bool> >(); 

	return ok;
}


// END PROGRAM
