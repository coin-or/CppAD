# ifndef CPPAD_LOCAL_SWEEP_FORWARD_DIR_HPP
# define CPPAD_LOCAL_SWEEP_FORWARD_DIR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>
# include <cppad/local/var_op/atomic_op.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/forward_dir.hpp
Compute one Taylor coefficient for each direction requested.
*/

/*!
\def CPPAD_FORWARD2_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every forward2sweep computation is printed.
*/
# define CPPAD_FORWARD2_TRACE 0

/*!
Compute multiple directions forward mode Taylor coefficients.

\tparam Base
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type AD<Base>.

\param q
is the order of the Taylor coefficients
that are computed during this call;
<code>q > 0</code>.

\param r
is the number of Taylor coefficients
that are computed during this call.

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

\param taylor
\n
\b Input:
For <code>i = 1 , ... , numvar-1</code>,
<code>taylor[ (J-1)*r*i + i + 0 ]</code>
is the zero order Taylor coefficient corresponding to
the i-th variable and all directions.
For <code>i = 1 , ... , numvar-1</code>,
For <code>k = 1 , ... , q-1</code>,
<code>ell = 0 , ... , r-1</code>,
<code>taylor[ (J-1)*r*i + i + (k-1)*r + ell + 1 ]</code>
is the k-th order Taylor coefficient corresponding to
the i-th variabel and ell-th direction.
\n
\n
\b Input:
For <code>i = 1 , ... , n</code>,
<code>ell = 0 , ... , r-1</code>,
<code>taylor[ (J-1)*r*i + i + (q-1)*r + ell + 1 ]</code>
is the q-th order Taylor coefficient corresponding to
the i-th variable and ell-th direction
(these are the independent varaibles).
\n
\n
\b Output:
For <code>i = n+1 , ... , numvar-1</code>,
<code>ell = 0 , ... , r-1</code>,
<code>taylor[ (J-1)*r*i + i + (q-1)*r + ell + 1 ]</code>
is the q-th order Taylor coefficient corresponding to
the i-th variable and ell-th direction.

\param cskip_op
Is a vector with size play->num_op_rec().
If cskip_op[i] is true, the operator with index i
does not affect any of the dependent variable (given the value
of the independent variables).

\param load_op2var
is a vector with size play->num_var_load_rec().
It is the variable index corresponding to each the
load instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\param not_used_rec_base
Specifies RecBase for this call.

*/

template <class Base, class RecBase>
void forward_dir(
   const local::player<Base>*  play,
   const size_t                q,
   const size_t                r,
   const size_t                n,
   const size_t                numvar,
   const size_t                J,
   Base*                       taylor,
   const bool*                 cskip_op,
   const pod_vector<addr_t>&   load_op2var,
   const RecBase&              not_used_rec_base
)
{
   CPPAD_ASSERT_UNKNOWN( q > 0 );
   CPPAD_ASSERT_UNKNOWN( J >= q + 1 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec() == numvar );

   // only compute one order at a time when using multi-direction forward
   size_t p = q;

   // information used by atomic function operators
   const size_t order_up  = q;
   const size_t n_dir     = r;

   // work space used by atomic funcions
   var_op::atomic_op_work<Base> atom_work;

   // information defined by atomic function operators
   size_t atom_index=0, atom_id=0, atom_m=0, atom_n=0;
   //
   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // pointer to the beginning of the parameter vector
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();

   // temporary indices
   size_t i;

   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   op_code_var     op;
   size_t          i_var;
   const addr_t*   arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
# if CPPAD_FORWARD2_TRACE
   bool atom_trace  = true;
   std::cout << std::endl;
   CppAD::vector<Base> Z_vec(q+1);
# else
   bool atom_trace = false;
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
         case AbsOp:
         var_op::abs_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AddvvOp:
         var_op::addvv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case AddpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::addpv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::acos_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::acosh_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::asin_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::asinh_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::atan_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::atanh_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_forward_dir(
            q, r, i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::cos_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case CoshOp:
         // sinh(x), cosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::cosh_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case CSkipOp:
         // CSkipOp only does somthing on order zero.
         CPPAD_ASSERT_UNKNOWN( p > 0 );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         var_op::csum_forward_dir(
            q, i_var, arg, num_par, parameter, r, J, taylor
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DisOp:
         var_op::dis_forward_dir<RecBase>(p, q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case DivvvOp:
         var_op::divvv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case DivpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::divpv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::divvp_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case EndOp:
         // needed for sparse_jacobian test
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;
         // -------------------------------------------------

         case ErfOp:
         case ErfcOp:
         var_op::erf_forward_dir(op, q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ExpOp:
         var_op::exp_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case Expm1Op:
         var_op::expm1_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
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
         // ---------------------------------------------------

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
         CPPAD_ASSERT_UNKNOWN(q > 0 );
         break;
         // -------------------------------------------------

         case LogOp:
         var_op::log_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case Log1pOp:
         var_op::log1p_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // ---------------------------------------------------

         case MulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::mulpv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case MulvvOp:
         var_op::mulvv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case NegOp:
         var_op::neg_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case ParOp:
         var_op::par_forward_dir(
            q, r, i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::powpv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::powvp_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowvvOp:
         var_op::powvv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PriOp:
         CPPAD_ASSERT_UNKNOWN(q > 0);
         break;
         // -------------------------------------------------

         case SignOp:
         // sign(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sign_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sin_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::sinh_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case SqrtOp:
         var_op::sqrt_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         CPPAD_ASSERT_UNKNOWN(q > 0 );
         break;
         // -------------------------------------------------

         case SubvvOp:
         var_op::subvv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case SubpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::subpv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case SubvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::subvp_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::tan_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         var_op::tanh_forward_dir(q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case AFunOp:
         var_op::atomic_forward_dir<Base, RecBase>(
            itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            J,
            order_up,
            n_dir,
            taylor
         );
         break;

         case FunapOp:
         case FunavOp:
         case FunrpOp:
         case FunrvOp:
         break;
         // -------------------------------------------------

         case ZmulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::zmulpv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::zmulvp_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         var_op::zmulvv_forward_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_FORWARD2_TRACE
      if( op != AFunOp )
      {  printOp<Base, RecBase>(
            std::cout,
            play,
            itr.op_index(),
            i_var,
            op,
            arg
         );
         Base* Z_tmp = nullptr;
         if( op == FunavOp )
            Z_tmp = taylor + size_t(arg[0])*((J-1) * r + 1);
         else if( NumRes(op) > 0 )
            Z_tmp = taylor + i_var*((J-1)*r + 1);
         if( Z_tmp != nullptr )
         {  Z_vec[0]    = Z_tmp[0];
            for(ell = 0; ell < r; ell++)
            {  std::cout << std::endl << "     ";
               for(size_t p_tmp = 1; p_tmp <= q; p_tmp++)
                  Z_vec[p_tmp] = Z_tmp[ (p_tmp-1)*r + ell + 1];
               printOpResult(
                  std::cout,
                  q + 1,
                  Z_vec.data(),
                  0,
                  (Base *) nullptr
               );
            }
         }
         std::cout << std::endl;
      }
   }
   std::cout << std::endl;
# else
   }
# endif

   return;
}

// preprocessor symbols that are local to this file
# undef CPPAD_FORWARD2_TRACE

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE
# endif
