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
	Tanh
$$

$section Tan and Tanh as User Atomic Operations: Example and Test$$

$index tan, user_atomic$$
$index user, atomic tan$$
$index atomic, tan$$
$index test, user_atomic$$
$index user_atomic, example$$
$index example, user_atomic$$

$head Theory$$
The code below uses the $cref/tan_forward/$$ and $cref/tan_reverse/$$
to implement the tangent ($icode%id% == 0%$$) and hyperbolic tangent
($icode%id% == 1%$$) functions as user atomic operations.

$code
$verbatim%example/user_tan.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

namespace { // Begin empty namespace 
	using CppAD::vector;

	// a utility to compute the union of two sets.
	void my_union(
		std::set<size_t>&         result  ,
		const std::set<size_t>&   left    ,
		const std::set<size_t>&   right   )
	{	std::set<size_t> temp;
		std::set_union(
			left.begin()              ,
			left.end()                ,
			right.begin()             ,
			right.end()               ,
			std::inserter(temp, temp.begin())
		);
		result.swap(temp);
	}

	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool user_tan_forward(
		size_t                   id ,
		size_t                order ,
		size_t                    n ,
		size_t                    m ,
		const vector<bool>&      vx ,
		vector<bool>&           vzy ,
		const vector<double>&    tx ,
		vector<double>&         tzy
	)
	{
		assert( n == 1 );
		assert( m == 2 );
		assert( id == 0 || id == 1 );
		assert( tx.size() >= (order+1) * n );
		assert( tzy.size() >= (order+1) * m );

		size_t n_order = order + 1;
		size_t j = order;
		size_t k;

		// check if this is during the call to user_tan(id, ax, ay)
		if( vx.size() > 0 )
		{	assert( vx[0] == true  );
			assert( vx.size() >= n );
			assert( vzy.size() >= m );
			
			// now setvzy
			vzy[0] = true;
			vzy[1] = true;
		}

		if( j == 0 )
		{	// z^{(0)} = tan( x^{(0)} ) or tanh( x^{(0)} )
			if( id == 0 )
				tzy[0] = tan( tx[0] );
			else	tzy[0] = tanh( tx[0] );

			// y^{(0)} = z^{(0)} * z^{(0)}
			tzy[n_order + 0] = tzy[0] * tzy[0];
		}
		else
		{	double j_inv = 1. / double(j);
			if( id == 1 )
				j_inv = - j_inv;

			// z^{(j)} = x^{(j)} +- sum_{k=1}^j k x^{(k)} y^{(j-k)} / j
			tzy[j] = tx[j];  
			for(k = 1; k <= j; k++)
				tzy[j] += tx[k] * tzy[n_order + j-k] * k * j_inv;

			// y^{(j)} = sum_{k=0}^j z^{(k)} z^{(j-k)}
			tzy[n_order + j] = 0.;
			for(k = 0; k <= j; k++)
				tzy[n_order + j] += tzy[k] * tzy[j-k];
		}
			
		// All orders are implemented and there are no possible errors
		return true;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	bool user_tan_reverse(
		size_t                   id ,
		size_t                order ,
		size_t                    n ,
		size_t                    m ,
		const vector<double>&    tx ,
		const vector<double>&   tzy ,
		vector<double>&          px ,
		const vector<double>&   pzy
	)
	{	assert( n == 1 );
		assert( m == 2 );
		assert( id == 0 || id == 1 );
		assert( tx.size() >= (order+1) * n );
		assert( tzy.size() >= (order+1) * m );
		assert( px.size() >= (order+1) * n );
		assert( pzy.size() >= (order+1) * m );

		size_t n_order = order + 1;
		size_t j, k;

		// copy because partials w.r.t. y and z need to change
		vector<double> qzy = pzy;

		// initialize accumultion of reverse mode partials
		for(k = 0; k < n_order; k++)
			px[k] = 0.;

		// eliminate positive orders
		for(j = order; j > 0; j--)
		{	double j_inv = 1. / double(j);
			if( id == 1 )
				j_inv = - j_inv;

			// H_{x^{(k)}} += delta(j-k) +- H_{z^{(j)} y^{(j-k)} * k / j
			px[j] += qzy[j];
			for(k = 1; k <= j; k++)
				px[k] += qzy[j] * tzy[n_order + j-k] * k * j_inv;  

			// H_{y^{j-k)} += +- H_{z^{(j)} x^{(k)} * k / j
			for(k = 1; k <= j; k++)
				qzy[n_order + j-k] += qzy[j] * tx[k] * k * j_inv;  

			// H_{z^{(k)}} += H_{y^{(j-1)}} * z^{(j-k-1)} * 2. 
			for(k = 0; k < j; k++)
				qzy[k] += qzy[n_order + j-1] * tzy[j-k-1] * 2.; 
		}

		// eliminate order zero
		if( id == 0 )
			px[0] += qzy[0] * (1. + tzy[n_order + 0]);
		else
			px[0] += qzy[0] * (1. - tzy[n_order + 0]);

		return true; 
	}
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	bool user_tan_for_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	// sparsity for z and y are the same as for x
		s[0] = r[0];
		s[1] = r[0];

		return true;
	}
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	bool user_tan_rev_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		vector< std::set<size_t> >&           r ,
		const vector< std::set<size_t> >&     s )
	{	// note that, if the users code only uses z, and not y,
		// we could just set r[0] = s[0]	
		my_union(r[0], s[0], s[1]);
		return true; 
	}
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
	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> > u(m);

	// temporary vector for user_tan computations
	// (user_tan computes tan or tanh and its square)
	CPPAD_TEST_VECTOR< AD<double> > z(2);

	// call user tan function and store tan(x) in u[0] (ignore tan(x)^2)
	size_t id = 0;
	user_tan(id, x, z);
	u[0] = z[0];

	// call user tanh function and store tanh(x) in u[1] (ignore tanh(x)^2)
	id = 1;
	user_tan(id, x, z);
	u[1] = z[0];

	// put a constant in u[2] (for sparsity pattern testing)
	u[2] = 1.; 

	// create f: x -> u and stop tape recording
	CppAD::ADFun<double> f(x, u); 

	// check value 
	double tan = std::tan(x0);
	ok &= NearEqual(u[0] , tan,  eps, eps);
	double tanh = std::tanh(x0);
	ok &= NearEqual(u[1] , tanh,  eps, eps);

	// compute first partial of f w.r.t. x[0] using forward mode
	CPPAD_TEST_VECTOR<double> dx(n), du(m);
	dx[0] = 1.;
	du    = f.Forward(1, dx);

	// compute derivative of tan - tanh using reverse mode
	CPPAD_TEST_VECTOR<double> w(m), dw(n);
	w[0]  = 1.;
	w[1]  = 1.;
	dw    = f.Reverse(1, w);

	// tan'(x)   = 1 + tan(x)  * tan(x) 
	// tanh'(x)  = 1 - tanh(x) * tanh(x) 
	double tanp  = 1. + tan * tan; 
	double tanhp = 1. - tanh * tanh; 
	ok   &= NearEqual(du[0], tanp, eps, eps);
	ok   &= NearEqual(du[1], tanhp, eps, eps);
	ok   &= NearEqual(dw[0], w[0]*tanp + w[1]*tanhp, eps, eps);

	// compute second partial of f w.r.t. x[0] using forward mode
	CPPAD_TEST_VECTOR<double> ddx(n), ddu(m);
	ddx[0] = 0.;
	ddu    = f.Forward(2, ddx);

	// compute second derivative of tan - tanh using reverse mode
	CPPAD_TEST_VECTOR<double> ddw(2);
	ddw   = f.Reverse(2, w);

	// tan''(x)   = 2 *  tan(x) * tan'(x) 
	// tanh''(x)  = - 2 * tanh(x) * tanh'(x) 
	// Note that second order Taylor coefficient for u half the
	// corresponding second derivative.
	double tanpp  =   2. * tan * tanp;
	double tanhpp = - 2. * tanh * tanhp;
	ok   &= NearEqual(2. * ddu[0], tanpp, eps, eps);
	ok   &= NearEqual(2. * ddu[1], tanhpp, eps, eps);
	ok   &= NearEqual(ddw[0], w[0]*tanp  + w[1]*tanhp , eps, eps);
	ok   &= NearEqual(ddw[1], w[0]*tanpp + w[1]*tanhpp, eps, eps);

	// Forward mode computation of sparsity pattern for f.
	size_t q = n;
	// user vectorBool because m and n are small
	CppAD::vectorBool r(q), s(m * q);
	r[0] = true;            // propogate sparsity for x[0]
	s    = f.ForSparseJac(q, r);
	ok  &= (s[0] == true);  // u[0] depends on x[0]
	ok  &= (s[1] == true);  // u[1] depends on x[0]
	ok  &= (s[2] == false); // u[2] does not depend on x[0]

	// Reverse mode computation of sparsity pattern for f.
	size_t p = m;
	CppAD::vectorBool S(p * m), R(p * n);
	// Sparsity pattern for identity matrix
	size_t i, j;
	for(i = 0; i < p; i++)
	{	for(j = 0; j < m; j++)
			S[i * p + j] = (i == j);
	}
	R    = f.RevSparseJac(p, S);
	ok  &= (R[0] == true);  // u[0] depends on x[0]
	ok  &= (R[1] == true);  // u[1] depends on x[0]
	ok  &= (R[2] == false); // u[2] does not depend on x[0]

	// --------------------------------------------------------------------
	// Free all temporary work space associated with user_atomic objects. 
	// (If there are future calls to user atomic functions, they will 
	// create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
// END PROGRAM
