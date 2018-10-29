/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>
# include <omp.h>

extern bool implicit_ctor(void);
extern bool prefer_reverse(void);

namespace {

	typedef CPPAD_TESTVECTOR(double)               d_vector;
	typedef CPPAD_TESTVECTOR( CppAD::AD<double> ) ad_vector;


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

}

// multi_thread_checkpoint
bool multi_checkpoint(void)
{	bool ok = true;

	// OpenMP setup
	size_t num_threads = 4;      // number of threads
	omp_set_dynamic(0);          // turn off dynamic thread adjustment
	omp_set_num_threads( int(num_threads) );  // set number of OMP threads

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
	for(size_t thread = 0; thread < num_threads; thread++)
		y[thread] = 0.0;

	# pragma omp parallel for
	for(int thread = 0; thread < int(num_threads); thread++)
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
		y[ size_t(thread) ] = v[0];
	}

	// check the results
	for(size_t thread = 0; thread < num_threads; thread++)
	{	double check = double( n_sum_ * (thread + 1) );
		ok          &= check == y[thread];
	}
	return ok;
}
