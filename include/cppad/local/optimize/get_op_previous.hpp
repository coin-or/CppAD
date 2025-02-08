# ifndef CPPAD_LOCAL_OPTIMIZE_GET_OP_PREVIOUS_HPP
# define CPPAD_LOCAL_OPTIMIZE_GET_OP_PREVIOUS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/optimize/match_op.hpp>
# include <cppad/local/optimize/usage.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {
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
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
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

// BEGIN_PROTOTYPE
template <class Addr, class Base>
bool get_op_previous(
   size_t                                      collision_limit     ,
   const player<Base>*                         play                ,
   const play::const_random_iterator<Addr>&    random_itr          ,
   sparse::list_setvec&                        cexp_set            ,
   pod_vector<addr_t>&                         op_previous         ,
   pod_vector<usage_t>&                        op_usage            )
// END_PROTOTYPE
{  bool exceed_collision_limit = false;
   //
   // number of operators in the tape
   const size_t num_op = random_itr.num_op();
   CPPAD_ASSERT_UNKNOWN( op_previous.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( op_usage.size() == num_op );
   op_previous.resize( num_op );
   //
   // number of conditional expressions in the tape
   //
   // initialize mapping from variable index to operator index
   CPPAD_ASSERT_UNKNOWN(
      size_t( (std::numeric_limits<addr_t>::max)() ) >= num_op
   );
   // ----------------------------------------------------------------------
   // compute op_previous
   // ----------------------------------------------------------------------
   sparse::list_setvec  hash_table_op;
   hash_table_op.resize(CPPAD_HASH_TABLE_SIZE, num_op);
   //
   pod_vector<bool> work_bool;
   pod_vector<addr_t> work_addr_t;
   for(size_t i_op = 0; i_op < num_op; ++i_op)
   {  op_previous[i_op] = 0;

      if( op_usage[i_op] == usage_t(yes_usage) )
      switch( random_itr.get_op(i_op) )
      {
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
         break;

         // ----------------------------------------------------------------
         // check for a previous match
         // BEGIN_SORT_THIS_LINE_PLUS_1
         case AbsOp:
         case AcosOp:
         case AcoshOp:
         case AddpvOp:
         case AddvvOp:
         case AsinOp:
         case AsinhOp:
         case AtanOp:
         case AtanhOp:
         case CosOp:
         case CoshOp:
         case DisOp:
         case DivpvOp:
         case DivvpOp:
         case DivvvOp:
         case EqppOp:
         case EqpvOp:
         case EqvvOp:
         case ErfOp:
         case ErfcOp:
         case ExpOp:
         case Expm1Op:
         case LeppOp:
         case LepvOp:
         case LevpOp:
         case LevvOp:
         case Log1pOp:
         case LogOp:
         case LtppOp:
         case LtpvOp:
         case LtvpOp:
         case LtvvOp:
         case MulpvOp:
         case MulvvOp:
         case NegOp:
         case NeppOp:
         case NepvOp:
         case NevvOp:
         case PowpvOp:
         case PowvpOp:
         case PowvvOp:
         case SignOp:
         case SinOp:
         case SinhOp:
         case SqrtOp:
         case SubpvOp:
         case SubvpOp:
         case SubvvOp:
         case TanOp:
         case TanhOp:
         case ZmulpvOp:
         case ZmulvpOp:
         case ZmulvvOp:
         // END_SORT_THIS_LINE_MINUS_1
         exceed_collision_limit |= match_op(
            collision_limit,
            random_itr,
            op_previous,
            i_op,
            hash_table_op,
            work_bool,
            work_addr_t
         );
         if( op_previous[i_op] != 0 )
         {  // like a unary operator that assigns i_op equal to previous.
            size_t previous = size_t( op_previous[i_op] );
            bool sum_op = false;
            CPPAD_ASSERT_UNKNOWN( previous < i_op );
            op_inc_arg_usage(
               play, sum_op, i_op, previous, op_usage, cexp_set
            );
         }
         break;

         // ----------------------------------------------------------------
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
      }
   }
   /* ---------------------------------------------------------------------
   // Print out hash code usage summary
   CppAD::vector<size_t> count(collision_limit + 1);
   for(size_t i = 0; i <= collision_limit; ++i)
      count[i] = 0;
   for(size_t code = 0; code < CPPAD_HASH_TABLE_SIZE; ++code)
   {  size_t size = hash_table_op.number_elements(code);
      ++count[size];
   }
   std::cout << "count = " << count << "\n";
   --------------------------------------------------------------------- */
   return exceed_collision_limit;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
