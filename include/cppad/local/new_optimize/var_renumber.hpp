# ifndef CPPAD_LOCAL_NEW_OPTIMIZE_VAR_RENUMBER_HPP
# define CPPAD_LOCAL_NEW_OPTIMIZE_VAR_RENUMBER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <algorithm>
# include <cppad/local/optimize/usage.hpp>
# include <cppad/local/new_optimize/var_op_info.hpp>
# include <cppad/local/new_optimize/prev_op_search.hpp>

/*
{xrst_begin optimize_var_renumber dev}
{xrst_spell
   cexp
}

Replace Variables Using Equivalent Previous Variables
#####################################################

Syntax
******
| *exceed_collision_limit* = ``var_renumber`` (
| |tab| *collision_limit* ,
| |tab| *play* ,
| |tab| *cexp_set* ,
| |tab| *op_usage*
| )

Prototype
*********
{xrst_literal
   // BEGIN_DYN_RENUMBER
   // END_DYN_RENUMBER
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
is the player for this operation sequence.
It is both an input and output for this routine.

cexp_set
********
set[i] is a set of elements for the i-th operator.
Suppose that e is an element of set[i], j = e / 2, k = e % 2.
If the comparison for the j-th conditional expression is equal to bool(k),
the i-th operator can be skipped (is not used by any of the results).
Note the j indexes the CExpOp operators in the operation sequence.
On input, cexp_set is does not count previous optimization.
On output, it does count previous optimization.

op_usage
********
The size of this vector is the number of operators in the
operation sequence.i.e., play->nun_var_op().
On input, op_usage[i] is the usage for
the i-th operator in the operation sequence not counting previous optimization.
On output, it is the usage counting previous operator optimization.
If the variable with index *i_var* gets replaced,
op_usage[i] is equal to yes_usage on input and no_usage upon return.


exceed_collision_limit
**********************
If the *collision_limit* is exceeded (is not exceeded),
the return value is true (false).

{xrst_end optimize_var_renumber}
*/

// BEGIN_DYN_RENUMBER
// exceed_collison_limit = var_renumber(
//    collision_limit, play, cexp_set, op_usage
// )
namespace CppAD { namespace local { namespace optimize {
template <class Base>
bool var_renumber(
   size_t                                      collision_limit     ,
   pod_vector<size_t>&                         dep_taddr           ,
   player<Base>*                               play                ,
   sparse::list_setvec&                        cexp_set            ,
   pod_vector<usage_t>&                        op_usage            )
{
   // END_DYN_RENUMBER
   //
   // op_info_t, index_t, op_info
   typedef var_op_info_t< player<Base> > op_info_t;
   typedef typename op_info_t::index_t index_t;
   op_info_t op_info(*play);
   //
   // n_op, n_var
   size_t n_op  = op_info.n_op();
   size_t n_var = op_info.n_var();
   CPPAD_ASSERT_UNKNOWN(size_t( std::numeric_limits<addr_t>::max() ) >= n_op );
   CPPAD_ASSERT_UNKNOWN(size_t( std::numeric_limits<addr_t>::max() ) >= n_var );
   //
   // prev_op_search
   index_t n_hash_code  = index_t( n_op ) + 2;
   prev_op_search_t<op_info_t> prev_op_search(
      op_info, n_hash_code, index_t( collision_limit )
   );
   //
   // var_previous
   pod_vector<addr_t> var_previous(n_var);
   for(size_t i_var = 0; i_var < n_var; ++i_var)
      var_previous[i_var] = addr_t( i_var );
   //
   // var_previous, op_usage
   for(size_t i_op = 0; i_op < n_op; ++i_op)
   {
      //
      // op, i_var
      op_code_var   op    = op_info.op_enum(i_op);
      size_t        i_var = op_info.var_index(i_op);
      //
      // skip
      bool skip = op_usage[i_op] != usage_t(yes_usage);
      switch( op )
      {  default:
         break;

         // ----------------------------------------------------------------
         // these operators never match pevious operators
         case BeginOp:
         case CExpOp:
         case CSkipOp:
         case CSumOp:
         case EndOp:
         case InvOp:
         case LdpOp:
         case LdvOp:
         case ParOp:
         case PriOp:
         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         case AFunOp:
         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         skip = true;
         break;
      }
      if( ! skip )
      {  size_t j_op = size_t(
            prev_op_search.match_op( index_t(i_op), var_previous)
         );
         if( j_op != i_op )
         {  CPPAD_ASSERT_UNKNOWN( j_op < i_op );
            //
            // op_usage
            CPPAD_ASSERT_UNKNOWN( op_usage[j_op] != no_usage );
            op_usage[i_op] = no_usage;
            op_usage[j_op] = yes_usage;
            //
            // cexp_set[j_op]
            if( cexp_set.n_set() != 0 )
            {  cexp_set.process_post(j_op);
               cexp_set.binary_intersection(j_op, j_op, i_op, cexp_set);
            }
            //
            // var_previous
            op_code_var op_match  = op_info.op_enum(j_op);
            size_t      j_var     = op_info.var_index(j_op);
            CPPAD_ASSERT_UNKNOWN( op == op_match );
            if( NumRes(op_match) > 0 )
            {  CPPAD_ASSERT_UNKNOWN( j_var < i_var );
               var_previous[i_var] = addr_t( j_var );
            }
         }
      }
   }
   //
   // var_op, cummulative, arg_one, is_one
   typename op_info_t::op_enum_t  var_op;
   bool                            commutative;
   mutable_subvector_t             arg_one;
   typename op_info_t::vec_bool_t  is_var_one;
   //
   // play->var_arg_
   // renumber variables in variable operaitons sequence
   for(size_t i_op = 0; i_op < op_info.n_op(); ++i_op)
   {  //
      // var_op, cummulative, arg_one, is_one
      op_info.get(i_op, var_op, commutative, arg_one, is_var_one);
      for(size_t j = 0; j < arg_one.size(); ++j)
         if( is_var_one[j] )
            arg_one[j] = var_previous[ arg_one[j] ];
   }
   //
   // dep_taddr
   // renumber the dependent variable vector
   for(size_t i = 0; i < dep_taddr.size(); ++i)
      dep_taddr[i] = size_t( var_previous[ dep_taddr[i] ] );
   //
   /*
   // Print out hash code usage summary
   CppAD::vector<Addr> count = prev_op_search.different_count();
   std::cout << "different_count = " << count << "\n";
   */
   return prev_op_search.exceed_limit();
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
