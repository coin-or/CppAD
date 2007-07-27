/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Test the use of the special parameters zero and one with the multiply operator
*/

# include <cppad/cppad.hpp>

typedef CppAD::AD<double>      ADdouble;
typedef CppAD::AD< ADdouble > ADDdouble;

bool DivZeroOne(void)
{	
	using namespace CppAD;

	bool ok = true;

	size_t i;
	for(i = 0; i < 3; i++)
	{	// run through the cases x = 0, 1, 2

		size_t j;
		for(j = 0; j < 3; j++)
		{	// run through the cases y = 0, 1, 2

			CPPAD_TEST_VECTOR< ADdouble > x(1);
			x[0] = double(i);
			Independent(x);

			CPPAD_TEST_VECTOR< ADDdouble > y(1);
			y[0] = ADDdouble(j);
			Independent(y);

			CPPAD_TEST_VECTOR< ADDdouble > z(2);
			if( j == 0 )
				z[0] = ADDdouble(0);
			else	z[0] = x[0] / y[0];
			if( i == 0 )
				z[1] = ADDdouble(0);
			else
			{	z[1]  = y[0] / x[0];
				z[1] /= x[0];
			}

			// f(y) = z = { x / y , y / (x * x) }
			ADFun< ADdouble > f(y, z);
			CPPAD_TEST_VECTOR< ADdouble > u( f.Domain() );
			CPPAD_TEST_VECTOR< ADdouble > v( f.Range() );

			// v = f'(y)
			u[0] = ADdouble(1.);
			v = f.Forward(1, u);

			// check derivatives of f
			ADdouble check = - double(i) / double(j * j);
			if( j != 0 ) ok &= NearEqual(
				v[0], check, 1e-10, 1e-10 );

			check = 1. / double(i * i);
			if( i != 0 ) ok &= NearEqual(
				v[1], check, 1e-10, 1e-10);

			// g(x) = f'(y) = {-x/y^2 , 1/(x * x)}
			ADFun<double> g(x, v);
			CPPAD_TEST_VECTOR< double > a( g.Domain() );
			CPPAD_TEST_VECTOR< double > b( g.Range() );

			// b = g'(x)
			a[0] = 1.;
			b = g.Forward(1, a);

			// check derivatives of g
			if( j != 0 ) ok &= NearEqual(
				b[0], - 1./double(j*j), 1e-10, 1e-10 );
			if( i != 0 ) ok &= NearEqual(
				b[1], -2./double(i*i*i), 1e-10, 1e-10);

		}
	}

	return ok;
}
