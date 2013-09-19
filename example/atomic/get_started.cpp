// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin atomic_get_started.cpp$$

$section Getting Started with Atomic Operations: Example and Test$$
$index get_started, atomic operation$$
$index atomic, get_started $$
$index operation, atomic get_started$$

$head Purpose$$
This example demonstrates the minimal amount of information
necessary for a $cref atomic_base$$ operation.

$code
$verbatim%example/atomic/get_started.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace { // Begin empty namespace 
// ----------------------------------------------------------------------
using CppAD::vector;

//
class atomic_get_started : public CppAD::atomic_base<double> {
public:
	// ----------------------------------------------------------------------
	// constructor (example use of const std::string& instead of const char*)
	atomic_get_started(const std::string& name) : 
	CppAD::atomic_base<double>(name)
	{ }
private:
	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	virtual bool forward(
		size_t                    q ,
		size_t                    p ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<double>&    tx ,
		      vector<double>&    ty
	)
	{	size_t n = tx.size() / (p + 1);
		size_t m = ty.size() / (p + 1);
		assert( n == 1 );
		assert( m == 1 );

		// return flag
		bool ok = p == 0;
		if( ! ok )
			return ok;

		// check for defining variable information
		// This case must always be implemented
		if( vx.size() > 0 )
			vy[0] = vx[0];

		// Order zero forward mode.
		// This case must always be implemented
		// y^0 = f( x^0 ) = 1 / x^0
		double f = 1. / tx[0];
		if( q <= 0 )
			ty[0] = f;
		return ok;
	}
}; // End of atomic_get_started class
}  // End empty namespace

bool get_started(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();

	// --------------------------------------------------------------------
	// Create the atomic get_started object
	atomic_get_started afun("atomic_get_started");
	// --------------------------------------------------------------------
	// Create the function f(x)
	//
	// domain space vector
	size_t n  = 1;
	double  x0 = 0.5;
	vector< AD<double> > ax(n);
	ax[0]     = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector 
	size_t m = 1;
	vector< AD<double> > ay(m);

	// call user function and store get_started(x) in au[0] 
	vector< AD<double> > au(m);
	afun(ax, au);        // u = 1 / x

	// now use AD division to invert to invert the operation
	ay[0] = 1.0 / au[0]; // y = 1 / u = x

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f;
	f.Dependent (ax, ay);  // f(x) = x

	// --------------------------------------------------------------------
	// Check forward mode results
	//
	// check function value 
	double check = x0;
	ok &= NearEqual( Value(ay[0]) , check,  eps, eps);

	// check zero order forward mode
	size_t p;
	vector<double> x_p(n), y_p(m);
	p      = 0;
	x_p[0] = x0;
	y_p    = f.Forward(p, x_p);
	ok &= NearEqual(y_p[0] , check,  eps, eps);

	return ok;
}
// END C++
