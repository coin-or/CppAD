# ifndef CPPAD_CORE_CHKPOINT_ONE_SET_HES_SPARSE_BOOL_HPP
# define CPPAD_CORE_CHKPOINT_ONE_SET_HES_SPARSE_BOOL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

template <class Base>
void checkpoint<Base>::set_hes_sparse_bool(void)
{  // make sure member_ is allocated for this thread
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
   {  for(size_t i = 0; i < n; i++)
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

} // END_CPPAD_NAMESPACE
# endif
