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
$begin NumericType.cpp$$

$section The NumericType: Example and Test$$
$index NumericType, example$$
$index example, NumericType$$
$index test, NumericType$$

$code
$verbatim%Example/NumericType.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace { // Empty namespace

	// -------------------------------------------------------------------
	class MyType {
	private:
		double d;
	public:
		// constructor from void 
		MyType(void) : d(0.)
		{ }
		// constructor from an int 
		MyType(int d_) : d(d_)
		{ }
		// copy constructor
		MyType(const MyType &x) 
		{	d = x.d; }
		// assignment operator
		void operator = (const MyType &x)
		{	d = x.d; }
		// member function that converts to double
		double Double(void) const
		{	return d; }
		// unary plus
		MyType operator + (void) const
		{	MyType x;
			x.d =  d;
			return x; 
		}
		// unary plus
		MyType operator - (void) const
		{	MyType x;
			x.d = - d;
			return x; 
		}
		// binary addition
		MyType operator + (const MyType &x) const
		{	MyType y;
			y.d = d + x.d ;
			return y; 
		}
		// binary subtraction
		MyType operator - (const MyType &x) const
		{	MyType y;
			y.d = d - x.d ;
			return y; 
		}
		// binary multiplication
		MyType operator * (const MyType &x) const
		{	MyType y;
			y.d = d * x.d ;
			return y; 
		}
		// binary division
		MyType operator / (const MyType &x) const
		{	MyType y;
			y.d = d / x.d ;
			return y; 
		}
		// computed assignment addition
		void operator += (const MyType &x)
		{	d += x.d; }
		// computed assignment subtraction
		void operator -= (const MyType &x)
		{	d -= x.d; }
		// computed assignment multiplication
		void operator *= (const MyType &x)
		{	d *= x.d; }
		// computed assignment division
		void operator /= (const MyType &x)
		{	d /= x.d; }
	};
}
bool NumericType(void)
{	bool ok  = true;
	using CppAD::AD;
	using CppAD::CheckNumericType;

	CheckNumericType<MyType>            ();

	CheckNumericType<int>               ();
	CheckNumericType<double>            ();
	CheckNumericType< AD<double> >      ();
	CheckNumericType< AD< AD<double> > >();

	return ok;
}

// END PROGRAM
