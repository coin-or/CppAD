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
$begin Value.cpp$$
$spell
	Cpp
	cstddef
$$

$section Conversion to Base Type: Example and Test$$
$index Base Value$$
$index example, Value$$
$index test, Value$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/Value.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Value(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s] = 3.;
	U[t] = 4.;
	Independent(U);

	// cannot call Value after Independent (tape is recording)

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(1);
	size_t x = 0;

	// dependent variable values
	Z[x] = - U[t];

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// can call Value after ADFun constructor (tape is no longer recording)

	// check value of s
	double sValue = Value(U[s]);
	ok &= ( sValue == 3. );

	// check value of x
	double xValue = Value(Z[x]);
	ok &= ( xValue == -4. );

	return ok;
}
// END PROGRAM
