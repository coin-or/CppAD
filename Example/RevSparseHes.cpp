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
$index example, sparsity Hessian$$
$index test, sparsity Hessian$$
$index sparsity, Hessian$$

$code
$verbatim%Example/RevSparseHes.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
namespace { // -------------------------------------------------------------
// define the template function RevSparseHesCases<Vector> in empty namespace
template <typename Vector> // vector class, elements of type bool
bool RevSparseHesCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 3; 
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;
	X[2] = 2.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 2;
	CppADvector< AD<double> > Y(m);
	Y[0] = sin( X[2] );
	Y[1] = X[0] * X[1];

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

	// compute sparsity pattern for J(x) = F^{(1)} (x)
	f.ForSparseJac(n, r);

	// compute sparsity pattern for H(x) = F_0^{(2)} (x)
	Vector s(m);
	for(i = 0; i < m; i++)
		s[i] = false;
	s[0] = true;
	Vector h(n * n);
	h    = f.RevSparseHes(n, s);

	// check values
	ok &= (h[ 0 * n + 0 ] == false);  // second partial w.r.t X[0], X[0]
	ok &= (h[ 0 * n + 1 ] == false);  // second partial w.r.t X[0], X[1]
	ok &= (h[ 0 * n + 2 ] == false);  // second partial w.r.t X[0], X[2]

	ok &= (h[ 1 * n + 0 ] == false);  // second partial w.r.t X[1], X[0]
	ok &= (h[ 1 * n + 1 ] == false);  // second partial w.r.t X[1], X[1]
	ok &= (h[ 1 * n + 2 ] == false);  // second partial w.r.t X[1], X[2]

	ok &= (h[ 2 * n + 0 ] == false);  // second partial w.r.t X[2], X[0]
	ok &= (h[ 2 * n + 1 ] == false);  // second partial w.r.t X[2], X[1]
	ok &= (h[ 2 * n + 2 ] == true);   // second partial w.r.t X[2], X[2]

	// compute sparsity pattern for H(x) = F_1^{(2)} (x)
	for(i = 0; i < m; i++)
		s[i] = false;
	s[1] = true;
	h    = f.RevSparseHes(n, s);

	// check values
	ok &= (h[ 0 * n + 0 ] == false);  // second partial w.r.t X[0], X[0]
	ok &= (h[ 0 * n + 1 ] == true);   // second partial w.r.t X[0], X[1]
	ok &= (h[ 0 * n + 2 ] == false);  // second partial w.r.t X[0], X[2]

	ok &= (h[ 1 * n + 0 ] == true);   // second partial w.r.t X[1], X[0]
	ok &= (h[ 1 * n + 1 ] == false);  // second partial w.r.t X[1], X[1]
	ok &= (h[ 1 * n + 2 ] == false);  // second partial w.r.t X[1], X[2]

	ok &= (h[ 2 * n + 0 ] == false);  // second partial w.r.t X[2], X[0]
	ok &= (h[ 2 * n + 1 ] == false);  // second partial w.r.t X[2], X[1]
	ok &= (h[ 2 * n + 2 ] == false);  // second partial w.r.t X[2], X[2]

	return ok;
}
} // End empty namespace

# include <vector>
# include <valarray>
bool RevSparseHes(void)
{	bool ok = true;
	// Run with Vector equal to four different cases
	// all of which are Simple Vectors with elements of type bool.
	ok &= RevSparseHesCases< CppAD::vector  <bool> >();
	ok &= RevSparseHesCases< CppAD::vectorBool     >();
	ok &= RevSparseHesCases< std::vector    <bool> >(); 
	ok &= RevSparseHesCases< std::valarray  <bool> >(); 

	return ok;
}


// END PROGRAM
