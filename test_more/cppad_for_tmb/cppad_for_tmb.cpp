/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>

// ===========================================================================
namespace { // BEGIN_EMPTY_NAMESPACE

using CppAD::AD;
using CppAD::vector;
using CppAD::NearEqual;

// ---------------------------------------------------------------------------
// implicit constructor from double
bool implicit_constructor(void)
{	bool ok = true;
	//
	AD< AD<double> > x = 5.0;
	ok &= Value(x) == 5.0;
	//
	return ok;
}

// ----------------------------------------------------------------------------
// prefer reverse mode during computation of Jacobians

// example_tmb_atomic
class example_tmb_atomic : public CppAD::atomic_base<double> {
public:
	// constructor
	example_tmb_atomic(const std::string& name)
	: CppAD::atomic_base<double>(name)
	{ }
	// forward (only implement zero order)
	virtual bool forward(
		size_t                p  ,
		size_t                q  ,
		const vector<bool>&   vx ,
		vector<bool>&         vy ,
		const vector<double>& tx ,
		vector<double>&       ty )
	{
		// check for errors in usage
		bool ok = p == 0 && q == 0;
		ok     &= tx.size() == 1;
		ok     &= ty.size() == 1;
		ok     &= vx.size() <= 1;
		if( ! ok )
			return false;

		// variable information
		if( vx.size() > 0 )
			vy[0] = vx[0];

		// y = 1 / x
		ty[0] = 1.0 / tx[0];

		return ok;
	}
	// reverse (implement first order)
	virtual bool reverse(
		size_t                q  ,
		const vector<double>& tx ,
		const vector<double>& ty ,
		vector<double>&       px ,
		const vector<double>& py )
	{
		// check for errors in usage
		bool ok = q == 0;
		ok     &= tx.size() == 1;
		ok     &= ty.size() == 1;
		ok     &= px.size() == 1;
		ok     &= py.size() == 1;
		if( ! ok )
			return false;

		// y = 1 / x
		// dy/dx = - 1 / (x * x)
		double dy_dx = -1.0 / ( tx[0] * tx[0] );
		px[0]        = py[0] * dy_dx;

		return ok;
	}
};

bool prefer_reverse(void)
{	bool ok = true;
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

	// Create atomic functions
	example_tmb_atomic afun("reciprocal");

	// Declare independent variables
	size_t n = 1;
	CPPAD_TESTVECTOR( AD<double> ) ax(n);
	ax[0] = 5.0;
	CppAD::Independent(ax);

	// Compute dependent variables
	size_t m = 1;
	CPPAD_TESTVECTOR( AD<double> ) ay(m);
	afun(ax, ay);

	// Create f(x) = 1 / x
	CppAD::ADFun<double> f(ax, ay);

	// Use Jacobian to compute f'(x) = - 1 / (x * x).
	// This would fail with the normal CppAD distribution because it would use
	// first order forward mode for the  calculation.
	CPPAD_TESTVECTOR(double) x(n), dy_dx(m);
	x[0]   = 2.0;
	dy_dx  = f.Jacobian(x);

	// check the result
	double check = -1.0 / (x[0] * x[0]);
	ok &= NearEqual(dy_dx[0], check, eps99, eps99);

	return ok;
}


} // END_EMPTY_NAMESPACE
// ===========================================================================

int main(void)
{	std::string group = "test_more/cppad_for_tmb";
	size_t      width = 25;
	CppAD::test_boolofvoid Run(group, width);

	Run( implicit_constructor,     "implicit_constructor" );
	Run( prefer_reverse,           "prefer_reverse"       );

	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok  = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
