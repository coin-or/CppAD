// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin reciprocal.cpp$$
$spell
	Tanh
$$

$section Tan and Tanh as User Atomic Operations: Example and Test$$

$index reciprocal, user_atomic$$
$index user, atomic reciprocal$$
$index atomic, reciprocal$$
$index test, user_atomic$$
$index user_atomic, example$$
$index example, user_atomic$$

$head Theory$$
The example below defines the user atomic function 
$latex f : \B{R}^n \rightarrow \B{R}^m$$ where
$latex n = 1$$, $latex m = 1$$, and $latex f(x) = 1 / x$$.

$code
$verbatim%example/reciprocal.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace { // Begin empty namespace 
	using CppAD::vector;

	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool reciprocal_forward(
		size_t                   id ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<bool>&      vx ,
		vector<bool>&            vy ,
		const vector<double>&    tx ,
		vector<double>&          ty
	)
	{	assert( id == 0 );
		assert( n == 1 );
		assert( m == 1 );
		assert( k == 0 || vx.size() == 0 );
		bool ok = false;	
		double f, fp, fpp;
		switch(k)
		{	case 0:
			// this case must  be implemented
			if( vx.size() > 0 )
				vy[0] = vx[0];
			// y^0 = f( x^0 ) = 1 / x^0
			ty[0] = 1. / tx[0];
			ok    = true;
			break;

			case 1:
			// needed if first order forward mode is used
			assert( vx.size() == 0 );
			// y^1 = f'( x^0 ) x^1
			f     = ty[0];
			fp    = - f / tx[0];
			ty[1] = fp * tx[1]; 
			ok    = true;
			break;

			case 2:
			// needed if second order forward mode is used
			assert( vx.size() == 0 );
			// Y''(t) = X'(t)^\R{T} f''[X(t)] X'(t) + f'[X(t)] X''(t)
			// 2 y^2  = x^1 * f''( x^0 ) x^1 + 2 f'( x^0 ) x^2
			f     = ty[0];
			fp    = - f / tx[0];
			fpp   = - 2.0 * fp / tx[0];
			ty[2] = tx[1] * fpp * tx[1] / 2.0 + fp * tx[2];
			ok    = true;
			break;
		}
		return ok;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	bool reciprocal_reverse(
		size_t                   id ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<double>&    tx ,
		const vector<double>&    ty ,
		vector<double>&          px ,
		const vector<double>&    py
	)
	{	assert( id == 0 );
		assert( n == 1 );
		assert( m == 1 );
		bool ok = false;	

		double f, fp, fpp, fppp;
		switch(k)
		{	case 0:
			// needed if first order reverse mode is used
			// reverse: F^0 ( tx ) = y^0 = f( x^0 )
			f     = ty[0];
			fp    = - f / tx[0];
			px[0] = py[0] * fp;;
			ok    = true;
			break;

			case 1:
			// needed if second order reverse mode is used
			// reverse: F^1 ( tx ) = y^1 = f'( x^0 ) x^1
			f      = ty[0];
			fp     = - f / tx[0];
			fpp    = - 2.0 * fp / tx[0];
			px[1]  = py[1] * fp;
			px[0]  = py[1] * fpp * tx[1];
			// reverse: F^0 ( tx ) = y^0 = f( x^0 );
			px[0] += py[0] * fp;

			ok     = true;
			break;

			case 2:
			// needed if third order reverse mode is used
			// reverse: F^2 ( tx ) = y^2 =
			//            = x^1 * f''( x^0 ) x^1 / 2 + f'( x^0 ) x^2
			f      = ty[0];
			fp     = - f / tx[0];
			fpp    = - 2.0 * fp / tx[0];
			fppp   = - 3.0 * fpp / tx[0];
			px[2]  = py[2] * fp;
			px[1]  = py[2] * fpp * tx[1];
			px[0]  = py[2] * tx[1] * fppp * tx[1] / 2.0 + fpp * tx[2]; 
			// reverse: F^1 ( tx ) = y^1 = f'( x^0 ) x^1
			px[1] += py[1] * fp;
			px[0] += py[1] * fpp * tx[1];
			// reverse: F^0 ( tx ) = y^0 = f( x^0 );
			px[0] += py[0] * fp;

			ok = true;
			break;
		}
		return ok;
	}
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	bool reciprocal_for_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	return false; }
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	bool reciprocal_rev_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		vector< std::set<size_t> >&           r ,
		const vector< std::set<size_t> >&     s )
	{	return false; }
	// ----------------------------------------------------------------------
	// reverse Hessian sparsity routine called by CppAD
	bool reciprocal_rev_hes_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		const vector<bool>&                   s ,
		vector<bool>&                         t ,
		const vector< std::set<size_t> >&     u ,
		vector< std::set<size_t> >&           v )
	{	return false; }
	// ---------------------------------------------------------------------
	// Declare the AD<double> routine reciprocal(id, ax, ay)
	CPPAD_USER_ATOMIC(
		reciprocal                 , 
		CppAD::vector              ,
		double                     , 
		reciprocal_forward         , 
		reciprocal_reverse         ,
		reciprocal_for_jac_sparse  ,
		reciprocal_rev_jac_sparse  ,
		reciprocal_rev_hes_sparse  
	)
} // End empty namespace

bool reciprocal(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();

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

	// call user function and store reciprocal(x) in au[0] 
	vector< AD<double> > au(m);
	size_t id = 0;           // not used
	reciprocal(id, ax, au);	// u = 1 / x

	// call user function and store reciprocal(u) in ay[0] 
	reciprocal(id, au, ay);	// y = 1 / u = x

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);  // f(x) = x

	// --------------------------------------------------------------------
	// Check forward mode results
	//
	// check function value 
	double check = x0;
	ok &= NearEqual( Value(ay[0]) , check,  eps, eps);

	// check first order forward mode
	size_t p;
	vector<double> x_p(n), y_p(m);
	p      = 1;
	x_p[0] = 1;
	y_p    = f.Forward(p, x_p);
	check  = 1.;
	ok &= NearEqual(y_p[0] , check,  eps, eps);

	// check second order forward mode
	p      = 2;
	x_p[0] = 0;
	y_p    = f.Forward(p, x_p);
	check  = 0.;
	ok &= NearEqual(y_p[0] , check,  eps, eps);

	// --------------------------------------------------------------------
	// Check reverse mode results
	//
	// third order reverse mode 
	p     = 3;
	vector<double> w(m), dw(n * p);
	w[0]  = 1.;
	dw    = f.Reverse(p, w);
	check = 1.;
	ok &= NearEqual(dw[0] , check,  eps, eps);
	check = 0.;
	ok &= NearEqual(dw[1] , check,  eps, eps);
	ok &= NearEqual(dw[2] , check,  eps, eps);

	// -----------------------------------------------------------------
	// Free all temporary work space associated with user_atomic objects. 
	// (If there are future calls to user atomic functions, they will 
	// create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
// END C++
