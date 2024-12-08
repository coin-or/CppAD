# ifndef CPPAD_LOCAL_SWEEP_REV_JAC_HPP
# define CPPAD_LOCAL_SWEEP_REV_JAC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>

// This value is either zero or one.  Zero is the normal operational value.
// If it is one, a trace of every rev_jac_sweep computation is printed.
# define CPPAD_REV_JAC_TRACE 0

/*
{xrst_begin local_sweep_rev_jac dev}
{xrst_spell
}

Reverse Mode Jacobian Sparsity Patterns
#######################################

Syntax
******
| ``local::sweep::rev_jac`` (
| |tab| *play*               ,
| |tab| *dependency*         ,
| |tab| *n*                  ,
| |tab| *num_var*            ,
| |tab| *var_sparsity*       ,
| |tab| ``not_used_rec_base``
| )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Base
****
this operation sequence was recorded using ``AD`` < *Base* > .

Vector_set
**********
is the type used for vectors of sets. It can be either
``sparse::pack_setvec`` or ``sparse::list_setvec`` .
{xrst_comment 2DO: in previous line change code to cref}

RecBase
*******
Is the base type when this function was recorded.
This is different from *Base* if
this function object was created by :ref:`base2ad-name` .

play
****
The information stored in play
is a recording of the operations corresponding to a function
:math:`F : \B{R}^n \rightarrow \B{R}^m`
where *m* is the number of dependent variables.

dependency
**********
Are we computing dependency relations, or only concerned with
possibly non-zero derivatives. For example,
are the derivatives with respect to
*left* and *right* of the expression below
considered to be non-zero:

   ``CondExpRel`` ( *left* , *right* , *if_true* , *if_false* )

This is used by the optimizer to obtain the correct dependency relations.

n
*
is the number of independent variables in the tape.

num_var
*******
is the total number of variables in the tape; i.e.,
*play* ``->num_var_rec`` () .
This is also the number of rows in all the sparsity patterns.

var_sparsity
************

On Input
========
For *i* = 0 , ... , *num_var* ``-1`` ,
if *i* corresponds to a dependent variables,
the set with index *i* is an input.
Otherwise the set with index *i* is empty.

On Output
=========
For *i* = 0 , ... , *num_var* ``-1`` ,
the sparsity pattern for the variable with index *j* ``-1``
is given by the set with index *j* in *var_sparsity* .
Note that one dependent variable may depend on the value of another,
in which case its output sparsity pattern may be different than its
input pattern.

not_used_rec_base
*****************
Specifies *RecBase* for this call.

{xrst_end local_sweep_rev_jac}
*/

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {

// BEGIN_PROTOTYPE
template <class Base, class Vector_set, class RecBase>
void rev_jac(
   const local::player<Base>* play               ,
   bool                       dependency         ,
   size_t                     n                  ,
   size_t                     num_var            ,
   Vector_set&                var_sparsity       ,
   const RecBase&             not_used_rec_base  )
// END_PROTOTYPE
{
   size_t            i, j, k;

   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // check num_var argument
   CPPAD_ASSERT_UNKNOWN( num_var > 0 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec()   == num_var );
   CPPAD_ASSERT_UNKNOWN( var_sparsity.n_set() == num_var );

   // upper limit (exclusive) for elements in the set
   size_t limit = var_sparsity.end();

   // vecad_sparsity contains a sparsity pattern for each VecAD object.
   // vecad_ind maps a VecAD index (beginning of the VecAD object)
   // to the index of the corresponding set in vecad_sparsity.
   size_t num_vecad_ind   = play->num_var_vecad_ind_rec();
   size_t num_vecad_vec   = play->num_var_vecad_rec();
   Vector_set  vecad_sparsity;
   pod_vector<size_t> vecad_ind;
   if( num_vecad_vec > 0 )
   {  size_t length;
      vecad_sparsity.resize(num_vecad_vec, limit);
      vecad_ind.extend(num_vecad_ind);
      j             = 0;
      for(i = 0; i < num_vecad_vec; i++)
      {  // length of this VecAD
         length   = play->GetVecInd(j);
         // set to proper index for this VecAD
         vecad_ind[j] = i;
         for(k = 1; k <= length; k++)
            vecad_ind[j+k] = num_vecad_vec; // invalid index
         // start of next VecAD
         j       += length + 1;
      }
      CPPAD_ASSERT_UNKNOWN( j == play->num_var_vecad_ind_rec() );
   }
   // ----------------------------------------------------------------------
   //
   // work space used by atomic funcions
   var_op::atomic_op_work<Base> atom_work;
   //
   // pointer to the beginning of the parameter vector
   // (used by atomic functions
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();
   //
   // skip the EndOp at the end of the recording
   play::const_sequential_iterator itr = play->end();
   // op_info
   op_code_var   op;
   size_t        i_var;
   const addr_t* arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == EndOp );
# if CPPAD_REV_JAC_TRACE
   std::cout << std::endl;
   CppAD::vectorBool z_value(limit);
   bool atom_trace = true;
# else
   bool atom_trace = false;
# endif
   bool more_operators = true;
   while(more_operators)
   {  //
      // next op
      (--itr).op_info(op, arg, i_var);

      // rest of information depends on the case
      switch( op )
      {  //
         // operators with one primary result and
         // where the first argument is the only variable
         case AbsOp:
         case AcosOp:
         case AcoshOp:
         case AsinOp:
         case AsinhOp:
         case AtanOp:
         case AtanhOp:
         case CosOp:
         case CoshOp:
         case DivvpOp:
         case ErfOp:
         case ErfcOp:
         case ExpOp:
         case Expm1Op:
         case LogOp:
         case NegOp:
         case Log1pOp:
         case PowvpOp:
         case SinOp:
         case SinhOp:
         case SqrtOp:
         case SubvpOp:
         case TanOp:
         case TanhOp:
         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( 0 < NumArg(op) );
         var_op::one_var_rev_jac(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         // operators with one primary result and
         // where the second argument is the only variable
         case AddpvOp:
         case DivpvOp:
         case MulpvOp:
         case PowpvOp:
         case SubpvOp:
         case ZmulpvOp:
         CPPAD_ASSERT_UNKNOWN( 1 < NumArg(op) );
         var_op::one_var_rev_jac(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AddvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case BeginOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         more_operators = false;
         break;
         // -------------------------------------------------

         case CSkipOp:
         itr.correct_after_decrement(arg);
         break;
         // -------------------------------------------------

         case CSumOp:
         itr.correct_after_decrement(arg);
         var_op::csum_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_rev_jac(
            dependency, i_var, arg, num_par, var_sparsity
         );
         break;
         // ---------------------------------------------------

         case DisOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         // derivative is identically zero but dependency is not
         if( dependency ) var_op::one_var_rev_jac(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case DivvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         var_op::load_rev_jac(
            op,
            num_vecad_ind,
            i_var,
            arg,
            dependency,
            vecad_ind,
            var_sparsity,
            vecad_sparsity
         );
         break;
         // -------------------------------------------------

         case EqppOp:
         case EqpvOp:
         case EqvvOp:
         case LtppOp:
         case LtpvOp:
         case LtvpOp:
         case LtvvOp:
         case LeppOp:
         case LepvOp:
         case LevpOp:
         case LevvOp:
         case NeppOp:
         case NepvOp:
         case NevvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 0);
         break;
         // -------------------------------------------------

         case MulvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case ParOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);

         break;
         // -------------------------------------------------

         case PowvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 3);
         var_op::two_var_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case PriOp:
         CPPAD_ASSERT_NARG_NRES(op, 5, 0);
         break;
         // -------------------------------------------------

         case SignOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         // derivative is identically zero but dependency is not
         if( dependency ) var_op::one_var_rev_jac(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         var_op::store_rev_jac(
            op,
            num_vecad_ind,
            arg,
            dependency,
            vecad_ind,
            var_sparsity,
            vecad_sparsity
         );
         break;
         // -------------------------------------------------

         case SubvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case AFunOp:
         var_op::atomic_rev_jac<Vector_set, Base, RecBase>(
            itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            dependency,
            var_sparsity
         );
         break;

         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         CPPAD_ASSERT_UNKNOWN( false );
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_rev_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_REV_JAC_TRACE
      if( op != AFunOp )
      {  for(j = 0; j < limit; j++)
            z_value[j] = false;
         typename Vector_set::const_iterator itr_sparse(var_sparsity, i_var);
         j = *itr_sparse;
         while( j < limit )
         {  z_value[j] = true;
            j          = *(++itr_sparse);
         }
         printOp<Base, RecBase>(
            std::cout,
            play,
            itr.op_index(),
            i_var,
            op,
            arg
         );
         // Note that sparsity for FunrvOp are computed before call to
         // atomic function so no need to delay printing (as in forward mode)
         if( NumRes(op) > 0 && op != BeginOp ) printOpResult(
            std::cout,
            0,
            (CppAD::vectorBool *) nullptr,
            1,
            &z_value
         );
         std::cout << std::endl;
      }
   }
   std::cout << std::endl;
# else
   }
# endif
   // values corresponding to BeginOp
   CPPAD_ASSERT_UNKNOWN( itr.op_index() == 0 );
   CPPAD_ASSERT_UNKNOWN( i_var == 0 );

   return;
}

// preprocessor symbols that are local to this file
# undef CPPAD_REV_JAC_TRACE

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

# endif
