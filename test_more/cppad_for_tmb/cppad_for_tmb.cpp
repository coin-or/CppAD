/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>
# include <omp.h>

// ===========================================================================
namespace { // BEGIN_EMPTY_NAMESPACE

using CppAD::AD;
using CppAD::vector;
using CppAD::NearEqual;

typedef CPPAD_TESTVECTOR(double)        d_vector;
typedef CPPAD_TESTVECTOR( AD<double> ) ad_vector;


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

// ----------------------------------------------------------------------------
// Special version of atomic_base so one functions works for multiple threads.
//
// algorithm that we are checkpoingint
const size_t n_sum_ = 1000;
void square_algo(const ad_vector& ax, ad_vector& ay)
{	ay[0] = 0.0;
	for(size_t i = 0; i < n_sum_; ++i)
		ay[0] += ax[0];
	return;
}
// inform CppAD if we are in parallel mode
bool in_parallel(void)
{	return omp_in_parallel() != 0; }
//
// inform CppAD of the current thread number
size_t thread_num(void)
{	return static_cast<size_t>( omp_get_thread_num() ); }
//
// multi_thread_checkpoint
bool multi_thread_checkpoint(void)
{	bool ok = true;

	// OpenMP setup
	int num_threads = 4;               // number of threads
	omp_set_dynamic(0);                // turn off dynamic thread adjustment
	omp_set_num_threads(num_threads);  // set number of OMP threads

	// check that multi-threading is possible on this machine
	if( omp_get_max_threads() < 2 )
	{	std::cout << "This machine does not support multi-threading: ";
	}

	// create checkpoint version of algorithm
	size_t n(1), m(1);
	ad_vector ax(n), ay(m);
	ax[0] = 2.0;
	CppAD::atomic_base<double>::option_enum sparsity =
		CppAD::atomic_base<double>::set_sparsity_enum;
	bool optimize = false;
	CppAD::checkpoint<double> atom_fun(
		"square", square_algo, ax, ay, sparsity, optimize
	);

	// setup for using CppAD in paralle mode
	CppAD::thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);
	CppAD::thread_alloc::hold_memory(true);
	CppAD::parallel_ad<double>();

	// place to hold result for each thread
	d_vector y(num_threads);
	for(int thread = 0; thread < num_threads; thread++)
		y[thread] = 0.0;

	# pragma omp parallel for
	for(int thread = 0; thread < num_threads; thread++)
	{	ad_vector au(n), av(m);
		au[0] = 1.0;
		CppAD::Independent(au);
		atom_fun(au, av);
		CppAD::ADFun<double> f(au, av);
		//
		d_vector x(n), v(m);
		x[0]      = double( thread + 1 );
		v         = f.Forward(0, x);
		//
		// this assigment has false sharing; i.e., will case cache resets
		y[thread] = v[0];
	}

	// check the results
	for(int thread = 0; thread < num_threads; thread++)
	{	double check = double( n_sum_ * (thread + 1) );
		ok          &= check == y[thread];
	}
	return ok;
}

} // END_EMPTY_NAMESPACE
// ===========================================================================

int main(void)
{	std::string group = "test_more/cppad_for_tmb";
	size_t      width = 30;
	CppAD::test_boolofvoid Run(group, width);

	Run( implicit_constructor,     "implicit_constructor"    );
	Run( prefer_reverse,           "prefer_reverse"          );
	Run( prefer_reverse,           "prefer_reverse"          );
	Run( multi_thread_checkpoint,  "multi_thread_checkpoint" );

	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok  = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
