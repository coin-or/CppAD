# ifndef CPPAD_LOCAL_SPARSE_LIST_SETVEC_HPP
# define CPPAD_LOCAL_SPARSE_LIST_SETVEC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/define.hpp>
# include <cppad/local/is_pod.hpp>
# include <cppad/local/sparse/size_setvec.hpp>

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {

/*
{xrst_begin_parent list_setvec dev}
{xrst_spell
   typedef
}

The list_setvec Class
#####################
This class is a :ref:`SetVector-title` with elements of type ``size_t``.
It is implemented using the template class
:ref:`size_setvec-name` where the elements any positive integer type.
{xrst_code cpp} */
typedef size_setvec<size_t>                 list_setvec;
typedef size_setvec_const_iterator<size_t>  list_setvec_const_iterator;
/*{xrst_code}

{xrst_toc_table
   include/cppad/local/sparse/size_setvec.hpp
}

{xrst_end list_setvec}
// ----------------------------------------------------------------------------
{xrst_begin sparsity_user2internal_list_setvec dev}
{xrst_spell
   msg
}

Copy A Vector of Standard Sets To A list_setvec Object
######################################################

SetVector
*********
is a :ref:`simple vector<SimpleVector-name>` type with elements of type
``std::set<size_t>`` .

internal
********
The input value of this object does not matter.
Upon return it contains the same sparsity pattern as *user*
(or its transpose).

user
****
is the sparsity pattern we are copying to *internal* .

n_set
*****
is the number of sets in the output sparsity pattern *internal* .
If *transpose* is false, *n_set* is equal to
*user* . ``size`` () .

end
***
is the end value for the output sparsity pattern *internal* .
``list_setvec`` sparsity pattern *internal* .
If *transpose* is true, *end* is equal to
*user* . ``size`` () .

transpose
*********
If *transpose* is false,
element *j* is in the *i*-th *internal* set if
*j* is in the *user* [ *i* ] .
Otherwise,
element *j* is in the *i*-th *internal* set if
*i* is in the *user* [ *j* ] .

error_msg
*********
is the error message to display if some values in the *user*
sparsity pattern are not valid.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
template<class SetVector>
void sparsity_user2internal(
   list_setvec&            internal  ,
   const SetVector&        user      ,
   size_t                  n_set     ,
   size_t                  end       ,
   bool                    transpose ,
   const char*             error_msg )
/* {xrst_code}
{xrst_spell_on}

{xrst_end sparsity_user2internal_list_setvec}
**/
{
# ifndef NDEBUG
   if( transpose )
      CPPAD_ASSERT_KNOWN( end == size_t( user.size() ), error_msg);
   if( ! transpose )
      CPPAD_ASSERT_KNOWN( n_set == size_t( user.size() ), error_msg);
# endif

   // iterator for user set
   std::set<size_t>::const_iterator itr;

   // size of internal sparsity pattern
   internal.resize(n_set, end);

   if( transpose )
   {  // transposed pattern case
      for(size_t j = 0; j < end; j++)
      {  itr = user[j].begin();
         while(itr != user[j].end())
         {  size_t i = *itr++;
            CPPAD_ASSERT_KNOWN(i < n_set, error_msg);
            internal.post_element(i, j);
         }
      }
      for(size_t i = 0; i < n_set; ++i)
         internal.process_post(i);
   }
   else
   {  for(size_t i = 0; i < n_set; i++)
      {  itr = user[i].begin();
         while(itr != user[i].end())
         {  size_t j = *itr++;
            CPPAD_ASSERT_KNOWN( j < end, error_msg);
            internal.post_element(i, j);
         }
         internal.process_post(i);
      }
   }
   return;
}

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE

// =========================================================================
// Tell pod_vector class that each pair_size_t is plain old data and hence
// the corresponding constructor need not be called.
namespace CppAD { namespace local {
   template <> inline bool
   is_pod<sparse::size_setvec<size_t>::pair_s_type>(void)
   {  return true; }
} }

# endif
