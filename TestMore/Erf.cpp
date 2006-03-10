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
Old example now just used for validation testing
*/

# include <CppAD/CppAD.h>

bool Erf(void)
{	bool ok = true;
	using namespace CppAD;
	using CppAD::atan;
	using CppAD::exp;
	using CppAD::sqrt;

	// Construct function object corresponding to erf
	CppADvector< AD<double> > X(1);
	CppADvector< AD<double> > Y(1);
	X[0] = 0.;
	Independent(X);
	Y[0] = erf(X[0]);
	ADFun<double> Erf(X, Y);

	// vectors to use with function object
	CppADvector<double> x(1);
	CppADvector<double> y(1);
	CppADvector<double> dx(1);
	CppADvector<double> dy(1);

	// check value at zero
	x[0]  = 0.;
	y = Erf.Forward(0, x);	
	ok &= NearEqual(0., y[0], 1e-10, 1e-10);

	// check the derivative of error function
	dx[0]         = 1.;
	double pi     = 4. * atan(1.);
	double factor = 2. / sqrt( pi );
	int i;
	for(i = -30; i <= 30; i++)
	{	x[0] = i / 4.;
		y    = Erf.Forward(0, x);	

		// check derivative
		double derf = factor * exp( - x[0] * x[0] );
		dy          = Erf.Forward(1, dx);
		ok         &= NearEqual(derf, dy[0], 1e-10, 1e-10);

		// test using erf with AD< AD<double> >
		AD< AD<double> > X0 = x[0];
		AD< AD<double> > Y0 = erf(X0);

		ok &= ( y[0] == Value( Value(Y0) ) );
	}
	return ok;
}
