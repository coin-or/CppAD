# ifndef CPPAD_LOCAL_SWEEP_REVERSE_HPP
# define CPPAD_LOCAL_SWEEP_REVERSE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


# include <cppad/local/play/atom_op_info.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/reverse.hpp
Compute derivatives of arbitrary order Taylor coefficients.
*/

/*!
\def CPPAD_REVERSE_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every reverse_sweep computation is printed.
*/
# define CPPAD_REVERSE_TRACE 0

/*!
Compute derivative of arbitrary order forward mode Taylor coefficients.

\tparam Base
this operation sequence was recorded using AD<Base>
and computations by this routine are done using type Base.

\param n
is the number of independent variables on the tape.

\param num_var
is the total number of variables on the tape.
This is also equal to the number of rows in the matrix Taylor; i.e.,
play->num_var_rec().

\param play
The information stored in play
is a recording of the operations corresponding to the function
\f[
   F : {\bf R}^n \rightarrow {\bf R}^m
\f]
where \f$ n \f$ is the number of independent variables and
\f$ m \f$ is the number of dependent variables.
We define \f$ u^{(k)} \f$ as the value of <code>x_k</code> in the previous call
of the form
<code>
   f.Forward(k, x_k)
</code>
We define
\f$ X : {\bf R}^{n \times d} \rightarrow {\bf R}^n \f$ by
\f[
   X(t, u) =  u^{(0)} + u^{(1)} t + \cdots + u^{(d)} t^d
\f]
We define
\f$ Y : {\bf R}^{n \times d} \rightarrow {\bf R}^m \f$ by
\f[
   Y(t, u) =  F[ X(t, u) ]
\f]
We define the function
\f$ W : {\bf R}^{n \times d} \rightarrow {\bf R} \f$ by
\f[
W(u)
=
\sum_{k=0}^{d} ( w^{(k)} )^{\rm T}
   \frac{1}{k !} \frac{\partial^k}{\partial t^k} Y(0, u)
\f]
(The matrix \f$ w \in {\bf R}^m \f$,
is defined below under the heading Partial.)
Note that the scale factor  1 / k  converts
the k-th partial derivative to the k-th order Taylor coefficient.
This routine computes the derivative of \f$ W(u) \f$
with respect to all the Taylor coefficients
\f$ u^{(k)} \f$ for \f$ k = 0 , ... , d \f$.

\param J
Is the number of columns in the coefficient matrix Taylor.
This must be greater than or equal d + 1.

\param Taylor
For i = 1 , ... , num_var, and for k = 0 , ... , d,
 Taylor [ i * J + k ]
is the k-th order Taylor coefficient corresponding to
variable with index i on the tape.
The value \f$ u \in {\bf R}^{n \times d} \f$,
at which the derivative is computed,
is defined by
\f$ u_j^{(k)} \f$ = Taylor [ j * J + k ]
for j = 1 , ... , n, and for k = 0 , ... , d.

\param K
Is the number of columns in the partial derivative matrix Partial.
It must be greater than or equal d + 1.

\param Partial
\b Input:
The last \f$ m \f$ rows of Partial are inputs.
The matrix \f$ w \f$, used to define \f$ W(u) \f$,
is specified by these rows.
For i = 0 , ... , m - 1,
for k = 0 , ... , d,
<code>Partial [ (num_var - m + i ) * K + k ] = w[i,k]</code>.
\n
\n
\b Temporary:
For i = n+1 , ... , num_var - 1 and for k = 0 , ... , d,
the value of Partial [ i * K + k ] is used for temporary work space
and its output value is not defined.
\n
\n
\b Output:
For j = 1 , ... , n and for k = 0 , ... , d,
 Partial [ j * K + k ]
is the partial derivative of \f$ W( u ) \f$ with
respect to \f$ u_j^{(k)} \f$.

\param cskip_op
Is a vector with size play->num_op_rec().
If cskip_op[i] is true, the operator index i in the recording
does not affect any of the dependent variable (given the value
of the independent variables).
Note that all the operators in an atomic function call are skipped as a block,
so only the last AFunOp fore each call needs to have cskip_op[i] true.

\param load_op2var
is a vector with size play->num_var_load_rec().
It contains the variable index corresponding to each load instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\tparam Iterator
This is either player::const_iteratoror player::const_subgraph_iterator.

\param play_itr
On input this is either play->end(), for the entire graph,
or play->end(subgraph), for a subgraph.
This routine mode will use --play_itr to iterate over the graph or subgraph.
It is assumes that the iterator starts just past the EndOp and it will
continue until it reaches the BeginOp.
If i_var is a variable index, and the corresponding operator
is not in the subgraph,
then the partials with respect to i_var are not modified and need to be
initialized as zero. Note that this means the partial for the independent
varaibles, that are not in the subgraph are not calculated.
If part of an atomic function call is in the subgraph,
the entire atomic function call must be in the subgraph.

\param not_used_rec_base
Specifies RecBase for this call.

\par Assumptions
The first operator on the tape is a BeginOp,
and the next n operators are InvOp operations for the
corresponding independent variables; see play->check_inv_op(n_ind).
*/
template <class Base, class Iterator, class RecBase>
void reverse(
   size_t                      num_var,
   const local::player<Base>*  play,
   size_t                      cap_order,
   const Base*                 Taylor,
   size_t                      K,
   Base*                       Partial,
   bool*                       cskip_op,
   const pod_vector<addr_t>&   load_op2var,
   Iterator&                   play_itr,
   const RecBase&              not_used_rec_base
)
{
   // check num_var argument
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec() == num_var );
   CPPAD_ASSERT_UNKNOWN( num_var > 0 );

   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // pointer to the beginning of the parameter vector
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();

   // work space used by atomic funcions
   var_op::atomic_op_work<Base> atom_work;

   //
   // information defined by atomic forward
   size_t atom_index=0, atom_old=0, atom_m=0, atom_n=0;

   // A vector with unspecified contents declared here so that operator
   // routines do not need to re-allocate it
   vector<Base> work;

   // Initialize
# if CPPAD_REVERSE_TRACE
   std::cout << std::endl;
   bool atom_trace = true;
# else
   bool atom_trace = false;
# endif
   op_code_var   op;
   const addr_t* arg;
   size_t        i_var;
   play_itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == EndOp );
   while(op != BeginOp )
   {  //
      // next op
      (--play_itr).op_info(op, arg, i_var);

      // check if we are skipping this operation
      size_t i_op = play_itr.op_index();
      while( cskip_op[i_op] )
      {  switch(op)
         {
            case AFunOp:
            {  // get information for this atomic function call
               play::atom_op_info<Base>(
                  op, arg, atom_index, atom_old, atom_m, atom_n
               );
               //
               // skip to the first AFunOp
               for(size_t i = 0; i < atom_m + atom_n + 1; ++i)
                  --play_itr;
               play_itr.op_info(op, arg, i_var);
               CPPAD_ASSERT_UNKNOWN( op == AFunOp );
            }
            break;

            default:
            break;
         }
         (--play_itr).op_info(op, arg, i_var);
         i_op = play_itr.op_index();
      }
# if CPPAD_REVERSE_TRACE
      if( op != AFunOp )
      {
         size_t       i_tmp  = i_var;
         const Base*  Z_tmp  = Taylor + i_var * cap_order;
         const Base*  pZ_tmp = Partial + i_var * K;
         printOp<Base, RecBase>(
            std::cout,
            play,
            i_op,
            i_tmp,
            op,
            arg
         );
         if( NumRes(op) > 0 && op != BeginOp ) printOpResult(
            std::cout,
            K,
            Z_tmp,
            K,
            pZ_tmp
         );
         std::cout << std::endl;
      }
# endif
      switch( op )
      {
         case AbsOp:
         var_op::abs_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::acos_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::acosh_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AddvvOp:
         var_op::addvv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AddpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::addpv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::asin_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::asinh_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::atan_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::atanh_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case BeginOp:
         CPPAD_ASSERT_NARG_NRES(op, 1, 1);
         CPPAD_ASSERT_UNKNOWN( i_op == 0 );
         break;
         // --------------------------------------------------

         case CSkipOp:
         // CSkipOp has a zero order forward action.
         play_itr.correct_after_decrement(arg);
         break;
         // -------------------------------------------------

         case CSumOp:
         play_itr.correct_after_decrement(arg);
         var_op::csum_reverse(
            i_var, arg, K, Partial
         );
         // end of a cumulative summation
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_reverse(
            i_var,
            arg,
            num_par,
            parameter,
            cap_order,
            Taylor,
            K,
            Partial
         );
         break;
         // --------------------------------------------------

         case CosOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::cos_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case CoshOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::cosh_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case DisOp:
         // Derivative of discrete operation is zero so no
         // contribution passes through this operation.
         break;
         // --------------------------------------------------

         case DivvvOp:
         var_op::divvv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case DivpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::divpv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::divvp_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------
         case EndOp:
         CPPAD_ASSERT_UNKNOWN(
            i_op == play->num_op_rec() - 1
         );
         break;

         // --------------------------------------------------

         case ErfOp:
         case ErfcOp:
         var_op::erf_reverse(
            op, i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case ExpOp:
         var_op::exp_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case Expm1Op:
         var_op::expm1_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case InvOp:
         break;
         // --------------------------------------------------

         case LdpOp:
         case LdvOp:
         var_op::load_reverse(
            op, i_var, arg, load_op2var, cap_order, K, Partial
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
         break;
         // -------------------------------------------------

         case LogOp:
         var_op::log_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case Log1pOp:
         var_op::log1p_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case MulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::mulpv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case MulvvOp:
         var_op::mulvv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case NegOp:
         var_op::neg_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case ParOp:
         break;
         // --------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::powvp_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial, work
         );
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::powpv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case PowvvOp:
         var_op::powvv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case PriOp:
         // no result so nothing to do
         break;
         // --------------------------------------------------

         case SignOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::sign_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case SinOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::sin_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case SinhOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::sinh_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case SqrtOp:
         var_op::sqrt_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case StppOp:
         break;
         // --------------------------------------------------

         case StpvOp:
         break;
         // -------------------------------------------------

         case StvpOp:
         break;
         // -------------------------------------------------

         case StvvOp:
         break;
         // --------------------------------------------------

         case SubvvOp:
         var_op::subvv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case SubpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::subpv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case SubvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::subvp_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case TanOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::tan_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // -------------------------------------------------

         case TanhOp:
         CPPAD_ASSERT_UNKNOWN( i_var < num_var );
         var_op::tanh_reverse(
            i_var, arg, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case AFunOp:
         var_op::atomic_reverse<Base, RecBase, Iterator>(
            play_itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            cap_order,
            K,
            Taylor,
            Partial
         );
         break;

         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
         // ------------------------------------------------------------

         case ZmulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::zmulpv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::zmulvp_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         case ZmulvvOp:
         var_op::zmulvv_reverse(
            i_var, arg, parameter, cap_order, Taylor, K, Partial
         );
         break;
         // --------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(false);
      }
   }
# if CPPAD_REVERSE_TRACE
   std::cout << std::endl;
# endif
}

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE

// preprocessor symbols that are local to this file
# undef CPPAD_REVERSE_TRACE

# endif
