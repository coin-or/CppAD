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
$begin BoolFun.cpp$$
$spell
	bool
	Geq
	Cpp
$$

$section AD Boolean Functions: Example and Test$$

$index example, AD bool$$
$index test, AD bool$$

$code
$verbatim%Example/BoolFun.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <complex>


// define abbreviation for double precision complex 
typedef std::complex<double> Complex;

namespace {
	// a unary bool function with Complex argument
	static bool IsReal(const Complex &x)
	{	return x.imag() == 0.; }

	// a binary bool function with Complex arguments
	static bool AbsGeq(const Complex &x, const Complex &y)
	{	double axsq = x.real() * x.real() + x.imag() * x.imag();
		double aysq = y.real() * y.real() + y.imag() * y.imag();

		return axsq >= aysq;
	}

	// Create version of IsReal with AD<Complex> argument
	// inside of namespace and outside of any other function.
	CppADCreateUnaryBool(Complex, IsReal)

	// Create version of AbsGeq with AD<Complex> arguments
	// inside of namespace and outside of any other function.
	CppADCreateBinaryBool(Complex, AbsGeq)

}
bool BoolFun(void)
{	bool ok = true;

	CppAD::AD<Complex> x = Complex(1.,  0.);
	CppAD::AD<Complex> y = Complex(1.,  1.);

	ok &= IsReal(x);
	ok &= ! AbsGeq(x, y);

	return ok;
}

// END PROGRAM
