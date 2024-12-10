# ifndef CPPAD_LOCAL_SWEEP_FORWARD_ANY_HPP
# define CPPAD_LOCAL_SWEEP_FORWARD_ANY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/atom_op_info.hpp>
# include <cppad/local/sweep/call_atomic.hpp>
# include <cppad/local/var_op/compare_op.hpp>
# include <cppad/local/var_op/atomic_op.hpp>

# define CPPAD_FORWARD_ANY_TRACE 0

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*
 ------------------------------------------------------------------------------
{xrst_begin sweep_forward_any dev}
{xrst_spell
   cskip
}

{xrst_template ;
   include/cppad/local/sweep/template/forward_sweep.xrst

   headers all:     CPPAD_@NAME@_TRACE, Portotype, Base, RecBase, play, num_var
   headers all:     cap_order, cskip, load_op2var, taylor
   headers 0 & any: change_count, change_number, change_op_index, sout, print
   headers any:     order_low, order_up

   @title@  The title for this forward sweep
   @#####@  Underlining for the tilte
   @NAME@   is one of FORWARD_0, FORWARD_ANY, FORWARD_DIR
   @****@   Underlining for @NAME@

   @title@ ; Compute Any Order Forward Mode Taylor Coefficients
   @#####@ ; ##################################################
   @NAME@  ; FORWARD_ANY
   @****@  ; ***********
}

{xrst_end sweep_forward_any}
*/
// BEGIN_FORWARD_ANY
template <class Base, class RecBase>
void forward_any(
   const RecBase&             not_used_rec_base,
   const local::player<Base>* play,
   const size_t               num_var,
   const size_t               cap_order,
   bool*                      cskip_op,
   pod_vector<addr_t>&        load_op2var,
   size_t                     change_count,
   size_t&                    change_number,
   size_t&                    change_op_index,
   std::ostream&              s_out,
   const bool                 print,
   const size_t               order_low,
   const size_t               order_up,
   Base*                      taylor
)
// END_FORWARD_ANY
{
   // number of directions
   const size_t r = 1;

   CPPAD_ASSERT_UNKNOWN(order_low <= order_up );
   CPPAD_ASSERT_UNKNOWN( cap_order >= order_up + 1 );
   CPPAD_ASSERT_UNKNOWN( play->num_var_rec() == num_var );

   /*
   <!-- replace forward0sweep_code_define -->
   */

   // initialize the comparison operator counter
   if( order_low == 0 )
   {  change_number   = 0;
      change_op_index = 0;
   }

   // If this includes a zero calculation, initialize this information
   pod_vector<bool>   vec_ad2isvar;
   pod_vector<size_t> vec_ad2index;
   if( order_low == 0 )
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
# if CPPAD_FORWARD_ANY_TRACE
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
         var_op::compare_forward_any(op,
            arg, parameter, cap_order, taylor, itr.op_index(),
            change_count, change_number, change_op_index
         );
         break;
         // -------------------------------------------------

         case AbsOp:
         var_op::abs_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AddvvOp:
         var_op::addvv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AddpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::addpv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AcosOp:
         // sqrt(1 - x * x), acos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::acos_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AcoshOp:
         // sqrt(x * x - 1), acosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::acosh_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AsinOp:
         // sqrt(1 - x * x), asin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::asin_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AsinhOp:
         // sqrt(1 + x * x), asinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::asinh_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AtanOp:
         // 1 + x * x, atan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::atan_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AtanhOp:
         // 1 - x * x, atanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::atanh_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case CExpOp:
         var_op::cexp_forward_any(
            order_low, order_up, i_var, arg, num_par, parameter, cap_order, taylor
         );
         break;
         // ---------------------------------------------------

         case CosOp:
         // sin(x), cos(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::cos_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // ---------------------------------------------------

         case CoshOp:
         // sinh(x), cosh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::cosh_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case CSkipOp:
         if( order_low == 0 )
         {  var_op::cskip_forward_0(
               i_var, arg, num_par, parameter, cap_order, taylor, cskip_op
            );
         }
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case CSumOp:
         var_op::csum_forward_any(
            order_low, order_up, i_var, arg, num_par, parameter, cap_order, taylor
         );
         itr.correct_before_increment();
         break;
         // -------------------------------------------------

         case DisOp:
         var_op::dis_forward_dir<RecBase>(
            order_low, order_up, r, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case DivvvOp:
         var_op::divvv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case DivpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::divpv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case DivvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::divvp_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case EndOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 0);
         more_operators = false;
         break;
         // -------------------------------------------------

         case ErfOp:
         case ErfcOp:
         var_op::erf_forward_any(
            op, order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case ExpOp:
         var_op::exp_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // ---------------------------------------------------

         case Expm1Op:
         var_op::expm1_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // ---------------------------------------------------

         case InvOp:
         CPPAD_ASSERT_NARG_NRES(op, 0, 1);
         break;
         // -------------------------------------------------

         case LdpOp:
         case LdvOp:
         if( order_low == 0 )
         {  var_op::load_forward_0(
               op,
               i_var,
               play->num_var_vecad_ind_rec(),
               arg,
               num_var,
               num_par,
               parameter,
               cap_order,
               taylor,
               vec_ad2isvar,
               vec_ad2index,
               load_op2var
            );
            if(order_low < order_up ) var_op::load_forward_nonzero(
               op,
               i_var,
               arg,
               order_low + 1,
               order_up,
               r,
               cap_order,
               load_op2var,
               taylor
            );
         }
         else
            var_op::load_forward_nonzero(
            op,
            i_var,
            arg,
            order_low,
            order_up,
            r,
            cap_order,
            load_op2var,
            taylor
         );
         break;
         // -------------------------------------------------

         case LogOp:
         var_op::log_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case Log1pOp:
         var_op::log1p_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case MulpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::mulpv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case MulvvOp:
         var_op::mulvv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // --------------------------------------------------

         case NegOp:
         var_op::neg_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case ParOp:
         var_op::par_forward_any(
            order_low, order_up, i_var, arg, num_par, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case PowvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::powvp_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case PowpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::powpv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case PowvvOp:
         var_op::powvv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case PriOp:
         if( ( order_low == 0 ) & print ) var_op::pri_forward_0(s_out,
            arg, num_text, text, num_par, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case SignOp:
         // sign(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::sign_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case SinOp:
         // cos(x), sin(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::sin_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case SinhOp:
         // cosh(x), sinh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::sinh_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case SqrtOp:
         var_op::sqrt_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case StppOp:
         case StpvOp:
         case StvpOp:
         case StvvOp:
         if( order_low == 0 )
         {  var_op::store_forward_0(
               op,
               arg,
               num_var,
               num_par,
               parameter,
               cap_order,
               taylor,
               vec_ad2isvar,
               vec_ad2index
            );
         }
         break;
         // -------------------------------------------------

         case SubvvOp:
         var_op::subvv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case SubpvOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
         var_op::subpv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case SubvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::subvp_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case TanOp:
         // tan(x)^2, tan(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::tan_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case TanhOp:
         // tanh(x)^2, tanh(x)
         CPPAD_ASSERT_UNKNOWN( i_var < num_var  );
         var_op::tanh_forward_any(
            order_low, order_up, i_var, arg, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case AFunOp:
         // start of an atomic function call
         var_op::atomic_forward_any<Base, RecBase>(
            itr,
            play,
            parameter,
            atom_trace,
            atom_work,
            cap_order,
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
         var_op::zmulpv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case ZmulvpOp:
         CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
         var_op::zmulvp_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         case ZmulvvOp:
         var_op::zmulvv_forward_any(
            order_low, order_up, i_var, arg, parameter, cap_order, taylor
         );
         break;
         // -------------------------------------------------

         default:
         CPPAD_ASSERT_UNKNOWN(false);
      }
# if CPPAD_FORWARD_ANY_TRACE
      Base*           Z_tmp   = taylor + cap_order * i_var;
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
            order_up + 1,
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
   if( ( order_low == 0 ) && (change_count == 0) )
      change_number = 0;
   return;
}

// preprocessor symbols that are local to this file
# undef CPPAD_FORWARD_ANY_TRACE

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE
# endif
