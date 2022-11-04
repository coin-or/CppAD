# ifndef CPPAD_CORE_CHKPOINT_ONE_SET_JAC_SPARSE_BOOL_HPP
# define CPPAD_CORE_CHKPOINT_ONE_SET_JAC_SPARSE_BOOL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

template <class Base>
void checkpoint<Base>::set_jac_sparse_bool(void)
{  // make sure member_ is allocated for this thread
   size_t thread = thread_alloc::thread_num();
   allocate_member(thread);
   //
   CPPAD_ASSERT_UNKNOWN( member_[thread]->jac_sparse_bool_.size() == 0 );
   bool transpose  = false;
   bool dependency = true;
   size_t n = member_[thread]->f_.Domain();
   size_t m = member_[thread]->f_.Range();
   // Use the choice for forward / reverse that results in smaller
   // size for the sparsity pattern of all variables in the tape.
   if( n <= m )
   {  vectorBool identity(n * n);
      for(size_t j = 0; j < n; j++)
      {  for(size_t i = 0; i < n; i++)
            identity[ i * n + j ] = (i == j);
      }
      member_[thread]->jac_sparse_bool_ = member_[thread]->f_.ForSparseJac(
         n, identity, transpose, dependency
      );
      member_[thread]->f_.size_forward_bool(0);
   }
   else
   {  vectorBool identity(m * m);
      for(size_t j = 0; j < m; j++)
      {  for(size_t i = 0; i < m; i++)
            identity[ i * m + j ] = (i == j);
      }
      member_[thread]->jac_sparse_bool_ = member_[thread]->f_.RevSparseJac(
         m, identity, transpose, dependency
      );
   }
   CPPAD_ASSERT_UNKNOWN( member_[thread]->f_.size_forward_bool() == 0 );
   CPPAD_ASSERT_UNKNOWN( member_[thread]->f_.size_forward_set() == 0 );
}

} // END_CPPAD_NAMESPACE
# endif
