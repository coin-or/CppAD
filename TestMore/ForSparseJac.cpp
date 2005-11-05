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
# include <vector>
# include <valarray>

# define CheckOp(Op)                   \
	Y[index] = X[0] Op 2.;         \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = false;  \
	index++;                       \
	Y[index] = X[0] Op X[1];       \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = true;   \
	index++;                       \
	Y[index] = 3.   Op X[1];       \
	Check[index * n + 0] = false;  \
	Check[index * n + 1] = true;   \
	index++;

# define CheckFun(Fun)                 \
	Y[index] = Fun(X[0]);          \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = false;  \
	index++;                       \
	Y[index] = Fun(X[0] + X[1]);   \
	Check[index * n + 0] = true;   \
	Check[index * n + 1] = true;   \
	index++;                       \
	Y[index] = Fun(X[1]);          \
	Check[index * n + 0] = false;  \
	Check[index * n + 1] = true;   \
	index++;
 


bool ForSparseJac(void)
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 2; 

	// dimension of the range space
	size_t m = 16 * 3;

	// independent variable vector 
	CppADvector< AD<double> > X(n);
	X[0] = .1; 
	X[1] = .2;
	Independent(X);

	// compute product of elements in X
	CppADvector< AD<double> > Y(m);
	CppADvector< bool >       Check(m * n);

	// initialize index into Y
	size_t index = 0;

	// 4 binary operators
	CheckOp(+);
	CheckOp(-);
	CheckOp(*);
	CheckOp(/);

	// 11 unary functions
	CheckFun(abs);
	CheckFun(acos);
	CheckFun(asin);
	CheckFun(atan);
	CheckFun(cos);
	CheckFun(cosh);
	CheckFun(exp);
	CheckFun(log);
	CheckFun(sin);
	CheckFun(sinh);
	CheckFun(sqrt);

	// conditional expression
	Y[index] = CondExpLt(X[0], X[1], X[0], AD<double>(2.));
	Check[index * n + 0] = true;
	Check[index * n + 1] = false;
	index++;
	Y[index] = CondExpLt(X[0], X[1], X[0], X[1]);
	Check[index * n + 0] = true;
	Check[index * n + 1] = true;
	index++;
	Y[index] = CondExpLt(X[0], X[1], AD<double>(3.), X[1]);
	Check[index * n + 0] = false;
	Check[index * n + 1] = true;
	index++;

	// check final index
	assert( index == m );


	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// dependency matrix for the identity function W(x) = x
	CppADvector< bool > Px(n * n);
	size_t i, j;
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
