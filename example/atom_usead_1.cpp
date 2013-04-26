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
$begin atom_usead_1.cpp$$
$spell
	checkpoint
	var
$$

$section Using AD to Compute Atomic Function Derivatives$$

$index AD, inside atomic$$
$index user, atomic AD inside$$
$index atomic, AD inside$$
$index checkpoint$$

$head Purpose$$
Consider the case where an inner function is used repeatedly in the 
definition of an outer function.
In this case, it may reduce the number of variables
$cref/size_var/seq_property/size_var/$$,
and hence the required memory.
This is similar the 
$cref/checkpoint/checkpoint.cpp/$$.

$head Simple Case$$
This example is the same as $cref reciprocal.cpp$$, except that it 
uses AD to compute the 
derivatives needed by an atomic function.
This is a simple example of an inner function, and hence not really
useful for the purpose above; 
see $cref atom_usead_2.cpp$$ for a more complete example.

$code
$verbatim%example/atom_usead_1.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace { // Begin empty namespace 
	using CppAD::AD;
	using CppAD::ADFun;
	using CppAD::vector;

	// ----------------------------------------------------------------------
	// function that computes reciprocal
	ADFun<double>* r_ptr_;
	void create_r(void)
	{	vector< AD<double> > ax(1), ay(1);
		ax[0]  = 1;
		CppAD::Independent(ax);
		ay[0]  = 1.0 / ax[0];
		r_ptr_ = new ADFun<double>(ax, ay);
	}
	void destroy_r(void)
	{	delete r_ptr_; 
		r_ptr_ = CPPAD_NULL;
	}

	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool atom_usead_1_forward(
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
		bool ok = true;	
		vector<double> x_p(1), y_p(1);

		// check for special case
		if( vx.size() > 0 )
			vy[0] = vx[0];

		// make sure r_ has proper lower order Taylor coefficients stored
		// then compute ty[k]
		for(size_t p = 0; p <= k; p++)
		{	x_p[0] = tx[p];
			y_p    = r_ptr_->Forward(p, x_p);
			if( p == k )
				ty[k] = y_p[0];
			assert( p == k || ty[p] == y_p[0] );
		}
		return ok;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	bool atom_usead_1_reverse(
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
		bool ok = true;	
		vector<double> x_p(1), w(k+1), dw(k+1);

		// make sure r_ has proper forward mode coefficients 
		size_t p;
		for(p = 0; p <= k; p++)
		{	x_p[0] = tx[p];
# ifdef NDEBUG
			r_ptr_->Forward(p, x_p);
# else
			vector<double> y_p(1);
			y_p    = r_ptr_->Forward(p, x_p);
			assert( ty[p] == y_p[0] );
# endif
		}
		for(p = 0; p <=k; p++)
			w[p] = py[p];
		dw = r_ptr_->Reverse(k+1, w);
		for(p = 0; p <=k; p++)
			px[p] = dw[p];

		return ok;
	}
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	bool atom_usead_1_for_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	assert( id == 0 );
		assert( n == 1 );
		assert( m == 1 );
		bool ok = true;

		vector< std::set<size_t> > R(1), S(1);
		R[0] = r[0];
		S = r_ptr_->ForSparseJac(q, R);
		s[0] = S[0];

		return ok; 
	}
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	bool atom_usead_1_rev_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		vector< std::set<size_t> >&           r ,
		const vector< std::set<size_t> >&     s )
	{
		assert( id == 0 );
		assert( n == 1 );
		assert( m == 1 );
		bool ok = true;

		vector< std::set<size_t> > R(q), S(q);
		size_t p;
		for(p = 0; p < q; p++)
			S[p] = s[p];
		R = r_ptr_->RevSparseJac(q, S);
		for(p = 0; p < q; p++)
			r[p] = R[p];

		return ok; 
	}
	// ----------------------------------------------------------------------
	// reverse Hessian sparsity routine called by CppAD
	bool atom_usead_1_rev_hes_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		const vector<bool>&                   s ,
		vector<bool>&                         t ,
		const vector< std::set<size_t> >&     u ,
		vector< std::set<size_t> >&           v )
	{	// Can just return false if not use RevSparseHes.
		assert( id == 0 );
		assert( n == 1 );
		assert( m == 1 );
		bool ok = true;

		// compute sparsity pattern for T(x) = S(x) * f'(x) 
		vector<bool> T(1), S(1);
		S[0]   = s[0];
		T      = r_ptr_->RevSparseJac(1, S);
		t[0]   = T[0];

		// compute sparsity pattern for A(x) = U(x)^T * f'(x)
		vector<bool> Ut(q), A(q);
		size_t p;
		for(p = 0; p < q; p++)
			Ut[p] = false;
		std::set<size_t>::iterator itr;
		for(itr = u[0].begin(); itr != u[0].end(); itr++)
			Ut[*itr] = true;
		A = r_ptr_-> RevSparseJac(q, Ut);

		// compute sparsity pattern for H(x) = R^T * (S * F)''(x)
		vector<bool> H(q), R(n);
		for(p = 0; p < q; p++)
			R[p] = false;
		for(itr = r[0].begin(); itr != r[0].end(); itr++)
			R[*itr] = true;
		r_ptr_->ForSparseJac(q, R);
		H = r_ptr_->RevSparseHes(q, S);

		// compute sparsity pattern for V(x) = A(x)^T + H(x)^T
		v[0].clear();
		for(p = 0; p < q; p++)
			if( A[p] | H[p] )
				v[0].insert(p);

		return ok;
	}
	// ---------------------------------------------------------------------
	// Declare the AD<double> routine atom_usead_1(id, ax, ay)
	CPPAD_USER_ATOMIC(
		atom_usead_1                 , 
		CppAD::vector              ,
		double                     , 
		atom_usead_1_forward         , 
		atom_usead_1_reverse         ,
		atom_usead_1_for_jac_sparse  ,
		atom_usead_1_rev_jac_sparse  ,
		atom_usead_1_rev_hes_sparse  
	)
} // End empty namespace

bool atom_usead_1(void)
{	bool ok = true;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();

	// --------------------------------------------------------------------
	// Create the ADFun<doulbe> r_
	create_r();

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

	// call user function and store atom_usead_1(x) in au[0] 
	vector< AD<double> > au(m);
	size_t id = 0;           // not used
	atom_usead_1(id, ax, au);	// u = 1 / x

	// call user function and store atom_usead_1(u) in ay[0] 
	atom_usead_1(id, au, ay);	// y = 1 / u = x

	// create f: x -> y and stop tape recording
	ADFun<double> f(ax, ay);  // f(x) = x

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

	// --------------------------------------------------------------------
	// forward mode sparstiy pattern
	size_t q = n;
	CppAD::vectorBool r1(n * q), s1(m * q);
	r1[0] = true;          // compute sparsity pattern for x[0]
	s1    = f.ForSparseJac(q, r1);
	ok  &= s1[0] == true;  // f[0] depends on x[0]  

	// --------------------------------------------------------------------
	// reverse mode sparstiy pattern
	p = m;
	CppAD::vectorBool s2(p * m), r2(p * n);
	s2[0] = true;          // compute sparsity pattern for f[0]
	r2    = f.RevSparseJac(p, s2);
	ok  &= r2[0] == true;  // f[0] depends on x[0]  

	// --------------------------------------------------------------------
	// Hessian sparsity (using previous ForSparseJac call) 
	CppAD::vectorBool s3(m), h(q * n);
	s3[0] = true;        // compute sparsity pattern for f[0]
	h     = f.RevSparseJac(q, s3);
	ok  &= h[0] == true; // second partial of f[0] w.r.t. x[0] may be non-zero

	// -----------------------------------------------------------------
	// Free all memory associated with the object r_ptr
	destroy_r();

	// -----------------------------------------------------------------
	// Free all temporary work space associated with user_atomic objects. 
	// (If there are future calls to user atomic functions, they will 
	// create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
// END C++
