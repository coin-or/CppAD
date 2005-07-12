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
Old Forward example now used just for valiadation testing
*/

# include <CppAD/CppAD.h>

bool Forward(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector 
	CppADvector< AD<double> > U(3);
	U[0] = 0.; U[1] = 1.; U[2] = 2.;
	Independent(U);

	// compute sum and product of elements in U
	AD<double> sum  = 0.;
	AD<double> prod = 1.;
	size_t i;
	for(i = 0; i < 3; i++)
	{	sum  += U[i];
		prod *= U[i];
	}

	// dependent variable vector 
	CppADvector< AD<double> > V(2);
	V[0] = sum;
	V[1] = prod;

	// V = f(U)
	ADFun<double> f(U, V);

	// use ADFun object to evaluate f[ (1, 2, 3)^T ] -----------------
	CppADvector<double> u0( f.Domain() );
	CppADvector<double> v0( f.Range() );
	size_t p;
	p     = 0;
	u0[0] = 1.; u0[1] = 2.; u0[2] = 3.;
	v0    = f.Forward(p, u0);

	// direct evaluation of f[ u0 ]
	CppADvector<double> f0(2);
	f0[0] = u0[0] + u0[1] + u0[2];
	f0[1] = u0[0] * u0[1] * u0[2];

	// compare values
	ok &= NearEqual(v0[0] , f0[0], 1e-10, 1e-10);
	ok &= NearEqual(v0[1] , f0[1], 1e-10, 1e-10);

	// use ADFun object to evaluate f^(1) [ u0 ] * u1 -----------------
	CppADvector<double> u1( f.Domain() );
	CppADvector<double> v1( f.Range() );
	p     = 1;
	u1[0] = 1.; u1[1] = 1.; u1[2] = 1.;
	v1    = f.Forward(p, u1);

	// direct evaluation of gradients of components of f
	CppADvector<double> g0(3), g1(3);
	g0[0] =          1.; g0[1] =          1.; g0[2] =          1.;
	g1[0] = u0[1]*u0[2]; g1[1] = u0[0]*u0[2]; g1[2] = u0[0]*u0[1];

	// compare values
	ok &= NearEqual(v1[0] , 
		g0[0]*u1[0] + g0[1]*u1[1] + g0[2]*u1[2] , 1e-10, 1e-10);
	ok &= NearEqual(v1[1] , 
		g1[0]*u1[0] + g1[1]*u1[1] + g1[2]*u1[2] , 1e-10, 1e-10);

	// use ADFun object to evaluate ------------------------------------
	// (1/2) * { f^(1)[ u0 ] * u2 + u1^T * f^(2)[ u0 ] * u1 }
	CppADvector<double> u2( f.Domain() );
	CppADvector<double> v2( f.Range() );
	p     = 2;
	u2[0] = .5; u2[1] = .4; u2[2] = .3;  
	v2    = f.Forward(p, u2);

	// direct evaluation of Hessian of second components of f
	// (the Hessian of the first component is zero)
	CppADvector<double> H1(9);
	H1[0] =    0.; H1[1] = u0[2]; H1[2] = u0[1];
	H1[3] = u0[2]; H1[4] =    0.; H1[5] = u0[0];
	H1[6] = u0[1]; H1[7] = u0[0]; H1[8] =    0.;

	// compare values
	ok &= NearEqual(v2[0] , 
		g0[0]*u2[0] + g0[1]*u2[1] + g0[2]*u2[2] , 1e-10, 1e-10);

	size_t j;
	double v2_1 = 0.;
	for(i = 0; i < 3; i++)
	{	v2_1 += g1[i] * u2[i];	 
		for(j = 0; j < 3; j++)
			v2_1 += .5 * u1[i] * H1[i * 3 + j] * u1[j];
	}
	ok &= NearEqual(v2[1], v2_1, 1e-10, 1e-10);


	return ok;
}
// END PROGRAM
