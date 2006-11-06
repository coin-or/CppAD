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
Test the use of the special parameters zero and one with the multiply operator
*/

# include <CppAD/CppAD.h>

typedef CppAD::AD<double>      ADdouble;
typedef CppAD::AD< ADdouble > ADDdouble;

bool AddZero(void)
{	
	using namespace CppAD;

	bool ok = true;

	size_t i;
	for(i = 0; i < 2; i++)
	{	// run through the cases x = 0, 1

		size_t j;
		for(j = 0; j < 2; j++)
		{	// run through the cases y = 0, 1

			CppADvector< ADdouble > x(1);
			x[0] = double(i);
			Independent(x);

			CppADvector< ADDdouble > y(1);
			y[0] = ADDdouble(j);
			Independent(y);

			CppADvector< ADDdouble > z(2);
			z[0]  = x[0] + y[0];
			z[1]  = y[0] + x[0];
			z[1] += x[0];

			// f(y) = z = { x + y , y + x + x }
			ADFun< ADdouble > f(y, z);
			CppADvector< ADdouble > u( f.Domain() );
			CppADvector< ADdouble > v( f.Range() );

			// v = f(y)
			u[0] = ADdouble(j);
			v = f.Forward(0, u);

			// check value of f
			ok &= v[0] == x[0] + ADdouble(j);
			ok &= v[1] == ADdouble(j) + x[0] + x[0];

			// g(x) = f(y) = {x + y , y + x + x}
			ADFun<double> g(x, v);
			CppADvector< double > a( g.Domain() );
			CppADvector< double > b( g.Range() );

			// b = g'(x)
			a[0] = 1.;
			b = g.Forward(1, a);

			// check derivatives of g
			ok &= (b[0] == 1.);
			ok &= (b[1] == 2.);

		}
	}

	return ok;
}
