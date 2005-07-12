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
$begin NearEqual$$
$spell
	cout
	endl
	Microsoft
	std
	Cpp
	namespace
	const
	bool
$$

$section Determine if Two Values Are Nearly Equal$$

$index NearEqual, double and complex$$
$index equal, near$$
$index difference, absolute or relative$$
$index absolute, difference$$
$index relative, difference$$

$table
$bold Syntax$$ 
$cnext
$code # include <CppAD/NearEqual.h>$$
$rnext $cnext
$syntax%%b% = NearEqual(%x%, %y%, %r%, %a%)%$$
$tend

$fend 30$$

$head Description$$
The $code NearEqual$$ function returns true,
if $italic x$$ and $italic y$$ are nearly equal.
Otherwise it returns false.

$head x, y$$
The argument $italic x$$ and $italic x$$ have the prototype
$syntax%
	const double &%x%, &%y%
%$$
or they have the prototype
$syntax%
	const std::complex<%double%> &%x%, &%y%
%$$
These are the two values that are being compared to see
if they are nearly equal.

$head r$$
The relative error criteria $italic r$$ has prototype
$syntax%
	double %r%
%$$
It must be greater than or equal to zero.
The relative error condition is defined as:
$latex \[
	\frac{ | x - y | } { |x| + |y| } \leq r
\] $$

$head a$$
The absolute error criteria $italic a$$ has prototype
$syntax%
	double %a%
%$$
It must be greater than or equal to zero.
The absolute error condition is defined as:
$latex \[
	| x - y | \leq a
\] $$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	bool %b%
%$$
If either $italic x$$ or $italic y$$ is infinite or not a number, 
the return value is false.
Otherwise, if either the relative or absolute error 
condition (defined above) is satisfied, the return value is true.
Otherwise, the return value is false.


$head Include Files$$
The file $code CppAD/NearEqual.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head Linkage$$
The CppAD Unix installation
compiles this function and places the object
in the $code CppAD.a$$ library; i.e., you will need the command
line argument $code -lCppAD$$ when you link to this routine under Unix.
If you are using Microsoft visual studio,
you will have to include the file 
$code lib/NearEqual.cpp$$ in your set of project source files.

$head Example$$
$children%
	Example/Near_Equal.cpp
%$$
The file $xref/Near_Equal.cpp/$$ contains an example
and test of $code NearEqual$$.
It return true if it succeeds and false otherwise.

$head Exercise$$
$index exercise, NearEqual$$
Create and run a program that contains the following code:
$codep
	using std::complex;
	using std::cout;
	using std::endl;

	complex<double> one(1., 0), i(0., 1);
	complex<double> x = one / i;
	complex<double> y = - i;
	double          r = 1e-12;
	double          a = 0;
	bool           ok = CppAD::NearEqual(x, y, r, a);
	if( ok )
		cout << "Ok"    << endl;
	else	cout << "Error" << endl;
$$

$end

*/

# include <complex>
# include <CppAD/CppADError.h>

# ifdef _MSC_VER
# include <float.h>
namespace {
	bool isfinite(double zero1, double zero2, double x, double y)
	{	return ( _finite(x) && _finite(y) ); }
}
# else
namespace {
	bool isfinite(double zero1, double zero2, double x, double y)
	{	double infinity = 1. / zero1;
		double nan      = zero1 / zero2;

		// handle case where compiler returns true for nan == nan
		bool xNan = ( x != x || x == nan );
		bool yNan = ( y != y || y == nan );

		// infinite cases
		bool xInf = (x == infinity   || x == - infinity);
		bool yInf = (x == infinity   || x == - infinity);
		
		return ! (xNan | yNan | xInf | yInf);
	}
}
# endif
	 

namespace CppAD {

bool NearEqual(const double &x, const double &y, double r, double a)
{
	CppADUsageError(
		r >= 0.,
		"Error in NearEqual: relative error is a negative value"
	);
	CppADUsageError(
		a >= 0.,
		"Error in NearEqual: absolute error is a negative value"
	);

	// check for special cases
	if( ! isfinite(0., 0., x, y) )
		return false;

	double ax = x;
	if( ax < 0. )
		ax = - ax;

	double ay = y;
	if( ay < 0. )
		ay = - ay;

	double ad = x - y;
	if( ad < 0. )
		ad = - ad;

	if( ad <= a )
		return true;

	if( ad / (ax + ay) <= r )
		return true;

	return false;
}

bool NearEqual( const std::complex<double> &x, 
	const std::complex<double> &y, double r, double a )
{
	CppADUsageError(
		r >= 0.,
		"Error in NearEqual: relative error is a negative value"
	);
	CppADUsageError(
		a >= 0.,
		"Error in NearEqual: absolute error is a negative value"
	);

	// check for special cases
	if( ! isfinite(0., 0., x.real(), x.imag()) )
		return false;
	if( ! isfinite(0., 0., y.real(), y.imag()) )
		return false;

	if( x == y )
		return true;

	std::complex<double> d = x - y;

	double ax = sqrt( x.real() * x.real() + x.imag() * x.imag() );
	double ay = sqrt( y.real() * y.real() + y.imag() * y.imag() );
	double ad = sqrt( d.real() * d.real() + d.imag() * d.imag() );

	if( ad <= a )
		return true;

	if( ad / (ax + ay) <= r )
		return true;

	return false;
}


} // END CppAD namespace
