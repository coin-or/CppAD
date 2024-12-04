# ifndef CPPAD_LOCAL_SWEEP_FORWARD0_HPP
# define CPPAD_LOCAL_SWEEP_FORWARD0_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>
# include <cppad/local/var_op/compare_op.hpp>
# include <cppad/local/var_op/atomic_op.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/forward0.hpp
Compute zero order forward mode Taylor coefficients.
*/

/*
 ------------------------------------------------------------------------------
{xrst_begin sweep_forward0 dev}
{xrst_spell
   cskip
   numvar
   pri
}
Compute Zero Order Forward Mode Taylor Coefficients
###################################################

Syntax
******
| ``forward0`` (
| |tab| *play* ,
| |tab| *s_out* ,
| |tab| *print* ,
| |tab| *n* ,
| |tab| *numvar* ,
| |tab| *J* ,
| |tab| *taylor* ,
| |tab| *cskip_op* ,
| |tab| *load_op2var* ,
| |tab| *compare_change_count* ,
| |tab| *compare_change_number* ,
| |tab| *compare_change_op_index* ,
| |tab| *not_used_rec_base*
| )

CPPAD_FORWARD0_TRACE
********************
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every zero order forward mode computation is printed.
{xrst_spell_off}
{xrst_code hpp} */
# define CPPAD_FORWARD0_TRACE 0
/* {xrst_code}
{xrst_spell_on}

Base
****
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type ``AD`` < *Base* > .

s_out
*****
Is the stream where output corresponding to PriOp operations will
be written.

print
*****
If print is false,
suppress the output that is otherwise generated by the PriOp instructions.

n
*
is the number of independent variables on the tape.

numvar
******
is the total number of variables on the tape.
This is also equal to the number of rows in the matrix taylor; i.e.,
*play* ``->num_var_rec`` () .

play
****
The information stored in play
is a recording of the operations corresponding to a function

.. math::

   f : \B{R}^n \rightarrow \B{R}^m

where *n* is the number of independent variables and
*m* is the number of dependent variables.

J
*
Is the number of columns in the coefficient matrix taylor.
This must be greater than or equal one.

taylor
******
Is the matrix of Taylor coefficients.

Input
=====
For *i* = 1 , ... , *n* ,
*taylor* [% *i* % * % *J* % + 0]%
is the zero order Taylor coefficient for variable with index
*j* on the tape (these are the independent variables).

Output
======
For *i* = *n* +1 , ... , *numvar* ``-1`` ,
*taylor* [% *i* % * % *J* % + 0]%
is the zero order Taylor coefficient for the variable with
index i on the tape.

cskip_op
********
Is a vector with size *play* ``->num_op_rec`` () .
The input value of the elements does not matter.
Upon return, if *cskip_op* [ *i* ] is true,
the operator index *i* does not affect any of the dependent variable
(given the value of the independent variables).

load_op2var
***********
Is a vector with size *play* ``->num_var_load_rec`` () .
The input value of the elements does not matter.
Upon return, *load_op2var* [ *i* ]
is the variable index corresponding to the *i*-th variable VecAD load operator.
Note that even though the VecAD vector is a variable, the load
can correspond to an element that is a parameter in which case
*load_op2var* [ *i* ] is zero.

compare_change_count
********************
Is the compare change count value at which *compare_change_op_index*
is returned. If it is zero, the comparison changes are not counted.

compare_change_number
*********************
If *compare_change_count* is zero, this value is set to zero.
Otherwise, the return value is the number of comparison operations
that have a different result from when the information in
*play* was recorded.

compare_change_op_index
***********************
If *compare_change_count* is zero, this value is set to zero.
Otherwise it is the operator index (see forward_next) for the
comparison operation that has a different result from when the information in
play was recorded.
This is not the first comparison that is different,
but rather the *compare_change_count* comparison.

not_used_rec_base
*****************
Specifies *RecBase* for this call.

{xrst_end sweep_forward0}
*/

template <class Base, class RecBase>
void forward0(
   const local::player<Base>* play,
   std::ostream&              s_out,
   bool                       print,
   size_t                     n,
   size_t                     numvar,
   size_t                     J,
   Base*                      taylor,
   bool*                      cskip_op,
   pod_vector<addr_t>&        load_op2var,
   size_t                     compare_change_count,
   size_t&                    compare_change_number,
   size_t&                    compare_change_op_index,
   const RecBase&             not_used_rec_base
)
{  CPPAD_ASSERT_UNKNOWN( J >= 1 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec() == numvar );

   // use p, q, r so other forward sweeps can use code defined here
   size_t p = 0;
   size_t q = 0;
   size_t r = 1;

   // initialize the comparison operator counter
   if( p == 0 )
   {  compare_change_number   = 0;
      compare_change_op_index = 0;
   }

   // If this includes a zero calculation, initialize this information
   pod_vector<bool>   vec_ad2isvar;
   pod_vector<size_t> vec_ad2index;
   if( p == 0 )
   {  size_t i;

      // this includes order zero calculation, initialize vector indices
      size_t num = play->num_var_vecad_ind_rec();
      if( num > 0 )
      {  vec_ad2isvar.extend(num);
         vec_ad2index.extend(num);
         for(i = 0; i < num; i++)
         {  vec_ad2index[i] = play->GetVecInd(i);
            vec_ad2isvar[i] = false;
         }
      }
      // includes zero order, so initialize conditional skip flags
      num = play->num_op_rec();
      for(i = 0; i < num; i++)
         cskip_op[i] = false;
   }

   // information used by atomic function operators
   const size_t order_low = p;
   const size_t order_up  = q;

   // work space used by atomic funcions
   var_op::atomic_op_work<Base> atom_work;

   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // pointer to the beginning of the parameter vector
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();

   // length of the text vector (used by CppAD assert macros)
   const size_t num_text = play->num_text_rec();

   // pointer to the beginning of the text vector
   const char* text = nullptr;
   if( num_text > 0 )
      text = play->GetTxt(0);

# if CPPAD_FORWARD0_TRACE
   // flag as to when to trace atomic function values
   bool atom_trace  = true;
# else
   bool atom_trace  = false;
# endif

   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   op_code_var   op;
   size_t        i_var;
   const addr_t* arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
   //
# if CPPAD_FORWARD0_TRACE
   std::cout << std::endl;
# endif
   bool more_operators = true;
   while(more_operators)
   {
      // next op
      (++itr).op_info(op, arg, i_var);
      CPPAD_ASSERT_UNKNOWN( itr.op_index() < play->num_op_rec() );

      // check if we are skipping this operation
      while( cskip_op[itr.op_index()] )
      {  switch(op)
         {
            case AFunOp:
            {  // get information for this atomic function call
               size_t atom_index, atom_id, atom_m, atom_n;
               play::atom_op_info<Base>(
                  op, arg, atom_index, atom_id, atom_m, atom_n
               );
               //
               // skip to the second AFunOp
               for(size_t i = 0; i < atom_m + atom_n + 1; ++i)
                  ++itr;
# ifndef NDEBUG
               itr.op_info(op, arg, i_var);
               CPPAD_ASSERT_UNKNOWN( op == AFunOp );
# endif
            }
            break;

            case CSkipOp:
            case CSumOp:
            itr.correct_before_increment();
            break;

            default:
            break;
         }
         (++itr).op_info(op, arg, i_var);
      }

      // action to take depends on the case
      switch( op )
      {
         case EqppOp:
         case EqpvOp:
         case EqvvOp:
         case LeppOp:
         case LepvOp:
         case LevpOp:
         case LevvOp:
         case LtppOp:
         case LtpvOp:
         case LtvpOp:
         case LtvvOp:
         case NeppOp:
         case NepvOp:
         case NevvOp:
         var_op::compare_forward_op(op,
            arg, parameter, J, taylor, itr.op_index(), compare_change_count,
            compare_change_number, compare_change_op_index
         );
         break;
         // -------------------------------------------------

         case AbsOp:
         var_op::abs_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AddvvOp:
         var_op::addvv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case AddpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::addpv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::acos_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::acosh_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::asin_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::asinh_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::atan_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::atanh_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case CExpOp:
         // Use the general case with d == 0
         // (could create an optimzied version for this case)
         var_op::cexp_forward_0(
            i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::cos_forward_0(i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case CoshOp:
         // sinh(x), cosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::cosh_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case CSkipOp:
         var_op::cskip_forward_0(
            i_var, arg, num_par, parameter, J, taylor, cskip_op
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         var_op::csum_forward_op(
            0, 0, i_var, arg, num_par, parameter, J, taylor
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DisOp:
         var_op::dis_forward_dir<RecBase>(p, q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case DivvvOp:
         var_op::divvv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case DivpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::divpv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::divvp_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case EndOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;
         // -------------------------------------------------

         case ErfOp:
         case ErfcOp:
         var_op::erf_forward_0(op, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ExpOp:
         var_op::exp_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case Expm1Op:
         var_op::expm1_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // ---------------------------------------------------

         case LdpOp:
         case LdvOp:
         var_op::load_forward_0(
            op,
            i_var,
            play->num_var_vecad_ind_rec(),
            arg,
            numvar,
            num_par,
            parameter,
            J,
            taylor,
            vec_ad2isvar,
            vec_ad2index,
            load_op2var
         );
         break;
         // -------------------------------------------------

         case LogOp:
         var_op::log_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case Log1pOp:
         var_op::log1p_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case MulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::mulpv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case MulvvOp:
         var_op::mulvv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case NegOp:
         var_op::neg_forward_0(i_var, arg, J, taylor);
         break;

         // -------------------------------------------------

         case ParOp:
         var_op::par_forward_0(
            i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::powvp_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::powpv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowvvOp:
         var_op::powvv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PriOp:
         if( print ) var_op::pri_forward_0(s_out,
            arg, num_text, text, num_par, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case SignOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sign_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sin_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sinh_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SqrtOp:
         var_op::sqrt_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         var_op::store_forward_0(
            op,
            arg,
            numvar,
            num_par,
            parameter,
            J,
            taylor,
            vec_ad2isvar,
            vec_ad2index
         );
         break;
         // -------------------------------------------------

         case SubvvOp:
         var_op::subvv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case SubpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::subpv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case SubvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::subvp_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::tan_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::tanh_forward_0(i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AFunOp:
         // start of an atomic function call
         var_op::atomic_forward_op<Base, RecBase>(
            itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            J,
            order_low,
            order_up,
            taylor
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
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::zmulpv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::zmulvp_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         var_op::zmulvv_forward_0(i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(false);
      }
# if CPPAD_FORWARD0_TRACE
      size_t  d  = 0;
      Base*           Z_tmp   = taylor + i_var * J;
      if( op != AFunOp )
      {
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
            d + 1,
            Z_tmp,
            0,
            (Base *) nullptr
         );
         std::cout << std::endl;
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
# undef CPPAD_FORWARD0_TRACE

# endif
