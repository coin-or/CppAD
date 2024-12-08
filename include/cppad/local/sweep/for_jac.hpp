# ifndef CPPAD_LOCAL_SWEEP_FOR_JAC_HPP
# define CPPAD_LOCAL_SWEEP_FOR_JAC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <set>
# include <cppad/local/pod_vector.hpp>
# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/for_jac.hpp
Compute Forward mode Jacobian sparsity patterns.
*/

/*!
\def CPPAD_FOR_JAC_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every for_jac_sweep computation is printed.
*/
# define CPPAD_FOR_JAC_TRACE 0

/*!
Given the sparsity pattern for the independent variables,
ForJacSweep computes the sparsity pattern for all the other variables.

\tparam Base
this operation sequence was recorded using AD<Base>.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
   CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param n
is the number of independent variables on the tape.

\param num_var
is the total number of variables on the tape; i.e.,
 play->num_var_rec().

\param play
The information stored in play
is a recording of the operations corresponding to a function
\f[
   F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables
and \f$ m \f$ is the number of dependent variables.

\param var_sparsity
\b Input: For j = 1 , ... , n,
the sparsity pattern for the independent variable with index (j-1)
corresponds to the set with index j in var_sparsity.
\n
\n
\b Output: For i = n + 1 , ... , num_var - 1,
the sparsity pattern for the variable with index i on the tape
corresponds to the set with index i in var_sparsity.

\par Checked Assertions:
\li num_var == var_sparsity.n_set()
\li num_var == play->num_var_rec()

\param not_used_rec_base
Specifies RecBase for this call.
*/

template <class Vector_set, class Base, class RecBase>
void for_jac(
   const local::player<Base>* play,
   bool                       dependency        ,
   size_t                     n                 ,
   size_t                     num_var           ,
   Vector_set&                var_sparsity,
   const RecBase&             not_used_rec_base
)
{
   size_t            i, j, k;

   // check num_var argument
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec()  == num_var );
   CPPAD_ASSERT_UNKNOWN( var_sparsity.n_set() == num_var );

   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // cum_sparsity accumulates sparsity pattern a cumulative sum
   size_t limit = var_sparsity.end();

   // vecad_sparsity contains a sparsity pattern from each VecAD object
   // to all the other variables.
   // vecad_ind maps a VecAD index (the beginning of the
   // VecAD object) to its from index in vecad_sparsity
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

   // work space used by atomic funcions
   var_op::atomic_op_work<Base> atom_work;
   //
   //
   // pointer to the beginning of the parameter vector
   // (used by atomic functions)
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();
   //
# if CPPAD_FOR_JAC_TRACE
   std::cout << std::endl;
   CppAD::vectorBool z_value(limit);
   bool atom_trace = true;
# else
   bool atom_trace = false;
# endif

   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   op_code_var op;
   size_t i_var;
   const addr_t*   arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
   //
   bool more_operators = true;
   while(more_operators)
   {  //
      // this op
      (++itr).op_info(op, arg, i_var);
      //
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
         var_op::one_var_for_jac(
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
         var_op::one_var_for_jac(
            i_var, size_t(arg[1]), var_sparsity
         );
         break;
         // -------------------------------------------------

         case AddvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_for_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case CSkipOp:
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         var_op::csum_for_jac(
            i_var, arg, var_sparsity
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_for_jac(
            dependency, i_var, arg, num_par, var_sparsity
         );
         break;
         // --------------------------------------------------

         case DisOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         // derivative is identically zero but dependency is not
         if( dependency ) var_op::one_var_for_jac(
            i_var, size_t(arg[1]), var_sparsity
         );
         else
            var_sparsity.clear(i_var);
         break;
         // -------------------------------------------------

         case DivvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1);
         var_op::two_var_for_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case EndOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         // sparsity pattern is already defined
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         var_op::load_for_jac(
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
         var_op::two_var_for_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case ParOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         var_sparsity.clear(i_var);
         break;
         // -------------------------------------------------

         case PowvvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 3);
         var_op::two_var_for_jac(
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
         if( dependency ) var_op::one_var_for_jac(
            i_var, size_t(arg[0]), var_sparsity
         );
         else
            var_sparsity.clear(i_var);
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         var_op::store_for_jac(
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
         var_op::two_var_for_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         case AFunOp:
         var_op:: atomic_for_jac<Vector_set, Base, RecBase>(
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
         var_op::two_var_for_jac(
            i_var, arg, var_sparsity
         );
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_FOR_JAC_TRACE
      if( op != AFunOp )
      {  // value for this variable
         for(j = 0; j < limit; j++)
            z_value[j] = false;
         typename Vector_set::const_iterator sparse_itr(var_sparsity, i_var);
         j = *sparse_itr;
         while( j < limit )
         {  z_value[j] = true;
            j = *(++sparse_itr);
         }
         printOp<Base, RecBase>(
            std::cout,
            play,
            itr.op_index(),
            i_var,
            op,
            arg
         );
         if( NumRes(op) > 0 ) printOpResult(
            std::cout,
            1,
            &z_value,
            0,
            (CppAD::vectorBool *) nullptr
         );
         std::cout << std::endl;
      }
# endif
   }
   return;
}

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

// preprocessor symbols that are local to this file
# undef CPPAD_FOR_JAC_TRACE

# endif
