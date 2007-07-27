/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old OdeImplicit example now used just for valiadation testing of Rosen34
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

# include <iostream>
# include <cassert>

/*
Case where
x[0](0) = 1, x[0]'(t) = - w[0] * x[0](t)
x[1](0) = 1, x[1]'(t) = - w[1] * x[1](t) 
x[2](0) = 0, x[2]'(t) =   w[2] * t

x[0](t) = exp( - w[0] * t )
x[1](t) = exp( - w[1] * t )
x[2](t) = w[2] * t^2 / 2
*/

namespace {  // BEGIN Empty namespace
	class TestFun {
	public:
		TestFun(const CPPAD_TEST_VECTOR< CppAD::AD<double> > &w_)
		{	w.resize( w_.size() );
			w = w_;
		}
		void Ode(
			const CppAD::AD<double>                      &t, 
			const CPPAD_TEST_VECTOR< CppAD::AD<double> > &x, 
			CPPAD_TEST_VECTOR< CppAD::AD<double> >       &f) 
		{
			f[0] = - w[0] * x[0];
			f[1] = - w[1] * x[1];
			f[2] =   w[2] * t;
	
		}
	
		void Ode_ind(
			const CppAD::AD<double>                      &t, 
			const CPPAD_TEST_VECTOR< CppAD::AD<double> > &x, 
			CPPAD_TEST_VECTOR< CppAD::AD<double> >       &f_t) 
		{
			f_t[0] = 0.;
			f_t[1] = 0.;
			f_t[2] = w[2];
	
		}
	
		void Ode_dep(
			const CppAD::AD<double>                      &t, 
			const CPPAD_TEST_VECTOR< CppAD::AD<double> > &x, 
			CPPAD_TEST_VECTOR< CppAD::AD<double> >       &f_x) 
		{
			f_x[0] = - w[0];    f_x[1] = 0.;      f_x[2] = 0.;
			f_x[3] = 0.;        f_x[4] = - w[1];  f_x[5] = 0.;
			f_x[6] = 0.;        f_x[7] = 0.;      f_x[8] = 0.;
	
		}
	
	private:
		CPPAD_TEST_VECTOR< CppAD::AD<double> > w;
	};
}	// END empty namespace

bool Rosen34(void)
{	bool ok = true;

	using namespace CppAD;

	CPPAD_TEST_VECTOR< AD<double> > x(3);
	CPPAD_TEST_VECTOR< AD<double> > w(3);
	size_t         n     = 3;
	size_t         nstep = 20;
	AD<double>     t0    = 0.;
	AD<double>     t1    = 1.;

	// set independent variables
	size_t i;
	for(i = 0; i < n; i++)
		w[i] = 100 * i + 1.;
	Independent(w);

	// construct the function object using the independent variables
	TestFun  fun(w);
	
	// initial value of x
	CPPAD_TEST_VECTOR< AD<double> > xini(3);
	xini[0] = 1.;
	xini[1] = 1.;
	xini[2] = 0.;
	

	// integrate the differential equation
	x  = Rosen34(fun, nstep, t0, t1, xini);

	// create f : w -> x and vectors for evaluating derivatives
	ADFun<double> f(w, x);
	CPPAD_TEST_VECTOR<double> q( f.Domain() );
	CPPAD_TEST_VECTOR<double> r( f.Range() );

	// check function values
	AD<double> x0 = exp( - w[0] * t1 );
	ok &= NearEqual(x[0], x0, 0., 1. / (nstep * nstep) );

	AD<double> x1 = exp( - w[1] * t1 );
	ok &= NearEqual(x[1],  x1, 0., 1. / (nstep * nstep) );

	AD<double> x2 = w[2] * t1 * t1 / 2.;
	ok &= NearEqual(x[2],  x2, 1e-14, 1e-14);

	// check dx[0] / dw[0]
	for(i = 0; i < w.size(); i++)
		q[i] = 0.;
	q[0] = 1.;
	r    = f.Forward(1, q);
	ok &= NearEqual(r[0], - w[0] * x0, 0., 1. / (nstep * nstep) );

	// check dx[1] / dw[1]
	q[0] = 0.;
	q[1] = 1.;
	r    = f.Forward(1, q);
	ok &= NearEqual(r[1], - w[1] * x1, 0., 1. / (nstep * nstep) );

	// check dx[2] / dw[2]
	q[1] = 0.;
	q[2] = 1.;
	r    = f.Forward(1, q);
	ok &= NearEqual(r[2], x2 / w[2],  1e-14, 1e-14 );

	return ok;
}

// END PROGRAM
