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
$begin atomic_norm_sq.cpp$$
$spell
	sq
$$

$section Euclidean Norms: Example and Test$$
$index norm_sq, atomic operation$$
$index atomic, norm_sq$$
$index Euclidean, norm example$$
$index operation, norm_sq$$

$head Theory$$
This example demonstrates using $cref atomic_base$$
to define the operation
$latex f : \B{R}^n \rightarrow \B{R}^m$$ where
$latex n = 2$$, $latex m = 1$$, where
$latex \[
	f(x) =  \sqrt{ x_0^2 + x_1^2 } 
\] $$

$nospell

$head Start Class Definition$$
$codep */
# include <cppad/cppad.hpp>
namespace {           // isolate items below to this file
using CppAD::vector;  // abbreviate as vector
//
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
class atomic_norm_sq : public CppAD::atomic_base<double> {
/* $$
$head Constructor $$
$codep */
	public:
	// constructor (could use const char* for name)
	atomic_norm_sq(const std::string& name) : 
	CppAD::atomic_base<double>(name)
	{ }
	private:
/* $$
$head forward$$
$codep */
	// forward mode routine called by CppAD
	virtual bool forward(
		size_t                    q ,
		size_t                    p ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<double>&    tx ,
		      vector<double>&    ty
	)
	{	size_t n = tx.size() / (p+1);
		size_t m = ty.size() / (p+1);
		assert( n == 2 );
		assert( m == 1 );
		assert( q <= p );

		// return flag
		bool ok = p <= 1;

		// check for defining variable information
		// This case must always be implemented
		if( vx.size() > 0 )
			vy[0] = vx[0] | vx[1];

		// Order zero forward mode.
		// This case must always be implemented
		// y^0 = f( x^0 ) 
		double x_00 = tx[ 0*(p+1) + 0];
		double x_10 = tx[ 1*(p+1) + 0];
		double f = x_00 * x_00 + x_10 * x_10;
		if( q <= 0 )
			ty[0] = f;
		if( p <= 0 )
			return ok;
		assert( vx.size() == 0 );

		// Order one forward mode.
		// This case needed if first order forward mode is used.
		// y^1 = f'( x^0 ) x^1
		double x_01 = tx[ 0*(p+1) + 1];
		double x_11 = tx[ 1*(p+1) + 1];
		double fp_0 = 2.0 * x_00;
		double fp_1 = 2.0 * x_10;
		if( q <= 1 )
			ty[1] = fp_0 * x_01 + fp_1 * x_11; 
		if( p <= 1 )
			return ok;

		// Assume we are not using forward mode with order > 2
		assert( ! ok );
		return ok;
	}
/* $$
$head reverse$$
$codep */
	// reverse mode routine called by CppAD
	virtual bool reverse(
		size_t                    p ,
		const vector<double>&    tx ,
		const vector<double>&    ty ,
		      vector<double>&    px ,
		const vector<double>&    py
	)
	{	size_t n = tx.size() / (p+1);
		size_t m = ty.size() / (p+1);	
		assert( px.size() == n * (p+1) );
		assert( py.size() == m * (p+1) );
		assert( n == 2 );
		assert( m == 1 );
		bool ok = p <= 1;	

		double fp_0, fp_1;
		switch(p)
		{	case 0:
			// This case needed if first order reverse mode is used
			// reverse: F^0 ( tx ) = y^0 = f( x^0 )
			fp_0  =  2.0 * tx[0];
			fp_1  =  2.0 * tx[1];
			px[0] = py[0] * fp_0;;
			px[1] = py[0] * fp_1;;
			assert(ok);
			break;

			default:
			assert(!ok);
		}
		return ok;
	}
/* $$
$head for_sparse_jac$$
$codep */
	// forward Jacobian bool sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		size_t                                q ,
		const vector<bool>&                   r ,
		      vector<bool>&                   s )
	{	// This function needed if using f.ForSparseJac 
		// with afun.option( CppAD::atomic_base<double>::bool_sparsity_enum )
		size_t n = r.size() / q;
		size_t m = s.size() / q;
		assert( n == 2 );
		assert( m == 1 );

		// sparsity for S(x) = f'(x) * R 
		for(size_t j = 0; j < q; j++)
		{	s[j] = false;
			for(size_t i = 0; i < n; i++)
				s[j] |= r[i * q + j];
		}
		return true; 
	}
	// forward Jacobian set sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		      vector< std::set<size_t> >&     s )
	{	// This function needed if using f.ForSparseJac 
		// with afun.option( CppAD::atomic_base<double>::set_sparsity_enum )
		size_t n = r.size();
		size_t m = s.size();
		assert( n == 2 );
		assert( m == 1 );

		// sparsity for S(x) = f'(x) * R 
		my_union(s[0], r[0], r[1]);

		return true; 
	}
/* $$
$head rev_sparse_jac$$
$codep */
	// reverse Jacobian bool sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		size_t                                q  ,
		const vector<bool>&                   rt ,
		      vector<bool>&                   st )
	{	// This function needed if using RevSparseJac or optimize
		// with afun.option( CppAD::atomic_base<double>::bool_sparsity_enum )
		size_t n = st.size() / q;
		size_t m = rt.size() / q;
		assert( n == 2 );
		assert( m == 1 );

		// sparsity for S(x)^T = f'(x)^T * R^T 
		for(size_t j = 0; j < q; j++)
			for(size_t i = 0; i < n; i++)
				st[i * q + j] = rt[j];

		return true; 
	}
	// reverse Jacobian set sparsity routine called by CppAD
	virtual bool rev_sparse_jac(
		size_t                                q  ,
		const vector< std::set<size_t> >&     rt ,
		      vector< std::set<size_t> >&     st )
	{	// This function needed if using RevSparseJac or optimize
		// with afun.option( CppAD::atomic_base<double>::set_sparsity_enum )
		size_t n = st.size();
		size_t m = rt.size();
		assert( n == 2 );
		assert( m == 1 );

		// sparsity for S(x)^T = f'(x)^T * R^T 
		st[0] = rt[0];
		st[1] = rt[0];

		return true; 
	}
/* $$
$head rev_sparse_hes$$
$codep */
	// reverse Hessian bool sparsity routine called by CppAD
	virtual bool rev_sparse_hes(
		const vector<bool>&                   vx,
		const vector<bool>&                   s ,
		      vector<bool>&                   t ,
		size_t                                q ,
		const vector<bool>&                   r ,
		const vector<bool>&                   u ,
		      vector<bool>&                   v )
	{	// This function needed if using RevSparseHes
		// with afun.option( CppAD::atomic_base<double>::bool_sparsity_enum )
		size_t m = s.size();
		size_t n = t.size();
		assert( r.size() == n * q );
		assert( u.size() == m * q );
		assert( v.size() == n * q );
		assert( n == 2 );
		assert( m == 1 );

		// There are no cross term second derivatives for this case,
		// so it is not necessary to vx.

		// sparsity for T(x) = S(x) * f'(x) 
		t[0] = s[0];
		t[1] = s[0];

		// V(x) = f'(x)^T * g''(y) * f'(x) * R  +  g'(y) * f''(x) * R 
		// U(x) = g''(y) * f'(x) * R
		// S(x) = g'(y)
		
		// back propagate the sparsity for U
		size_t j;
		for(j = 0; j < q; j++)
			for(size_t i = 0; i < n; i++)
				v[ i * q + j] = u[j];

		// include forward Jacobian sparsity in Hessian sparsity
		// sparsity for g'(y) * f''(x) * R  (Note f''(x) has same sparsity
		// as the identity matrix)
		if( s[0] )
		{	for(j = 0; j < q; j++)
				for(size_t i = 0; i < n; i++)
					v[ i * q + j] |= r[ i * q + j];
		}

		return true;
	}
	// reverse Hessian set sparsity routine called by CppAD
	virtual bool rev_sparse_hes(
		const vector<bool>&                   vx,
		const vector<bool>&                   s ,
		      vector<bool>&                   t ,
		size_t                                q ,
		const vector< std::set<size_t> >&     r ,
		const vector< std::set<size_t> >&     u ,
		      vector< std::set<size_t> >&     v )
	{	// This function needed if using RevSparseHes
		// with afun.option( CppAD::atomic_base<double>::set_sparsity_enum )
		size_t n = vx.size();
		size_t m = s.size();
		assert( t.size() == n );
		assert( r.size() == n );
		assert( u.size() == m );
		assert( v.size() == n );
		assert( n == 2 );
		assert( m == 1 );

		// There are no cross term second derivatives for this case,
		// so it is not necessary to vx.

		// sparsity for T(x) = S(x) * f'(x) 
		t[0] = s[0];
		t[1] = s[0];
	
		// V(x) = f'(x)^T * g''(y) * f'(x) * R  +  g'(y) * f''(x) * R 
		// U(x) = g''(y) * f'(x) * R
		// S(x) = g'(y)
		
		// back propagate the sparsity for U, note f'(x) may be non-zero
		v[0] = u[0];
		v[1] = u[0];

		// include forward Jacobian sparsity in Hessian sparsity
		// (note sparsty for f''(x) * R same as for R)
		if( s[0] )
		{	for(size_t i = 0; i < n; i++)
				my_union(v[i], v[i], r[i] );
		}

		return true;
	}
/* $$
$head End Class Definition$$
$codep */
}; // End of atomic_norm_sq class
}  // End empty namespace

/* $$
$head Use Atomic Function$$
$codep */
bool norm_sq(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();
/* $$
$subhead Constructor$$
$codep */
	// --------------------------------------------------------------------
	// Create the atomic reciprocal object
	atomic_norm_sq afun("atomic_norm_sq");
/* $$
$subhead Recording$$
$codep */
	// Create the function f(x)
	//
	// domain space vector
	size_t  n  = 2;
	double  x0 = 0.25;
	double  x1 = 0.75;
	vector< AD<double> > ax(n);
	ax[0]      = x0;
	ax[1]      = x1;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector 
	size_t m = 1;
	vector< AD<double> > ay(m);

	// call user function and store norm_sq(x) in au[0] 
	afun(ax, ay);        // y_0 = x_0 * x_0 + x_1 * x_1

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f;
	f.Dependent (ax, ay); 
/* $$
$subhead forward$$
$codep */
	// check function value 
	double check = x0 * x0 + x1 * x1;
	ok &= NearEqual( Value(ay[0]) , check,  eps, eps);

	// check zero order forward mode
	size_t p;
	vector<double> x_p(n), y_p(m);
	p      = 0;
	x_p[0] = x0;
	x_p[1] = x1;
	y_p    = f.Forward(p, x_p);
	ok &= NearEqual(y_p[0] , check,  eps, eps);

	// check first order forward mode
	p      = 1;
	x_p[0] = 0.3;
	x_p[1] = 0.7;
	y_p    = f.Forward(p, x_p);
	check  = 2.0 * x0 * x_p[0] + 2.0 * x1 * x_p[1];
	ok &= NearEqual(y_p[0] , check,  eps, eps);

/* $$
$subhead reverse$$
$codep */
	// first order reverse mode 
	p     = 1;
	vector<double> w(m), dw(n * p);
	w[0]  = 1.;
	dw    = f.Reverse(p, w);
	check = 2.0 * x0;
	ok &= NearEqual(dw[0] , check,  eps, eps);
	check = 2.0 * x1;
	ok &= NearEqual(dw[1] , check,  eps, eps);
/* $$
$subhead for_sparse_jac$$
$codep */
	// forward mode sparstiy pattern
	size_t q = n;
	CppAD::vectorBool r1(n * q), s1(m * q);
	r1[0] = true;  r1[1] = false; // sparsity pattern identity
	r1[2] = false; r1[3] = true;
	//
	afun.option( CppAD::atomic_base<double>::bool_sparsity_enum );
	s1    = f.ForSparseJac(q, r1);
	ok  &= s1[0] == true;  // f[0] depends on x[0]  
	ok  &= s1[1] == true;  // f[0] depends on x[1]  
	//
	afun.option( CppAD::atomic_base<double>::set_sparsity_enum );
	s1    = f.ForSparseJac(q, r1);
	ok  &= s1[0] == true;  // f[0] depends on x[0]  
	ok  &= s1[1] == true;  // f[0] depends on x[1]  
/* $$
$subhead rev_sparse_jac$$
$codep */
	// reverse mode sparstiy pattern
	p = m;
	CppAD::vectorBool s2(p * m), r2(p * n);
	s2[0] = true;          // compute sparsity pattern for f[0]
	//
	afun.option( CppAD::atomic_base<double>::bool_sparsity_enum );
	r2    = f.RevSparseJac(p, s2);
	ok  &= r2[0] == true;  // f[0] depends on x[0]  
	ok  &= r2[1] == true;  // f[0] depends on x[1]  
	//
	afun.option( CppAD::atomic_base<double>::set_sparsity_enum );
	r2    = f.RevSparseJac(p, s2);
	ok  &= r2[0] == true;  // f[0] depends on x[0]  
	ok  &= r2[1] == true;  // f[0] depends on x[1]  
/* $$
$subhead rev_sparse_hes$$
$codep */
	// Hessian sparsity (using previous ForSparseJac call) 
	CppAD::vectorBool s3(m), h(q * n);
	s3[0] = true;        // compute sparsity pattern for f[0]
	//
	afun.option( CppAD::atomic_base<double>::bool_sparsity_enum );
	h     = f.RevSparseHes(q, s3);
	ok  &= h[0] == true;  // partial of f[0] w.r.t. x[0],x[0] is non-zero
	ok  &= h[1] == false; // partial of f[0] w.r.t. x[0],x[1] is zero
	ok  &= h[2] == false; // partial of f[0] w.r.t. x[1],x[0] is zero
	ok  &= h[3] == true;  // partial of f[0] w.r.t. x[1],x[1] is non-zero
	//
	afun.option( CppAD::atomic_base<double>::set_sparsity_enum );
	h     = f.RevSparseHes(q, s3);
	ok  &= h[0] == true;  // partial of f[0] w.r.t. x[0],x[0] is non-zero
	ok  &= h[1] == false; // partial of f[0] w.r.t. x[0],x[1] is zero
	ok  &= h[2] == false; // partial of f[0] w.r.t. x[1],x[0] is zero
	ok  &= h[3] == true;  // partial of f[0] w.r.t. x[1],x[1] is non-zero

	return ok;
}
/* $$
$$ $comment end nospell$$
$end
*/
