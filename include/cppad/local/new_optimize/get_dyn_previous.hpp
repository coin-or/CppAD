# ifndef CPPAD_LOCAL_NEW_OPTIMIZE_GET_DYN_PREVIOUS_HPP
# define CPPAD_LOCAL_NEW_OPTIMIZE_GET_DYN_PREVIOUS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
/*!
\file get_cexp_info.hpp
Create operator information tables
*/

# include <cppad/local/optimize/usage.hpp>
# include <cppad/local/optimize/hash_code.hpp>
# include <cppad/local/new_optimize/dyn_op_info.hpp>
# include <cppad/local/new_optimize/prev_op_search.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

/*
{xrst_begin get_dyn_previous dev}

Map Dynamic Parameters to Equivalent Previous Dynamic Parameters
################################################################

Syntax
******
{xrst_literal
   // BEGIN_GET_DYN_PREVIOUS
   // END_GET_DYN_PREVIOUS
}

Base
****
base type for the operator; i.e., this player was recorded using AD< *Base* >.

collision_limit
***************
is the maximum number of collisions (matches)
allowed in the hash expression has table.

play
****
This is the player for this operation sequence.

par_usage
*********
The size of this vector is the number of parameters in the operation sequence.
If *par_usage* [ *i_par* ] is true (false) the parameter with index *i_par*
is used (is not used) to calculate the dependent variables for this
operation sequence.

dyn_previous
************
The input size of this vector must be zero.
Upon return it has size equal to the number of dynamic parameters; i.e.,
play->num_dynamic_par().
Fix i_dyn, a dynamic parameter index and
::

   Let j_dyn         = dyn_previous[i_dyn]
   Let dyn2par_index = play->dyn_ind2par_ind

The j_dyn-th dynamic parameter
can be used as a replacement for the i_dyn-th dynamic parameter and
::

   If j_dyn != i_dyn then
      j_dyn < i_dyn
      j_dyn == dyn_previous[j_dyn].
      par_usage[ dyn2par_index[i_par] ] == true
      par_usage[ dyn2par_index[j_dyn] ] == true

exceed_limit
************
If the *collision_limit* is exceeded (is not exceeded),
the return value is true (false).

{xrst_end get_dyn_previous}
*/

// BEGIN_GET_DYN_PREVIOUS
// exceet_limit = get_dyn_previous(play, par_usage, dyn_previous)
namespace CppAD { namespace local { namespace optimize {
template <class Base> bool get_dyn_previous(
   addr_t                       collision_limit     ,
   const player<Base>*          play                ,
   const pod_vector<bool>&      par_usage           ,
   pod_vector<addr_t>&          dyn_previous        )
{  CPPAD_ASSERT_UNKNOWN( dyn_previous.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( par_usage.size() == play->num_par_rec() );
   // END_GET_DYN_PREVIOUS

   // n_par
   // number of parameters in the recording
   addr_t n_par = addr_t( play->num_par_rec() );

   // n_dyn
   // number of dynamic parameters in the recording
   addr_t n_dyn = addr_t( play->num_dynamic_par() );

   // check
   CPPAD_ASSERT_UNKNOWN( par_usage.size() == size_t(n_par) );
   CPPAD_ASSERT_UNKNOWN( n_dyn <= n_par );

   // dyn2par_index, dyn_par_op
   const pod_vector<addr_t>&   dyn2par_index( play->dyn_ind2par_ind() );
   const pod_vector<opcode_t>& dyn_par_op( play->dyn_par_op() );

   //
   // op_info
   typedef dyn_op_info_t< player<Base> > op_info_t;
   op_info_t op_info(*play);
   //
   // prev_op_search
   addr_t n_hash_code     = addr_t(n_dyn) + 2;
   prev_op_search_t<op_info_t> prev_op_search(
      op_info, n_hash_code, collision_limit
   );
   //
   // par_previous
   pod_vector<addr_t> par_previous;
   par_previous.resize( size_t(n_par) );
   for(addr_t i_par = 0; i_par < n_par; ++i_par)
      par_previous[i_par] = i_par;
   //
   // dyn_previous
   dyn_previous.resize( size_t(n_dyn) );
   for(addr_t i_dyn = 0; i_dyn < n_dyn; ++i_dyn)
      dyn_previous[i_dyn] = i_dyn;
   //
   // i_dyn
   for(addr_t i_dyn = 0; i_dyn < n_dyn; ++i_dyn)
   {  // i_par
      // parameter index for this dynamic parameter
      addr_t i_par = dyn2par_index[i_dyn];
      CPPAD_ASSERT_UNKNOWN( play->dyn_par_is()[i_par] );
      //
      // skip
      op_code_dyn op = op_code_dyn( dyn_par_op[i_dyn] );
      bool skip       = op == result_dyn || op == ind_dyn || op == atom_dyn;
      //
      if( par_usage[i_par]  && ! skip )
      {  //
         // j_op
         addr_t j_dyn = prev_op_search.match_op(i_dyn, par_previous);
         //
         if( j_dyn != i_dyn )
         {  CPPAD_ASSERT_UNKNOWN( j_dyn < i_dyn );
            CPPAD_ASSERT_UNKNOWN( dyn2par_index[j_dyn] < i_par );
            CPPAD_ASSERT_UNKNOWN( j_dyn == dyn_previous[j_dyn] )
            dyn_previous[i_dyn] = j_dyn;
            par_previous[i_par] = dyn2par_index[j_dyn];
         }
      }
   }
   return prev_op_search.exceed_limit();
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
