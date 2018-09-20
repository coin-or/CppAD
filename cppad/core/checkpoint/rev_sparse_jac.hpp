# ifndef CPPAD_CORE_CHECKPOINT_REV_SPARSE_JAC_HPP
# define CPPAD_CORE_CHECKPOINT_REV_SPARSE_JAC_HPP

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
template <class sparsity_type>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const sparsity_type&                    rt ,
	      sparsity_type&                    st ,
	const vector<Base>&                     x  )
{	// during user sparsity calculations
	size_t m = f_.Range();
	size_t n = f_.Domain();
	if( jac_sparse_bool_.size() == 0 )
		set_jac_sparse_bool();
	if( jac_sparse_set_.n_set() != 0 )
		jac_sparse_set_.resize(0, 0);
	CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_.size() == m * n );
	CPPAD_ASSERT_UNKNOWN( jac_sparse_set_.n_set() == 0 );
	CPPAD_ASSERT_UNKNOWN( rt.size() == m * q );
	CPPAD_ASSERT_UNKNOWN( st.size() == n * q );
	bool ok  = true;
	//
	// S = R * J where J is jacobian
	for(size_t i = 0; i < q; i++)
	{	for(size_t j = 0; j < n; j++)
		{	// initialize sparsity for S(i,j)
			bool s_ij = false;
			// S(i,j) = sum_k R(i,k) * J(k,j)
			for(size_t k = 0; k < m; k++)
			{	// sparsity for R(i, k)
				bool R_ik = rt[ k * q + i ];
				bool J_kj = jac_sparse_bool_[ k * n + j ];
				s_ij     |= (R_ik & J_kj);
			}
			// set sparsity for S^T
			st[ j * q + i ] = s_ij;
		}
	}
	return ok;
}
template <class Base>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const vectorBool&                       rt ,
	      vectorBool&                       st ,
	const vector<Base>&                     x  )
{	return rev_sparse_jac< vectorBool >(q, rt, st, x);
}
template <class Base>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const vector<bool>&                     rt ,
	      vector<bool>&                     st ,
	const vector<Base>&                     x  )
{	return rev_sparse_jac< vector<bool> >(q, rt, st, x);
}
template <class Base>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const vector< std::set<size_t> >&       rt ,
	      vector< std::set<size_t> >&       st ,
	const vector<Base>&                     x  )
{	// during user sparsity calculations
	size_t m = f_.Range();
	size_t n = f_.Domain();
	if( jac_sparse_bool_.size() != 0 )
		jac_sparse_bool_.clear();
	if( jac_sparse_set_.n_set() == 0 )
		set_jac_sparse_set();
	CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_.size() == 0 );
	CPPAD_ASSERT_UNKNOWN( jac_sparse_set_.n_set() == m );
	CPPAD_ASSERT_UNKNOWN( jac_sparse_set_.end()   == n );
	CPPAD_ASSERT_UNKNOWN( rt.size() == m );
	CPPAD_ASSERT_UNKNOWN( st.size() == n );
	//
	bool ok  = true;
	//
	for(size_t j = 0; j < n; j++)
		st[j].clear();
	//
	// sparsity for  s = r * jac_sparse_set_
	// s^T = jac_sparse_set_^T * r^T
	for(size_t i = 0; i < m; i++)
	{	// i is the row index in r^T
		std::set<size_t>::const_iterator itr_i;
		const std::set<size_t>& r_i( rt[i] );
		for(itr_i = r_i.begin(); itr_i != r_i.end(); itr_i++)
		{	// k is the column index in r^T
			size_t k = *itr_i;
			CPPAD_ASSERT_UNKNOWN( k < q );
			//
			// i is column index in jac_sparse_set^T
			local::sparse_list::const_iterator set_itr(
				jac_sparse_set_, i
			);
			size_t j = *set_itr;
			while( j < n )
			{	// j is row index in jac_sparse_set^T
				st[j].insert(k);
				j = *(++set_itr);
			}
		}
	}

	return ok;
}
# else // CPPAD_MULTI_THREAD_TMB
# define THREAD omp_get_thread_num()
template <class Base>
template <class sparsity_type>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const sparsity_type&                    rt ,
	      sparsity_type&                    st ,
	const vector<Base>&                     x  )
{	// during user sparsity calculations
	size_t m = f_[THREAD].Range();
	size_t n = f_[THREAD].Domain();
	if( jac_sparse_bool_[THREAD].size() == 0 )
		set_jac_sparse_bool();
	if( jac_sparse_set_[THREAD].n_set() != 0 )
		jac_sparse_set_[THREAD].resize(0, 0);
	CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_[THREAD].size() == m * n );
	CPPAD_ASSERT_UNKNOWN( jac_sparse_set_[THREAD].n_set() == 0 );
	CPPAD_ASSERT_UNKNOWN( rt.size() == m * q );
	CPPAD_ASSERT_UNKNOWN( st.size() == n * q );
	bool ok  = true;
	//
	// S = R * J where J is jacobian
	for(size_t i = 0; i < q; i++)
	{	for(size_t j = 0; j < n; j++)
		{	// initialize sparsity for S(i,j)
			bool s_ij = false;
			// S(i,j) = sum_k R(i,k) * J(k,j)
			for(size_t k = 0; k < m; k++)
			{	// sparsity for R(i, k)
				bool R_ik = rt[ k * q + i ];
				bool J_kj = jac_sparse_bool_[THREAD][ k * n + j ];
				s_ij     |= (R_ik & J_kj);
			}
			// set sparsity for S^T
			st[ j * q + i ] = s_ij;
		}
	}
	return ok;
}
template <class Base>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const vectorBool&                       rt ,
	      vectorBool&                       st ,
	const vector<Base>&                     x  )
{	return rev_sparse_jac< vectorBool >(q, rt, st, x);
}
template <class Base>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const vector<bool>&                     rt ,
	      vector<bool>&                     st ,
	const vector<Base>&                     x  )
{	return rev_sparse_jac< vector<bool> >(q, rt, st, x);
}
template <class Base>
bool checkpoint<Base>::rev_sparse_jac(
	size_t                                  q  ,
	const vector< std::set<size_t> >&       rt ,
	      vector< std::set<size_t> >&       st ,
	const vector<Base>&                     x  )
{	// during user sparsity calculations
	size_t m = f_[THREAD].Range();
	size_t n = f_[THREAD].Domain();
	if( jac_sparse_bool_[THREAD].size() != 0 )
		jac_sparse_bool_[THREAD].clear();
	if( jac_sparse_set_[THREAD].n_set() == 0 )
		set_jac_sparse_set();
	CPPAD_ASSERT_UNKNOWN( jac_sparse_bool_[THREAD].size() == 0 );
	CPPAD_ASSERT_UNKNOWN( jac_sparse_set_[THREAD].n_set() == m );
	CPPAD_ASSERT_UNKNOWN( jac_sparse_set_[THREAD].end()   == n );
	CPPAD_ASSERT_UNKNOWN( rt.size() == m );
	CPPAD_ASSERT_UNKNOWN( st.size() == n );
	//
	bool ok  = true;
	//
	for(size_t j = 0; j < n; j++)
		st[j].clear();
	//
	// sparsity for  s = r * jac_sparse_set_
	// s^T = jac_sparse_set_^T * r^T
	for(size_t i = 0; i < m; i++)
	{	// i is the row index in r^T
		std::set<size_t>::const_iterator itr_i;
		const std::set<size_t>& r_i( rt[i] );
		for(itr_i = r_i.begin(); itr_i != r_i.end(); itr_i++)
		{	// k is the column index in r^T
			size_t k = *itr_i;
			CPPAD_ASSERT_UNKNOWN( k < q );
			//
			// i is column index in jac_sparse_set^T
			local::sparse_list::const_iterator set_itr(
				jac_sparse_set_[THREAD], i
			);
			size_t j = *set_itr;
			while( j < n )
			{	// j is row index in jac_sparse_set^T
				st[j].insert(k);
				j = *(++set_itr);
			}
		}
	}

	return ok;
}
# undef THREAD
# endif //  CPPAD_MULTI_THREAD_TMB

} // END_CPPAD_NAMESPACE
# endif
