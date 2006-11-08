/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin OdeTaylor.cpp$$
$spell
	Taylor
	Cpp
	const
	std
$$

$section Taylor's Method Ode Solver: An Example and Test$$
$index ODE, Taylor$$
$index Taylor, ODE$$
$index example, ODE$$
$index test, ODE$$

$code
$verbatim%example/ode_taylor.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
--------------------------------------------------------------------------
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

# include <cmath>

// ----------- Define types -------------------------------

// define types for each level
typedef CppAD::AD<double>      ADdouble; 
typedef CppAD::AD< ADdouble >  ADDdouble;

// -------------------------------------------------------------------------


// y0(t) =               a[0] * t^1 / 1 ! 
// y1(t) =        a[1] * a[0] * t^2 / 2 !
// y2(t) = a[2] * a[1] * a[0] * t^3 / 3 !
//      ...
class Ode {
public:
	Ode( CppADvector< ADdouble > a_ )
	{	a.resize( a_.size() );
		a = a_;
	}

	CppADvector< ADDdouble > operator()(
		const CppADvector< ADDdouble > &y
	) const
	{	size_t n = y.size();
		CppADvector< ADDdouble > dydt(n);
		size_t i;
		dydt[0] = a[0];
		for(i = 1; i < n; i++)
			dydt[i] = a[i] * y[i-1];

		return dydt;
	}
private:
	CppADvector< ADdouble > a;
};

CppADvector < ADdouble > TaylorOde(
	Ode                     f     ,
	size_t                  order ,
	size_t                  nstep ,
	ADdouble                &dt   , 
	CppADvector< ADdouble > &y0   )
{
	using namespace CppAD;

	size_t n = y0.size();

	CppADvector< ADDdouble >      Y(n);
	CppADvector< ADDdouble >   DYDT(n);
	CppADvector< ADdouble >       y(n);
	CppADvector< ADdouble >     y_j(n);
	CppADvector< ADdouble >    y_jp(n);
	
	// initialize y
	size_t i;
	for(i = 0; i < n; i++)
		y[i] = y0[i];

	size_t k;
	for(k = 0; k < nstep; k++)
	{	for(i = 0; i < n; i++)
			Y[i] = y_j[i] = y[i];
		
		// define differentiable version of F: Y -> DYDT
		Independent(Y);
		DYDT = f(Y);
		ADFun<ADdouble> F(Y, DYDT);

		// Use Taylor's method to take a step
		size_t j;
		ADdouble dt_j = dt;
		for(j = 0; j <= order; j++)
		{	// evaluate j-th Taylor coefficient for derivative of y
			y_jp = F.Forward(j, y_j);
 
			for(i = 0; i < n; i++)
			{	// convert to (j+1)-Taylor coefficient for y
				y_jp[i] /= ADdouble(j + 1);

				// add term for to this Taylor coefficient
				y[i]    += y_jp[i] * dt_j;
			}

			// next power of t
			dt_j *= dt;

			// next Taylor coefficient
			y_j   = y_jp;
		}
	}
	return y;
}

bool OdeTaylor(void)
{	// initialize the return value as true	
	bool ok = true;

	// CppAD namespace (so we do not need CppAD in front of its utilities)  
	using namespace CppAD;

	// number of components in differential equation
	size_t n = 4;

	// parameter vector in differential equation
	CppADvector<ADdouble> A(n);
	CppADvector<double>   a(n);
	size_t i;
	for(i = 0; i < n; i++)
		A[i] = a[i] = double(i + 1);

	// declare the parameters as the independent variable
	Independent(A);

	// arguments to Taylor's method for integrating differential equations
	Ode F(A);
	size_t   order = n;
	size_t   nstep = 2;
	ADdouble Dt    = 1.;
	CppADvector< ADdouble > Y0(n);
	for(i = 0; i < n; i++)
		Y0[i] = 0.;

	// integrate the differential equation
	CppADvector< ADdouble > Y = TaylorOde(F, order, nstep, Dt, Y0);

	// create g: A -> Y
	ADFun<double> g(A, Y);
	CppADvector<double> u( g.Domain() );
	CppADvector<double> v( g.Range() );

	// check function values
	double yi = 1.;
	for(i = 0; i < n; i++)
	{	yi *= a[i] * (nstep * Value(Dt)) / double(i + 1);
		ok &= NearEqual(Value(Y[i]), yi, 1e-10, 1e-10);
	}

	// check derivative values
	size_t j;
	for(j = 0; j < n; j++)
	{
		// compute partial derivative w.r.t a[j]
		for(i = 0; i < n; i++)
			u[i] = 0.;
		u[j] = 1.;
		v    = g.Forward(1, u);


		for(i = 0; i < j; i++)
				ok &= NearEqual(v[i], 0., 1e-10, 1e-10);

		for(i = j; i < n; i++)
		{	yi  = Value(Y[i]);
			ok &= NearEqual(v[i], yi / a[j], 1e-10, 1e-10);
		}
	}


	return ok;
}


// END PROGRAM
