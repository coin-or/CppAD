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
Test of CondExp with AD< AD< Base > > types
*/

# include <CppAD/CppAD.h>

typedef CppAD::AD< double >     ADdouble;
typedef CppAD::AD< ADdouble > ADADdouble;

bool CondExpAD(void)
{	bool ok = true;

	using namespace CppAD;

	// ADdouble independent variable vector
	CppADvector< ADdouble > Xa(3);
	Xa[0] = -1.;
	Xa[1] =  0.;
	Xa[2] =  1.;
	Independent(Xa);

	// ADdouble independent variable vector
	CppADvector< ADADdouble > Xaa(3);
	Xaa[0] = Xa[0];
	Xaa[1] = Xa[1];
	Xaa[2] = Xa[2];
	Independent(Xaa);

	// ADADdouble parameter
	ADADdouble p = ADADdouble(Xa[0]);
	ADADdouble q = ADADdouble(Xa[1]);
	ADADdouble r = ADADdouble(Xa[2]);

	// ADADdouble dependent variable vector
	CppADvector< ADADdouble > Yaa(8);

	// CondExp(parameter, parameter, parameter)
	Yaa[0] = CondExp(p, q, r);

	// CondExp(parameter, parameter, variable)
	Yaa[1] = CondExp(p, q, Xaa[2]);

	// CondExp(parameter, varaible, parameter)
	Yaa[2] = CondExp(p, Xaa[1], r);

	// CondExp(parameter, variable, variable)
	Yaa[3] = CondExp(p, Xaa[1], Xaa[2]);

	// CondExp(variable, variable, variable)
	Yaa[5] = CondExp(Xaa[0], Xaa[1], Xaa[2]);

	// CondExp(variable, variable, parameter)
	Yaa[4] = CondExp(Xaa[0], Xaa[1], r);

	// CondExp(variable, parameter, variable)
	Yaa[6] =  CondExp(Xaa[0], q, Xaa[2]);

	// CondExp(variable, parameter, parameter)
	Yaa[7] =  CondExp(Xaa[0], q, r);

	// create fa: Xaa -> Yaa function object
	ADFun< ADdouble > fa(Xaa, Yaa);

	// function values 
	CppADvector< ADdouble > Ya(8);
	Ya  = fa.Forward(0, Xa);

	// create f: Xa -> Ya function object
	ADFun<double> f(Xa, Ya);

	// now evaluate the function object
	CppADvector<double> x(3);
	CppADvector<double> y(8);
	x[0] = 1.;
	x[1] = 0.;
	x[2] = -1.;
	y = f.Forward(0, x);

	// Yaa[0] = CondExp(   p,      q,      r);
	// y[0]   = CondExp(x[0],   x[1],   x[2]);
	if( x[0] > 0. )
		ok &= (y[0] == x[1]);
	else	ok &= (y[0] == x[2]);

	// Yaa[1] = CondExp(   p,      q, Xaa[2]);
	// y[1]   = CondExp(x[0],   x[1],   x[2]);
	if( x[0] > 0. )
		ok &= (y[1] == x[1]);
	else	ok &= (y[1] == x[2]);

	// Yaa[2] = CondExp(   p, Xaa[1],      r);
	// y[3]   = CondExp(x[0],   x[1],   x[2]);
	if( x[0] > 0. )
		ok &= (y[2] == x[1]);
	else	ok &= (y[2] == x[2]);

	// Yaa[3] = CondExp(   p, Xaa[1], Xaa[2]);
	// y[3]   = CondExp(x[0],   x[1],   x[2]);
	if( x[0] > 0. )
		ok &= (y[3] == x[1]);
	else	ok &= (y[3] == x[2]);

	// Yaa[5] = CondExp(Xaa[0], Xaa[1], Xaa[2]);
	// y[5]   = CondExp(  x[0],   x[1],   x[2]);
	if( x[0] > 0. )
		ok &= (y[5] == x[1]);
	else	ok &= (y[5] == x[2]);
	
	// Yaa[4] = CondExp(Xaa[0], Xaa[1],    r);
	// y[4]   = CondExp(  x[0],   x[1], x[2]);
	if( x[0] > 0. )
		ok &= (y[4] == x[1]);
	else	ok &= (y[4] == x[2]);

	// Yaa[6] =  CondExp(Xaa[0],    q, Xaa[2]);
	// y[6]   =  CondExp(  x[0], x[1],   x[2]);
	if( x[0] > 0. )
		ok &= (y[6] == x[1]);
	else	ok &= (y[6] == x[2]);
  
	// Yaa[7] =  CondExp(Xaa[0],    q,    r);
	//   y[7] =  CondExp(  x[0], x[1], x[2]);
	if( x[0] > 0. )
		ok &= (y[7] == x[1]);
	else	ok &= (y[7] == x[2]);

	// check forward mode derivatives
	CppADvector<double> dx(3);
	CppADvector<double> dy(8);
	dx[0] = 1.;
	dx[1] = 2.;
	dx[2] = 3.;
	dy    = f.Forward(1, dx);
	if( x[0] > 0. )
		ok &= (dy[0] == dx[1]);
	else	ok &= (dy[0] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[1] == dx[1]);
	else	ok &= (dy[1] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[2] == dx[1]);
	else	ok &= (dy[2] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[3] == dx[1]);
	else	ok &= (dy[3] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[5] == dx[1]);
	else	ok &= (dy[5] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[4] == dx[1]);
	else	ok &= (dy[4] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[6] == dx[1]);
	else	ok &= (dy[6] == dx[2]);
	if( x[0] > 0. )
		ok &= (dy[7] == dx[1]);
	else	ok &= (dy[7] == dx[2]);

	// check reverse mode derivatives
	size_t i;
	for(i = 0; i < 8; i++)
		dy[i] = double(i);
	dx    = f.Reverse(1, dy);
	CppADvector<double> sum(3);
	sum[0] = sum[1] = sum[2] = 0.;
	if( x[0] > 0. )
		sum[1] += dy[0];
	else	sum[2] += dy[0];
	if( x[0] > 0. )
		sum[1] += dy[1];
	else	sum[2] += dy[1];
	if( x[0] > 0. )
		sum[1] += dy[2];
	else	sum[2] += dy[2];
	if( x[0] > 0. )
		sum[1] += dy[3];
	else	sum[2] += dy[3];
	if( x[0] > 0. )
		sum[1] += dy[5];
	else	sum[2] += dy[5];
	if( x[0] > 0. )
		sum[1] += dy[4];
	else	sum[2] += dy[4];
	if( x[0] > 0. )
		sum[1] += dy[6];
	else	sum[2] += dy[6];
	if( x[0] > 0. )
		sum[1] += dy[7];
	else	sum[2] += dy[7];

	return ok;
}
// END PROGRAM
