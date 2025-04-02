# ifndef CPPAD_LOCAL_NEW_OPTIMIZE_GET_DYN_OP_PREV_HPP
# define CPPAD_LOCAL_NEW_OPTIMIZE_GET_DYN_OP_PREV_HPP
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
{xrst_begin get_dyn_op_prev dev}

Replace Dynamic Parameters Using Equivalent Previous Parameters
###############################################################

Syntax
******
{xrst_literal
   // BEGIN_GET_DYN_OP_PREV
   // END_GET_DYN_OP_PREV
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
If a the parameter with index *i_par* gets replaced:

#. It is a dynamic parameter
#. *par_usage* [ *i_par* ] was true on input
#. *par_usage* [ *i_par* ] is false upon return


dyn_op_prev
***********
The input size of this vector must be zero.
Upon return it has size equal to the number of dynamic parameters; i.e.,
play->num_dynamic_par().
Fix i_dyn, a dynamic parameter index and
::

   Let j_dyn         = dyn_op_prev[i_dyn]
   Let dyn2par_index = play->dyn2par_index

The j_dyn-th dynamic parameter
can be used as a replacement for the i_dyn-th dynamic parameter and on input
::

   If j_dyn != i_dyn then
      j_dyn < i_dyn
      j_dyn == dyn_op_prev[j_dyn].
      par_usage[ dyn2par_index[i_par] ] == true
      par_usage[ dyn2par_index[j_dyn] ] == true

exceed_limit
************
If the *collision_limit* is exceeded (is not exceeded),
the return value is true (false).

{xrst_end get_dyn_op_prev}
*/

// BEGIN_GET_DYN_OP_PREV
// exceet_limit = get_dyn_op_prev(play, par_usage, dyn_op_prev)
namespace CppAD { namespace local { namespace optimize {
template <class Base> bool get_dyn_op_prev(
   addr_t                       collision_limit     ,
   player<Base>*                play                ,
   pod_vector<bool>&            par_usage           ,
   pod_vector<addr_t>&          dyn_op_prev         )
{  CPPAD_ASSERT_UNKNOWN( dyn_op_prev.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( par_usage.size() == play->num_par_all() );
   // END_GET_DYN_OP_PREV

   // n_par
   // number of parameters in the recording
   addr_t n_par = addr_t( play->num_par_all() );

   // n_dyn
   // number of dynamic parameters in the recording
   addr_t n_dyn = addr_t( play->num_dynamic_par() );

   // check
   CPPAD_ASSERT_UNKNOWN( par_usage.size() == size_t(n_par) );
   CPPAD_ASSERT_UNKNOWN( n_dyn <= n_par );

   // dyn2par_index, dyn_par_op
   const pod_vector<addr_t>&   dyn2par_index( play->dyn2par_index() );
   const pod_vector<opcode_t>& dyn_par_op( play->dyn_par_op() );
   //
   // dyn_info
   typedef dyn_op_info_t< player<Base> > dyn_info_t;
   dyn_info_t dyn_info(*play);
   //
   // var_info
   typedef var_op_info_t< player<Base> > var_info_t;
   var_info_t var_info(*play);
   //
   // prev_op_search
   addr_t n_hash_code     = addr_t(n_dyn) + 2;
   prev_op_search_t<dyn_info_t> prev_op_search(
      dyn_info, n_hash_code, collision_limit
   );
   //
   // par_previous
   pod_vector<addr_t> par_previous;
   par_previous.resize( size_t(n_par) );
   for(addr_t i_par = 0; i_par < n_par; ++i_par)
      par_previous[i_par] = i_par;
   //
   // dyn_op_prev
   dyn_op_prev.resize( size_t(n_dyn) );
   for(addr_t i_dyn = 0; i_dyn < n_dyn; ++i_dyn)
      dyn_op_prev[i_dyn] = i_dyn;
   //
   // par_previous, par_usage, dyn_op_prev
   for(addr_t i_dyn = 0; i_dyn < n_dyn; ++i_dyn)
   {  //
      // i_par
      // parameter index for this dynamic parameter
      addr_t i_par = dyn2par_index[i_dyn];
      CPPAD_ASSERT_UNKNOWN( play->par_is_dyn()[i_par] );
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
            CPPAD_ASSERT_UNKNOWN( j_dyn == dyn_op_prev[j_dyn] )
            dyn_op_prev[i_dyn] = j_dyn;
            par_previous[i_par] = dyn2par_index[j_dyn];
            CPPAD_ASSERT_UNKNOWN( par_previous[i_par] < i_par );
            par_usage[i_par]    = false;
         }
      }
   }
   //
   // dyn_op, var_op, cummulative, arg_one, is_one
   typename dyn_info_t::op_enum_t  dyn_op;
   typename var_info_t::op_enum_t  var_op;
   bool                            commutative;
   mutable_subvector_t             arg_one;
   typename dyn_info_t::vec_bool_t is_res_one;
   //
   // renumber parameters in dynamic parameter operaitons sequence
   for(size_t i_dyn = 0; i_dyn < dyn_info.n_op(); ++i_dyn)
   {  //
      // dyn_op, cummulative, arg_one, is_one
      dyn_info.get(i_dyn, dyn_op, commutative, arg_one, is_res_one);
      for(size_t j = 0; j < arg_one.size(); ++j)
         if( is_res_one[j] )
            arg_one[j] = par_previous[ arg_one[j] ];
   }
   //
   // is_parameter
   pod_vector<bool> is_parameter;
   //
   // renumber parameters in variable operaitons sequence
   for(size_t i_op = 0; i_op < var_info.n_op(); ++i_op)
   {  //
      // var_op, cummulative, arg_one, is_one
      var_info.get(i_op, var_op, commutative, arg_one, is_res_one);
      arg_is_parameter(var_op, arg_one, is_parameter);
      for(size_t j = 0; j < arg_one.size(); ++j)
         if( is_parameter[j] )
            arg_one[j] = par_previous[ arg_one[j] ];
   }
   //
   return prev_op_search.exceed_limit();
}



} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
