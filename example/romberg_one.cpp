// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
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
$verbatim%example/romberg_one.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/romberg_one.hpp>
# include <cppad/vector.hpp>
# include <cppad/near_equal.hpp>

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
