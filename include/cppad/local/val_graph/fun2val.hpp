# ifndef  CPPAD_LOCAL_VAL_GRAPH_FUN2VAL_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_FUN2VAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// --------------------------------------------------------------------------

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/pod_vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

template <class Base, class RecBase>
void ADFun<Base, RecBase>::fun2val(
   local::val_graph::tape_t<Base>& val_tape  )
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
   dyn_op2val_op[local::sub_dyn] = local::val_graph::sub_op_enum;
   //
   // var_op2val_op
   Vector<op_enum_t> var_op2val_op(local::NumberOp);
   for(size_t i = 0; i < size_t(local::NumberOp); ++i)
      var_op2val_op[i] = number_op_enum; // invalid
   //
   // add
   var_op2val_op[local::AddpvOp] = local::val_graph::add_op_enum;
   var_op2val_op[local::AddvvOp] = local::val_graph::add_op_enum;
   // sub
   var_op2val_op[local::SubpvOp] = local::val_graph::sub_op_enum;
   var_op2val_op[local::SubvpOp] = local::val_graph::sub_op_enum;
   var_op2val_op[local::SubvvOp] = local::val_graph::sub_op_enum;
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
   // zero_val_index
   // initialize value vector tape
   addr_t zero_val_index = val_tape.set_ind( n_val_ind );
   CPPAD_ASSERT_UNKNOWN( size_t(zero_val_index) == n_val_ind );
   //
   // par2val_index
   // Initialize mapping from parameter index to index in value vector.
   Vector<addr_t> par2val_index(n_parameter);
   for(size_t i = 0; i < n_parameter; ++i)
      par2val_index[i] = invalid_addr_t;
   for(addr_t i = 0; i < addr_t( n_dynamic_ind ); ++i)
      par2val_index[i + 1] = i;
   //
   // val_op_arg
   Vector<addr_t> val_op_arg;
   //
   // i_arg
   // initial index in dyn_par_arg
   size_t i_arg = 0;
   //
   // val_tape
   // record dynamic parameter operations
   for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
   {  //
      // i_par
      size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
      //
      // dyn_op
      // operator for this dynamic parameter
      local::op_code_dyn dyn_op = local::op_code_dyn( dyn_par_op[i_dyn] );
      //
      // val_op
      op_enum_t val_op = dyn_op2val_op[dyn_op];
      CPPAD_ASSERT_KNOWN( val_op < number_op_enum ,
         "This dynamic operator not yet implemented"
      );
      //
      // n_arg
      // number of parameter arguments with exception of atom_dyn
      size_t n_arg = num_arg_dyn(dyn_op);
      //
      // val_op_arg
      val_op_arg.resize(n_arg);
      for(size_t i = 0; i < n_arg; ++i)
      {  size_t par_index = size_t( dyn_par_arg[i_arg + i] );
         if( par2val_index[par_index] != invalid_addr_t )
           val_op_arg[i] = par2val_index[par_index];
         else
         {  Base constant = parameter[par_index];
            val_op_arg[i] = val_tape.record_con_op( constant );
            par2val_index[par_index] = val_op_arg[i];
         }
      }
      //
      // record_op, val_index
      addr_t val_index = val_tape.record_op(val_op, val_op_arg);
      //
      // par2val_index
      par2val_index[i_par] = val_index;
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
   // itr, var_op, arg, i_var, is_var, more_operators
   local::play::const_sequential_iterator itr  = play_.begin();
   Vector<bool>       is_var(2);
   bool more_operators = true;
   //
   // val_tape
   // record variable operations
   while(more_operators)
   {  //
      //
      // var_op, arg, i_var
      local::OpCode    var_op;
      const            addr_t* arg;
      size_t           i_var;
      (++itr).op_info(var_op, arg, i_var);
      //
      // n_arg
      size_t n_arg = local::NumArg( var_op );
      //
      // n_res
      size_t n_res = local::NumRes( var_op );
      //
      switch( var_op )
      {
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

         // EndOp:
         case local::EndOp:
         more_operators = false;
         break;

         // InvOp:
         // The independent variable indices are already assigned
         case local::InvOp:
         n_res = 0;
         break;

         default:
         CPPAD_ASSERT_KNOWN( var_op > local::NumberOp,
            "This variable operator not yet implemented"
         );
      }
      //
      // val_op_arg
      val_op_arg.resize(n_arg);
      for(size_t i = 0; i < n_arg; ++i)
      {  if( is_var[i] )
            val_op_arg[i] = var2val_index[ arg[i] ];
         else if( par2val_index[ arg[i] ] != invalid_addr_t )
            val_op_arg[i] = par2val_index[ arg[i] ];
         else
         {  Base constant = parameter[ arg[i] ];
            val_op_arg[i] = val_tape.record_con_op( constant );
            par2val_index[ arg[i] ] = val_op_arg[i];
         }
      }
      if( n_res > 0 )
      {  CPPAD_ASSERT_UNKNOWN( n_res == 1 );
         //
         // val_op
         op_enum_t val_op = var_op2val_op[var_op];
         CPPAD_ASSERT_KNOWN( val_op < number_op_enum ,
            "This variable operator not yet implemented"
         );
         //
         // record_op, val_index
         addr_t val_index = val_tape.record_op(val_op, val_op_arg);
         //
         // var2val_index
         var2val_index[i_var] = val_index;
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
