# ifndef CPPAD_CORE_CHECKPOINT_CTOR_HPP
# define CPPAD_CORE_CHECKPOINT_CTOR_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

# if ! CPPAD_MULTI_THREAD_TMB
template <class Base>
template <class Algo, class ADVector>
checkpoint<Base>::checkpoint(
	const char*                    name            ,
	Algo&                          algo            ,
	const ADVector&                ax              ,
	ADVector&                      ay              ,
	option_enum                    sparsity        ,
	bool                           optimize
) : atomic_base<Base>(name, sparsity)
{
# ifndef NDEBUG
	if( thread_alloc::in_parallel() )
	{	std::string msg = name;
		msg += ": checkpoint constructor called in parallel mode.";
		CPPAD_ASSERT_KNOWN(false, msg.c_str() );
	}
# endif
	for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
		member_[thread] = CPPAD_NULL;
	//
	// make sure member_ is allocated for this (the master) thread
	// (only thread is possible when not in parallel mode)
	size_t master = thread_alloc::thread_num();
	CPPAD_ASSERT_UNKNOWN( master == 0 );
	allocate_member(master);
	//
	CheckSimpleVector< CppAD::AD<Base> , ADVector>();
	//
	// make a copy of ax because Independent modifies AD information
	ADVector x_tmp(ax);
	// delcare x_tmp as the independent variables
	Independent(x_tmp);
	// record mapping from x_tmp to ay
	algo(x_tmp, ay);
	// create function f_ : x -> y
	member_[master]->f_.Dependent(ay);
	if( optimize )
	{	// suppress checking for nan in f_ results
		// (see optimize documentation for atomic functions)
		member_[master]->f_.check_for_nan(false);
		//
		// now optimize
		member_[master]->f_.optimize();
	}
	// now disable checking of comparison operations
	// 2DO: add a debugging mode that checks for changes and aborts
	member_[master]->f_.compare_change_count(0);
}
# else // CPPAD_MULTI_THREAD_TMB
# define THREAD omp_get_thread_num()
template <class Base>
template <class Algo, class ADVector>
checkpoint<Base>::checkpoint(
	const char*                    name            ,
	Algo&                          algo            ,
	const ADVector&                ax              ,
	ADVector&                      ay              ,
	option_enum                    sparsity        ,
	bool                           optimize
) :
atomic_base<Base>(name  , sparsity)        ,
f_( size_t( omp_get_max_threads() ) )                ,
jac_sparse_set_( size_t( omp_get_max_threads() ) )   ,
jac_sparse_bool_( size_t( omp_get_max_threads() ) )  ,
hes_sparse_set_( size_t( omp_get_max_threads() ) )   ,
hes_sparse_bool_( size_t( omp_get_max_threads() ) )
{
	CheckSimpleVector< CppAD::AD<Base> , ADVector>();

	// make a copy of ax because Independent modifies AD information
	ADVector x_tmp(ax);
	// delcare x_tmp as the independent variables
	Independent(x_tmp);
	// record mapping from x_tmp to ay
	algo(x_tmp, ay);
	// create function f_ : x -> y
	f_[0].Dependent(ay);
	if( optimize )
	{	// suppress checking for nan in f_ results
		// (see optimize documentation for atomic functions)
		f_[0].check_for_nan(false);
		//
		// now optimize
		f_[0].optimize();
	}
	// now disable checking of comparison operations
	// 2DO: add a debugging mode that checks for changes and aborts
	f_[0].compare_change_count(0);
	//
	// copy for use during multi-threading
	for(int i = 1; i < omp_get_max_threads(); ++i)
		f_[i] = f_[0];
}
# undef THREAD
# endif //  CPPAD_MULTI_THREAD_TMB

} // END_CPPAD_NAMESPACE
# endif
