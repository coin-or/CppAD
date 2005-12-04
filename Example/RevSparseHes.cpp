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
$begin RevSparseHes.cpp$$
$spell
	Hessian
	Jac
	Hes
	Dep
	Cpp
$$

$section Reverse Mode Hessian Sparsity: Example and Test$$
$index RevSparseHes$$
$index example, reverse Hessian sparsity$$
$index example, Hessian  reverse sparsity$$
$index test, reverse Hessian sparsity$$
$index test, Hessian reverse sparsity$$

$code
$verbatim%Example/RevSparseHes.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <vector>
# include <valarray>


namespace { // Begin empty namespace
template <typename VectorBool> // vector class, elements of type bool
bool RevSparseHesCases(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 3; 

	// dimension of the range space
	size_t m = 2;

	// independent variable vector 
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;
	X[2] = 2.;
	Independent(X);

	// compute product of elements in X
	CppADvector< AD<double> > Y(m);
	Y[0] = sin( X[2] );
	Y[1] = X[0] * X[1];

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

	// compute sparsity pattern for Jacobian of F(U(x))
	F.ForSparseJac(n, Px);

	// compute sparsity pattern for Hessian of F_0 ( U(x) ) 
	VectorBool Py(m);
	for(i = 0; i < m; i++)
		Py[i] = false;
	Py[0] = true;
	VectorBool Pxx(n * n);
	Pxx = F.RevSparseHes(n, Py);

	// check values
	ok &= (Pxx[ 0 * n + 0 ] == false);  // second partial w.r.t X[0], X[0]
	ok &= (Pxx[ 0 * n + 1 ] == false);  // second partial w.r.t X[0], X[1]
	ok &= (Pxx[ 0 * n + 2 ] == false);  // second partial w.r.t X[0], X[2]

	ok &= (Pxx[ 1 * n + 0 ] == false);  // second partial w.r.t X[1], X[0]
	ok &= (Pxx[ 1 * n + 1 ] == false);  // second partial w.r.t X[1], X[1]
	ok &= (Pxx[ 1 * n + 2 ] == false);  // second partial w.r.t X[1], X[2]

	ok &= (Pxx[ 2 * n + 0 ] == false);  // second partial w.r.t X[2], X[0]
	ok &= (Pxx[ 2 * n + 1 ] == false);  // second partial w.r.t X[2], X[1]
	ok &= (Pxx[ 2 * n + 2 ] == true);   // second partial w.r.t X[2], X[2]

	// compute sparsity pattern for Hessian of F_1 ( U(x) ) 
	for(i = 0; i < m; i++)
		Py[i] = false;
	Py[1] = true;
	Pxx = F.RevSparseHes(n, Py);

	// check values
	ok &= (Pxx[ 0 * n + 0 ] == false);  // second partial w.r.t X[0], X[0]
	ok &= (Pxx[ 0 * n + 1 ] == true);   // second partial w.r.t X[0], X[1]
	ok &= (Pxx[ 0 * n + 2 ] == false);  // second partial w.r.t X[0], X[2]

	ok &= (Pxx[ 1 * n + 0 ] == true);   // second partial w.r.t X[1], X[0]
	ok &= (Pxx[ 1 * n + 1 ] == false);  // second partial w.r.t X[1], X[1]
	ok &= (Pxx[ 1 * n + 2 ] == false);  // second partial w.r.t X[1], X[2]

	ok &= (Pxx[ 2 * n + 0 ] == false);  // second partial w.r.t X[2], X[0]
	ok &= (Pxx[ 2 * n + 1 ] == false);  // second partial w.r.t X[2], X[1]
	ok &= (Pxx[ 2 * n + 2 ] == false);  // second partial w.r.t X[2], X[2]

	return ok;
}
} // End empty namespace

bool RevSparseHes(void)
{	bool ok = true;

	ok &= RevSparseHesCases< CppAD::vector  <bool> >();
	ok &= RevSparseHesCases< CppAD::vectorBool     >();
	ok &= RevSparseHesCases< std::vector    <bool> >(); 
	ok &= RevSparseHesCases< std::valarray  <bool> >(); 

	return ok;
}


// END PROGRAM
