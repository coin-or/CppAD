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
Old CondExp example now only used for testing
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool CondExp(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > V(3);
	size_t s = 0;
	size_t t = 1;
	size_t u = 2;
	V[s]     = 1.;
	V[t]     = 2.;
	V[u]     = 3.;
	Independent(V);

	// dependent variable vector and indices
	CppADvector< AD<double> > W(5);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;
	size_t a = 3;
	size_t b = 4;

	// CondExp(parameter, variable, variable)
	AD<double> p = 1.;
	W[x] = CondExp(p, V[t], V[u]);

	// CondExp(variable, variable, variable)
	W[y] = CondExp(V[s], V[t], V[u]);

	// CondExp(variable, variable, parameter)
	W[z] = CondExp(V[s], V[t], p);

	// CondExp(variable, parameter, variable)
	W[a] = CondExp(V[s], p, V[u]);

	// CondExp(variable, parameter, parameter)
	AD<double> q = 2.;
	W[b] = CondExp(V[s], p, q);

	// create f: V -> W and vectors used for derivative calculations
	ADFun<double> f(V, W);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	CppADvector<double> dv( f.Domain() );
	CppADvector<double> dw( f.Range() );

	// check function values
	ok &= ( W[x] == V[t] );
	ok &= ( W[y] == V[t] );
	ok &= ( W[z] == V[t] );
	ok &= ( W[a] == p );
	ok &= ( W[b] == p );

	// a case where V[s] <= 0
	v[s] = -1.;
	v[t] = 2.;
	v[u] = 3.;

	// function values 
	w    = f.Forward(0, v);
	ok &= ( w[x] == v[t] );
	ok &= ( w[y] == v[u] );
	ok &= ( w[z] == p );
	ok &= ( w[a] == v[u] );
	ok &= ( w[b] == q );

	// forward mode derivative values
	dv[s] = 1.;
	dv[t] = 2.;
	dv[u] = 3.;
	dw    = f.Forward(1, dv);
	ok   &= (dw[x] == dv[t] );
	ok   &= (dw[y] == dv[u] );
	ok   &= (dw[z] == 0. );
	ok   &= (dw[a] == dv[u] );
	ok   &= (dw[b] == 0. );

	// reverse mode derivative values
	dw[x] = 1.;
	dw[y] = 2.;
	dw[z] = 3.;
	dw[a] = 4.;
	dw[b] = 5.;
	dv    = f.Reverse(1, dw);
	ok   &= (dv[s] == 0.);
	ok   &= (dv[t] == dw[x] );
	ok   &= (dv[u] == dw[y] + dw[a] );
	
	return ok;
}
// END PROGRAM
