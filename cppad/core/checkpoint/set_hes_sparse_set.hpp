# ifndef CPPAD_CORE_CHECKPOINT_SET_HES_SPARSE_SET_HPP
# define CPPAD_CORE_CHECKPOINT_SET_HES_SPARSE_SET_HPP

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
void checkpoint<Base>::set_hes_sparse_set(void)
{	CPPAD_ASSERT_UNKNOWN( hes_sparse_set_.n_set() == 0 );
	size_t n = f_.Domain();
	size_t m = f_.Range();
	//
	// set version of sparsity for vector of all ones
	vector<bool> all_one(m);
	for(size_t i = 0; i < m; i++)
		all_one[i] = true;

	// set version of sparsity for n by n idendity matrix
	local::sparse_list identity;
	identity.resize(n, n);
	for(size_t j = 0; j < n; j++)
	{	// use add_element because only adding one element per set
		identity.add_element(j, j);
	}

	// compute sparsity pattern for H(x) = sum_i f_i(x)^{(2)}
	bool transpose  = false;
	bool dependency = false;
	f_.ForSparseJacCheckpoint(
		n, identity, transpose, dependency, jac_sparse_set_
	);
	f_.RevSparseHesCheckpoint(
		n, all_one, transpose, hes_sparse_set_
	);
	CPPAD_ASSERT_UNKNOWN( hes_sparse_set_.n_set() == n );
	CPPAD_ASSERT_UNKNOWN( hes_sparse_set_.end()   == n );
	//
	// drop the forward sparsity results from f_
	f_.size_forward_set(0);
}
# else // CPPAD_MULTI_THREAD_TMB
# define THREAD omp_get_thread_num()
template <class Base>
void checkpoint<Base>::set_hes_sparse_set(void)
{	CPPAD_ASSERT_UNKNOWN( hes_sparse_set_[THREAD].n_set() == 0 );
	size_t n = f_[THREAD].Domain();
	size_t m = f_[THREAD].Range();
	//
	// set version of sparsity for vector of all ones
	vector<bool> all_one(m);
	for(size_t i = 0; i < m; i++)
		all_one[i] = true;

	// set version of sparsity for n by n idendity matrix
	local::sparse_list identity;
	identity.resize(n, n);
	for(size_t j = 0; j < n; j++)
	{	// use add_element because only adding one element per set
		identity.add_element(j, j);
	}

	// compute sparsity pattern for H(x) = sum_i f_i(x)^{(2)}
	bool transpose  = false;
	bool dependency = false;
	f_[THREAD].ForSparseJacCheckpoint(
		n, identity, transpose, dependency, jac_sparse_set_[THREAD]
	);
	f_[THREAD].RevSparseHesCheckpoint(
		n, all_one, transpose, hes_sparse_set_[THREAD]
	);
	CPPAD_ASSERT_UNKNOWN( hes_sparse_set_[THREAD].n_set() == n );
	CPPAD_ASSERT_UNKNOWN( hes_sparse_set_[THREAD].end()   == n );
	//
	// drop the forward sparsity results from f_
	f_[THREAD].size_forward_set(0);
}
# undef THREAD
# endif //  CPPAD_MULTI_THREAD_TMB

} // END_CPPAD_NAMESPACE
# endif
