/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


/*
Test higher order derivatives for tan(x) function.
*/

# include <cppad/cppad.hpp>

namespace {
	bool tan_case(bool tan_first)
	{	bool ok = true;
		double eps = 100. * std::numeric_limits<double>::epsilon();
		using CppAD::AD;
		using CppAD::NearEqual;
	
		// independent variable vector, indices, values, and declaration
		size_t n = 1;
		CPPAD_TESTVECTOR(AD<double>) ax(n);
		ax[0]     = .7;
		Independent(ax);
	
		// dependent variable vector and indices
		size_t m = 1;
		CPPAD_TESTVECTOR(AD<double>) ay(m);
		if( tan_first )
			ay[0] = atan( tan( ax[0] ) );
		else	ay[0] = tan( atan( ax[0] ) );
	
		// check value 
		ok &= NearEqual(ax[0] , ay[0],  eps, eps);
	
		// create f: x -> y and vectors used for derivative calculations
		CppAD::ADFun<double> f(ax, ay); 
		CPPAD_TESTVECTOR(double) dx(n), dy(m);
	
		// forward computation of partials w.r.t. x
		dx[0] = 1.;
		dy    = f.Forward(1, dx);
		ok   &= NearEqual(dy[0], 1e0, eps, eps);
		size_t p, order = 5;
		dx[0] = 0.;
		for(p = 2; p < order; p++)
		{	dy    = f.Forward(p, dx);
			ok   &= NearEqual(dy[0], 0e0, eps, eps);
		}
	
		// reverse computation of order partial
		CPPAD_TESTVECTOR(double)  w(m), dw(n * order);
		w[0] = 1.;
		dw   = f.Reverse(order, w);
		ok   &= NearEqual(dw[0], 1e0, eps, eps);
		for(p = 1; p < order; p++)
			ok   &= NearEqual(dw[p], 0e0, eps, eps);
	
		return ok;
	}
	bool tanh_case(bool tanh_first)
	{	bool ok = true;
		double eps = 100. * std::numeric_limits<double>::epsilon();
		using CppAD::AD;
		using CppAD::NearEqual;
	
		// independent variable vector, indices, values, and declaration
		size_t n = 1;
		CPPAD_TESTVECTOR(AD<double>) ax(n);
		ax[0]     = .5;
		Independent(ax);
	
		// dependent variable vector and indices
		size_t m = 1;
		CPPAD_TESTVECTOR(AD<double>) ay(m);
		AD<double> z;
		if( tanh_first )
		{	z     = tanh( ax[0] );
			ay[0] = .5 * log( (1. + z) / (1. - z) );
		}
		else
		{	z     = .5 * log( (1. + ax[0]) / (1. - ax[0]) );
			ay[0] = tanh(z);
		}
		// check value 
		ok &= NearEqual(ax[0] , ay[0],  eps, eps);
	
		// create f: x -> y and vectors used for derivative calculations
		CppAD::ADFun<double> f(ax, ay); 
		CPPAD_TESTVECTOR(double) dx(n), dy(m);
	
		// forward computation of partials w.r.t. x
		dx[0] = 1.;
		dy    = f.Forward(1, dx);
		ok   &= NearEqual(dy[0], 1e0, eps, eps);
		size_t p, order = 5;
		dx[0] = 0.;
		for(p = 2; p < order; p++)
		{	dy    = f.Forward(p, dx);
			ok   &= NearEqual(dy[0], 0e0, eps, eps);
		}
	
		// reverse computation of order partial
		CPPAD_TESTVECTOR(double)  w(m), dw(n * order);
		w[0] = 1.;
		dw   = f.Reverse(order, w);
		ok   &= NearEqual(dw[0], 1e0, eps, eps);
		for(p = 1; p < order; p++)
			ok   &= NearEqual(dw[p], 0e0, eps, eps);
	
		return ok;
	}
}
bool tan(void)
{	bool ok = true;
	ok     &= tan_case(true);
	ok     &= tan_case(false);
	ok     &= tanh_case(true);
	ok     &= tanh_case(false);
	return ok;
}
