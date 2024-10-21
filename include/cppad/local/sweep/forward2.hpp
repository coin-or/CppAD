# ifndef CPPAD_LOCAL_SWEEP_FORWARD2_HPP
# define CPPAD_LOCAL_SWEEP_FORWARD2_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>

# include <cppad/local/op_class/enum2instance.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/forward2.hpp
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

template <class Addr, class Base, class RecBase>
void forward2(
   const local::player<Base>*  play,
   const size_t                q,
   const size_t                r,
   const size_t                n,
   const size_t                numvar,
   const size_t                J,
   Base*                       taylor,
   const bool*                 cskip_op,
   const pod_vector<Addr>&     load_op2var,
   const RecBase&              not_used_rec_base
)
{
   CPPAD_ASSERT_UNKNOWN( q > 0 );
   CPPAD_ASSERT_UNKNOWN( J >= q + 1 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec() == numvar );

   // only compute one order at a time when using multi-direction forward
   size_t p = q;

   // information used by atomic function operators
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
   const size_t need_y    = size_t( variable_enum );
   const size_t order_low = p;
   const size_t order_up  = q;

   // vectors used by atomic function operators
   vector<Base> atom_par_x;          // argument parameter values
   vector<ad_type_enum> atom_type_x; // argument type
   vector<Base> atom_tx_one;         // argument vector Taylor coefficients
   vector<Base> atom_tx_all;
   vector<Base> atom_ty_one;         // result vector Taylor coefficients
   vector<Base> atom_ty_all;
   //
   // information defined by atomic function operators
   size_t atom_index=0, atom_id=0, atom_m=0, atom_n=0, atom_i=0, atom_j=0;
   enum_atom_state atom_state = start_atom; // proper initialization
   //
   // length of the parameter vector (used by CppAD assert macros)
   const size_t num_par = play->num_par_rec();

   // pointer to the beginning of the parameter vector
   CPPAD_ASSERT_UNKNOWN( num_par > 0 )
   const Base* parameter = play->GetPar();

   // temporary indices
   size_t i, j, k, ell;

   // number of orders for this atomic calculation
   // (not needed for order zero)
   const size_t atom_q1 = q+1;

   // variable indices for results vector
   vector<size_t> atom_iy;

   // select_y for an atomic function call
   vector<bool> atom_sy;

   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   OpCode op;
   size_t i_var;
   const Addr*   arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
# if CPPAD_FORWARD2_TRACE
   bool atom_trace  = false;
   std::cout << std::endl;
   CppAD::vector<Base> Z_vec(q+1);
# endif
   bool flag; // a temporary flag to use in switch cases
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
               CPPAD_ASSERT_UNKNOWN( atom_state == start_atom );
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
         case CoshOp:
         case DivpvOp:
         case DivvpOp:
         case DivvvOp:
         case Expm1Op:
         case MulpvOp:
         case MulvvOp:
         case PowvpOp:
         case SubpvOp:
         case SubvpOp:
         case SubvvOp:
         case ZmulpvOp:
         case ZmulvvOp:
         // END_SORT_THIS_LINE_MINUS_1
         op_class::enum2instance<Base>(op)->forward_dir(
            q, r, i_var, arg, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case CExpOp:
         forward_cond_op_dir(
            q, r, i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_cos_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // ---------------------------------------------------

         case CSkipOp:
         // CSkipOp only does somthing on order zero.
         CPPAD_ASSERT_UNKNOWN( p > 0 );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         forward_csum_op_dir(
            q, r, i_var, arg, num_par, parameter, J, taylor
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DisOp:
         forward_dis_op<RecBase>(p, q, r, i_var, arg, J, taylor);
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
         forward_erf_op_dir(op, q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ExpOp:
         forward_exp_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         forward_load_op(
            play,
            op,
            p,
            q,
            r,
            J,
            i_var,
            arg,
            load_op2var.data(),
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
         forward_log_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // ---------------------------------------------------

         case Log1pOp:
         forward_log1p_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // ---------------------------------------------------

         case NegOp:
         forward_neg_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case ParOp:
         k = i_var*(J-1)*r + i_var + (q-1)*r + 1;
         for(ell = 0; ell < r; ell++)
            taylor[k + ell] = Base(0.0);
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         forward_powpv_op_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowvvOp:
         forward_powvv_op_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PriOp:
         CPPAD_ASSERT_UNKNOWN(q > 0);
         break;
         // -------------------------------------------------

         case SignOp:
         // sign(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_sign_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_sin_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_sinh_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case SqrtOp:
         forward_sqrt_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         CPPAD_ASSERT_UNKNOWN(q > 0 );
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_tan_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_tanh_op_dir(q, r, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case AFunOp:
         // start or end an atomic function call
         flag = atom_state == start_atom;
         play::atom_op_info<RecBase>(
            op, arg, atom_index, atom_id, atom_m, atom_n
         );
         if( flag )
         {  atom_state = arg_atom;
            atom_i     = 0;
            atom_j     = 0;
            //
            atom_par_x.resize(atom_n);
            atom_type_x.resize(atom_n);
            //
            atom_tx_one.resize(atom_n * atom_q1);
            atom_tx_all.resize(atom_n * (q * r + 1));
            //
            atom_ty_one.resize(atom_m * atom_q1);
            atom_ty_all.resize(atom_m * (q * r + 1));
            //
            atom_iy.resize(atom_m);
            atom_sy.resize(atom_m);
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( atom_i == atom_m );
            CPPAD_ASSERT_UNKNOWN( atom_j == atom_n );
            atom_state = start_atom;
            //
            // call atomic function for this operation
            for(ell = 0; ell < r; ell++)
            {  // set atom_tx
               for(j = 0; j < atom_n; j++)
               {  size_t j_all     = j * (q * r + 1);
                  size_t j_one     = j * atom_q1;
                  atom_tx_one[j_one+0] = atom_tx_all[j_all+0];
                  for(k = 1; k < atom_q1; k++)
                  {  size_t k_all       = j_all + (k-1)*r+1+ell;
                     size_t k_one       = j_one + k;
                     atom_tx_one[k_one] = atom_tx_all[k_all];
                  }
               }
               // set atom_ty
               for(i = 0; i < atom_m; i++)
               {  size_t i_all     = i * (q * r + 1);
                  size_t i_one     = i * atom_q1;
                  atom_ty_one[i_one+0] = atom_ty_all[i_all+0];
                  for(k = 1; k < q; k++)
                  {  size_t k_all       = i_all + (k-1)*r+1+ell;
                     size_t k_one       = i_one + k;
                     atom_ty_one[k_one] = atom_ty_all[k_all];
                  }
               }
               // set atom_sy
               for(i = 0; i < atom_m; ++i)
                  atom_sy[i] = atom_iy[i] != 0;
               call_atomic_forward<Base,RecBase>(
                  atom_par_x,
                  atom_type_x,
                  need_y,
                  atom_sy,
                  order_low,
                  order_up,
                  atom_index,
                  atom_id,
                  atom_tx_one,
                  atom_ty_one
               );
               for(i = 0; i < atom_m; i++)
               {  if( atom_iy[i] > 0 )
                  {  size_t i_taylor = atom_iy[i]*((J-1)*r+1);
                     size_t q_taylor = i_taylor + (q-1)*r+1+ell;
                     size_t q_one    = i * atom_q1 + q;
                     taylor[q_taylor] = atom_ty_one[q_one];
                  }
               }
            }
# if CPPAD_FORWARD2_TRACE
            atom_trace = true;
# endif
         }
         break;

         case FunapOp:
         // parameter argument for an atomic function
         CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
         CPPAD_ASSERT_UNKNOWN( atom_state == arg_atom );
         CPPAD_ASSERT_UNKNOWN( atom_i == 0 );
         CPPAD_ASSERT_UNKNOWN( atom_j < atom_n );
         CPPAD_ASSERT_UNKNOWN( size_t( arg[0] ) < num_par );
         //
         if( dyn_par_is[ arg[0] ] )
            atom_type_x[atom_j] = dynamic_enum;
         else
            atom_type_x[atom_j] = constant_enum;
         atom_par_x[atom_j]      = parameter[ arg[0] ];
         atom_tx_all[atom_j*(q*r+1) + 0] = parameter[ arg[0]];
         for(ell = 0; ell < r; ell++)
            for(k = 1; k < atom_q1; k++)
               atom_tx_all[atom_j*(q*r+1) + (k-1)*r+1+ell] = Base(0.0);
         //
         ++atom_j;
         if( atom_j == atom_n )
            atom_state = ret_atom;
         break;

         case FunavOp:
         // variable argument for an atomic function
         CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
         CPPAD_ASSERT_UNKNOWN( atom_state == arg_atom );
         CPPAD_ASSERT_UNKNOWN( atom_i == 0 );
         CPPAD_ASSERT_UNKNOWN( atom_j < atom_n );
         //
         atom_type_x[atom_j] = variable_enum;
         atom_par_x[atom_j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         atom_tx_all[atom_j*(q*r+1)+0] =
            taylor[size_t(arg[0])*((J-1)*r+1)+0];
         for(ell = 0; ell < r; ell++)
         {  for(k = 1; k < atom_q1; k++)
            {  atom_tx_all[atom_j*(q*r+1) + (k-1)*r+1+ell] =
                  taylor[size_t(arg[0])*((J-1)*r+1) + (k-1)*r+1+ell];
            }
         }
         //
         ++atom_j;
         if( atom_j == atom_n )
            atom_state = ret_atom;
         break;

         case FunrpOp:
         // parameter result for an atomic function
         CPPAD_ASSERT_NARG_NRES(op, 1, 0);
         CPPAD_ASSERT_UNKNOWN( atom_state == ret_atom );
         CPPAD_ASSERT_UNKNOWN( atom_i < atom_m );
         CPPAD_ASSERT_UNKNOWN( atom_j == atom_n );
         //
         atom_iy[atom_i] = 0;
         atom_ty_all[atom_i*(q*r+1) + 0] = parameter[ arg[0]];
         for(ell = 0; ell < r; ell++)
            for(k = 1; k < atom_q1; k++)
               atom_ty_all[atom_i*(q*r+1) + (k-1)*r+1+ell] = Base(0.0);
         //
         ++atom_i;
         if( atom_i == atom_m )
            atom_state = end_atom;
         break;

         case FunrvOp:
         // variable result for an atomic function
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         CPPAD_ASSERT_UNKNOWN( atom_state == ret_atom );
         CPPAD_ASSERT_UNKNOWN( atom_i < atom_m );
         CPPAD_ASSERT_UNKNOWN( atom_j == atom_n );
         //
         atom_iy[atom_i] = i_var;
         atom_ty_all[atom_i*(q*r+1)+0] = taylor[i_var*((J-1)*r+1)+0];
         for(ell = 0; ell < r; ell++)
         {  for(k = 1; k < atom_q1; k++)
            {  atom_ty_all[atom_i*(q*r+1) + (k-1)*r+1+ell] =
                  taylor[i_var*((J-1)*r+1) + (k-1)*r+1+ell];
            }
         }
         ++atom_i;
         if( atom_i == atom_m )
            atom_state = end_atom;
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         forward_zmulvp_op_dir(q, r, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_FORWARD2_TRACE
      if( atom_trace )
      {  atom_trace = false;
         CPPAD_ASSERT_UNKNOWN( op == AFunOp );
         CPPAD_ASSERT_UNKNOWN( NumArg(FunrvOp) == 0 );
         for(i = 0; i < atom_m; i++) if( atom_iy[i] > 0 )
         {  size_t i_tmp   = (itr.op_index() + i) - atom_m;
            printOp<Base, RecBase>(
               std::cout,
               play,
               i_tmp,
               atom_iy[i],
               FunrvOp,
               nullptr
            );
            Base* Z_tmp = taylor + atom_iy[i]*((J-1) * r + 1);
            {  Z_vec[0]    = Z_tmp[0];
               for(ell = 0; ell < r; ell++)
               {  std::cout << std::endl << "     ";
                  for(size_t p_tmp = 1; p_tmp <= q; p_tmp++)
                     Z_vec[p_tmp] = Z_tmp[(p_tmp-1)*r+ell+1];
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
      if( op != FunrvOp )
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
   CPPAD_ASSERT_UNKNOWN( atom_state == start_atom );

   return;
}

// preprocessor symbols that are local to this file
# undef CPPAD_FORWARD2_TRACE

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE
# endif
