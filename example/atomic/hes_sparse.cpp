// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin atomic_hes_sparse.cpp$$

$section Atomic Operation Hessian Sparsity: Example and Test$$
$mindex sparsity$$

$head Purpose$$
This example demonstrates calculation of the Hessian sparsity pattern
for an atomic operation.

$nospell

$head Start  Class Definition$$
$codep */
# include <cppad/cppad.hpp>
namespace {          // isolate items below to this file
using CppAD::vector; // abbreviate as vector
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
class atomic_hes_sparse : public CppAD::atomic_base<double> {
/* $$
$head Constructor $$
$codep */
	public:
	// constructor (could use const char* for name)
	atomic_hes_sparse(const std::string& name) :
	CppAD::atomic_base<double>(name)
	{ }
	private:
/* $$
$head forward$$
$codep */
	// forward mode routine called by CppAD
	virtual bool forward(
		size_t                    p ,
		size_t                    q ,
		const vector<bool>&      vx ,
		      vector<bool>&      vy ,
		const vector<double>&    tx ,
		      vector<double>&    ty
	)
	{	size_t n = tx.size() / (q + 1);
		size_t m = ty.size() / (q + 1);
		assert( n == 2 );
		assert( m == 2 );

		// return flag
		bool ok = q == 0;
		if( ! ok )
			return ok;

		// check for defining variable information
		// This case must always be implemented
		if( vx.size() > 0 )
		{	vy[0] = vx[0];
			vy[1] = vx[0] || vy[0];
		}

		// Order zero forward mode.
		// This case must always be implemented
		// y^0 = f( x^0 ) = [ x0_0 * x0_0  , x0_0 * x0_1 ]^T
		assert( p <= 0 );
		if( p <= 0 )
		{	ty[0] = tx[0] * tx[0];
			ty[1] = tx[0] * tx[1];
		}
		return ok;
	}
/* $$
$head for_sparse_jac$$
$codep */
	// forward Jacobian set sparsity routine called by CppAD
	virtual bool for_sparse_jac(
		size_t                                p ,
		const vector< std::set<size_t> >&     r ,
		      vector< std::set<size_t> >&     s )
	{	// This function needed if using f.ForSparseJac
		// with afun.option( CppAD::atomic_base<double>::set_sparsity_enum )
		size_t n = r.size();
		size_t m = s.size();
		assert( n == 2 );
		assert( m == 2 );

		// sparsity for S(x) = f'(x) * R is union of rows of R
		s[0] = r[0];
		my_union(s[1], r[0], r[1]);

		return true;
	}
/* $$
$head rev_sparse_hes$$
$codep */
	// reverse Hessian set sparsity routine called by CppAD
	virtual bool rev_sparse_hes(
		const vector<bool>&                   vx,
		const vector<bool>&                   s ,
		      vector<bool>&                   t ,
		size_t                                p ,
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
		assert( m == 2 );

		// There are no cross term second derivatives for this case,
		// so it is not necessary to vx.

		// sparsity for T(x) = S(x) * f'(x)
		t[0] = s[0];
		t[1] = s[0] || s[1];

		// V(x) = f'(x)^T * g''(y) * f'(x) * R  +  g'(y) * f''(x) * R
		// U(x) = g''(y) * f'(x) * R
		// S(x) = g'(y)

		// back propagate the sparsity for U,
		// note both components of f'(x) may be non-zero;
		v[0] = u[0];
		my_union(v[1], u[0], u[1]);

		// include forward Jacobian sparsity in Hessian sparsity
		// f_0''(x) * R
		if( s[0] )
			my_union(v[0], v[0], r[0] );
		// f_1''(x) * R
		if( s[1] )
		{	my_union(v[1], v[1], r[0] );
			my_union(v[0], v[0], r[1] );
		}
		return true;
	}
}; // End of atomic_hes_sparse class

/* $$
$head Use Atomic Function$$
$codep */
bool use_atomic_hes_sparse(bool x_1_variable)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();
	//
	// Create the atomic hes_sparse object
	atomic_hes_sparse afun("atomic_hes_sparse");
	//
	// Create the function f(u)
	//
	// domain space vector
	size_t n  = 2;
	double x0_0 = 0.75, x0_1 = 2.00;
	vector< AD<double> > au(n);
	au[0] = x0_0;
	au[1] = x0_1;

	// declare independent variables and start tape recording
	CppAD::Independent(au);

	// range space vector
	size_t m = 2;
	vector< AD<double> > ay(m);

	// call user function
	vector< AD<double> > ax(n);
	ax[0] = au[0];
	if( x_1_variable )
		ax[1] = au[1];
	else
		ax[1] = x0_1;
	afun(ax, ay);        // y = [ x[0] * x[0] ,  x[0] * x[1] ] ^T

	// create f: u -> y and stop tape recording
	CppAD::ADFun<double> f;
	f.Dependent (au, ay);  // f(u) = y
	//
	// check function value
	double check_y0_0 = x0_0 * x0_0;
	double check_y0_1 = x0_0 * x0_1;
	ok &= NearEqual( Value(ay[0]) , check_y0_0,  eps, eps);
	ok &= NearEqual( Value(ay[1]) , check_y0_1,  eps, eps);

	// check zero order forward mode
	size_t q;
	vector<double> uq(n), yq(m);
	q     = 0;
	uq[0] = x0_0;
	uq[1] = x0_1;
	yq    = f.Forward(q, uq);
	ok &= NearEqual(yq[0] , check_y0_0,  eps, eps);
	ok &= NearEqual(yq[1] , check_y0_1,  eps, eps);

	// forward sparse jacobian
	afun.option( CppAD::atomic_base<double>::set_sparsity_enum );
	vector< std::set<size_t> > r(2), jac_s(2), check_s(2);
	r[0].insert(0);
	r[1].insert(1);
	jac_s = f.ForSparseJac(n, r);
	check_s[0].insert(0);
	ok &= jac_s[0] == check_s[0];
	check_s[1].insert(0);
	if( x_1_variable )
		check_s[1].insert(1);
	ok &= jac_s[1] == check_s[1];

	// reverse sparse Hessian
	vector< std::set<size_t> > h(2), check_h(2), hes_s(1);
	hes_s[0].insert(0);
	hes_s[0].insert(1);
	h = f.RevSparseHes(n, hes_s);
	check_h[0].insert(0);
	if( x_1_variable )
		check_h[0].insert(1);
	ok &= h[0] == check_h[0];
	if( x_1_variable )
		check_h[1].insert(0);
	ok &= h[1] == check_h[1];

	return ok;
}
}  // End empty namespace
/* $$
$head Test with x_1 Both a Variable and a Parameter$$
$codep */
bool hes_sparse(void)
{	bool ok = true;
	// test with x_1 a variable
	ok     &= use_atomic_hes_sparse(true);
	// test with x_1 a parameter
	ok     &= use_atomic_hes_sparse(false);
	return ok;
}
/* $$
$$ $comment end nospell$$
$end
*/
