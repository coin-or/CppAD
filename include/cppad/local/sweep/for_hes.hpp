# ifndef CPPAD_LOCAL_SWEEP_FOR_HES_HPP
# define CPPAD_LOCAL_SWEEP_FOR_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/var_op/load_op.hpp>
# include <cppad/local/var_op/store_op.hpp>
# include <cppad/local/var_op/csum_op.hpp>

/*
{xrst_begin local_sweep_for_hes dev}
{xrst_spell
   inv
}

Forward Mode Hessian Sparsity Patterns
######################################

Syntax
******
| ``local::sweep::for_hes`` (
| |tab| *play*               ,
| |tab| *n*                  ,
| |tab| *num_var*            ,
| |tab| *select_domain*      ,
| |tab| *rev_jac_sparse*     ,
| |tab| *for_hes_sparse*     ,
| |tab| ``not_used_rec_base``
| )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
Given the forward Jacobian sparsity pattern for all the variables,
and the reverse Jacobian sparsity pattern for the dependent variables,
``for_hes`` computes the Hessian sparsity pattern for all the independent
variables.

CPPAD_FOR_HES_TRACE
*******************
This value is either zero or one.  Zero is the normal operational value.
If it is one, a trace of Jacobian and Hessian sparsity result for every
operation for every ``for_hes`` sweep is printed.
The sparsity patterns are printed as binary numbers with 1 (0) meaning that
the corresponding index is (is not) in the set.
{xrst_code hpp} */
# define CPPAD_FOR_HES_TRACE 0
/* {xrst_code}

Base
****
The operation sequence in *play* was recorded using
``AD`` < *Base* > .

RecBase
*******
Is the base type when this function was recorded.
This is different from *Base* if
this function object was created by :ref:`base2ad-name` .

SetVector
*********
This is a :ref:`SetVector-name` type.

play
****
The information stored in play
is a recording of the operations corresponding to a function
:math:`F : \B{R}^n \rightarrow \B{R}^m`
where *m* is the number of dependent variables.

n
*
is the number of independent variables in the tape.

num_var
*******
is the total number of variables in the tape; i.e.,
*play* ``->num_var_rec`` () .
This is also the number of sets in all the sparsity patterns.

select_domain
*************
is a vector with size *n* that specifies
which components of the domain to include in the Hessian sparsity pattern.
For *j* = 0, ..., *n* ``-1`` , the *j*-th independent variable
will be included if and only if *select_domain* [ *j* ] is true.
This assumes that the order of the independent variables is the same
as the order of the InvOp operators.

rev_jac_sparse
**************
Is a sparsity pattern with size *num_var* by one.
For *i* =1, ..., *num_var* ``-1`` ,
the if the scalar valued function we are computing the Hessian sparsity for
has a non-zero derivative w.r.t. variable with index *i* ,
the set with index *i* has the element zero.
Otherwise it has no elements.

for_hes_sparse
**************
This is a sparsity pattern with *n* + 1 + *num_var* sets
and end value *n* + 1 .
On input, all of the sets are empty.
On output, it contains the two sparsity patterns described below:

Hessian Sparsity
================
For *j* equal 1 to *n* ,
if *i* is in set with index *j* ,
the Hessian may have a non-zero partial with respect to the
independent variables with indices ( *i* - 1, *j* - 1 ) .
Note that the index zero is not used because it corresponds to the
phantom variable on the tape.

Jacobian Sparsity
=================
For *k* equal 1 to *num_var* - 1 ,
if *i* is in the set with index *n* + 1 + *k* ,
the variable with index *k* may have a non-zero partial with resect to the
independent variable with index *i* - 1 .

Method
======
For *k* equal 1 to *num_var* - 1,
the Jacobian sparsity pattern for variable with index *k* is computed using
the previous Jacobian sparsity patterns.
The Hessian sparsity pattern is updated using linear and non-linear
interactions for the variable with index *k* and the previous Jacobian
sparsity patterns.

not_used_rec_base
*****************
This argument is only used to specify the type *RecBase* for this call.

{xrst_end local_sweep_for_hes}
*/

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {

// BEGIN PROTOTYPE
template <class Base, class SetVector, class RecBase>
void for_hes(
   const local::player<Base>* play                ,
   size_t                     n                   ,
   size_t                     num_var             ,
   const pod_vector<bool>&    select_domain       ,
   const SetVector&           rev_jac_sparse      ,
   SetVector&                 for_hes_sparse      ,
   const RecBase&             not_used_rec_base   )
// END PROTOTYPE
{
   // length of the parameter vector (used by CppAD assert macros)
# ifndef NDEBUG
   const size_t num_par = play->num_par_rec();
# endif

   // check arguments
   size_t np1 = n+1;
   CPPAD_ASSERT_UNKNOWN( select_domain.size()   == n );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec()    == num_var );
   CPPAD_ASSERT_UNKNOWN( rev_jac_sparse.n_set() == num_var );
   CPPAD_ASSERT_UNKNOWN( for_hes_sparse.n_set() == np1+num_var );
   //
   CPPAD_ASSERT_UNKNOWN( rev_jac_sparse.end()   == 1   );
   CPPAD_ASSERT_UNKNOWN( for_hes_sparse.end()   == np1 );
   //
   CPPAD_ASSERT_UNKNOWN( num_var > 0 );
   //
   // vecad_sparsity: forward Jacobian sparsity pattern for each VecAD object.
   // vecad_ind: maps the VecAD index at beginning of the VecAD object
   //            to the index for the corresponding set in vecad_sparsity.
   size_t num_vecad_ind   = play->num_var_vecad_ind_rec();
   size_t num_vecad_vec   = play->num_var_vecad_rec();
   SetVector vecad_sparsity;
   pod_vector<size_t> vecad_ind;
   if( num_vecad_vec > 0 )
   {  size_t length;
      vecad_sparsity.resize(num_vecad_vec, np1);
      vecad_ind.extend(num_vecad_ind);
      size_t j  = 0;
      for(size_t i = 0; i < num_vecad_vec; i++)
      {  // length of this VecAD
         length   = play->GetVecInd(j);
         // set vecad_ind to proper index for this VecAD
         vecad_ind[j] = i;
         // make all other values for this vector invalid
         for(size_t k = 1; k <= length; k++)
            vecad_ind[j+k] = num_vecad_vec;
         // start of next VecAD
         j       += length + 1;
      }
      CPPAD_ASSERT_UNKNOWN( j == play->num_var_vecad_ind_rec() );
   }
   // ------------------------------------------------------------------------
   // work space used by atomic funcions
   var_op::atomic_op_work<Base> atom_work;
   //
   //
   // pointer to the beginning of the parameter vector
   // (used by atomic functions)
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();
   //
   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   op_code_var   op;
   size_t        i_var;
   const addr_t* arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
# if CPPAD_FOR_HES_TRACE
   std::cout << std::endl;
   bool atom_trace = true;
# else
   bool atom_trace = false;
# endif
   bool   more_operators = true;
   size_t count_independent = 0;
   while(more_operators)
   {  bool linear[3];

      // next op
      (++itr).op_info(op, arg, i_var);

      // does the Hessian in question have a non-zero derivative
      // with respect to this variable
      bool include = NumRes(op) > 0;
      if( include )
         include = rev_jac_sparse.is_element(i_var, 0);
      switch( op )
      {
         // include
         // operators that must always be included
         case EndOp:
         case CSkipOp:
         case AFunOp:
         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         include = true;
         break;

         // count_independent
         case InvOp:
         if( ! include )
            ++count_independent;
         break;

         // itr
         case CSumOp:
         if( ! include )
            itr.correct_before_increment();
         break;

         // default
         default:
         break;
      }
      //
      if( include ) switch( op )
      {  // operators that should not occurr
         // case BeginOp

         // operators that do not affect Jacobian or Hessian
         // and where with a fixed number of arguments and results
         case CExpOp:
         case DisOp:
         case ParOp:
         case PriOp:
         case SignOp:
         break;
         // -------------------------------------------------

         // independent variable operator: set J(i_var) = { i_var }
         case InvOp:
         CPPAD_ASSERT_UNKNOWN( for_hes_sparse.number_elements(i_var) == 0 );
         if( select_domain[count_independent] )
         {  // Not using post_element because only adding one element
            // per set
            for_hes_sparse.add_element(np1 + i_var, i_var);
         }
         ++count_independent;
         break;

         // -------------------------------------------------
         // linear operators where arg[0] is the only variable
         case AbsOp:
         case DivvpOp:
         case SubvpOp:
         case ZmulvpOp:
         linear[0] = true;
         var_op::one_var_for_hes(
            np1, num_var, i_var, size_t(arg[0]), linear, for_hes_sparse
         );
         break;

         // -------------------------------------------------
         // linear operators where arg[1] is the only variable
         case AddpvOp:
         case MulpvOp:
         case SubpvOp:
         linear[0] = true;
         var_op::one_var_for_hes(
            np1, num_var, i_var, size_t(arg[1]), linear, for_hes_sparse
         );
         break;

         // -------------------------------------------------
         // linear operators where arg[0] and arg[1] are variables
         case AddvvOp:
         case SubvvOp:
         linear[0] = true;
         linear[1] = true;
         linear[2] = true;
         var_op::two_var_for_hes(
            np1, num_var, i_var, arg, linear, for_hes_sparse
         );
         break;

         // ------------------------------------------------------
         // VecAD load operators
         case LdvOp:
         case LdpOp:
         var_op::load_for_hes(
            op, arg, num_vecad_ind, i_var, n,
            vecad_ind, vecad_sparsity, for_hes_sparse
         );
         break;
         //
         // VecAD store operators
         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         var_op::store_for_hes(op,
            arg, num_vecad_ind, n,
            vecad_ind, vecad_sparsity, for_hes_sparse
         );
         break;

         // ------------------------------------------------------
         // nonlinear operators where arg[0] is the only variable
         case AcosOp:
         case AsinOp:
         case AtanOp:
         case CosOp:
         case CoshOp:
         case ExpOp:
         case LogOp:
         case NegOp:
         case SinOp:
         case SinhOp:
         case SqrtOp:
         case TanOp:
         case TanhOp:
         case AcoshOp:
         case AsinhOp:
         case AtanhOp:
         case Expm1Op:
         case Log1pOp:
         case ErfOp:
         case ErfcOp:
         CPPAD_ASSERT_UNKNOWN( 0 < NumArg(op) )
         linear[0] = false;
         var_op::one_var_for_hes(
               np1, num_var, i_var, size_t(arg[0]), linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------

         case CSkipOp:
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         var_op::csum_for_hes(arg, i_var, n, for_hes_sparse);
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DivvvOp:
         linear[0] = true;
         linear[1] = false;
         linear[2] = false;
         CPPAD_ASSERT_NARG_NRES(op, 2, 1)
         var_op::two_var_for_hes(
            np1, num_var, i_var, arg, linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------
         // nonlinear operators where arg[1] is the only variable
         case DivpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1)
         linear[0] = false;
         var_op::one_var_for_hes(
               np1, num_var, i_var, size_t(arg[1]), linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------

         case EndOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;

         // -------------------------------------------------
         // logical comparison operators
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
         case NepvOp:
         case NeppOp:
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
         var_op::two_var_for_hes(
            np1, num_var, i_var, arg, linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 3)
         linear[0] = false;
         var_op::one_var_for_hes(
               np1, num_var, i_var, size_t(arg[1]), linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_NARG_NRES(op, 2, 1)
         linear[0] = false;
         var_op::one_var_for_hes(
               np1, num_var, i_var, size_t(arg[0]), linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------

         case PowvvOp:
         linear[0] = false;
         linear[1] = false;
         linear[2] = false;
         CPPAD_ASSERT_NARG_NRES(op, 2, 3)
         var_op::two_var_for_hes(
            np1, num_var, i_var, arg, linear, for_hes_sparse
         );
         break;
         // -------------------------------------------------

         case AFunOp:
         var_op::atomic_for_hes<SetVector, Base, RecBase>(
            itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            np1,
            rev_jac_sparse,
            for_hes_sparse
         );
         break;

         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         CPPAD_ASSERT_UNKNOWN( false );
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_FOR_HES_TRACE
      if( op != AFunOp )
      {  //
         printOp<Base, RecBase>(
            std::cout,
            play,
            itr.op_index(),
            i_var,
            op,
            arg
         );
         //
         if( NumRes(op) > 0 )
         {  typedef typename SetVector::const_iterator itr_sparse_t;
            CPPAD_ASSERT_UNKNOWN( np1 == for_hes_sparse.end() );
            CppAD::vectorBool jac_row(np1);
            for(size_t j = 0; j < np1; ++j)
               jac_row[j] = false;
            itr_sparse_t itr_jac(for_hes_sparse, np1 + i_var);
            {  size_t j = *itr_jac;
               while( j < np1 )
               {  jac_row[j] = true;
                  j = *(++itr_jac);
               }
            }
            printOpResult(
               std::cout,
               1,
               &jac_row,
               0,
               (CppAD::vectorBool *) nullptr
            );
            std::cout << std::endl;
            //
            CppAD::vector< CppAD::vectorBool > hes(np1);
            for(size_t i = 0; i < np1; ++i)
            {  hes[i].resize(np1);
               for(size_t j = 0; j < np1; ++j)
                  hes[i][j] = false;
               itr_sparse_t itr_hes(for_hes_sparse, i);
               size_t j = *itr_hes;
               while( j < np1 )
               {  hes[i][j] = true;
                  j = *(++itr_hes);
               }
            }
            printOpResult(
               std::cout,
               np1,
               hes.data(),
               0,
               (CppAD::vectorBool *) nullptr
            );
            std::cout << std::endl;
         }
      }
   }
   std::cout << std::endl;
# else
   }
# endif

   return;
}
} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

// preprocessor symbols that are local to this file
# undef CPPAD_FOR_HES_TRACE

# endif
