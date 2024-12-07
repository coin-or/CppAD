# ifndef CPPAD_LOCAL_SWEEP_FORWARD1_HPP
# define CPPAD_LOCAL_SWEEP_FORWARD1_HPP
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
\file sweep/forward1.hpp
Compute one Taylor coefficient for each order requested.
*/

/*!
\def CPPAD_FORWARD1_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every forward1sweep computation is printed.
*/
# define CPPAD_FORWARD1_TRACE 0

/*!
Compute arbitrary order forward mode Taylor coefficients.

<!-- replace forward0_doc_define -->
\tparam Base
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type AD<Base>.

\param s_out
Is the stream where output corresponding to PriOp operations will
be written.

\param print
If print is false,
suppress the output that is otherwise generated by the c PriOp instructions.

\param n
is the number of independent variables on the tape.

\param numvar
is the total number of variables on the tape.
This is also equal to the number of rows in the matrix taylor; i.e.,
play->num_var_rec().

\param play
The information stored in play
is a recording of the operations corresponding to the function
\f[
   F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables and
\f$ m \f$ is the number of dependent variables.

\param J
Is the number of columns in the coefficient matrix taylor.
This must be greater than or equal one.

<!-- end forward0_doc_define -->

\param cskip_op
Is a vector with size play->num_op_rec().
\n
\n
<tt>p = 0</tt>
\n
In this case,
the input value of the elements does not matter.
Upon return, if cskip_op[i] is true, the operator with index i
does not affect any of the dependent variable
(given the value of the independent variables).
\n
\n
<tt>p > 0</tt>
\n
In this case cskip_op is not modified and has the same meaning
as its return value above.

\param load_op2var
is a vector with size play->num_var_load_rec().
\n
\n
<tt>p == 0</tt>
\n
In this case,
The input value of the elements does not matter.
Upon return,
it is the variable index corresponding the result for each load operator.
In the case where the index is zero,
the load operator results in a parameter (not a variable).
Note that the is no variable with index zero on the tape.
\n
\n
<tt>p > 0</tt>
\n
In this case load_op2var is not modified and has the meaning
as its return value above.

\param p
is the lowest order of the Taylor coefficients
that are computed during this call.

\param q
is the highest order of the Taylor coefficients
that are computed during this call.

\param taylor
\n
\b Input:
For <code>i = 1 , ... , numvar-1</code>,
<code>k = 0 , ... , p-1</code>,
<code>taylor[ J*i + k]</code>
is the k-th order Taylor coefficient corresponding to
the i-th variable.
\n
\n
\b Input:
For <code>i = 1 , ... , n</code>,
<code>k = p , ... , q</code>,
<code>taylor[ J*j + k]</code>
is the k-th order Taylor coefficient corresponding to
the i-th variable
(these are the independent varaibles).
\n
\n
\b Output:
For <code>i = n+1 , ... , numvar-1</code>, and
<code>k = 0 , ... , p-1</code>,
<code>taylor[ J*i + k]</code>
is the k-th order Taylor coefficient corresponding to
the i-th variable.


\param compare_change_count
Is the count value for changing number and op_index during
zero order forward mode.

\param compare_change_number
If p is non-zero, this value is not changed, otherwise:
If compare_change_count is zero, this value is set to zero, otherwise:
this value is set to the number of comparison operations
that have a different result from when the information in
play was recorded.

\param compare_change_op_index
if p is non-zero, this value is not changed, otherwise:
If compare_change_count is zero, this value is set to zero.
Otherwise it is the operator index (see forward_next) for the count-th
comparison operation that has a different result from when the information in
play was recorded.

\param not_used_rec_base
Specifies RecBase for this call.
*/

template <class Base, class RecBase>
void forward1(
   const local::player<Base>* play,
   std::ostream&              s_out,
   const bool                 print,
   const size_t               p,
   const size_t               q,
   const size_t               n,
   const size_t               numvar,
   const size_t               J,
   Base*                      taylor,
   bool*                      cskip_op,
   pod_vector<addr_t>&        load_op2var,
   size_t                     compare_change_count,
   size_t&                    compare_change_number,
   size_t&                    compare_change_op_index,
   const RecBase&             not_used_rec_base
)
{
   // number of directions
   const size_t r = 1;

   CPPAD_ASSERT_UNKNOWN( p <= q );
   CPPAD_ASSERT_UNKNOWN( J >= q + 1 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec() == numvar );

   /*
   <!-- replace forward0sweep_code_define -->
   */

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

   // information defined by atomic function operators
   size_t atom_index, atom_id, atom_m, atom_n;

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
   /*
   <!-- end forward0sweep_code_define -->
   */
   // temporary indices
   size_t i;

   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   op_code_var   op;
   size_t        i_var;
   const addr_t* arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
   //
# if CPPAD_FORWARD1_TRACE
   // flag as to when to trace atomic function values
   bool atom_trace  = true;
# else
   bool atom_trace  = false;
# endif
   //
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
               play::atom_op_info<Base>(
                  op, arg, atom_index, atom_id, atom_m, atom_n
               );
               //
               // skip to the second AFunOp
               for(i = 0; i < atom_m + atom_n + 1; ++i)
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

      // action depends on the operator
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
         var_op::abs_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AddvvOp:
         var_op::addvv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case AddpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::addpv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::acos_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::acosh_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::asin_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::asinh_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::atan_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::atanh_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_forward_op(
            p, q, i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::cos_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case CoshOp:
         // sinh(x), cosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::cosh_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case CSkipOp:
         if( p == 0 )
         {  var_op::cskip_forward_0(
               i_var, arg, num_par, parameter, J, taylor, cskip_op
            );
         }
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         var_op::csum_forward_op(
            p, q, i_var, arg, num_par, parameter, J, taylor
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DisOp:
         var_op::dis_forward_dir<RecBase>(p, q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case DivvvOp:
         var_op::divvv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case DivpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::divpv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::divvp_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case EndOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;
         // -------------------------------------------------

         case ErfOp:
         case ErfcOp:
         var_op::erf_forward_op(op, p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ExpOp:
         var_op::exp_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case Expm1Op:
         var_op::expm1_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         if( p == 0 )
         {  var_op::load_forward_0(
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
            if( p < q ) var_op::load_forward_nonzero(
               op,
               i_var,
               arg,
               p+1,
               q,
               r,
               J,
               load_op2var,
               taylor
            );
         }
         else
            var_op::load_forward_nonzero(
            op,
            i_var,
            arg,
            p,
            q,
            r,
            J,
            load_op2var,
            taylor
         );
         break;
         // -------------------------------------------------

         case LogOp:
         var_op::log_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case Log1pOp:
         var_op::log1p_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case MulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::mulpv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case MulvvOp:
         var_op::mulvv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // --------------------------------------------------

         case NegOp:
         var_op::neg_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case ParOp:
         var_op::par_forward_op(
            p, q, i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::powvp_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::powpv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowvvOp:
         var_op::powvv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PriOp:
         if( (p == 0) & print ) var_op::pri_forward_0(s_out,
            arg, num_text, text, num_par, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case SignOp:
         // sign(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sign_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sin_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sinh_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SqrtOp:
         var_op::sqrt_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         if( p == 0 )
         {  var_op::store_forward_0(
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
         }
         break;
         // -------------------------------------------------

         case SubvvOp:
         var_op::subvv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case SubpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::subpv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case SubvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::subvp_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::tan_forward_op(p, q, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::tanh_forward_op(p, q, i_var, arg, J, taylor);
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
         var_op::zmulpv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::zmulvp_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         var_op::zmulvv_forward_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(false);
      }
# if CPPAD_FORWARD1_TRACE
      Base*           Z_tmp   = taylor + J * i_var;
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
            q + 1,
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
   if( (p == 0) && (compare_change_count == 0) )
      compare_change_number = 0;
   return;
}

// preprocessor symbols that are local to this file
# undef CPPAD_FORWARD1_TRACE

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE
# endif
