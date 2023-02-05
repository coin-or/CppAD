# ifndef  CPPAD_LOCAL_VAL_GRAPH_FUN2VAL_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_FUN2VAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
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
It is also :ref:`val_graph_tape@Value` type for the tape.

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .

val_tape
********
This is a :ref:`val_graph_tape-name` object.
The input contents of the tape does not matter.
Upon return it is a
:ref:`val_graph-name` representation of the function.
The independent dynamic parameters have the same order as in
*afun* and come first (in the value graph independent vector).
The independent variables have the same order as in
*afun* and come after the independent dynamic parameters.

afun
****
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
# include <cppad/local/val_graph/type_var_op.hpp>
# include <cppad/local/val_graph/type_dyn_op.hpp>

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
   // op_code_dyn, number_dyn, OpCode
   using local::op_code_dyn;
   op_code_dyn number_dyn = local::number_dyn;
   using local::OpCode;
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
   var_op2val_op[local::NegOp] = local::val_graph::neg_op_enum;
   //
   // add
   var_op2val_op[local::AddpvOp] = local::val_graph::add_op_enum;
   var_op2val_op[local::AddvvOp] = local::val_graph::add_op_enum;
   // sub
   var_op2val_op[local::SubpvOp] = local::val_graph::sub_op_enum;
   var_op2val_op[local::SubvpOp] = local::val_graph::sub_op_enum;
   var_op2val_op[local::SubvvOp] = local::val_graph::sub_op_enum;
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
   size_t n_val_ind = n_dynamic_ind + n_variable_ind;
   //
   // nan_val_index
   // initialize value vector tape
   addr_t nan_val_index = val_tape.set_ind( n_val_ind );
   CPPAD_ASSERT_UNKNOWN( size_t(nan_val_index) == n_val_ind );
   //
   // par2val_index
   // Initialize mapping from parameter index to index in value vector.
   Vector<addr_t> par2val_index(n_parameter);
   for(size_t i = 0; i < n_parameter; ++i)
      par2val_index[i] = invalid_addr_t;
   for(addr_t i = 0; i < addr_t( n_dynamic_ind ); ++i)
      par2val_index[i + 1] = i;
   //
   // val_op_arg, var_op_res
   Vector<addr_t> val_op_arg, var_op_res;
   //
   // i_arg
   // initial index in dyn_par_arg
   size_t i_arg = 0;
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
      size_t n_arg;
      if( is_unary || is_binary )
      {  //
         // val_tape, val_index, par2val_index
         n_arg = num_arg_dyn(dyn_op);
         val_op_arg.resize(n_arg);
         for(size_t i = 0; i < n_arg; ++i)
         {  addr_t par_index = dyn_par_arg[i_arg + i];
            if( par2val_index[par_index] != invalid_addr_t )
               val_op_arg[i] = par2val_index[par_index];
            else
            {  Base constant = parameter[par_index];
               val_op_arg[i] = val_tape.record_con_op( constant );
               par2val_index[par_index] = val_op_arg[i];
            }
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
         //
         // result_dyn
         // This is a place holder for multiple result operators
         case local::result_dyn:
         CPPAD_ASSERT_UNKNOWN( val_index != invalid_addr_t );
         n_arg      = 0;
         val_index += 1;
         break;
         //
         // atom_dyn
         case local::atom_dyn:
         {  //
            // atomic_index, call_id, n_call_arg, n_res
            size_t atomic_index = size_t( dyn_par_arg[i_arg + 0] );
            size_t call_id      = size_t( dyn_par_arg[i_arg + 1] );
            size_t n_call_arg   = size_t( dyn_par_arg[i_arg + 2] );
            size_t n_res        = size_t( dyn_par_arg[i_arg + 3] );
            // num_dyn          = size_t( dyn_par_arg[i_age + 4] );
            //
            // val_op_arg
            val_op_arg.resize( n_call_arg );
            for(size_t i = 0; i < n_call_arg; i++)
            {  addr_t par_index = dyn_par_arg[i_arg + i + 5];
               if( par2val_index[par_index] != invalid_addr_t )
                  val_op_arg[i] = par2val_index[par_index];
               else
               {  // val_tape, val_op_arg, par2val_index
                  Base constant = parameter[par_index];
                  val_op_arg[i] = val_tape.record_con_op( constant );
                  par2val_index[par_index] = val_op_arg[i];
               }
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
      local::OpCode    var_op;
      const            addr_t* var_op_arg;
      size_t           i_var;
      (++itr).op_info(var_op, var_op_arg, i_var);
      //
      // is_unary, is_bianry
      bool is_unary   = local::val_graph::unary_var_op(var_op);
      bool is_binary  = local::val_graph::binary_var_op(var_op);
      bool is_compare = local::val_graph::compare_var_op(var_op);
      //
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
            is_var[0]   = false;
            is_var[1]   = true;
            break;

            // first argument a variable, second argument a parameter
            case local::SubvpOp:
            is_var[0]   = true;
            is_var[1]   = false;
            break;

            // first argument a variable, second argument a variable
            case local::AddvvOp:
            case local::SubvvOp:
            is_var[0]   = true;
            is_var[1]   = true;
            break;
         }
         //
         // val_tape, par2val_index
         val_op_arg.resize(2);
         for(size_t i = 0; i < 2; ++i)
         {  if( is_var[i] )
               val_op_arg[i] = var2val_index[ var_op_arg[i] ];
            else if( par2val_index[ var_op_arg[i] ] != invalid_addr_t )
               val_op_arg[i] = par2val_index[ var_op_arg[i] ];
            else
            {  Base constant = parameter[ var_op_arg[i] ];
               val_op_arg[i] = val_tape.record_con_op( constant );
               par2val_index[ var_op_arg[i] ] = val_op_arg[i];
            }
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
         break;
      }
      else switch(var_op)
      {
         default:
         CPPAD_ASSERT_KNOWN(false,
            "val_graph::fun2val: This variable operator not yet implemented"
         );
         break;

         // ParOp:
         case local::ParOp:
         if( par2val_index[ var_op_arg[0] ] != invalid_addr_t )
            val_index = par2val_index[ var_op_arg[0] ];
         else
         {  Base constant = parameter[ var_op_arg[0] ];
            val_index     = val_tape.record_con_op( constant );
            par2val_index[ var_op_arg[0] ] = val_index;
         }
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

         // --------------------------------------------------------------
         case local::FunapOp:
         val_op_arg.push_back( par2val_index[var_op_arg[0]] );
         break;

         case local::FunavOp:
         CPPAD_ASSERT_UNKNOWN( size_t(var_op_arg[0]) <= i_var );
         val_op_arg.push_back( var2val_index[var_op_arg[0]] );
         break;

         case local::FunrpOp:
         var_op_res.push_back( var_op_arg[0] );
         break;

         case local::FunrvOp:
         var_op_res.push_back( addr_t(i_var) );
         break;

         case local::AFunOp:
         in_atomic_call = ! in_atomic_call;
         if( in_atomic_call )
         {  val_op_arg.resize(0);
            var_op_res.resize(0);
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
# endif
            // var2val_index
            addr_t res_index = val_tape.record_call_op(
               atomic_index, call_id, n_res, val_op_arg
            );
            for(addr_t i = 0; i < n_res; ++i)
               var2val_index[ var_op_res[i] ] = res_index + i;
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
