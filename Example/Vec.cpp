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

/*
$begin Vec.cpp$$
$spell
	Vec
	Cpp
	cstddef
$$

$section Using VecAD: Example and Test$$

$index VecAD$$
$index example, VecAD$$
$index test, VecAD$$
$index index, record$$
$index vector, record index$$

$code
$verbatim%Example/Vec.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cmath>

// create the discrete function AD<double> Floor(const AD<double> &X) 
double Floor(const double &x)
{	return std::floor(x); }	
CppADCreateDiscrete(double, Floor)

bool Vec(void)
{	bool ok = true;
	using namespace CppAD;
	
	double pi    = 4. * CppAD::atan(1.);
	size_t nx    = 10;                       // number of x grid point
	double xLow  = 0;                        // minimum value for x
	double xUp   = 2 * pi;                   // maximum value for x
	double xStep = (xUp - xLow) / (nx - 1);  // step size in x
	double xCur;                             // current value for x

	// fill in the data vector on a uniform grid
	VecAD<double> Data(nx);
	size_t i;
	for(i = 0; i < nx; i++)
	{	xCur = xLow + double(i) * xStep; 
		// use size_t indexing of Data while not taping
		Data[i] = CppAD::sin(xCur); 
	}

	// declare independent variable 
	CppADvector< AD<double> > X(1);
	X[0] = 2.;
	Independent(X);

	// compute index corresponding to current value of X[0]
	AD<double> I = X[0] / xStep;
	AD<double> Ifloor = Floor(I);

	// make sure Ifloor >= 0  (during taping)
	AD<double> Zero(0);
	Ifloor = CondExpLt(Ifloor, Zero, Zero, Ifloor);

	// make sure Ifloor <= nx - 2 (during taping)
	AD<double> Nxminus2(nx - 2);
	Ifloor = CondExpGt(Ifloor, Nxminus2, Nxminus2, Ifloor);

	// Iceil is Ifloor + 1
	AD<double> Iceil  = Ifloor + 1.;

	// linear interpolate Data
	CppADvector< AD<double> > Y(1);
	Y[0] = Data[Ifloor] + (I - Ifloor) * (Data[Iceil] - Data[Ifloor]);

	// create f: X -> Y that linearly interpolates the data vector
	ADFun<double> f(X, Y);

	// evaluate the linear interpolant at the mid point for first interval
	CppADvector<double>  x(1);
	CppADvector<double>  y(1);
	x[0] = xStep / 2.;
	y    = f.Forward(0, x);
	ok  &= NearEqual(y[0], (Data[0] + Data[1])/2., 1e-10, 1e-10);

	// evalute the derivative with respect to x
	CppADvector<double> dx(1);
	CppADvector<double> dy(1);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], (Data[1] - Data[0]) / xStep, 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
