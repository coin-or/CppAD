# ifndef CPPAD_CORE_CHECKPOINT_SET_JAC_SPARSE_BOOL_HPP
# define CPPAD_CORE_CHECKPOINT_SET_JAC_SPARSE_BOOL_HPP

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
void checkpoint<Base>::set_jac_sparse_bool(void)
{	CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_.size() == 0 );
	bool transpose  = false;
	bool dependency = true;
	size_t n = f_.Domain();
	size_t m = f_.Range();
	// Use the choice for forward / reverse that results in smaller
	// size for the sparsity pattern of all variables in the tape.
	if( n <= m )
	{	vectorBool identity(n * n);
		for(size_t j = 0; j < n; j++)
		{	for(size_t i = 0; i < n; i++)
				identity[ i * n + j ] = (i == j);
		}
		jac_sparse_bool_ = f_.ForSparseJac(
			n, identity, transpose, dependency
		);
		f_.size_forward_bool(0);
	}
	else
	{	vectorBool identity(m * m);
		for(size_t j = 0; j < m; j++)
		{	for(size_t i = 0; i < m; i++)
				identity[ i * m + j ] = (i == j);
		}
		jac_sparse_bool_ = f_.RevSparseJac(
			m, identity, transpose, dependency
		);
	}
	CPPAD_ASSERT_UNKNOWN( f_.size_forward_bool() == 0 );
	CPPAD_ASSERT_UNKNOWN( f_.size_forward_set() == 0 );
}
# else // CPPAD_MULTI_THREAD_TMB
# define THREAD omp_get_thread_num()
template <class Base>
void checkpoint<Base>::set_jac_sparse_bool(void)
{	CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_[THREAD].size() == 0 );
	bool transpose  = false;
	bool dependency = true;
	size_t n = f_[THREAD].Domain();
	size_t m = f_[THREAD].Range();
	// Use the choice for forward / reverse that results in smaller
	// size for the sparsity pattern of all variables in the tape.
	if( n <= m )
	{	vectorBool identity(n * n);
		for(size_t j = 0; j < n; j++)
		{	for(size_t i = 0; i < n; i++)
				identity[ i * n + j ] = (i == j);
		}
		jac_sparse_bool_[THREAD] = f_[THREAD].ForSparseJac(
			n, identity, transpose, dependency
		);
		f_[THREAD].size_forward_bool(0);
	}
	else
	{	vectorBool identity(m * m);
		for(size_t j = 0; j < m; j++)
		{	for(size_t i = 0; i < m; i++)
				identity[ i * m + j ] = (i == j);
		}
		jac_sparse_bool_[THREAD] = f_[THREAD].RevSparseJac(
			m, identity, transpose, dependency
		);
	}
	CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_forward_bool() == 0 );
	CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_forward_set() == 0 );
}
# undef THREAD
# endif //  CPPAD_MULTI_THREAD_TMB

} // END_CPPAD_NAMESPACE
# endif
