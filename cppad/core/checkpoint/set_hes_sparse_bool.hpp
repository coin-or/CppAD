# ifndef CPPAD_CORE_CHECKPOINT_SET_HES_SPARSE_BOOL_HPP
# define CPPAD_CORE_CHECKPOINT_SET_HES_SPARSE_BOOL_HPP

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
void checkpoint<Base>::set_hes_sparse_bool(void)
{	// make sure member_ is allocated for this thread
	size_t thread = thread_alloc::thread_num();
	allocate_member(thread);
	//
	CPPAD_ASSERT_UNKNOWN( member_[thread]->hes_sparse_bool_.size() == 0 );
	size_t n = member_[thread]->f_.Domain();
	size_t m = member_[thread]->f_.Range();
	//
	// set version of sparsity for vector of all ones
	vectorBool all_one(m);
	for(size_t i = 0; i < m; i++)
		all_one[i] = true;

	// set version of sparsity for n by n idendity matrix
	vectorBool identity(n * n);
	for(size_t j = 0; j < n; j++)
	{	for(size_t i = 0; i < n; i++)
			identity[ i * n + j ] = (i == j);
	}

	// compute sparsity pattern for H(x) = sum_i f_i(x)^{(2)}
	bool transpose  = false;
	bool dependency = false;
	member_[thread]->f_.ForSparseJac(n, identity, transpose, dependency);
	member_[thread]->hes_sparse_bool_ = member_[thread]->f_.RevSparseHes(n, all_one, transpose);
	CPPAD_ASSERT_UNKNOWN( member_[thread]->hes_sparse_bool_.size() == n * n );
	//
	// drop the forward sparsity results from f_
	member_[thread]->f_.size_forward_bool(0);
	CPPAD_ASSERT_UNKNOWN( member_[thread]->f_.size_forward_bool() == 0 );
	CPPAD_ASSERT_UNKNOWN( member_[thread]->f_.size_forward_set() == 0 );
}
# else // CPPAD_MULTI_THREAD_TMB
# define THREAD omp_get_thread_num()
template <class Base>
void checkpoint<Base>::set_hes_sparse_bool(void)
{	CPPAD_ASSERT_UNKNOWN( hes_sparse_bool_[THREAD].size() == 0 );
	size_t n = f_[THREAD].Domain();
	size_t m = f_[THREAD].Range();
	//
	// set version of sparsity for vector of all ones
	vectorBool all_one(m);
	for(size_t i = 0; i < m; i++)
		all_one[i] = true;

	// set version of sparsity for n by n idendity matrix
	vectorBool identity(n * n);
	for(size_t j = 0; j < n; j++)
	{	for(size_t i = 0; i < n; i++)
			identity[ i * n + j ] = (i == j);
	}

	// compute sparsity pattern for H(x) = sum_i f_i(x)^{(2)}
	bool transpose  = false;
	bool dependency = false;
	f_[THREAD].ForSparseJac(n, identity, transpose, dependency);
	hes_sparse_bool_[THREAD] = f_[THREAD].RevSparseHes(n, all_one, transpose);
	CPPAD_ASSERT_UNKNOWN( hes_sparse_bool_[THREAD].size() == n * n );
	//
	// drop the forward sparsity results from f_
	f_[THREAD].size_forward_bool(0);
	CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_forward_bool() == 0 );
	CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_forward_set() == 0 );
}
# undef THREAD
# endif //  CPPAD_MULTI_THREAD_TMB

} // END_CPPAD_NAMESPACE
# endif
