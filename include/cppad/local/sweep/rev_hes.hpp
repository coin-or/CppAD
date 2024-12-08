# ifndef CPPAD_LOCAL_SWEEP_REV_HES_HPP
# define CPPAD_LOCAL_SWEEP_REV_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/rev_hes.hpp
Compute Reverse mode Hessian sparsity patterns.
*/

/*!
\def CPPAD_REV_HES_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every rev_hes_sweep computation is printed.
*/
# define CPPAD_REV_HES_TRACE 0

/*!
Given the forward Jacobian sparsity pattern for all the variables,
and the reverse Jacobian sparsity pattern for the dependent variables,
RevHesSweep computes the Hessian sparsity pattern for all the independent
variables.

\tparam Base
this operation sequence was recorded using AD<Base>.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param n
is the number of independent variables on the tape.

\param num_var
is the total number of variables on the tape; i.e.,
 play->num_var_rec().
This is also the number of rows in the entire sparsity pattern
 rev_hes_sparse.

\param play
The information stored in play
is a recording of the operations corresponding to a function
\f[
   F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables
and \f$ m \f$ is the number of dependent variables.

\param for_jac_sparse
For i = 0 , ... , num_var - 1,
(for all the variables on the tape),
the forward Jacobian sparsity pattern for the variable with index i
corresponds to the set with index i in for_jac_sparse.

\param RevJac
\b Input:
For i = 0, ... , num_var - 1
the if the variable with index i on the tape is an dependent variable and
included in the Hessian, RevJac[ i ] is equal to true,
otherwise it is equal to false.
\n
\n
\b Output: The values in RevJac upon return are not specified; i.e.,
it is used for temporary work space.

\param rev_hes_sparse
The reverse Hessian sparsity pattern for the variable with index i
corresponds to the set with index i in rev_hes_sparse.
\n
\n
\b Input: For i = 0 , ... , num_var - 1
the reverse Hessian sparsity pattern for the variable with index i is empty.
\n
\n
\b Output: For j = 1 , ... , n,
the reverse Hessian sparsity pattern for the independent dependent variable
with index (j-1) is given by the set with index j
in rev_hes_sparse.
The values in the rest of rev_hes_sparse are not specified; i.e.,
they are used for temporary work space.

\param not_used_rec_base
Specifies RecBase for this call.
*/

template <class Base, class Vector_set, class RecBase>
void rev_hes(
   const local::player<Base>* play,
   size_t                     num_var,
   const Vector_set&          for_jac_sparse,
   bool*                      RevJac,
   Vector_set&                rev_hes_sparse,
   const RecBase&             not_used_rec_base
)
{
   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   size_t             i, j, k;

   // check num_var argument
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec()    == num_var );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse.n_set() == num_var );
   CPPAD_ASSERT_UNKNOWN( rev_hes_sparse.n_set() == num_var );
   CPPAD_ASSERT_UNKNOWN( num_var > 0 );

   // upper limit exclusive for set elements
   size_t limit   = rev_hes_sparse.end();
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse.end() == limit );

   // check number of sets match
   CPPAD_ASSERT_UNKNOWN(
      for_jac_sparse.n_set() == rev_hes_sparse.n_set()
   );

   // vecad_sparsity contains a sparsity pattern for each VecAD object.
   // vecad_ind maps a VecAD index (beginning of the VecAD object)
   // to the index for the corresponding set in vecad_sparsity.
   size_t num_vecad_ind   = play->num_var_vecad_ind_rec();
   size_t num_vecad_vec   = play->num_var_vecad_rec();
   Vector_set vecad_sparse;
   pod_vector<size_t> vecad_ind;
   pod_vector<bool>   vecad_jac;
   if( num_vecad_vec > 0 )
   {  size_t length;
      vecad_sparse.resize(num_vecad_vec, limit);
      vecad_ind.extend(num_vecad_ind);
      vecad_jac.extend(num_vecad_vec);
      j             = 0;
      for(i = 0; i < num_vecad_vec; i++)
      {  // length of this VecAD
         length   = play->GetVecInd(j);
         // set vecad_ind to proper index for this VecAD
         vecad_ind[j] = i;
         // make all other values for this vector invalid
         for(k = 1; k <= length; k++)
            vecad_ind[j+k] = num_vecad_vec;
         // start of next VecAD
         j       += length + 1;
         // initialize this vector's reverse jacobian value
         vecad_jac[i] = false;
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
# if CPPAD_REV_HES_TRACE
   std::cout << std::endl;
   CppAD::vectorBool zf_value(limit);
   CppAD::vectorBool zh_value(limit);
   bool atom_trace = true;
# else
   bool atom_trace = false;
# endif
   bool more_operators = true;
   while(more_operators)
   {  bool linear[3];
      //
      // next op
      (--itr).op_info(op, arg, i_var);

      // rest of information depends on the case
      switch( op )
      {  //
         // linear operators with one primary result
         // and where the first argument is the only variable
         case AbsOp:
         case DivvpOp:
         case NegOp:
         case SubvpOp:
         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( 0 < NumArg(op) );
         linear[0] = true;
         var_op::one_var_rev_hes(
            i_var, size_t(arg[0]), linear,
            RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         // non-linear operators with one primary result
         // and where the first argument is the only variable
         case AcosOp:
         case AcoshOp:
         case AsinOp:
         case AsinhOp:
         case AtanOp:
         case AtanhOp:
         case CosOp:
         case CoshOp:
         case ErfOp:
         case ErfcOp:
         case ExpOp:
         case Expm1Op:
         case LogOp:
         case Log1pOp:
         case PowvpOp:
         case SinOp:
         case SinhOp:
         case SqrtOp:
         case TanOp:
         case TanhOp:
         CPPAD_ASSERT_UNKNOWN( 0 < NumArg(op) );
         linear[0] = false;
         var_op::one_var_rev_hes(
            i_var, size_t(arg[0]), linear,
            RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         // linear and non-liner operators with one primary result
         // and where the second argument is the only variable
         case AddpvOp:
         case MulpvOp:
         case SubpvOp:
         case ZmulpvOp:
         CPPAD_ASSERT_UNKNOWN( 1 < NumArg(op) );
         linear[0] = true;
         var_op::one_var_rev_hes(
            i_var, size_t(arg[1]), linear,
            RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;

         case PowpvOp:
         case DivpvOp:
         CPPAD_ASSERT_UNKNOWN( 1 < NumArg(op) );
         linear[0] = false;
         var_op::one_var_rev_hes(
            i_var, size_t(arg[1]), linear,
            RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case AddvvOp:
         case SubvvOp:
         linear[0] = true;
         linear[1] = true;
         linear[2] = true;
         var_op::two_var_rev_hes(
            i_var, arg, linear, RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case BeginOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1)
         more_operators = false;
         break;
         // -------------------------------------------------

         case CSkipOp:
         itr.correct_after_decrement(arg);
         break;
         // -------------------------------------------------

         case CSumOp:
         itr.correct_after_decrement(arg);
         var_op::csum_rev_hes(
            i_var, arg, RevJac, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_rev_hes(
            i_var, arg, num_par, RevJac, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case DisOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1)
         // derivativve is identically zero
         break;
         // -------------------------------------------------

         case DivvvOp:
         linear[0] = true;
         linear[1] = false;
         linear[2] = false;
         CPPAD_ASSERT_NARG_NRES(op, 2, 1)
         var_op::two_var_rev_hes(
            i_var, arg, linear, RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1)
         // Z is already defined
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         var_op::load_rev_hes(
            op,
            arg,
            num_vecad_ind,
            i_var,
            vecad_ind,
            rev_hes_sparse,
            vecad_sparse,
            RevJac,
            vecad_jac
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
         case ZmulvvOp:
         linear[0] = true;
         linear[1] = true;
         linear[2] = false;
         CPPAD_ASSERT_NARG_NRES(op, 2, 1)
         var_op::two_var_rev_hes(
            i_var, arg, linear, RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case ParOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1)
         break;
         // -------------------------------------------------

         case PowvvOp:
         linear[0] = false;
         linear[1] = false;
         linear[2] = false;
         CPPAD_ASSERT_NARG_NRES(op, 2, 3)
         var_op::two_var_rev_hes(
            i_var, arg, linear, RevJac, for_jac_sparse, rev_hes_sparse
         );
         break;
         // -------------------------------------------------

         case PriOp:
         CPPAD_ASSERT_NARG_NRES(op, 5, 0);
         break;
         // -------------------------------------------------

         case SignOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         // Derivative is identiaclly zero
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         var_op::store_rev_hes(
            op,
            arg,
            num_vecad_ind,
            vecad_ind,
            rev_hes_sparse,
            vecad_sparse,
            RevJac,
            vecad_jac
         );
         break;
         // -------------------------------------------------

         case AFunOp:
         var_op::atomic_rev_hes<Vector_set, Base, RecBase>(
            itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            for_jac_sparse,
            RevJac,
            rev_hes_sparse
         );
         break;

         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_REV_HES_TRACE
      if( op != AFunOp )
      {  for(j = 0; j < limit; j++)
         {  zf_value[j] = false;
            zh_value[j] = false;
         }
         typename Vector_set::const_iterator itr_jac(for_jac_sparse, i_var);
         j = *itr_jac;
         while( j < limit )
         {  zf_value[j] = true;
            j = *(++itr_jac);
         }
         typename Vector_set::const_iterator itr_hes(rev_hes_sparse, i_var);
         j = *itr_hes;
         while( j < limit )
         {  zh_value[j] = true;
            j = *(++itr_hes);
         }
         printOp<Base, RecBase>(
            std::cout,
            play,
            itr.op_index(),
            i_var,
            op,
            arg
         );
         // should also print RevJac[i_var], but printOpResult does not
         // yet allow for this
         if( NumRes(op) > 0 && op != BeginOp ) printOpResult(
            std::cout,
            1,
            &zf_value,
            1,
            &zh_value
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
} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

// preprocessor symbols that are local to this file
# undef CPPAD_REV_HES_TRACE

# endif
