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
$begin GetStarted.cpp$$
$spell
	Cpp
	getstarted
	namespace
	iostream
	const
	std
	powz
	Jacobian
	endl
	da
	cout
$$

$section Getting Started Using CppAD$$
$index getstarted$$
$index simple, example$$
$index example, simple$$
$index start, using CppAD$$

$head Description$$
Use CppAD to compute the derivative of the function
$latex f : \R \rightarrow \R$$ defined by 
$latex \[
	f(z) = a_0 + a_1 z^1 + \cdots + a_{k-1} z^{k-1}
\] $$
where $italic a$$ is a vector of length $italic k$$.

$head Poly$$
The routine $code Poly$$ defined below (in the empty namespace)
is intended to be simple to understand.
A better version $xref/Poly/$$ is distributed as part of the
CppAD include library.


$head Source Code$$
$codep */
# include <iostream>      // standard input output library
# include <vector>        // standard vector
# include <CppAD/CppAD.h> // CppAD package

namespace { // begin definition of Poly(a, z)
	template <class Type>
	Type Poly(const std::vector<Type> &a, const Type &z)
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
{	using CppAD::AD;   // so we can use AD in place of CppAD::AD
	using std::vector; // so we can use vector in place of std::vector
	size_t n = 1;      // number of independent variables (domain dim)
	size_t m = 1;      // number of dependent variables (range dim)
	size_t k = 5;      // number of coefficients in poly for f(z)

	// polynomial coefficients for f(z)
	vector< AD<double> > A(k);  // as AD<double> elements
	vector<double>       a(k);  // as double     elements
	size_t i;
	for(i = 0; i < k; i++)
		A[i] = a[i] = 1.;   // value of the coefficients

	// independent variables
	vector< AD<double> > Z(n); // first create a vector for, then 
	Z[0] = 3.;                 // set the value of, and then
	CppAD::Independent(Z);     // declare the independent variables

	// first create a vector for the dependent variables
	vector< AD<double> > P(m);      

	// set the dependent variable using use Poly with Type = AD<double> 
	P[0] = Poly(A, Z[0]);

	// declare the dependent variables: f : Z -> P 
	CppAD::ADFun<double> f(Z, P);

	// use CppAD to compute derivative of polynomial
	vector<double> J(m * n);   // an m by n matrix for the Jacobian of f
	vector<double> z(n);       // argument value to compute Jacobian at
	z[0] = 3.;                 // placed in z[0]
	J    = f.Jacobian(z);      // value of Jacobian at z

	// use Poly with Type = double to evaluate polynomial for da
	vector<double> da(k-1);               // coefficients for derivative 
	for(i = 0; i < (k-1); i++)
		da[i] = double(i+1) * a[i+1]; // derivative coefficient
	double dp = Poly(da, z[0]);           // value of derivative at z[0]

	// print the results
	std::cout << "f'(z) computed by CppAD = " << J[0] << std::endl;
	std::cout << "f'(z) computed by Poly  = " << dp << std::endl;

	// return value not significant
	return 0;
}
/* $$

$head Output$$
The program above will generate the following output:
$code
$verbatim%GetStarted/GetStarted.out%$$
$$

$end
*/
