// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin user_tan.cpp$$
$spell
$$

$section Tan as User Atomic Operations: Example and Test$$

$index tan, user_atomic$$
$index user, atomic tan$$
$index atomic, tan$$
$index test, user_atomic$$
$index user_atomic, example$$
$index example, user_atomic$$

$code
$verbatim%example/user_tan.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

namespace { // Begin empty namespace 
	using CppAD::vector;

	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool user_tan_forward(
		size_t                   id ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<bool>&      vx ,
		vector<bool>&            vy ,
		const vector<double>&    tx ,
		vector<double>&          ty
	)
	{	assert( n == 1 );
		assert( m == 2 );

		// check if this is during the call to user_tan(id, ax, ay)
		if( vx.size() > 0 )
		{	assert( vx[0] == true  );
			assert( vx.size() >= n );
			assert( vy.size() >= m );
			
			// now set vy
			vy[0] = true;
			vy[1] = true;
		}
		size_t ell, kp = k + 1;

		// Notational conversion table;
		// user_tan       tan_forward.xml
		// k              j
		// ell            k
		// tx[ell]        x^{(ell}}
		// ty[ell]        z^{(ell)}
		// ty[kp + ell]   y^{(ell)}

		if( k == 0 )
		{	// z^{(0)} = tan( x^{(0)} )
			ty[0]      = tan( tx[0] );
			// y^{(0)} = z^{(0)} * z^{(0)}
			ty[kp + 0] = ty[0] * ty[0];
		}
		else
		{	// z^{(j)} = x^{(j)} + sum_{k=1}^j k x^{(k)} y^{(j-k)} / j
			ty[k] = tx[k];  
			for(ell = 1; ell < kp; ell++)
				ty[k] += ell * tx[ell] * ty[kp + k - ell] / k;

			// y^{(j)} = sum_{k=0}^j z^{(k)} z^{(j-k)}
			ty[kp + k] = 0.;
			for(ell = 0; ell < kp; ell++)
				ty[kp + k] += ty[ell] * ty[k - ell];
		}
			
		// All orders are implemented and there are no possible errors
		return true;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	bool user_tan_reverse(
		size_t                   id ,
		size_t                    k ,
		size_t                    n ,
		size_t                    m ,
		const vector<double>&    tx ,
		const vector<double>&    ty ,
		vector<double>&          px ,
		const vector<double>&    py
	)
	{	return false; }
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	bool user_tan_for_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	return false; }
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	bool user_tan_rev_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		vector< std::set<size_t> >&           r ,
		const vector< std::set<size_t> >&     s )
	{	return false; }
	// ----------------------------------------------------------------------
	// reverse Hessian sparsity routine called by CppAD
	bool user_tan_rev_hes_sparse(
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
	// Declare the AD<double> routine user_tan(id, ax, ay)
	CPPAD_USER_ATOMIC(
		user_tan                 , 
		CPPAD_TEST_VECTOR        ,
		double                   , 
		user_tan_forward         , 
		user_tan_reverse         ,
		user_tan_for_jac_sparse  ,
		user_tan_rev_jac_sparse  ,
		user_tan_rev_hes_sparse  
	)
} // End empty namespace

bool user_tan(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n  = 1;
	double x0 = 0.5;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(1);

	// user_tan computes both tan(x) and tan(x)^2
	CPPAD_TEST_VECTOR< AD<double> > z(2);

	// call user tan function
	size_t id = 0;
	user_tan(id, x, z);

	// we are only interested in tan(x)
	y[0] = z[0];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	double fx = std::tan(x0);
	ok &= NearEqual(y[0] , fx,  eps, eps);

	// compute first partial of f w.r.t. x[0] using forward mode
	CPPAD_TEST_VECTOR<double> dx(n), dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);

	// f'(x) = 1 + tan(x) * tan(x) = 1 + f(x)^2
	double fpx = 1. + fx * fx; 
	ok   &= NearEqual(dy[0], fpx, eps, eps);

	// compue second partial of f w.r.t. x[0] using forward mode
	CPPAD_TEST_VECTOR<double> ddx(n), ddy(m);
	ddx[0] = 0.;
	ddy    = f.Forward(2, ddx);

	// f''(x) = 2 * f(x) * f'(x)
	// Note that the second order Taylor coefficient for y is f''(x) / 2.
	double fppx = 2. * fx * fpx;
	ok   &= NearEqual(2. * ddy[0], fppx, eps, eps);

	// --------------------------------------------------------------------
	// Free temporary work space. (If there are future calls to 
	// user_tan they would create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
// END PROGRAM
