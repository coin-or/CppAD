# ifndef  CPPAD_LOCAL_VAL_GRAPH_FUN2VAL_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_FUN2VAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin fun2val_graph dev}

Create a Value Graph Corresponding to an ADFun Object
#####################################################

Syntax
******
| |tab| ``ADFun`` < *Base* > *fun*
| |tab| *fun* . ``fun2val`` ( *val_tape* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Base
****
is the type corresponding to this :ref:`adfun-name` object;
i.e., its calculations are done using the type *Base* .
It is also :ref:`val_tape@Value` type for the tape.

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .

val_tape
********
This is a :ref:`val_tape-name` object.
The input contents of the tape does not matter.
Upon return it is a
:ref:`val_graph-name` representation of the function.
The independent dynamic parameters have the same order as in
*fun* and come first (in the value graph independent vector).
The independent variables have the same order as in
*fun* and come after the independent dynamic parameters.

fun
***
This is the function that the value graph will correspond to.

Under Construction
******************
This routine is under construction and is only implemented
for a few of the possible :ref:`ADFun-name` operators.

{xrst_toc_hidden
   val_graph/fun2val_xam.cpp
}
Examples
********
The file :ref:`val_fun2val_xam.cpp-name`
is an example an test of this conversion.

{xrst_end fun2val_graph}
*/

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/pod_vector.hpp>
# include <cppad/local/val_graph/var_type.hpp>
# include <cppad/local/val_graph/dyn_type.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void ADFun<Base, RecBase>::fun2val(
   local::val_graph::tape_t<Base>& val_tape  )
// END_PROTOTYPE
{  //
   // Vector, addr_t, op_enum_t
   using local::val_graph::Vector;
   using local::val_graph::addr_t;
   using local::val_graph::op_enum_t;
   op_enum_t number_op_enum = local::val_graph::number_op_enum;
   //
   // op_code_dyn, number_dyn, op_code_var
   using local::op_code_dyn;
   op_code_dyn number_dyn = local::number_dyn;
   using local::op_code_var;
   //
   // pod_vector, opcode_t
   using local::pod_vector;
   using local::opcode_t;
   //
   // invalid_addr_t
   addr_t invalid_addr_t = std::numeric_limits<addr_t>::max();
   //
   // parameter
   const Base* parameter = play_.GetPar();
   //
   // dyn_op2val_op
   Vector<op_enum_t> dyn_op2val_op(number_dyn);
   for(size_t i = 0; i < size_t(number_dyn); ++i)
      dyn_op2val_op[i] = number_op_enum; // invalid
   dyn_op2val_op[local::add_dyn] = local::val_graph::add_op_enum;
   dyn_op2val_op[local::neg_dyn] = local::val_graph::neg_op_enum;
   dyn_op2val_op[local::sub_dyn] = local::val_graph::sub_op_enum;
   // ------------------------------------------------------------------------
   // var_op2val_op
   Vector<op_enum_t> var_op2val_op(local::NumberOp);
   for(size_t i = 0; i < size_t(local::NumberOp); ++i)
      var_op2val_op[i] = number_op_enum; // invalid
   //
   // unary operators
   // BEGIN_SORT_THIS_LINE_PLUS_1
   var_op2val_op[local::AbsOp]    = local::val_graph::abs_op_enum;
   var_op2val_op[local::AcosOp]   = local::val_graph::acos_op_enum;
   var_op2val_op[local::AcoshOp]  = local::val_graph::acosh_op_enum;
   var_op2val_op[local::AsinOp]   = local::val_graph::asin_op_enum;
   var_op2val_op[local::AsinhOp]  = local::val_graph::asinh_op_enum;
   var_op2val_op[local::AtanOp]   = local::val_graph::atan_op_enum;
   var_op2val_op[local::AtanhOp]  = local::val_graph::atanh_op_enum;
   var_op2val_op[local::CosOp]    = local::val_graph::cos_op_enum;
   var_op2val_op[local::CoshOp]   = local::val_graph::cosh_op_enum;
   var_op2val_op[local::ErfOp]    = local::val_graph::erf_op_enum;
   var_op2val_op[local::ErfcOp]   = local::val_graph::erfc_op_enum;
   var_op2val_op[local::ExpOp]    = local::val_graph::exp_op_enum;
   var_op2val_op[local::Expm1Op]  = local::val_graph::expm1_op_enum;
   var_op2val_op[local::Log1pOp]  = local::val_graph::log1p_op_enum;
   var_op2val_op[local::LogOp]    = local::val_graph::log_op_enum;
   var_op2val_op[local::NegOp]    = local::val_graph::neg_op_enum;
   var_op2val_op[local::SignOp]   = local::val_graph::sign_op_enum;
   var_op2val_op[local::SinOp]    = local::val_graph::sin_op_enum;
   var_op2val_op[local::SinhOp]   = local::val_graph::sinh_op_enum;
   var_op2val_op[local::SqrtOp]   = local::val_graph::sqrt_op_enum;
   var_op2val_op[local::TanOp]    = local::val_graph::tan_op_enum;
   var_op2val_op[local::TanhOp]   = local::val_graph::tanh_op_enum;
   // END_SORT_THIS_LINE_MINUS_1
   //
   // add
   var_op2val_op[local::AddpvOp] = local::val_graph::add_op_enum;
   var_op2val_op[local::AddvvOp] = local::val_graph::add_op_enum;
   //
   // sub
   var_op2val_op[local::SubpvOp] = local::val_graph::sub_op_enum;
   var_op2val_op[local::SubvpOp] = local::val_graph::sub_op_enum;
   var_op2val_op[local::SubvvOp] = local::val_graph::sub_op_enum;
   //
   // mul
   var_op2val_op[local::MulpvOp] = local::val_graph::mul_op_enum;
   var_op2val_op[local::MulvvOp] = local::val_graph::mul_op_enum;
   //
   // div
   var_op2val_op[local::DivpvOp] = local::val_graph::div_op_enum;
   var_op2val_op[local::DivvpOp] = local::val_graph::div_op_enum;
   var_op2val_op[local::DivvvOp] = local::val_graph::div_op_enum;
   //
   // pow
   var_op2val_op[local::PowpvOp] = local::val_graph::pow_op_enum;
   var_op2val_op[local::PowvpOp] = local::val_graph::pow_op_enum;
   var_op2val_op[local::PowvvOp] = local::val_graph::pow_op_enum;
   // ------------------------------------------------------------------------
   //
   // dyn_par_op
   // mapping from dynamic parameter index to operator
   const pod_vector<opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
   //
   // dyn_ind2par_ind
   // mapping from dynamic parameter index to parameter index
   const pod_vector<addr_t>& dyn_ind2par_ind ( play_.dyn_ind2par_ind() );
   //
   // dyn_par_arg
   // vector that contains arguments to all the dynamic parameter operators
   const pod_vector<addr_t>&  dyn_par_arg( play_.dyn_par_arg() );
   //
   // n_parameter
   // number of parameters
   size_t n_parameter = play_.num_par_rec();
   //
   // n_dynamic
   // number of dynamic parameters
   size_t n_dynamic = dyn_ind2par_ind.size();
   //
   // n_dynamic_ind
   // number of independent dynamic parameters
   size_t n_dynamic_ind = play_.num_dynamic_ind();
   //
   // n_variables
   // number of variables
   const size_t n_variable = play_.num_var_rec();
   //
   // n_variable_ind
   // number of indepedent variables
   size_t n_variable_ind = ind_taddr_.size();
   //
   // n_val_ind
   // number of indepedent valuse
   addr_t n_val_ind = addr_t( n_dynamic_ind + n_variable_ind );
   //
# ifndef NDEBUG
   // nan_val_index
   // initialize value vector tape
   addr_t nan_val_index = val_tape.set_ind( n_val_ind );
   CPPAD_ASSERT_UNKNOWN( nan_val_index == n_val_ind );
# else
   val_tape.set_ind( n_val_ind );
# endif
   //
   // val_tape, vec_info_vec
   // Put dynamic vectors in val_tape and create vec_info_vec
   struct vec_info_t { size_t size; size_t offset; };
   Vector<vec_info_t> vec_info_vec;
   {  size_t n_vecad_ind = play_.num_var_vecad_ind_rec();
      size_t index         = 0;
      while(index < n_vecad_ind)
      {  size_t size         = play_.GetVecInd(index++);
         size_t offset       = index;
         //
         Vector<addr_t> initial(size);
         for(size_t i = 0; i < size; ++i)
         {  size_t par_index = play_.GetVecInd(index++);
            addr_t val_index = val_tape.record_con_op( parameter[par_index] );
            initial[i]       = val_index;
         }
# ifdef NDEBUG
         val_tape.record_vec_op(initial);
# else
         addr_t which_vector = val_tape.record_vec_op(initial);
         CPPAD_ASSERT_UNKNOWN( size_t(which_vector) == vec_info_vec.size() );
# endif
         vec_info_t vec_info = {size, offset};
         vec_info_vec.push_back( vec_info );
      }
   }
   //
   // vec_offset2index
   // mapping from vecad offset to index in vec_info_vec
   auto vec_offset2index = [&vec_info_vec](addr_t offset)
   {  addr_t index    = 0;
      size_t offset_s = size_t(offset);
      while( vec_info_vec[index].offset < offset_s )
         ++index;
      CPPAD_ASSERT_UNKNOWN( vec_info_vec[index].offset == offset_s );
      return index;
   };
   //
   // par2val_index
   // Initialize mapping from parameter index to index in value vector.
   Vector<addr_t> par2val_index(n_parameter);
   for(size_t i = 0; i < n_parameter; ++i)
      par2val_index[i] = invalid_addr_t;
   for(addr_t i = 0; i < addr_t( n_dynamic_ind ); ++i)
      par2val_index[i + 1] = i;
   //
   // val_op_arg, var_op_res, res_is_par;
   Vector<addr_t> val_op_arg, var_op_res;
   Vector<bool>   res_is_par;
   //
   // i_arg
   // initial index in dyn_par_arg
   addr_t i_arg = 0;
   //
   // ensure_par2val_index
   // val_tape, par2val_index
   auto ensure_par2val_index =
   [&val_tape, &par2val_index, &parameter, invalid_addr_t] (addr_t par_index)
   {  addr_t result = par2val_index[par_index];
      if( result == invalid_addr_t )
      {  Base constant            = parameter[par_index];
         result                   = val_tape.record_con_op(constant);
         par2val_index[par_index] = result;
      }
      return result;
   };
   //
   // val_tape
   // record dynamic parameter operations
   //
   // val_tape, val_index, par2val_index
   addr_t val_index = invalid_addr_t;
   for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
   {  //
      // i_par
      size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
      //
      // dyn_op
      // operator for this dynamic parameter
      local::op_code_dyn dyn_op = local::op_code_dyn( dyn_par_op[i_dyn] );
      //
      // is_unary, is_binary
      bool is_unary  = local::val_graph::unary_dyn_op(dyn_op);
      bool is_binary = local::val_graph::binary_dyn_op(dyn_op);
      //
      // n_arg, val_tape, val_index, par2val_index
      addr_t n_arg;
      if( is_unary || is_binary )
      {  //
         // val_tape, val_index, par2val_index
         n_arg = addr_t( num_arg_dyn(dyn_op) );
         val_op_arg.resize(n_arg);
         for(addr_t i = 0; i < n_arg; ++i)
         {  addr_t par_index = dyn_par_arg[i_arg + i];
            val_op_arg[i]    = ensure_par2val_index(par_index);
         }
         // val_tape, val_index
         op_enum_t val_op = dyn_op2val_op[dyn_op];
         val_index = val_tape.record_op(val_op, val_op_arg);
      }
      else switch( dyn_op )
      {  //
         default:
         CPPAD_ASSERT_KNOWN(false,
            "val_graph::fun2val: This dynamic operator not yet implemented"
         );
         // -------------------------------------------------------------------
         // result_dyn
         // This is a place holder for multiple result operators
         case local::result_dyn:
         CPPAD_ASSERT_UNKNOWN( val_index != invalid_addr_t );
         n_arg      = 0;
         val_index += 1;
         break;
         // -------------------------------------------------------------------
         // dis_dyn
         case local::dis_dyn:
         {  // val_tape, val_index, par2val_index
            val_op_arg.resize(1);
            addr_t discrete_index = dyn_par_arg[i_arg + 0];
            addr_t par_index      = dyn_par_arg[i_arg + 1];
            val_op_arg[0]         = ensure_par2val_index(par_index);
            //
            // n_arg, val_tape, val_index
            n_arg = 2;
            val_index = val_tape.record_dis_op(
               discrete_index, val_op_arg[0]
            );
         }
         break;
         // -------------------------------------------------------------------
         // cond_exp
         case local::cond_exp_dyn:
         {  CompareOp cop = CompareOp( dyn_par_arg[i_arg + 0] );
            addr_t left     = ensure_par2val_index( dyn_par_arg[i_arg + 1] );
            addr_t right    = ensure_par2val_index( dyn_par_arg[i_arg + 2] );
            addr_t if_true  = ensure_par2val_index( dyn_par_arg[i_arg + 3] );
            addr_t if_false = ensure_par2val_index( dyn_par_arg[i_arg + 4] );
            local::val_graph::compare_enum_t compare_enum;
            switch( cop )
            {  case CompareLt:
               compare_enum = local::val_graph::compare_lt_enum;
               break;

               case CompareLe:
               compare_enum = local::val_graph::compare_le_enum;
               break;

               case CompareEq:
               compare_enum = local::val_graph::compare_eq_enum;
               break;

               case CompareGe:
               compare_enum = local::val_graph::compare_lt_enum;
               std::swap(if_true, if_false);
               break;

               case CompareGt:
               compare_enum = local::val_graph::compare_le_enum;
               std::swap(if_true, if_false);
               break;

               case CompareNe:
               compare_enum = local::val_graph::compare_eq_enum;
               std::swap(if_true, if_false);
               break;

               default:
               CPPAD_ASSERT_UNKNOWN(false);
               // avoid warning about value not set
               compare_enum = local::val_graph::compare_no_enum;
               break;
            }
            //
            // n_arg, val_tape, val_index
            n_arg     = 5;
            val_index = val_tape.record_cexp_op(
               compare_enum, left, right, if_true, if_false
            );
         }
         break;
         // -------------------------------------------------------------------
         // atom_dyn
         case local::atom_dyn:
         {  //
            // atomic_index, call_id, n_call_arg, n_res
            addr_t atomic_index = dyn_par_arg[i_arg + 0];
            addr_t call_id      = dyn_par_arg[i_arg + 1];
            addr_t n_call_arg   = dyn_par_arg[i_arg + 2];
            addr_t n_res        =  dyn_par_arg[i_arg + 3] ;
            // num_dyn          = size_t( dyn_par_arg[i_age + 4] );
            //
            // val_op_arg, val_tape, par2val_index
            val_op_arg.resize( n_call_arg );
            for(addr_t i = 0; i < n_call_arg; i++)
            {  addr_t par_index = dyn_par_arg[i_arg + i + 5];
               val_op_arg[i] = ensure_par2val_index(par_index);
            }
            // n_arg, val_tape, val_index
            n_arg     = n_call_arg + 5;
            val_index = val_tape.record_call_op(
               atomic_index, call_id, n_res, val_op_arg
            );
         }
         break;
      }
      // par2val_index
      // Each dyn_op has one result, result_dyn ops are added to make this so
      par2val_index[i_par] = val_index;
      //
      // i_arg
      i_arg += n_arg;
   }
   //
   // var2val_index
   // Initialize mapping from variable index to index in value vector.
   Vector<addr_t> var2val_index(n_variable);
   for(size_t i = 0; i < n_variable; ++i)
      var2val_index[i] = invalid_addr_t;
   for(addr_t i = 0; i < addr_t(n_variable_ind); ++i)
      var2val_index[i + 1] = addr_t( n_dynamic_ind )  + i;
   //
   // itr, is_var, more_operators
   local::play::const_sequential_iterator itr  = play_.begin();
   Vector<bool>       is_var(2);
   bool more_operators = true;
   bool in_atomic_call = false;
   //
   // val_tape, var2val_index, par2val_index
   while(more_operators)
   {  //
      //
      // itr, var_op, var_op_arg, i_var
      local::op_code_var var_op;
      const            addr_t* var_op_arg;
      size_t           i_var;
      (++itr).op_info(var_op, var_op_arg, i_var);
      //
      // is_unary, is_binary, is_compare
      // initialize to avoid compiler warning
      bool is_unary = false, is_binary = false, is_compare = false;
      local::val_graph::type_var_op(
         var_op, is_unary, is_binary, is_compare
      );
      // ----------------------------------------------------------------------
      if( is_unary )
      {  //
         // val_op
         op_enum_t val_op = var_op2val_op[var_op];
         CPPAD_ASSERT_UNKNOWN( val_op < number_op_enum );
         //
         // val_tape, val_index
         val_op_arg.resize(1);
         val_op_arg[0] = var2val_index[ var_op_arg[0] ];
         val_index = val_tape.record_op(val_op, val_op_arg);
         //
         // var2val_index
         var2val_index[i_var] = val_index;
      }
      // ----------------------------------------------------------------------
      else if( is_binary )
      {   switch( var_op )
         {
            default:
            CPPAD_ASSERT_KNOWN(false,
               "val_graph::fun2val: This variable operator not yet implemented"
            );

            // first argument a parameter, second argument a variable
            case local::AddpvOp:
            case local::SubpvOp:
            case local::MulpvOp:
            case local::DivpvOp:
            case local::PowpvOp:
            is_var[0]   = false;
            is_var[1]   = true;
            break;

            // first argument a variable, second argument a parameter
            case local::SubvpOp:
            case local::DivvpOp:
            case local::PowvpOp:
            is_var[0]   = true;
            is_var[1]   = false;
            break;

            // first argument a variable, second argument a variable
            case local::AddvvOp:
            case local::SubvvOp:
            case local::MulvvOp:
            case local::DivvvOp:
            case local::PowvvOp:
            is_var[0]   = true;
            is_var[1]   = true;
            break;
         }
         //
         // varl_op_arg, val_tape, par2val_index
         val_op_arg.resize(2);
         for(size_t i = 0; i < 2; ++i)
         {  if( is_var[i] )
               val_op_arg[i] = var2val_index[ var_op_arg[i] ];
            else
               val_op_arg[i] = ensure_par2val_index( var_op_arg[i] );
         }
         //
         // val_op
         op_enum_t val_op = var_op2val_op[var_op];
         CPPAD_ASSERT_UNKNOWN( val_op < number_op_enum );
         //
         // record_op, val_index
         val_index = val_tape.record_op(val_op, val_op_arg);
         //
         // var2val_index
         var2val_index[i_var] = val_index;
      }
      // ----------------------------------------------------------------------
      else if( is_compare )
      {  //
         // left_index, right_index
         addr_t left_index, right_index;
         switch( var_op )
         {
            // default
            default:
            CPPAD_ASSERT_UNKNOWN(false);
            left_index  = 0; // to avoid compiler warning
            right_index = 0;
            break;

            // both nodes parameters
            case local::EqppOp:
            case local::NeppOp:
            case local::LtppOp:
            case local::LeppOp:
            left_index  = par2val_index[ var_op_arg[0] ];
            right_index = par2val_index[ var_op_arg[1] ];
            break;

            // first node parameter, second variable
            case local::EqpvOp:
            case local::NepvOp:
            case local::LtpvOp:
            case local::LepvOp:
            left_index  = par2val_index[ var_op_arg[0] ];
            right_index = var2val_index[ var_op_arg[1] ];
            break;

            // first node variable, second parameter
            case local::LtvpOp:
            case local::LevpOp:
            left_index  = var2val_index[ var_op_arg[0] ];
            right_index = par2val_index[ var_op_arg[1] ];
            break;

            // both nodes variables
            case local::EqvvOp:
            case local::NevvOp:
            case local::LtvvOp:
            case local::LevvOp:
            left_index  = var2val_index[ var_op_arg[0] ];
            right_index = var2val_index[ var_op_arg[1] ];
            break;

         }
         // compare_enum
         local::val_graph::compare_enum_t compare_enum;
         // Set graph_op
         switch( var_op )
         {
            // default
            default:
            CPPAD_ASSERT_UNKNOWN(false);
            // set to avoid comiler warning
            compare_enum = local::val_graph::number_compare_enum;
            break;

            case local::EqppOp:
            case local::EqpvOp:
            case local::EqvvOp:
            compare_enum = local::val_graph::compare_eq_enum;
            break;

            case local::NeppOp:
            case local::NepvOp:
            case local::NevvOp:
            compare_enum = local::val_graph::compare_ne_enum;
            break;

            case local::LtppOp:
            case local::LtpvOp:
            case local::LtvpOp:
            case local::LtvvOp:
            compare_enum = local::val_graph::compare_lt_enum;
            break;

            case local::LeppOp:
            case local::LepvOp:
            case local::LevpOp:
            case local::LevvOp:
            compare_enum = local::val_graph::compare_le_enum;
            break;

         }
         //
         // tape
         val_index = val_tape.record_comp_op(
            compare_enum, left_index, right_index
         );
         CPPAD_ASSERT_UNKNOWN(val_index == 0); // no result for this operator
      }
      // ----------------------------------------------------------------------
      else switch(var_op)
      {
         default:
         CPPAD_ASSERT_KNOWN(false,
            "val_graph::fun2val: This variable operator not yet implemented"
         );
         break;

         // ParOp:
         // val_tape, par2val_index, var2val_index
         case local::ParOp:
         val_index            = ensure_par2val_index( var_op_arg[0] );
         var2val_index[i_var] = val_index;
         break;

         // EndOp:
         case local::EndOp:
         more_operators = false;
         break;

         // InvOp:
         // The independent variable indices are already assigned
         case local::InvOp:
         break;
         //
         // DisOp
         case local::DisOp:
         {  // val_tape, var2val_index
            val_op_arg.resize(1);
            addr_t discrete_index = var_op_arg[0];
            val_op_arg[0]         = var2val_index[ var_op_arg[1] ];
            val_index             = val_tape.record_dis_op(
               discrete_index, val_op_arg[0]
            );
            var2val_index[i_var]  = val_index;
         }
         break;
         // --------------------------------------------------------------
         case local::LdpOp:
         {  addr_t which_vector = vec_offset2index( var_op_arg[0] );
            addr_t vector_index = ensure_par2val_index( var_op_arg[1] );
            //
            var2val_index[i_var] = val_tape.record_load_op(
               which_vector, vector_index
            );
         }
         break;
         //
         case local::LdvOp:
         {  addr_t which_vector = vec_offset2index( var_op_arg[0] );
            addr_t vector_index = var2val_index[ var_op_arg[1] ];
            //
            var2val_index[i_var] = val_tape.record_load_op(
               which_vector, vector_index
            );
         }
         break;
         //
         case local::StppOp:
         {  addr_t which_vector = vec_offset2index( var_op_arg[0] );
            addr_t vector_index = ensure_par2val_index( var_op_arg[1] );
            addr_t value_index  = ensure_par2val_index( var_op_arg[2] );
            //
            val_tape.record_store_op(
               which_vector, vector_index, value_index
            );
         }
         break;
         //
         case local::StpvOp:
         {  addr_t which_vector = vec_offset2index( var_op_arg[0] );
            addr_t vector_index = ensure_par2val_index( var_op_arg[1] );
            addr_t value_index  = var2val_index[ var_op_arg[2] ];
            //
            val_tape.record_store_op(
               which_vector, vector_index, value_index
            );
         }
         break;
         //
         case local::StvpOp:
         {  addr_t which_vector = vec_offset2index( var_op_arg[0] );
            addr_t vector_index = var2val_index[ var_op_arg[1] ];
            addr_t value_index  = ensure_par2val_index( var_op_arg[2] );
            //
            val_tape.record_store_op(
               which_vector, vector_index, value_index
            );
         }
         break;
         //
         case local::StvvOp:
         {  addr_t which_vector = vec_offset2index( var_op_arg[0] );
            addr_t vector_index = var2val_index[ var_op_arg[1] ];
            addr_t value_index  = var2val_index[ var_op_arg[2] ];
            //
            val_tape.record_store_op(
               which_vector, vector_index, value_index
            );
         }
         break;
         // --------------------------------------------------------------
         case local::PriOp:
         {  //
            // before, after
            std::string before( play_.GetTxt( size_t( var_op_arg[2] ) ) );
            std::string after(  play_.GetTxt( size_t( var_op_arg[4] ) ) );
            //
            // flag_index
            addr_t flag_index;
            if( var_op_arg[0] & 1 )
               flag_index = var2val_index[ var_op_arg[1] ];
            else
               flag_index = ensure_par2val_index( var_op_arg[1] );
            //
            // value_index
            addr_t value_index;
            if( var_op_arg[0] & 1 )
               value_index = var2val_index[ var_op_arg[3] ];
            else
               value_index = ensure_par2val_index( var_op_arg[3] );
            //
            val_index = val_tape.record_pri_op(
               before, after, flag_index, value_index
            );
         }
         CPPAD_ASSERT_UNKNOWN(val_index == 0); // no result for this operator
         break;
         // --------------------------------------------------------------
         case local::CSumOp:
         {  //
            // add, sub
            Vector<addr_t> add, sub;
            //
            // add: constant term
            add.push_back( ensure_par2val_index( var_op_arg[0] ) );
            //
            // add: variables
            for(addr_t i = 5; i < var_op_arg[1]; ++i)
               add.push_back( var2val_index[ var_op_arg[i] ] );
            //
            // sub: variables
            for(addr_t i = var_op_arg[1]; i < var_op_arg[2]; ++i)
               sub.push_back( var2val_index[ var_op_arg[i] ] );
            //
            // add: dynamic parameters
            for(addr_t i = var_op_arg[2]; i < var_op_arg[3]; ++i)
               add.push_back( ensure_par2val_index( var_op_arg[i] ) );
            //
            // sub: dynamic parameters
            for(addr_t i = var_op_arg[3]; i < var_op_arg[4]; ++i)
               sub.push_back( ensure_par2val_index( var_op_arg[i] ) );
            //
            // val_tape, var2val_index
            var2val_index[i_var] = val_tape.record_csum_op(add, sub);
         }
         itr.correct_before_increment();
         break;
         // --------------------------------------------------------------
         case local::CExpOp:
         {  // cop, left, right, if_true, if_false
            CompareOp cop = CompareOp( var_op_arg[0] );
            addr_t left, right, if_true, if_false;
            if( var_op_arg[1] & 1 )
               left = var2val_index[ var_op_arg[2] ];
            else
               left = ensure_par2val_index( var_op_arg[2] );
            if( var_op_arg[1] & 2 )
               right = var2val_index[ var_op_arg[3] ];
            else
               right = ensure_par2val_index( var_op_arg[3] );
            if( var_op_arg[1] & 4 )
               if_true = var2val_index[ var_op_arg[4] ];
            else
               if_true = ensure_par2val_index( var_op_arg[4] );
            if( var_op_arg[1] & 8 )
               if_false = var2val_index[ var_op_arg[5] ];
            else
               if_false = ensure_par2val_index( var_op_arg[5] );
            //
            // compare_enum
            local::val_graph::compare_enum_t compare_enum;
            switch( cop )
            {  case CompareLt:
               compare_enum = local::val_graph::compare_lt_enum;
               break;

               case CompareLe:
               compare_enum = local::val_graph::compare_le_enum;
               break;

               case CompareEq:
               compare_enum = local::val_graph::compare_eq_enum;
               break;

               case CompareGe:
               compare_enum = local::val_graph::compare_lt_enum;
               std::swap(if_true, if_false);
               break;

               case CompareGt:
               compare_enum = local::val_graph::compare_le_enum;
               std::swap(if_true, if_false);
               break;

               case CompareNe:
               compare_enum = local::val_graph::compare_eq_enum;
               std::swap(if_true, if_false);
               break;

               default:
               CPPAD_ASSERT_UNKNOWN(false);
               // set to avoid compiler warning
               compare_enum = local::val_graph::number_compare_enum;
               break;
            }
            val_index = val_tape.record_cexp_op(
               compare_enum, left, right, if_true, if_false
            );
            var2val_index[i_var] = val_index;
         }
         break;

         // --------------------------------------------------------------
         case local::FunapOp:
         val_op_arg.push_back( ensure_par2val_index( var_op_arg[0] ) );
         break;

         case local::FunavOp:
         CPPAD_ASSERT_UNKNOWN( size_t(var_op_arg[0]) <= i_var );
         val_op_arg.push_back( var2val_index[var_op_arg[0]] );
         break;

         case local::FunrpOp:
         var_op_res.push_back( var_op_arg[0] );
         res_is_par.push_back(true);
         break;

         case local::FunrvOp:
         var_op_res.push_back( addr_t(i_var) );
         res_is_par.push_back(false);
         break;

         case local::AFunOp:
         in_atomic_call = ! in_atomic_call;
         if( in_atomic_call )
         {  val_op_arg.resize(0);
            var_op_res.resize(0);
            res_is_par.resize(0);
         }
         else
         {  // atomic_index, call_id, n_res
            addr_t atomic_index = var_op_arg[0];
            addr_t call_id      = var_op_arg[1];
            addr_t n_res        = var_op_arg[3];
# ifndef NDEBUG
            addr_t n_arg        = var_op_arg[2];
            CPPAD_ASSERT_UNKNOWN( val_op_arg.size() == size_t(n_arg) );
            CPPAD_ASSERT_UNKNOWN( var_op_res.size() == size_t(n_res) );
            CPPAD_ASSERT_UNKNOWN( res_is_par.size() == size_t(n_res) );
# endif
            // var2val_index
            addr_t res_index = val_tape.record_call_op(
               atomic_index, call_id, n_res, val_op_arg
            );
            for(addr_t i = 0; i < n_res; ++i)
            {  if( res_is_par[i] )
                  par2val_index[ var_op_res[i] ] = res_index + i;
               else
                  var2val_index[ var_op_res[i] ] = res_index + i;
            }
         }
         break;
      }
   }
   // dep_vec
   size_t n_dependent = dep_taddr_.size();
   Vector<addr_t> dep_vec(n_dependent);
   for(size_t i = 0; i < n_dependent; ++i)
      dep_vec[i] = var2val_index[ dep_taddr_[i] ];
   val_tape.set_dep( dep_vec );
}

} // END_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
# endif
