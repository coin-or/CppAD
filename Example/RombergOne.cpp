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
$begin RombergOne.cpp$$
$spell
	Romberg
$$

$section One Dimensional Romberg Integration: Example and Test$$

$index Romberg, example$$
$index example, Romberg$$
$index test, Romberg$$

$code
$verbatim%Example/RombergOne.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/RombergOne.h>
# include <CppAD/CppAD_vector.h>
# include <CppAD/NearEqual.h>

namespace {
	class Fun {
	private:
		const size_t degree;
	public:
		// constructor
		Fun(size_t degree_) : degree(degree_) 
		{ }

		// function F(x) = x^degree
		template <class Type>
		Type operator () (const Type &x)
		{	size_t i;
			Type   f = 1;
			for(i = 0; i < degree; i++)
				f *= x;
			return f;
		}
	};
}

bool RombergOne(void)
{	bool ok = true;
	size_t i;

	size_t degree = 4;
	Fun F(degree);

	// arguments to RombergOne
	double a = 0.;
	double b = 1.;
	size_t n = 4;
	size_t p;
	double r, e;

	// int_a^b F(x) dx = [ b^(degree+1) - a^(degree+1) ] / (degree+1) 
	double bpow = 1.;
	double apow = 1.;
	for(i = 0; i <= degree; i++)
	{	bpow *= b;
		apow *= a;
	}  
	double check = (bpow - apow) / (degree+1);

	// step size corresponding to r
	double step = (b - a) / exp(log(2.)*(n-1));
	// step size corresponding to error estimate
	step *= 2.;
	// step size raised to a power
	double spow = 1;

	for(p = 0; p < n; p++)
	{	spow = spow * step * step;

		r = CppAD::RombergOne(F, a, b, n, p, e);

		ok  &= e < (degree+1) * spow;
		ok  &= CppAD::NearEqual(check, r, 0., e);	
	}

	return ok;
}

// END PROGRAM
