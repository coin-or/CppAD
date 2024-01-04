# ifndef  CPPAD_LOCAL_VAL_GRAPH_VAL_OPTIMIZE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VAL_OPTIMIZE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin val_optimize dev}


Optimize the Value Graph Corresponding to This Function
#######################################################

Prototype
*********
{xrst_literal
   // BEGIN_VAL_OPTIMIZE
   // END_VAL_OPTIMIZE
}

options
=======
See :ref:`optimize@options` .

{xrst_end val_optimize}
*/

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/optimize/extract_option.hpp>


namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_VAL_OPTIMIZE
template <class Base, class RecBase>
void ADFun<Base, RecBase>::val_optimize(const std::string& options)
// END_VAL_OPTIMIZE
{  //
   // compare_op, cumulative_sum_op, print_for_op,
   local::optimize::options_t result = local::optimize::extract_option(options);
   bool compare_op          = result.compare_op;
   bool cumulative_sum_op   = result.cumulative_sum_op;
   bool print_for_op        = result.print_for_op;
   //
   CPPAD_ASSERT_UNKNOWN( result.val_graph == true );
   CPPAD_ASSERT_KNOWN( result.conditional_skip == false,
      "optimize options: "
      "val_graph is present and no_conditional_skip is not present"
   );
   // n_dyn_ind, n_var_ind
   size_t n_dyn_ind = size_dyn_ind();
   size_t n_var_ind = Domain();
   //
   // dyn_ind, var_ind
   using CppAD::local::val_graph::Vector;
   Vector<size_t> dyn_ind(n_dyn_ind), var_ind(n_var_ind);
   for(size_t j = 0; j < n_dyn_ind; ++j)
      dyn_ind[j] = j;
   for(size_t j = 0; j < n_var_ind; ++j)
      var_ind[j] = n_dyn_ind + j;
   //
   // val_tape
   // value operator representaiton corresonding to this function
   local::val_graph::tape_t<Base> val_tape;
   fun2val(val_tape);
   //
   //
   // this
   // free all the memory associated with this function
   {  ADFun<Base> g;
      swap(g);
   }
   /*
   Vector<Base> val_vec( val_tape.n_val() );
   for(addr_t i = 0; i < val_tape.n_ind(); ++i)
      val_vec[i] = Base(i + 1);
   bool   trace = true;
   val_tape.eval(trace, val_vec);
   */
   // val_tape: renumber
   val_tape.renumber();
   //
   // val_tape: fold_con();
   //
   // val_tape: summation
   if( cumulative_sum_op )
      val_tape.summation();
   //
   // val_tape: dead_code, val_use
   if( compare_op )
      val_tape.set_option("keep_compare", "true");
   else
      val_tape.set_option("keep_compare", "false");
   if( print_for_op )
      val_tape.set_option("keep_print", "true");
   else
      val_tape.set_option("keep_print", "false");
   vectorBool use_val = val_tape.dead_code();
   /*
   val_vec.resize( val_tape.n_val() );
   val_tape.eval(trace, val_vec);
   */
   // this
   // convert optimized value graph to fun
   val2fun(val_tape, dyn_ind, var_ind, use_val);
   //
   // no collision limit in value graph representaiton.
   return;
}

} // END_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
# endif
