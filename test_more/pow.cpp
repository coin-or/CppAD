/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old examples now just used for validation testing.
*/
# include <cppad/cppad.hpp>

namespace { // BEGIN empty namespace

bool PowTestOne(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 2;
	double x = 0.5;
	double y = 2.;
	CppADvector< AD<double> > XY(n);
	XY[0]      = x;
	XY[1]      = y;

	// declare independent variables and start tape recording
	CppAD::Independent(XY);

	// range space vector 
	size_t m = 3;
	CppADvector< AD<double> > Z(m);
	Z[0] = CppAD::pow(XY[0], XY[1]);  // pow(variable, variable)
	Z[1] = CppAD::pow(XY[0], y);      // pow(variable, parameter)
	Z[2] = CppAD::pow(x,     XY[1]);  // pow(parameter, variable)

	// create f: XY -> Z and stop tape recording
	CppAD::ADFun<double> f(XY, Z); 

	// check value 
	double check = std::pow(x, y);
	size_t i;
	for(i = 0; i < m; i++)
		ok &= NearEqual(Z[i] , check,  1e-10 , 1e-10);

	// forward computation of first partial w.r.t. x
	CppADvector<double> dxy(n);
	CppADvector<double> dz(m);
	dxy[0] = 1.;
	dxy[1] = 0.;
	dz    = f.Forward(1, dxy);
	check = y * std::pow(x, y-1.);
	ok   &= NearEqual(dz[0], check, 1e-10, 1e-10);
	ok   &= NearEqual(dz[1], check, 1e-10, 1e-10);
	ok   &= NearEqual(dz[2],    0., 1e-10, 1e-10);

	// forward computation of first partial w.r.t. y
	dxy[0] = 0.;
	dxy[1] = 1.;
	dz    = f.Forward(1, dxy);
	check = std::log(x) * std::pow(x, y);
	ok   &= NearEqual(dz[0], check, 1e-10, 1e-10);
	ok   &= NearEqual(dz[1],    0., 1e-10, 1e-10);
	ok   &= NearEqual(dz[2], check, 1e-10, 1e-10);

	// reverse computation of derivative of z[0] + z[1] + z[2]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0]  = 1.;
	w[1]  = 1.;
	w[2]  = 1.;
	dw    = f.Reverse(1, w);
	check = y * std::pow(x, y-1.);
	ok   &= NearEqual(dw[0], 2. * check, 1e-10, 1e-10);
	check = std::log(x) * std::pow(x, y);
	ok   &= NearEqual(dw[1], 2. * check, 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with pow
	CppAD::VecAD<double> v(2);
	AD<double> zero(0);
	AD<double> one(1);
	v[zero]           = XY[0];
	v[one]            = XY[1];
	AD<double> result = CppAD::pow(v[zero], v[one]);
	ok               &= NearEqual(result, Z[0], 1e-10, 1e-10);

	return ok;
}

bool PowTestTwo(void)
{	bool ok = true;

	using CppAD::pow;
	using CppAD::exp;
	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 2.;
	U[t]     = 3.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(2);
	size_t x = 0;
	size_t y = 1;


	// dependent variable values
	AD<double> u = exp(U[s]);        // u = exp(s)
	Z[x]         = pow(u, U[t]);     // x = exp(s * t)
	Z[y]         = pow(Z[x], u);     // y = exp( s * t * exp(s) )

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	/*
	u_s  (s, t) = u
	u_t  (s, t) = 0
	y_s  (s, t) = (1 + s) t * u * y
	y_t  (s, t) = s * u * y
	y_st (s, t) = ( u + s * u ) * y
                    + ( t * u + s * t * u ) * s * u * y
	*/

	// check values
	ok &= NearEqual(Z[x] , exp(2. * 3.),              1e-10 , 1e-10);
	ok &= NearEqual(Z[y] , exp( 2. * 3. * exp(2.) ),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	v[t] = 0.;
	w = f.Forward(1, v);
	ok &= ( w[x] == U[t] * Z[x] );                   // dx/ds
	ok &= ( w[y] == (1. + U[s]) * U[t] * u * Z[y] ); // dy/ds

	// forward computation of partials w.r.t. t
	v[s] = 0.;
	v[t] = 1.;
	w = f.Forward(1, v);
	ok &= ( w[y] == U[s] * u * Z[y] );               // dy/dt

	// forward computation of second Taylor coefficient w.r.t. t 
	v[t] = 1.;
	w    = f.Forward(1, v);
	v[t] = 0.;
	CppADvector<double> f_tt = f.Forward(2, v);

	// forward computation of second Taylor coefficient w.r.t. s 
	v[s] = 1.;
	w    = f.Forward(1, v);
	v[s] = 0.;
	CppADvector<double> f_ss = f.Forward(2, v);

	// second Taylor coefficient w.r.t. direction r = (s,t) 
	v[s] = 1.;
	v[t] = 1.;
	w    = f.Forward(1, v);
	v[s] = 0.;
	v[t] = 0.;
	CppADvector<double> f_rr = f.Forward(2, v);

	// check second order partial of y
	ok &= NearEqual(
		f_rr[y] - f_ss[y] - f_tt[y], 
		(1. + U[s]) * u * Z[y] + 
			(1. + U[s]) * U[t] * u * U[s] * u * Z[y],
		1e-10 ,
		1e-10 
	); 

	return ok;
}

bool PowTestThree(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 1;
	CppADvector< AD<double> > x(n);
	x[0]      = 2.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 4;
	CppADvector< AD<double> > y(m);

	// some special cases
	y[0] = pow(x[0], 0.);
	y[1] = pow(0., x[0]);
	y[2] = pow(x[0], 1.);
	y[3] = pow(1., x[0]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check function values
	ok  &= (Value(y[0]) == 1.);
	ok  &= (Value(y[1]) == 0.);
	ok  &= (Value(y[2]) == Value(x[0]));
	ok  &= (Value(y[3]) == 1.);

	// forward computation of first partial w.r.t. x
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= (dy[0] == 0.);
	ok   &= (dy[1] == 0.);
	ok   &= NearEqual(dy[2], 1., 1e-10, 1e-10);
	ok   &= (dy[3] == 0.);

	// reverse mode computation of derivative of y[0] + y[1]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0] = 1.;
	w[1] = 1.;
	w[2] = 1.;
	w[3] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0], 1., 1e-10, 1e-10);

	return ok;	
}

bool PowTestFour(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 1;
	double x0 = -2;
	CppADvector< AD<double> > x(n);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 5;
	CppADvector< AD<double> > y(m);

	// some special cases (skip zero raised to a negative power)
	y[0] = pow(1., x[0]);
	size_t i;
	for(i = 1; i < m; i++) 
		y[i] = pow(x[0], double(i-1));

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	ok  &= (Value(y[0]) == 1.);
	double check;
	for(i = 1; i < m; i++)
	{	check = std::pow(x0, double(i-1));
		ok   &= NearEqual(y[i], check, 1e-10, 1e-10);
	}

	// forward computation of first partial w.r.t. x
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= (dy[0] == 0.);
	double sum = 0;
	for(i = 1; i < m; i++)
	{	if( i == 1 )
			check = 0.;
		else	check = double(i-1) * std::pow(x0, double(i-2));
		ok   &= NearEqual(dy[i], check, 1e-10, 1e-10);
		sum  += check;
	}

	// reverse mode computation of derivative of y[0] + .. y[m-1];
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	for(i = 0; i < m; i++)
		w[i] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0], sum, 1e-10, 1e-10);

	return ok;	
}

} // END empty namespace
 
bool Pow(void)
{	bool ok = true;
	ok     &= PowTestOne();
	ok     &= PowTestTwo();
	ok     &= PowTestThree();
	ok     &= PowTestFour();
	return ok;
}
