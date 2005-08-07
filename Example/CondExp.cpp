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
$begin CondExp.cpp$$
$spell
	Cpp
	cstddef
	CondExp
$$

$section Conditional Expressions: Example and Test$$
$index CondExp$$
$index example, CondExp$$
$index test, CondExp$$

$head Description$$
Use $code CondExp$$ to compute
$latex \[
	f(x) = \sum_{i=0}^{m-1} \log( | x_i | )
\] $$
and its derivative at various argument values
with out having to re-tape; i.e.,
using only one $xref/ADFun/$$ object.

$code
$verbatim%Example/CondExp.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace {
	double Infinity(double zero)
	{	return 1. / zero; }
}

bool CondExp(void)
{	bool ok = true;

	using namespace CppAD;
	using CppAD::log;
	using CppAD::abs;

	// number of independent variables
	size_t m = 5;

	// independent variable vector
	CppADvector< AD<double> > X(m);
	size_t i;
	for(i = 0; i < m; i++)
		X[i] = 1.;
	Independent(X);

	// Sum with respect to i of log of absolute value of X[i]
	// sould be - infinity if any of the X[i] are zero
	AD<double> MinusInfinity = - Infinity(0.);
	AD<double> Sum           = 0.;
	AD<double> Zero(0);
	for(i = 0; i < m; i++)
	{	// if X[i] > 0
		Sum += CondExpGt(X[i], Zero, log(X[i]),     Zero);

		// if X[i] < 0
		Sum += CondExpLt(X[i], Zero, log(-X[i]),    Zero);

		// if X[i] == 0
		Sum += CondExpEq(X[i], Zero, MinusInfinity, Zero);
	}

	// dependent variable vector 
	CppADvector< AD<double> > Y(1);
	Y[0] = Sum;

	// create f: X -> Y and vectors used for derivative calculations
	ADFun<double> f(X, Y);
	CppADvector<double> x( f.Domain() );
	CppADvector<double> w( f.Range() );

	CppADvector<double> dx( f.Domain() );
	CppADvector<double> dw( f.Range() );

	// a case where all components > 0.
	double sum  = 0.;
	double sign = 1.;
	for(i = 0; i < m; i++)
	{	sign *= -1.;
		x[i] = sign * double(i + 1); 
		sum += log( abs( x[i] ) );
	}

	// function value 
	w   = f.Forward(0, x);
	ok &= ( w[0] == sum );

	// derivative value
	dw[0] = 1.;
	dx    = f.Reverse(1, dw);
	for(i = 0; i < m; i++)
	{	if( x[i] > 0. )
			ok &= NearEqual(dx[i], 1./abs( x[i] ), 1e-10, 1e-10); 
		else	ok &= NearEqual(dx[i], -1./abs( x[i] ), 1e-10, 1e-10); 
	}

	// a case where a component is equal to zero
	sign = 1.;
	for(i = 0; i < m; i++)
	{	sign *= -1.;
		x[i] = sign * double(i); 
	}

	// function value 
	w   = f.Forward(0, x);
	ok &= ( w[0] == -Infinity(0.) );

	// derivative value
	dw[0] = 1.;
	dx    = f.Reverse(1, dw);
	for(i = 0; i < m; i++)
	{	if( x[i] > 0. )
			ok &= NearEqual(dx[i], 1./abs( x[i] ), 1e-10, 1e-10); 
		else if( x[i] < 0. )
			ok &= NearEqual(dx[i], -1./abs( x[i] ), 1e-10, 1e-10); 
		else
		{	// in this case computing dx[i] ends up multiplying 
			// -infinity * zero and hence results in Nan
		}
	}
	
	return ok;
}
// END PROGRAM
