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
$begin CopyBase.cpp$$
$spell
	Cpp
$$

$section AD Constructor From Base Type: Example and Test$$

$index construct, from base type$$
$index base, convert to AD$$
$index example, construct from base$$
$index test, construct from base$$

$code
$verbatim%Example/CopyBase.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool CopyBase(void)
{	bool ok = true;    // initialize test result flag
	using CppAD::AD;   // so can use AD in place of CppAD::AD

	// construct directly from Base where Base is double 
	AD<double> x(1.); 

	// construct from a type that converts to Base where Base is double
	AD<double> y = 2;

	// construct from a type that converts to Base where Base = AD<double>
	AD< AD<double> > z(3); 

	// check that resulting objects are parameters
	ok &= Parameter(x);
	ok &= Parameter(y);
	ok &= Parameter(z);

	// check values of objects (compare AD<double> with double)
	ok &= ( x == 1.);
	ok &= ( y == 2.);
	ok &= ( Value(z) == 3.);

	// user constructor through the static_cast template function
	x   = static_cast < AD<double> >( 4 );
	z  = static_cast < AD< AD<double> > >( 5 );

	ok &= ( x == 4. );
	ok &= ( Value(z) == 5. );

	return ok;
}

// END PROGRAM
