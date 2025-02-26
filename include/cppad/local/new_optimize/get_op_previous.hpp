# ifndef CPPAD_LOCAL_NEW_OPTIMIZE_GET_OP_PREVIOUS_HPP
# define CPPAD_LOCAL_NEW_OPTIMIZE_GET_OP_PREVIOUS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <algorithm>
# include <cppad/local/optimize/match_op.hpp>
# include <cppad/local/optimize/usage.hpp>
# include <cppad/local/new_optimize/var_op_info.hpp>
# include <cppad/local/new_optimize/prev_op_search.hpp>

/*
{xrst_begin optimize_get_op_previous dev}
{xrst_spell
   cexp
}

Get Mapping From Op to Previous Op That is Equivalent
#####################################################

Syntax
******
| *exceed_collision_limit* = ``get_op_previous`` (
| |tab| *collision_limit* ,
| |tab| *play* ,
| |tab| *random_itr* ,
| |tab| *cexp_set* ,
| |tab| *op_previous* ,
| |tab| *op_usage*
| )

Prototype
*********
{xrst_literal
   // BEGIN_GET_OP_PREVIOUS
   // END_GET_OP_PREVIOUS
}

Base
****
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

collision_limit
***************
is the maximum number of collisions (matches)
allowed in the hash expression has table.

play
****
is the old operation sequence.

random_itr
**********
is a random iterator for the old operation sequence.

cexp_set
********
set[i] is a set of elements for the i-th operator.
Suppose that e is an element of set[i], j = e / 2, k = e % 2.
If the comparison for the j-th conditional expression is equal to bool(k),
the i-th operator can be skipped (is not used by any of the results).
Note the j indexes the CExpOp operators in the operation sequence.
On input, cexp_set is does not count previous optimization.
On output, it does count previous optimization.

op_previous
***********
The input size of this vector must be zero.
Upon return it has size equal to the number of operators
in the operation sequence; i.e., num_op = play->nun_var_rec().
Let j = op_previous[i]. If j = 0, no replacement was found for i-th operator.
If j != 0:

#. j < i
#. op_previous[j] == 0
#. op_usage[j] == usage_t(yes_usage)
#. i-th operator has NumArg(op) <= 3
#. i-th operator has 0 < NumRes(op)
#. i-th operator is not one of the following:
   {xrst_spell_off}
   PriOp, ParOp, InvOp, EndOp, CexpOp, BeginOp.
   {xrst_spell_on}
#. i-th operator is not one of the load store operator:
   {xrst_spell_off}
   LtpvOp, LtvpOp, LtvvOp, StppOp, StpvOp, StvpOp, StvvOp.
   {xrst_spell_on}
#. i-th operator is not a atomic function operator:
   {xrst_spell_off}
   AFunOp, FunapOp, FunavOp, FunrpOp, FunrvOp.
   {xrst_spell_on}

op_usage
********
The size of this vector is the number of operators in the
old operation sequence.i.e., play->nun_var_rec().
On input, op_usage[i] is the usage for
the i-th operator in the operation sequence not counting previous
optimization.
On output, it is the usage counting previous operator optimization.

exceed_collision_limit
**********************
If the *collision_limit* is exceeded (is not exceeded),
the return value is true (false).

{xrst_end optimize_get_op_previous}
*/

// BEGIN_GET_OP_PREVIOUS
// exceed_collison_limit = get_op_previous(
//    collision_limit, play, random_itr, cexp_set, op_previous, op_usage
// )
namespace CppAD { namespace local { namespace optimize {
template <class Addr, class Base>
bool get_op_previous(
   size_t                                      collision_limit     ,
   const player<Base>*                         play                ,
   const play::const_random_iterator<Addr>&    random_itr          ,
   sparse::list_setvec&                        cexp_set            ,
   pod_vector<addr_t>&                         op_previous         ,
   pod_vector<usage_t>&                        op_usage            )
{  CPPAD_ASSERT_UNKNOWN( op_previous.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( op_usage.size() == random_itr.num_op() );
   // END_GET_OP_PREVIOUS
   //
   // op_info_t, op_info
   typedef play::const_random_iterator<Addr> random_itr_t;
   typedef var_op_info_t<random_itr_t>       op_info_t;
   op_info_t op_info(random_itr);
   //
   // prev_op_search
   Addr n_hash_code  = Addr( std::max( random_itr.num_op() ,  size_t(2) ) );
   prev_op_search_t<op_info_t> prev_op_search(
      op_info, n_hash_code, Addr( collision_limit )
   );
   //
   // n_op, n_var
   size_t n_op  = random_itr.num_op();
   size_t n_var = random_itr.num_var();
   CPPAD_ASSERT_UNKNOWN( size_t( std::numeric_limits<Addr>::max() ) >= n_op );
   CPPAD_ASSERT_UNKNOWN( size_t( std::numeric_limits<Addr>::max() ) >= n_var );
   //
   // var_previous
   pod_vector<Addr> var_previous(n_var);
   for(size_t i_var = 0; i_var < n_var; ++i_var)
      var_previous[i_var] = Addr( i_var );
   //
   // op_previous
   op_previous.resize( n_op );
   for(size_t i_op = 0; i_op < n_op; ++i_op)
      op_previous[i_op] = 0;
   //
   for(size_t i_op = 0; i_op < n_op; ++i_op)
   {  //
      // op, i_var
      op_code_var   op;
      const addr_t* arg;
      size_t        i_var;
      random_itr.op_info(i_op, op, arg, i_var);
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
            prev_op_search.match_op( Addr(i_op), var_previous)
         );
         if( j_op != i_op )
         {  //
            // op_previous
            op_previous[i_op] = addr_t(j_op);
            //
            // op_usage, cexp_set
            CPPAD_ASSERT_UNKNOWN( j_op  < i_op );
            bool sum_op = false;
            op_inc_arg_usage(
               play, sum_op, i_op, j_op, op_usage, cexp_set
            );
            //
            // var_previous
            op_code_var op_match;
            size_t      var_match;
            random_itr.op_info(j_op, op_match, arg, var_match);
            CPPAD_ASSERT_UNKNOWN( op == op_match );
            if( NumRes(op) > 0 )
               var_previous[i_var] = Addr( var_match );
         }
      }
   }
   /*
   // Print out hash code usage summary
   CppAD::vector<Addr> count = prev_op_search.different_count();
   std::cout << "different_count = " << count << "\n";
   */
   return prev_op_search.exceed_limit();
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
