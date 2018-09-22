# ifndef CPPAD_CORE_CHECKPOINT_REVERSE_HPP
# define CPPAD_CORE_CHECKPOINT_REVERSE_HPP

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
bool checkpoint<Base>::reverse(
	size_t                    q  ,
	const vector<Base>&       tx ,
	const vector<Base>&       ty ,
	      vector<Base>&       px ,
	const vector<Base>&       py )
{	// make sure member_ is allocated for this thread
	size_t thread = thread_alloc::thread_num();
	allocate_member(thread);
	//

# ifndef NDEBUG
	size_t n = member_[thread]->f_.Domain();
	size_t m = member_[thread]->f_.Range();
# endif
	CPPAD_ASSERT_UNKNOWN( n == tx.size() / (q+1) );
	CPPAD_ASSERT_UNKNOWN( m == ty.size() / (q+1) );
	CPPAD_ASSERT_UNKNOWN( member_[thread]->f_.size_var() > 0 );
	CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
	CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
	bool ok  = true;

	// put proper forward mode coefficients in f_
# ifdef NDEBUG
	// compute forward results for orders zero through q
	member_[thread]->f_.Forward(q, tx);
# else
	CPPAD_ASSERT_UNKNOWN( px.size() == n * (q+1) );
	CPPAD_ASSERT_UNKNOWN( py.size() == m * (q+1) );
	size_t i, j, k;
	//
	// compute forward results for orders zero through q
	vector<Base> check_ty = member_[thread]->f_.Forward(q, tx);
	for(i = 0; i < m; i++)
	{	for(k = 0; k <= q; k++)
		{	j = i * (q+1) + k;
			CPPAD_ASSERT_UNKNOWN( check_ty[j] == ty[j] );
		}
	}
# endif
	// now can run reverse mode
	px = member_[thread]->f_.Reverse(q+1, py);

	// no longer need the Taylor coefficients in f_
	// (have to reconstruct them every time)
	size_t c = 0;
	size_t r = 0;
	member_[thread]->f_.capacity_order(c, r);
	return ok;
}
# else // CPPAD_MULTI_THREAD_TMB
# define THREAD omp_get_thread_num()
template <class Base>
bool checkpoint<Base>::reverse(
	size_t                    q  ,
	const vector<Base>&       tx ,
	const vector<Base>&       ty ,
	      vector<Base>&       px ,
	const vector<Base>&       py )
{
# ifndef NDEBUG
	size_t n = f_[THREAD].Domain();
	size_t m = f_[THREAD].Range();
# endif
	CPPAD_ASSERT_UNKNOWN( n == tx.size() / (q+1) );
	CPPAD_ASSERT_UNKNOWN( m == ty.size() / (q+1) );
	CPPAD_ASSERT_UNKNOWN( f_[THREAD].size_var() > 0 );
	CPPAD_ASSERT_UNKNOWN( tx.size() % (q+1) == 0 );
	CPPAD_ASSERT_UNKNOWN( ty.size() % (q+1) == 0 );
	bool ok  = true;

	// put proper forward mode coefficients in f_
# ifdef NDEBUG
	// compute forward results for orders zero through q
	f_[THREAD].Forward(q, tx);
# else
	CPPAD_ASSERT_UNKNOWN( px.size() == n * (q+1) );
	CPPAD_ASSERT_UNKNOWN( py.size() == m * (q+1) );
	size_t i, j, k;
	//
	// compute forward results for orders zero through q
	vector<Base> check_ty = f_[THREAD].Forward(q, tx);
	for(i = 0; i < m; i++)
	{	for(k = 0; k <= q; k++)
		{	j = i * (q+1) + k;
			CPPAD_ASSERT_UNKNOWN( check_ty[j] == ty[j] );
		}
	}
# endif
	// now can run reverse mode
	px = f_[THREAD].Reverse(q+1, py);

	// no longer need the Taylor coefficients in f_
	// (have to reconstruct them every time)
	size_t c = 0;
	size_t r = 0;
	f_[THREAD].capacity_order(c, r);
	return ok;
}
# undef THREAD
# endif //  CPPAD_MULTI_THREAD_TMB

} // END_CPPAD_NAMESPACE
# endif
