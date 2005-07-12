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
$begin BoolFun.cpp$$
$spell
	bool
	Geq
	Cpp
$$

$section Defining Boolean Functions with AD Arguments: Example and Test$$
$mindex IsReal AbsGeq$$
$index complex, boolean$$
$index computed, add$$
$index example, boolean$$
$index test, boolean$$

$comment This file is in the Example subdirectory$$ 
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

namespace { // place these functions in the empty namespace

	// create Complex and AD<Complex> versions of IsReal
	static bool IsReal(const Complex &x)
	{	return x.imag() == 0.; }
	CppADCreateUnaryBool(Complex, IsReal)


	// create Complex and AD<Complex> versions of AbsGeq
	static bool AbsGeq(const Complex &x, const Complex &y)
	{	double axsq = x.real() * x.real() + x.imag() * x.imag();
		double aysq = y.real() * y.real() + y.imag() * y.imag();

		return axsq >= aysq;
	}
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
