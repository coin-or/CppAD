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
   numvar
}

Reverse Mode Jacobian Sparsity Patterns
#######################################

Syntax
******
| ``local::sweep::rev_jac`` (
| |tab| *play*               ,
| |tab| *dependency*         ,
| |tab| *n*                  ,
| |tab| *numvar*             ,
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

numvar
******
is the total number of variables in the tape; i.e.,
*play* ``->num_var_rec`` () .
This is also the number of rows in all the sparsity patterns.

var_sparsity
************

On Input
========
For *i* = 0 , ... , *numvar* ``-1`` ,
if *i* corresponds to a dependent variables,
the set with index *i* is an input.
Otherwise the set with index *i* is empty.

On Output
=========
For *i* = 0 , ... , *numvar* ``-1`` ,
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
   size_t                     numvar             ,
   Vector_set&                var_sparsity       ,
   const RecBase&             not_used_rec_base  )
// END_PROTOTYPE
{
   size_t            i, j, k;

   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // check numvar argument
   CPPAD_ASSERT_UNKNOWN( numvar > 0 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec()   == numvar );
   CPPAD_ASSERT_UNKNOWN( var_sparsity.n_set() == numvar );

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
      {
         case AbsOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AddvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_binary_op(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case AddpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
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
         var_op::csum_reverse_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_reverse_jac(
            dependency, i_var, arg, num_par, var_sparsity
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // ---------------------------------------------------

         case CoshOp:
         // sinh(x), cosh(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case DisOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         // derivative is identically zero but dependency is not
         if( dependency ) sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case DivvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_binary_op(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case DivpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case ErfOp:
         case ErfcOp:
         // arg[1] is always the parameter 0
         // arg[0] is always the parameter 2 / sqrt(pi)
         CPPAD_ASSERT_NARG_NRES(op, 3, 5);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case ExpOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case Expm1Op:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         var_op::load_reverse_jac(
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

         case LogOp:
         case NegOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case Log1pOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case MulpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case MulvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_binary_op(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case ParOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);

         break;
         // -------------------------------------------------

         case PowvpOp:
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 3);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case PowvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 3);
         sparse::rev_jac_binary_op(
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
         if( dependency ) sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case SqrtOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         var_op::store_reverse_jac(
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
         sparse::rev_jac_binary_op(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case SubpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case SubvpOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_NARG_NRES(op, 1, 2);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AFunOp:
         var_op::atomic_reverse_jac<Vector_set, Base, RecBase>(
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

         case ZmulpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_unary_op(
            i_var, size_t(arg[0]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         sparse::rev_jac_binary_op(
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
