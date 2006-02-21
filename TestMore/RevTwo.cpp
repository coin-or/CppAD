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
# include <CppAD/CppAD.h>

bool RevTwo()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;
	using CppAD::NearEqual;


	size_t n = 2;
   	vector< AD<double> > X(n);
	X[0] = 1.;
	X[1] = 1.;
	Independent(X);

	size_t m = 1;
   	vector< AD<double> > Y(m);
	Y[0] = X[0] * X[0] + X[0] * X[1] + 2. * X[1] * X[1];
	CppAD::ADFun<double> F(X,Y);

	vector<double> x(n);
	x[0] = .5;
	x[1] = 1.5;

	size_t L = 1;
	vector<size_t> I(L);
	vector<size_t> J(L);
	vector<double> H(n);
	I[0] = 0;
	J[0] = 0;
	H    = F.RevTwo(x, I, J);
	ok  &= NearEqual(H[0], 2., 1e-10, 1e-10);
	ok  &= NearEqual(H[1], 1., 1e-10, 1e-10);
	J[0] = 1;
	H    = F.RevTwo(x, I, J);
	ok  &= NearEqual(H[0], 1., 1e-10, 1e-10);
	ok  &= NearEqual(H[1], 4., 1e-10, 1e-10);

	return ok;
}
