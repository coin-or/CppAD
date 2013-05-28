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
$begin atomic_tangent.cpp$$
$spell
	Tanh
$$

$section Tan and Tanh as User Atomic Operations: Example and Test$$
$index tangent, atomic operation$$
$index atomic, tangent operation$$
$index tan, atomic operation$$

$head Theory$$
The code below uses the $cref tan_forward$$ and $cref tan_reverse$$
to implement the tangent and hyperbolic tangent
functions as user atomic operations.

$code
$verbatim%example/atomic/tangent.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
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
//
class atomic_tangent : public CppAD::atomic_base<float> {
private:
	const bool hyperbolic_; // is this hyperbolic tangent
public:
	// ----------------------------------------------------------------------
	// constructor
	atomic_tangent(const char* name, bool hyperbolic) 
	: atomic_base<float>(name),
	hyperbolic_(hyperbolic)
	{ }
private:
	// ----------------------------------------------------------------------
	// forward mode routine called by CppAD
	bool forward(
		size_t                    q ,
		size_t                    p ,
		const vector<bool>&      vx ,
		      vector<bool>&     vzy ,
		const vector<float>&     tx ,
		      vector<float>&    tzy
	)
	{	size_t p1 = p + 1;
		size_t n  = tx.size()  / p1;
		size_t m  = tzy.size() / p1;
		assert( n == 1 );
		assert( m == 2 );
		assert( q <= p );
		size_t j, k;

		// check if this is during the call to old_tan(id, ax, ay)
		if( vx.size() > 0 )
		{	// set variable flag for both y an z
			vzy[0] = vx[0];
			vzy[1] = vx[0];
		}

		if( q == 0 )
		{	// z^{(0)} = tan( x^{(0)} ) or tanh( x^{(0)} )
			if( hyperbolic_ )
				tzy[0] = tanh( tx[0] );
			else	tzy[0] = tan( tx[0] );

			// y^{(0)} = z^{(0)} * z^{(0)}
			tzy[p1 + 0] = tzy[0] * tzy[0];
		
			q++;
		}
		for(j = q; j <= p; j++)
		{	float j_inv = 1.f / float(j);
			if( hyperbolic_ )
				j_inv = - j_inv;

			// z^{(j)} = x^{(j)} +- sum_{k=1}^j k x^{(k)} y^{(j-k)} / j
			tzy[j] = tx[j];  
			for(k = 1; k <= j; k++)
				tzy[j] += tx[k] * tzy[p1 + j-k] * k * j_inv;

			// y^{(j)} = sum_{k=0}^j z^{(k)} z^{(j-k)}
			tzy[p1 + j] = 0.;
			for(k = 0; k <= j; k++)
				tzy[p1 + j] += tzy[k] * tzy[j-k];
		}

		// All orders are implemented and there are no possible errors
		return true;
	}
	// ----------------------------------------------------------------------
	// reverse mode routine called by CppAD
	virtual bool reverse(
		size_t                    p ,
		const vector<float>&     tx ,
		const vector<float>&    tzy ,
		      vector<float>&     px ,
		const vector<float>&    pzy
	)
	{	size_t p1 = p + 1;
		size_t n  = tx.size()  / p1;
		size_t m  = tzy.size() / p1;	
		assert( px.size()  == n * p1 );
		assert( pzy.size() == m * p1 );
		assert( n == 1 );
		assert( m == 2 );

		size_t j, k;

		// copy because partials w.r.t. y and z need to change
		vector<float> qzy = pzy;

		// initialize accumultion of reverse mode partials
		for(k = 0; k < p1; k++)
			px[k] = 0.;

		// eliminate positive orders
		for(j = p; j > 0; j--)
		{	float j_inv = 1.f / float(j);
			if( hyperbolic_ )
				j_inv = - j_inv;

			// H_{x^{(k)}} += delta(j-k) +- H_{z^{(j)} y^{(j-k)} * k / j
			px[j] += qzy[j];
			for(k = 1; k <= j; k++)
				px[k] += qzy[j] * tzy[p1 + j-k] * k * j_inv;  

			// H_{y^{j-k)} += +- H_{z^{(j)} x^{(k)} * k / j
			for(k = 1; k <= j; k++)
				qzy[p1 + j-k] += qzy[j] * tx[k] * k * j_inv;  

			// H_{z^{(k)}} += H_{y^{(j-1)}} * z^{(j-k-1)} * 2. 
			for(k = 0; k < j; k++)
				qzy[k] += qzy[p1 + j-1] * tzy[j-k-1] * 2.f; 
		}

		// eliminate order zero
		if( hyperbolic_ )
			px[0] += qzy[0] * (1.f - tzy[p1 + 0]);
		else
			px[0] += qzy[0] * (1.f + tzy[p1 + 0]);

		return true; 
	}
	// ----------------------------------------------------------------------
	// forward Jacobian sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		size_t                                q ,
		const vector<bool>&                   r ,
		      vector<bool>&                   s )
	{	size_t n = r.size() / q;
		size_t m = s.size() / q;
		assert( n == 1 );
		assert( m == 2 );

		// sparsity for S(x) = f'(x) * R
		for(size_t j = 0; j < q; j++)
		{	s[0 * q + j] = r[j];
			s[1 * q + j] = r[j];
		}

		return true;
	}
	// forward Jacobian sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		      vector< std::set<size_t> >&     s )
	{	size_t n = r.size();
		size_t m = s.size();
		assert( n == 1 );
		assert( m == 2 );

		// sparsity for S(x) = f'(x) * R
		s[0] = r[0];
		s[1] = r[0];

		return true;
	}
	// ----------------------------------------------------------------------
	// reverse Jacobian sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		size_t                                q ,
		const vector<bool>&                  rt ,
		      vector<bool>&                  st )
	{	size_t n = st.size() / q;
		size_t m = rt.size() / q;
		assert( n == 1 );
		assert( m == 2 );

		// sparsity for S(x)^T = f'(x)^T * R^T
		for(size_t j = 0; j < q; j++)
			st[j] = rt[0 * q + j] | rt[1 * q + j];

		return true; 
	}
	// reverse Jacobian sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		size_t                                q ,
		const vector< std::set<size_t> >&    rt ,
		      vector< std::set<size_t> >&    st )
	{	size_t n = st.size();
		size_t m = rt.size();
		assert( n == 1 );
		assert( m == 2 );

		// sparsity for S(x)^T = f'(x)^T * R^T
		my_union(st[0], rt[0], rt[1]);
		return true; 
	}
	// ----------------------------------------------------------------------
	// reverse Hessian sparsity routine called by CppAD
	virtual bool rev_sparse_hes(
		const vector<bool>&                   vx,
		const vector<bool>&                   s ,
		      vector<bool>&                   t ,
		size_t                                q ,
		const vector<bool>&                   r ,
		const vector<bool>&                   u ,
		      vector<bool>&                   v )
	{
		size_t m = s.size();
		size_t n = t.size();
		assert( r.size() == n * q );
		assert( u.size() == m * q );
		assert( v.size() == n * q );
		assert( n == 1 );
		assert( m == 2 );

		// There are no cross term second derivatives for this case,
		// so it is not necessary to vx.

		// sparsity for T(x) = S(x) * f'(x) 
		t[0] =  s[0] | s[1];

		// V(x) = f'(x)^T * g''(y) * f'(x) * R  +  g'(y) * f''(x) * R 
		// U(x) = g''(y) * f'(x) * R
		// S(x) = g'(y)
		
		// back propagate the sparsity for U, note both components 
		// of f'(x) may be non-zero;
		size_t j;
		for(j = 0; j < q; j++)
			v[j] = u[ 0 * q + j ] | u[ 1 * q + j ];

		// include forward Jacobian sparsity in Hessian sparsity
		// (note sparsty for f''(x) * R same as for R)
		if( s[0] | s[1] )
		{	for(j = 0; j < q; j++)
				v[j] |= r[j];
		}

		return true;
	}
	// reverse Hessian sparsity routine called by CppAD
	virtual bool rev_sparse_hes(
		const vector<bool>&                   vx,
		const vector<bool>&                   s ,
		      vector<bool>&                   t ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		const vector< std::set<size_t> >&     u ,
		      vector< std::set<size_t> >&     v )
	{	size_t m = s.size();
		size_t n = t.size();
		assert( r.size() == n );
		assert( u.size() == m );
		assert( v.size() == n );
		assert( n == 1 );
		assert( m == 2 );

		// There are no cross term second derivatives for this case,
		// so it is not necessary to vx.

		// sparsity for T(x) = S(x) * f'(x) 
		t[0] =  s[0] | s[1];

		// V(x) = f'(x)^T * g''(y) * f'(x) * R  +  g'(y) * f''(x) * R 
		// U(x) = g''(y) * f'(x) * R
		// S(x) = g'(y)
		
		// back propagate the sparsity for U, note both components 
		// of f'(x) may be non-zero;
		my_union(v[0], u[0], u[1]);

		// include forward Jacobian sparsity in Hessian sparsity
		// (note sparsty for f''(x) * R same as for R)
		if( s[0] | s[1] )
			my_union(v[0], v[0], r[0]);

		return true;
	}
}; // End of atomic_tangent class
}  // End empty namespace

bool tangent(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	float eps = 10.f * CppAD::numeric_limits<float>::epsilon();

	// --------------------------------------------------------------------
	// Creater a tan and tanh object
	atomic_tangent my_tan("my_tan", false), my_tanh("my_tanh", true);
	// --------------------------------------------------------------------

	// domain space vector
	size_t n  = 1;
	float  x0 = 0.5;
	CppAD::vector< AD<float> > ax(n);
	ax[0]     = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector 
	size_t m = 3;
	CppAD::vector< AD<float> > af(m);

	// temporary vector for computations
	// (my_tan and my_tanh computes tan or tanh and its square)
	CppAD::vector< AD<float> > az(2);

	// call atomic tan function and store tan(x) in f[0] (ignore tan(x)^2)
	my_tan(ax, az);
	af[0] = az[0];

	// call atomic tanh function and store tanh(x) in f[1] (ignore tanh(x)^2)
	my_tanh(ax, az);
	af[1] = az[0];

	// put a constant in f[2] = tanh(1.) (for sparsity pattern testing)
	CppAD::vector< AD<float> > one(1);
	one[0] = 1.;
	my_tanh(one, az);
	af[2] = az[0]; 

	// create f: x -> f and stop tape recording
	CppAD::ADFun<float> F;
	F.Dependent(ax, af); 

	// check function value 
	float tan = std::tan(x0);
	ok &= NearEqual(af[0] , tan,  eps, eps);
	float tanh = std::tanh(x0);
	ok &= NearEqual(af[1] , tanh,  eps, eps);

	// check zero order forward
	CppAD::vector<float> x(n), f(m);
	x[0] = x0;
	f    = F.Forward(0, x);
	ok &= NearEqual(f[0] , tan,  eps, eps);
	ok &= NearEqual(f[1] , tanh,  eps, eps);

	// compute first partial of f w.r.t. x[0] using forward mode
	CppAD::vector<float> dx(n), df(m);
	dx[0] = 1.;
	df    = F.Forward(1, dx);

	// compute derivative of tan - tanh using reverse mode
	CppAD::vector<float> w(m), dw(n);
	w[0]  = 1.;
	w[1]  = 1.;
	w[2]  = 0.;
	dw    = F.Reverse(1, w);

	// tan'(x)   = 1 + tan(x)  * tan(x) 
	// tanh'(x)  = 1 - tanh(x) * tanh(x) 
	float tanp  = 1.f + tan * tan; 
	float tanhp = 1.f - tanh * tanh; 
	ok   &= NearEqual(df[0], tanp, eps, eps);
	ok   &= NearEqual(df[1], tanhp, eps, eps);
	ok   &= NearEqual(dw[0], w[0]*tanp + w[1]*tanhp, eps, eps);

	// compute second partial of f w.r.t. x[0] using forward mode
	CppAD::vector<float> ddx(n), ddf(m);
	ddx[0] = 0.;
	ddf    = F.Forward(2, ddx);

	// compute second derivative of tan - tanh using reverse mode
	CppAD::vector<float> ddw(2);
	ddw   = F.Reverse(2, w);

	// tan''(x)   = 2 *  tan(x) * tan'(x) 
	// tanh''(x)  = - 2 * tanh(x) * tanh'(x) 
	// Note that second order Taylor coefficient for u half the
	// corresponding second derivative.
	float two    = 2;
	float tanpp  =   two * tan * tanp;
	float tanhpp = - two * tanh * tanhp;
	ok   &= NearEqual(two * ddf[0], tanpp, eps, eps);
	ok   &= NearEqual(two * ddf[1], tanhpp, eps, eps);
	ok   &= NearEqual(ddw[0], w[0]*tanp  + w[1]*tanhp , eps, eps);
	ok   &= NearEqual(ddw[1], w[0]*tanpp + w[1]*tanhpp, eps, eps);

	// Forward mode computation of sparsity pattern for F.
	size_t q = n;
	// user vectorBool because m and n are small
	CppAD::vectorBool r1(q), s1(m * q);
	r1[0] = true;            // propagate sparsity for x[0]
	s1    = F.ForSparseJac(q, r1);
	ok  &= (s1[0] == true);  // f[0] depends on x[0]
	ok  &= (s1[1] == true);  // f[1] depends on x[0]
	ok  &= (s1[2] == false); // f[2] does not depend on x[0]

	// Reverse mode computation of sparsity pattern for F.
	size_t p = m;
	CppAD::vectorBool s2(p * m), r2(p * n);
	// Sparsity pattern for identity matrix
	size_t i, j;
	for(i = 0; i < p; i++)
	{	for(j = 0; j < m; j++)
			s2[i * p + j] = (i == j);
	}
	r2   = F.RevSparseJac(p, s2);
	ok  &= (r2[0] == true);  // f[0] depends on x[0]
	ok  &= (r2[1] == true);  // f[1] depends on x[0]
	ok  &= (r2[2] == false); // f[2] does not depend on x[0]

	// Hessian sparsity for f[0]
	CppAD::vectorBool s3(m), h(q * n);
	s3[0] = true;
	s3[1] = false;
	s3[2] = false;
	h    = F.RevSparseHes(q, s3);
	ok  &= (h[0] == true);  // Hessian is non-zero

	// Hessian sparsity for f[2]
	s3[0] = false;
	s3[2] = true;
	h    = F.RevSparseHes(q, s3);
	ok  &= (h[0] == false);  // Hessian is zero

	// check tanh results for a large value of x
	x[0]  = std::numeric_limits<float>::max() / two;
	f     = F.Forward(0, x);
	tanh  = 1.;
	ok   &= NearEqual(f[1], tanh, eps, eps);
	df    = F.Forward(1, dx);
	tanhp = 0.;
	ok   &= NearEqual(df[1], tanhp, eps, eps);
 
	// --------------------------------------------------------------------
	// Free all temporary work space associated with atomic_basen objects. 
	// (If there are future calls to user atomic functions, they will 
	// create new temporary work space.)
	CppAD::user_atomic<float>::clear();

	return ok;
}
// END C++
