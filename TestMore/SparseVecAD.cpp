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


# include <CppAD/CppAD.h>

bool SparseVecAD(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 2; 

	// dimension of the range space
	size_t m = 2;

	// independent variable vector 
	CppADvector< AD<double> > X(n);
	X[0] = 0; 
	Independent(X);

	// dependent variable vector
	CppADvector< AD<double> > Y(m);

	// check results vector
	CppADvector< bool >  Check(m * n);

	// VecAD equal to X
	VecAD<double> Z(n);
	AD<double> J;
	size_t     j;
	for(j = 0; j < n; j++)
	{	J    = AD<double>(j);
		Z[J] = X[j];
	}

	// compute dependent variables values
	AD<double> P = 1;
	size_t i;
	for(j = 0; j < n; j++)
	{	J    = AD<double>(j);
		P    = P * Z[J];
		Y[j] = P;
		for(i = 0; i < m; i++)
			Check[ i * n + j ] = (j <= i);
	}
	
	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// dependency matrix for the identity function W(x) = x
	CppADvector< bool > Px(n * n);
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			Px[ i * n + j ] = false;
		Px[ i * n + i ] = true;
	}

	// evaluate the dependency matrix for F(X(x))
	CppADvector< bool > Py(m * n);
	Py = F.ForSparseJac(n, Px);

	// check values
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			ok &= (Py[i * n + j] == Check[i * n + j]);
	}	

	return ok;
}
