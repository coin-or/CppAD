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

{xrst_end val_optimize}
*/

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/val_graph/tape.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_VAL_OPTIMIZE
template <class Base, class RecBase>
void ADFun<Base, RecBase>::val_optimize(void)
// END_VAL_OPTIMIZE
{  //
   // n_dyn_ind, n_var_ind
   size_t n_dyn_ind = size_dyn_ind();
   size_t n_var_ind = Domain();
   //
   // dyn_ind, var_ind
   CppAD::vector<size_t> dyn_ind(n_dyn_ind), var_ind(n_var_ind);
   for(size_t j = 0; j < n_dyn_ind; ++j)
      dyn_ind[j] = j;
   for(size_t j = 0; j < n_var_ind; ++j)
      var_ind[j] = n_dyn_ind + j;
   //
   // val_tape
   // value operator representaiton corresonding to this funciton
   local::val_graph::tape_t<Base> val_tape;
   fun2val(val_tape);
   //
   //
   // this
   // free all the memory associated with this funciton
   {  ADFun<Base> g;
      swap(g);
   }
   //
   // val_tape: optimize
   val_tape.renumber();
   // val_tape.fold_con();
   val_tape.summation();
   val_tape.dead_code();
   /*
   CppAD::vector<Base> val_vec( val_tape.n_val() );
   for(addr_t i = 0; i < val_tape.n_ind(); ++i)
      val_vec[i] = Base(i + 1);
   size_t compare_false = 0;
   bool   trace = true;
   val_tape.eval(trace, val_vec, compare_false);
   */
   // this
   // convert optimized value graph to fun
   val2fun(val_tape, dyn_ind, var_ind);
   //
   // no collision limit in value graph representaiton.
   return;
}

} // END_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
# endif
