# ifndef CPPAD_LOCAL_SWEEP_FORWARD1_HPP
# define CPPAD_LOCAL_SWEEP_FORWARD1_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>

# include <cppad/local/op_class/op_enum2instance.hpp>

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

template <class Addr, class Base, class RecBase>
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
   pod_vector<Addr>&          load_op2var,
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
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
   const size_t need_y    = size_t( variable_enum );
   const size_t order_low = p;
   const size_t order_up  = q;

   // vectors used by atomic function operators
   vector<Base>         atom_par_x;  // argument parameter values
   vector<ad_type_enum> atom_type_x; // argument type
   vector<Base>         atom_tx;     // argument vector Taylor coefficients
   vector<Base>         atom_ty;     // result vector Taylor coefficients
   vector<size_t>       atom_iy;     // variable indices for result vector
   vector<bool>         atom_sy;     // select_y for this atomic call
   //
   // information defined by atomic function operators
   size_t atom_index=0, atom_id=0, atom_m=0, atom_n=0, atom_i=0, atom_j=0;
   enum_atom_state atom_state = start_atom; // proper initialization

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
   size_t i, k;

   // number of orders for this atomic calculation
   // (not needed for order zero)
   const size_t atom_q1 = q+1;

   // skip the BeginOp at the beginning of the recording
   play::const_sequential_iterator itr = play->begin();
   // op_info
   OpCode op;
   size_t i_var;
   const Addr*   arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op == BeginOp );
   //
# if CPPAD_FORWARD1_TRACE
   bool atom_trace = false;
   std::cout << std::endl;
# endif
   //
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
         case AbsOp:
         forward_abs_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------
         // BEGIN_SORT_THIS_LINE_PLUS_1
         case AddpvOp:
         case AddvvOp:
         case DivpvOp:
         case DivvvOp:
         case MulpvOp:
         case MulvvOp:
         case SubpvOp:
         case SubvpOp:
         case SubvvOp:
         // END_SORT_THIS_LINE_MINUS_1
         op_enum2instance<Base>(op)->forward(
            p, q, i_var, arg, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_acos_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_acosh_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_asin_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_asinh_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_atan_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_atanh_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case CExpOp:
         forward_cond_op(
            p, q, i_var, arg, num_par, parameter, J, taylor
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_cos_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // ---------------------------------------------------

         case CoshOp:
         // sinh(x), cosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_cosh_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case CSkipOp:
         if( p == 0 )
         {  forward_cskip_op_0(
               i_var, arg, num_par, parameter, J, taylor, cskip_op
            );
         }
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         forward_csum_op(
            p, q, i_var, arg, num_par, parameter, J, taylor
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DisOp:
         forward_dis_op<RecBase>(p, q, r, i_var, arg, J, taylor);
         break;
         // -------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         forward_divvp_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case EndOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;
         // -------------------------------------------------

         case EqppOp:
         if( compare_change_count )
         {  forward_eqpp_op_0(
               compare_change_number, arg, parameter
            );
            {  if( compare_change_count == compare_change_number )
                  compare_change_op_index = itr.op_index();
            }
         }
         break;
         // -------------------------------------------------

         case EqpvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_eqpv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case EqvvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_eqvv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case ErfOp:
         case ErfcOp:
         forward_erf_op(op, p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ExpOp:
         forward_exp_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // ---------------------------------------------------

         case Expm1Op:
         forward_expm1_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // ---------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         if( p == 0 )
         {  forward_load_p_op_0(
               play,
               i_var,
               arg,
               parameter,
               J,
               taylor,
               vec_ad2isvar.data(),
               vec_ad2index.data(),
               load_op2var.data()
            );
            if( p < q ) forward_load_op(
               play,
               op,
               p+1,
               q,
               r,
               J,
               i_var,
               arg,
               load_op2var.data(),
               taylor
            );
         }
         else
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
         // -------------------------------------------------

         case LdvOp:
         if( p == 0 )
         {  forward_load_v_op_0(
               play,
               i_var,
               arg,
               parameter,
               J,
               taylor,
               vec_ad2isvar.data(),
               vec_ad2index.data(),
               load_op2var.data()
            );
            if( p < q ) forward_load_op(
               play,
               op,
               p+1,
               q,
               r,
               J,
               i_var,
               arg,
               load_op2var.data(),
               taylor
            );
         }
         else
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
         // -------------------------------------------------

         case LeppOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_lepp_op_0(
               compare_change_number, arg, parameter
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LepvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_lepv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LevpOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_levp_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LevvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_levv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LogOp:
         forward_log_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case Log1pOp:
         forward_log1p_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case LtppOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_ltpp_op_0(
               compare_change_number, arg, parameter
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LtpvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_ltpv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LtvpOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_ltvp_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case LtvvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_ltvv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case NegOp:
         forward_neg_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case NeppOp:
         if( compare_change_count )
         {  forward_nepp_op_0(
               compare_change_number, arg, parameter
            );
            {  if( compare_change_count == compare_change_number )
                  compare_change_op_index = itr.op_index();
            }
         }
         break;
         // -------------------------------------------------

         case NepvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_nepv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case NevvOp:
         if( ( p == 0 ) && ( compare_change_count > 0 ) )
         {  forward_nevv_op_0(
               compare_change_number, arg, parameter, J, taylor
            );
            if( compare_change_count == compare_change_number )
               compare_change_op_index = itr.op_index();
         }
         break;
         // -------------------------------------------------

         case ParOp:
         i = p;
         if( i == 0 )
         {  forward_par_op_0(
               i_var, arg, num_par, parameter, J, taylor
            );
            i++;
         }
         while(i <= q)
         {  taylor[ i_var * J + i] = Base(0.0);
            i++;
         }
         break;
         // -------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         forward_powvp_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         forward_powpv_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PowvvOp:
         forward_powvv_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case PriOp:
         if( (p == 0) & print ) forward_pri_0(s_out,
            arg, num_text, text, num_par, parameter, J, taylor
         );
         break;
         // -------------------------------------------------

         case SignOp:
         // sign(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_sign_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_sin_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_sinh_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case SqrtOp:
         forward_sqrt_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case StppOp:
         if( p == 0 )
         {  forward_store_pp_op_0(
               i_var,
               arg,
               num_par,
               parameter,
               J,
               taylor,
               vec_ad2isvar.data(),
               vec_ad2index.data()
            );
         }
         break;
         // -------------------------------------------------

         case StpvOp:
         if( p == 0 )
         {  forward_store_pv_op_0(
               i_var,
               arg,
               num_par,
               parameter,
               J,
               taylor,
               vec_ad2isvar.data(),
               vec_ad2index.data()
            );
         }
         break;
         // -------------------------------------------------

         case StvpOp:
         if( p == 0 )
         {  forward_store_vp_op_0(
               i_var,
               arg,
               num_par,
               J,
               taylor,
               vec_ad2isvar.data(),
               vec_ad2index.data()
            );
         }
         break;
         // -------------------------------------------------

         case StvvOp:
         if( p == 0 )
         {  forward_store_vv_op_0(
               i_var,
               arg,
               num_par,
               J,
               taylor,
               vec_ad2isvar.data(),
               vec_ad2index.data()
            );
         }
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_tan_op(p, q, i_var, size_t(arg[0]), J, taylor);
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < numvar  );
         forward_tanh_op(p, q, i_var, size_t(arg[0]), J, taylor);
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
            atom_tx.resize(atom_n * atom_q1);
            atom_ty.resize(atom_m * atom_q1);
            atom_iy.resize(atom_m);
            atom_sy.resize(atom_m);
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( atom_i == atom_m );
            CPPAD_ASSERT_UNKNOWN( atom_j == atom_n );
            atom_state = start_atom;
            //
            for(i = 0; i < atom_m; ++i)
               atom_sy[i] = atom_iy[i] != 0;
            //
            // call atomic function for this operation
            call_atomic_forward<Base, RecBase>(
               atom_par_x, atom_type_x, need_y, atom_sy,
               order_low, order_up, atom_index, atom_id, atom_tx, atom_ty
            );
            for(i = 0; i < atom_m; i++)
               if( atom_iy[i] > 0 )
                  for(k = p; k <= q; k++)
                     taylor[ atom_iy[i] * J + k ] =
                        atom_ty[ i * atom_q1 + k ];
# if CPPAD_FORWARD1_TRACE
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
         atom_par_x[atom_j]            = parameter[ arg[0] ];
         atom_tx[atom_j * atom_q1 + 0] = parameter[ arg[0]];
         for(k = 1; k < atom_q1; k++)
            atom_tx[atom_j * atom_q1 + k] = Base(0.0);
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
         for(k = 0; k < atom_q1; k++)
            atom_tx[atom_j * atom_q1 + k] = taylor[ size_t(arg[0]) * J + k];
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
         CPPAD_ASSERT_UNKNOWN( size_t( arg[0] ) < num_par );
         //
         atom_iy[atom_i] = 0;
         atom_ty[atom_i * atom_q1 + 0] = parameter[ arg[0]];
         for(k = 1; k < p; k++)
            atom_ty[atom_i * atom_q1 + k] = Base(0.0);
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
         for(k = 0; k < p; k++)
            atom_ty[atom_i * atom_q1 + k] = taylor[ i_var * J + k];
         //
         ++atom_i;
         if( atom_i == atom_m )
            atom_state = end_atom;
         break;
         // -------------------------------------------------

         case ZmulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         forward_zmulpv_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         forward_zmulvp_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         forward_zmulvv_op(p, q, i_var, arg, parameter, J, taylor);
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(0);
      }
# if CPPAD_FORWARD1_TRACE
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
            Base* Z_tmp = taylor + atom_iy[i] * J;
            printOpResult(
               std::cout,
               q + 1,
               Z_tmp,
               0,
               (Base *) nullptr
            );
            std::cout << std::endl;
         }
      }
      Base*           Z_tmp   = taylor + J * i_var;
      if( op != FunrvOp )
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
   CPPAD_ASSERT_UNKNOWN( atom_state == start_atom );

   if( (p == 0) && (compare_change_count == 0) )
      compare_change_number = 0;
   return;
}

// preprocessor symbols that are local to this file
# undef CPPAD_FORWARD1_TRACE

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE
# endif
