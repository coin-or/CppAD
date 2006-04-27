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
$begin GetStarted.cpp$$
$spell
	http://www.coin-or.org/CppAD/
	getstarted
	namespace
	iostream
	const
	std
	powz
	Jacobian
	jac
	endl
	da
	cout
$$

$section Getting Started Using CppAD$$
$index getstarted$$
$index simple, example$$
$index example, simple$$
$index start, using CppAD$$

$head Purpose$$
Demonstrate the use of CppAD by computing the derivative 
of a simple example function.

$head Function$$
The example function $latex f : \R \rightarrow \R$$ is defined by 
$latex \[
	f(z) = a_0 + a_1 * z^1 + \cdots + a_{k-1} * z^{k-1}
\] $$
where $italic a$$ is a vector of length $italic k$$.

$head Derivative$$
The derivative of the example function is given by
$latex \[
	f^{(1)} (z) = a_1 + 2 * a_2 * z +  \cdots + (k-1) * a_{k-1} * z^{k-2} 
\] $$

$head Value$$
For the particular case in the example,
$latex k$$ is equal to 5, $latex a = (1, 1, 1, 1, 1)$$, and $latex z = 3$$.
If follows that 
$latex \[
	f' ( 3 ) = 1 + 2 * 3 + 3 * 3^2 + 4 * 3^3 = 142
\] $$

$head Poly$$
The routine $code Poly$$ is defined below for this particular application.
A general purpose polynomial evaluation routine is documented and
distributed with CppAD (see $xref/Poly/$$).


$head Source Code$$
$codep */
# include <iostream>      // standard input/output 
# include <vector>        // standard vector
# include <CppAD/CppAD.h> // the CppAD package http://www.coin-or.org/CppAD/

namespace { // empty namespace
	template <class Type>
	Type Poly(const std::vector<double> &a, const Type &z)
	{	size_t k  = a.size();
		Type sum  = 0.;
		Type powz = 1.;
		size_t i;
		for(i = 0; i < k; i++)
		{	sum  += a[i] * powz;
			powz *= z;
		}
		return sum;
	}
}
int main(void)             // begin main program
{	using CppAD::AD;   // use AD in place of CppAD::AD
	using std::vector; // use vector in place of std::vector
	size_t i;          // temporary index

	// vector of polynomial coefficients
	size_t k = 5;            // size of the vector a
	vector<double> a(k);     // vector with double elements
	for(i = 0; i < k; i++)
		a[i] = 1.;       // value of the elements of a

	// domain space vector
	size_t n = 1;
	vector< AD<double> > Z(n);
	Z[0] = 3.;

	// declare independent variables and start tape recording
	CppAD::Independent(Z);

	// range space vector
	size_t m = 1;
	vector< AD<double> > P(m);      
	P[0] = Poly(a, Z[0]);

	// create f: Z -> P and stop tape recording
	CppAD::ADFun<double> f(Z, P);

	// compute derivative of f
	vector<double> jac(m * n);   // Jacobian of f (m by n matrix)
	vector<double> z(n);         // argument value to compute Jacobian at
	z[0] = 3.; 
	jac  = f.Jacobian(z);        // value of Jacobian at z

	// print the results
	std::cout << "f'(z) computed by CppAD = " << jac[0] << std::endl;

	// check if the derivative is correct
	int error_code;
	if( jac[0] == 142. )
		error_code = 0;  // correct answer
	else	error_code = 1;  // incorrect answer
	return error_code;
}
/* $$
$head Output$$
The program above will generate the following output:
$code
$verbatim%GetStarted/GetStarted.out%$$
$$
$end
*/
