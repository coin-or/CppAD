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
$begin atom_usead_2.cpp$$
$spell
	checkpoint
	var
$$

$section Using AD to Compute Atomic Function Derivatives$$

$index AD, inside atomic$$
$index user, atomic AD inside$$
$index atomic, AD inside$$
$index checkpoint$$

$head Warning$$
This example is currently under construction.

$head Purpose$$
Consider the case where an inner function is used repeatedly in the 
definition of an outer function.
In this case, it may reduce the number of variables
$cref/size_var/seq_property/size_var/$$,
and hence the required memory.
This is similar the 
$cref/checkpoint/checkpoint.cpp/$$.

$code
$verbatim%example/atom_usead_2.cpp%0%// BEGIN C++%// END C++%1%$$
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
	// ODE for [t, t^2 / 2 ] in form required by Runge45
	class Fun {
	public:
		void Ode(
			const AD<double>           &t, 
			const vector< AD<double> > &z, 
			vector< AD<double> >       &f)
		{	assert( z.size() == 2 );
			assert( f.size() == 2 );
			f[0] =  1.0;
			f[1] =  z[0];
		}
	};

	// ----------------------------------------------------------------------
	// Create function that takes on Runge45 step for the ODE above
	ADFun<double>* r_ptr_;
	void create_r(void)
	{	size_t n = 3, m = 2;
		vector< AD<double> > x(n), zi(m), y(m), e(m);
		// The value of x does not matter because the operation sequence
		// does not depend on x. 
		x[0]  = 0.0;  // initial value z_0 (t) at t = ti
		x[1]  = 0.0;  // initial value z_1 (t) at t = ti
		x[2]  = 0.1;  // final time for this integration
		CppAD::Independent(x);
		zi[0]         = x[0];  // z_0 (t) at t = ti
		zi[1]         = x[1];  // z_1 (t) at t = ti
		AD<double> ti = 0.0;   // t does not appear in ODE so does not matter
		AD<double> tf = x[2];  // final time
		size_t M      = 3;     // number of Runge45 steps to take
		Fun F;
		y             = CppAD::Runge45(F, M, ti, tf, zi, e);
		r_ptr_        = new ADFun<double>(x, y);
	}
	void destroy_r(void)
	{	delete r_ptr_; 
		r_ptr_ = CPPAD_NULL;
	}

	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool atom_usead_2_forward(
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
		assert( n == 3 );
		assert( m == 2 );
		assert( k == 0 || vx.size() == 0 );
		bool ok = true;	
		vector<double> xp(n), yp(m);
		size_t i, j;

		// check for special case
		if( vx.size() > 0 )
		{	vy[0] = vx[0] | vx[1] | vx[2];
			vy[1] = vy[0];
		}

		// make sure r_ has proper lower order Taylor coefficients stored
		// then compute ty[k]
		for(size_t p = 0; p <= k; p++)
		{	for(j = 0; j < n; j++)
				xp[j] = tx[j * (k+1) + p];
			yp    = r_ptr_->Forward(p, xp);
			if( p == k )
			{	for(i = 0; i < m; i++)
					ty[i * (k+1) + p] = yp[i];
			}
# ifndef NDEBUG
			else
			{	for(i = 0; i < m; i++)
					assert( ty[i * (k+1) + p] == yp[i] );
			}
# endif
		}
		return ok;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	bool atom_usead_2_reverse(
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
		assert( n == 3 );
		assert( m == 2 );
		bool ok = true;	
		vector<double> xp(n), w( (k+1) * m ), dw( (k+1) * n );

		// make sure r_ has proper forward mode coefficients 
		size_t i, j, p;
		for(p = 0; p <= k; p++)
		{	for(j = 0; j < n; j++)
				xp[j] = tx[j * (k+1) + p];
# ifdef NDEBUG
			r_ptr_->Forward(p, xp);
# else
			vector<double> yp(m);
			yp = r_ptr_->Forward(p, xp);
			for(i = 0; i < m; i++)
				assert( ty[i * (k+1) + p] == yp[i] );
# endif
		}
		for(i = 0; i < m; i++)
		{	for(p = 0; p <=k; p++)
				w[ i * (k+1) + p] = py[ i * (k+1) + p];
		}
		dw = r_ptr_->Reverse(k+1, w);
		for(j = 0; j < n; j++)
		{	for(p = 0; p <=k; p++)
				px[ j * (k+1) + p] = dw[ j * (k+1) + p];
		}

		return ok;
	}
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	bool atom_usead_2_for_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		vector< std::set<size_t> >&           s )
	{	assert( id == 0 );
		assert( n == 3 );
		assert( m == 2 );
		bool ok = true;

		vector< std::set<size_t> > R(n), S(m);
		for(size_t j = 0; j < n; j++)
			R[j] = r[j];
		S = r_ptr_->ForSparseJac(q, R);
		for(size_t i = 0; i < m; i++)
			s[i] = S[i];
		
		return ok; 
	}
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	bool atom_usead_2_rev_jac_sparse(
		size_t                               id ,             
		size_t                                n ,
		size_t                                m ,
		size_t                                q ,
		vector< std::set<size_t> >&           r ,
		const vector< std::set<size_t> >&     s )
	{
		assert( id == 0 );
		assert( n == 3 );
		assert( m == 2 );
		bool ok = true;

		vector< std::set<size_t> > R(q), S(q);
		std::set<size_t>::const_iterator itr;
		size_t i;
		// untranspose s
		for(i = 0; i < m; i++)
		{	for(itr = s[i].begin(); itr != s[i].end(); itr++)
				S[*itr].insert(i);
		}
		R = r_ptr_->RevSparseJac(q, S);
		// transpose r
		for(i = 0; i < m; i++)
			r[i].clear();
		for(i = 0; i < q; i++)
		{	for(itr = R[i].begin(); itr != R[i].end(); itr++)
				r[*itr].insert(i);
		}
		return ok; 
	}
	// ----------------------------------------------------------------------
	// reverse Hessian sparsity routine called by CppAD
	bool atom_usead_2_rev_hes_sparse(
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
		assert( n == 3 );
		assert( m == 2 );
		bool ok = true;
		std::set<size_t>::const_iterator itr;

		// compute sparsity pattern for T(x) = S(x) * f'(x)
		vector< std::set<size_t> > T(1), S(1);
		size_t i, j;
		S[0].clear();
		for(i = 0; i < m; i++)
			if( s[i] )
				S[0].insert(i);
		T = r_ptr_->RevSparseJac(1, S);
		for(i = 0; i < m; i++)
			t[i] = false;
		for(itr = T[0].begin(); itr != T[0].end(); itr++)
			t[*itr] = true;

		// compute sparsity pattern for A(x)^T = U(x)^T * f'(x)
		vector< std::set<size_t> > Ut(q), At(q);
		for(i = 0; i < m; i++)
		{	for(itr = u[i].begin(); itr != u[i].end(); itr++)
				Ut[*itr].insert(i);
		}
		At = r_ptr_->RevSparseJac(q, Ut);

		// compute sparsity pattern for H(x)^T = R^T * (S * F)''(x)
		vector< std::set<size_t> > R(n), Ht(q);
		for(j = 0; j < n; j++)
			R[j] = r[j];
		r_ptr_->ForSparseJac(q, R);
		Ht = r_ptr_->RevSparseHes(q, S);

		// compute sparsity pattern for V(x) = A(x) + H(x)^T
		for(j = 0; j < n; j++)
			v[j].clear();
		for(i = 0; i < q; i++)
		{	for(itr = At[i].begin(); itr != At[i].end(); itr++)
				v[*itr].insert(i);
			for(itr = Ht[i].begin(); itr != Ht[i].end(); itr++)
				v[*itr].insert(i);
		}

		return ok;
	}
	// ---------------------------------------------------------------------
	// Declare the AD<double> routine atom_usead_2(id, ax, ay)
	CPPAD_USER_ATOMIC(
		atom_usead_2                 , 
		CppAD::vector              ,
		double                     , 
		atom_usead_2_forward         , 
		atom_usead_2_reverse         ,
		atom_usead_2_for_jac_sparse  ,
		atom_usead_2_rev_jac_sparse  ,
		atom_usead_2_rev_hes_sparse  
	)
} // End empty namespace

bool atom_usead_2(void)
{	bool ok = true;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();

	// --------------------------------------------------------------------
	// Create the ADFun<doulbe> r_
	create_r();

	// --------------------------------------------------------------------
	// domain and range space vectors
	size_t n = 3, m = 2;
	vector< AD<double> > au(n), ax(n), ay(m);
	au[0]         = 0.0;        // value of z_0 (t) = t, at t = 0
	ax[1]         = 0.0;        // value of z_1 (t) = t^2/2, at t = 0
	au[2]         = 1.0;        // final t
	CppAD::Independent(au);
	size_t M      = 2;          // number of r steps to take
	ax[0]         = au[0];      // value of z_0 (t) = t, at t = 0
	ax[1]         = au[1];      // value of z_1 (t) = t^2/2, at t = 0
	AD<double> dt = au[2] / M;  // size of each r step
	ax[2]         = dt;
	for(size_t i_step = 0; i_step < M; i_step++)
	{	size_t id = 0;               // not used
		atom_usead_2(id, ax, ay); 
		ax[0] = ay[0];
		ax[1] = ay[1];
	}

	// create f: u -> y and stop tape recording
	// y_0(t) = u_0 + t                   = u_0 + u_2
	// y_1(t) = u_1 + u_0 * t + t^2 / 2   = u_1 + u_0 * u_2 + u_2^2 / 2
	// where t = u_2
	ADFun<double> f(au, ay); 

	// --------------------------------------------------------------------
	// Check forward mode results
	//
	// function values
	vector<double> up(n), yp(m);
	size_t p  = 0;
	double u0 = 0.5;
	double u1 = 0.25;
	double u2 = 0.75;
	double check;
	up[0]     = u0;
	up[1]     = u1;
	up[2]     = u2;
	yp        = f.Forward(p, up);
	check     = u0 + u2;
	ok       &= NearEqual( yp[0], check,  eps, eps);
	check     = u1 + u0 * u2 + u2 * u2 / 2.0;
	ok       &= NearEqual( yp[1], check,  eps, eps);
	//
	// forward mode first derivative w.r.t t
	p         = 1;
	up[0]     = 0.0;
	up[1]     = 0.0;
	up[2]     = 1.0;
	yp        = f.Forward(p, up);
	check     = 1.0;
	ok       &= NearEqual( yp[0], check,  eps, eps);
	check     = u0 + u2;
	ok       &= NearEqual( yp[1], check,  eps, eps);
	//
	// forward mode second order Taylor coefficient w.r.t t
	p         = 2;
	up[0]     = 0.0;
	up[1]     = 0.0;
	up[2]     = 0.0;
	yp        = f.Forward(p, up);
	check     = 0.0;
	ok       &= NearEqual( yp[0], check,  eps, eps);
	check     = 1.0 / 2.0;
	ok       &= NearEqual( yp[1], check,  eps, eps);
	// --------------------------------------------------------------------
	// reverse mode derivatives of \partial_t y_1 (t)
	vector<double> w(m * p), dw(n * p);
	w[0 * p + 0]  = 0.0;
	w[1 * p + 0]  = 0.0;
	w[0 * p + 1]  = 0.0;
	w[1 * p + 1]  = 1.0;
	dw        = f.Reverse(p, w);
	// derivative of y_1(u) = u_1 + u_0 * u_2 + u_2^2 / 2,  w.r.t. u
	// is equal deritative of \partial_u2 y_1(u) w.r.t \partial_u2 u
	check     = u2;
	ok       &= NearEqual( dw[0 * p + 1], check,  eps, eps);
	check     = 1.0;
	ok       &= NearEqual( dw[1 * p + 1], check,  eps, eps);
	check     = u0 + u2;
	ok       &= NearEqual( dw[2 * p + 1], check,  eps, eps);
	// derivative of \partial_t y_1 w.r.t u = u_0 + t,  w.r.t u
	check     = 1.0;
	ok       &= NearEqual( dw[0 * p + 0], check,  eps, eps);
	check     = 0.0;
	ok       &= NearEqual( dw[1 * p + 0], check,  eps, eps);
	check     = 1.0;
	ok       &= NearEqual( dw[2 * p + 0], check,  eps, eps);
	// --------------------------------------------------------------------
	// forward mode sparsity pattern for the Jacobian
	// f_u = [   1, 0,   1 ]
	//       [ u_2, 1, u_2 ] 
	size_t i, j, q = n;
	CppAD::vectorBool r(n * q), s(m * q);
	// r = identity sparsity pattern
	for(i = 0; i < n; i++)
		for(j = 0; j < q; j++)
			r[i*n +j] = (i == j); 
	s   = f.ForSparseJac(q, r);
	ok &= s[ 0 * q + 0] == true;
	ok &= s[ 0 * q + 1] == false;
	ok &= s[ 0 * q + 2] == true;
	ok &= s[ 1 * q + 0] == true;
	ok &= s[ 1 * q + 1] == true;
	ok &= s[ 1 * q + 2] == true;
	// --------------------------------------------------------------------
	// reverse mode sparsity pattern for the Jacobian
	p = m;
	s.resize(p * m);
	r.resize(p * n);
	// s = identity sparsity pattern
	for(i = 0; i < p; i++)
		for(j = 0; j < m; j++)
			s[i*m +j] = (i == j); 
	r   = f.RevSparseJac(p, s);
	ok &= r[ 0 * n + 0] == true;
	ok &= r[ 0 * n + 1] == false;
	ok &= r[ 0 * n + 2] == true;
	ok &= r[ 1 * n + 0] == true;
	ok &= r[ 1 * n + 1] == true;
	ok &= r[ 1 * n + 2] == true;

	// --------------------------------------------------------------------
	// Hessian sparsity for y_1 (u) = u_1 + u_0 * u_2 + u_2^2 / 2
	s.resize(m);
	s[0] = false;
	s[1] = true;
	r.resize(n * n);
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			r[ i * n + j ] = (i == j);
	CppAD::vectorBool h(n * n);
	h   = f.RevSparseHes(n, s);
	ok &= h[0 * n + 0] == false;
	ok &= h[0 * n + 1] == false;
	ok &= h[0 * n + 2] == true;
	ok &= h[1 * n + 0] == false;
	ok &= h[1 * n + 1] == false;
	ok &= h[1 * n + 2] == false;
	ok &= h[2 * n + 0] == true;
	ok &= h[2 * n + 1] == false;
	ok &= h[2 * n + 2] == true;
	
	// --------------------------------------------------------------------
	destroy_r();

	// Free all temporary work space associated with user_atomic objects. 
	// (If there are future calls to user atomic functions, they will 
	// create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
// END C++
