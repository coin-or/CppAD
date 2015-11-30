// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

# define NUMBER_THREADS     4
# define NUMBER_CALLS       40

// y[0] = x[0] + x[1]
// y[1] = x[1] + x[2]
namespace {
# ifdef CPPAD_TEST_OPENMP
	// ------------------------------------------------------------------
	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool>( omp_in_parallel() ); }
	// ------------------------------------------------------------------
	// used to inform CppAD of the current thread number
	size_t thread_number(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); }
# endif

	// y[0] = x[0] + x[1]
	bool user_atomic_forward(
		size_t                         id ,
		size_t                          k ,
		size_t                          n ,
		size_t                          m ,
		const CppAD::vector<bool>&     vx ,
		CppAD::vector<bool>&           vy ,
		const CppAD::vector<double>&   tx ,
		CppAD::vector<double>&         ty )
	{	assert(n >= 3 && m == 2);
		if( k > 0 )
			return false;

		ty[0] = tx[0] + tx[1];
		ty[1] = tx[1] + tx[2];

		if( vy.size() > 0 )
		{	vy[0] = (vx[0] | vx[1]);
			vy[1] = (vx[1] | vx[2]);
		}
		return true;
	}

	bool user_atomic_reverse(
		size_t                         id ,
		size_t                          k ,
		size_t                          n ,
		size_t                          m ,
		const CppAD::vector<double>&   tx ,
		const CppAD::vector<double>&   ty ,
		CppAD::vector<double>&         px ,
		const CppAD::vector<double>&   py )
	{	return false; }

	bool user_atomic_for_jac_sparse(
		size_t                                  id ,
		size_t                                   n ,
		size_t                                   m ,
		size_t                                   q ,
		const CppAD::vector< std::set<size_t> >& r ,
		CppAD::vector< std::set<size_t>  >&      s )
	{	return false; }

	bool user_atomic_rev_jac_sparse(
		size_t                                  id ,
		size_t                                   n ,
		size_t                                   m ,
		size_t                                   q ,
		CppAD::vector< std::set<size_t> >&       r ,
		const CppAD::vector< std::set<size_t> >& s )
	{	return false; }

	bool user_atomic_rev_hes_sparse(
		size_t                                  id ,
		size_t                                   n ,
		size_t                                   m ,
		size_t                                   q ,
		const CppAD::vector< std::set<size_t> >& r ,
		const CppAD::vector<bool>&               s ,
		CppAD::vector<bool>&                     t ,
		const CppAD::vector< std::set<size_t> >& u ,
		CppAD::vector< std::set<size_t> >&       v )
	{	return false; }

	CPPAD_USER_ATOMIC(
		my_user_atomic             ,
		CppAD::vector              ,
		double                     ,
		user_atomic_forward        ,
		user_atomic_reverse        ,
		user_atomic_for_jac_sparse ,
		user_atomic_rev_jac_sparse ,
		user_atomic_rev_hes_sparse
	)

	bool user_atomic(int call_index)
	{	bool ok = true;
		using CppAD::AD;
		using CppAD::vector;

		size_t j;
		size_t m = 2;
		size_t n = size_t(call_index + 3);
		vector< AD<double> > ax(n), ay(m);
		for(j = 0; j < n; j++)
			ax[j] = AD<double>(j + 1);
		CppAD::Independent(ax);

		// use user atomic function
		size_t id = 0;
		my_user_atomic(id, ax, ay);

		// create function
		CppAD::ADFun<double> g(ax, ay);

		// now use the tape with the user atomic function
		vector<double> x(n), y(m);
		for(j = 0; j < n; j++)
			x[j] = (j + 1) * (j + 1);
		y = g.Forward(0, x);
		// y[0] = x[0] + x[1]
		ok &= (y[0] == x[0] + x[1]);
		// y[1] = x[1] + x[2]
		ok &= (y[0] == x[0] + x[1]);

		return ok;
	}
}


# ifdef CPPAD_TEST_OPENMP

# include <omp.h>

bool user_atomic_openmp(void)
{	using CppAD::thread_alloc;

	bool ok = true;
	bool ok_all[NUMBER_CALLS];
	size_t num_threads = NUMBER_THREADS;
	int int_num_threads = int(num_threads);

	// make first call in sequential mode
	int call_index = 0;
	ok &= user_atomic(call_index);

	// turn off dynamic thread adjustment
	omp_set_num_threads( int_num_threads );

	// setup for using CppAD::AD<double> in parallel
	thread_alloc::parallel_setup(
		num_threads, in_parallel, thread_number
	);
	thread_alloc::hold_memory(true);
	CppAD::parallel_ad<double>();

# pragma omp parallel for
	for(call_index = 0; call_index < NUMBER_CALLS; call_index++)
		ok_all[call_index] = user_atomic(call_index);
// end omp parallel for

	// tear down parallel mode
	omp_set_num_threads(1);
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);
	thread_alloc::hold_memory(false);
	CppAD::parallel_ad<double>();

	// combine all the ok flags into one
	for(call_index = 0; call_index < int_num_threads; call_index++)
		ok &= ok_all[call_index];

	// --------------------------------------------------------------------
	// Free all temporary work space associated with user_atomic objects.
	// (If there are future calls to user atomic functions, they will
	// create new temporary work space.)
	CppAD::user_atomic<double>::clear();

	return ok;
}
# endif
