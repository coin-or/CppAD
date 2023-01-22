# ifndef CPPAD_VAL_GRAPH_FUN2VAL_HPP
# define CPPAD_VAL_GRAPH_FUN2VAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// --------------------------------------------------------------------------

template <class Base, class RecBase>
void CppAD::ADFun<Base, RecBase>:fun2val(
   CppAD::local::val_graph::tape_t& val_tape  )
{  //
   // n_dynamic_ind
   // number of independent dynamic parameters
   size_t n_dynmaic_ind = play_.num_dynamic_ind();
   //
   // n_variable_ind
   // number of indepedent variables
   size_t n_variable_ind = ind_taddr_.size();
   //
   // n_val_ind
   // number of indepedent valuse
   size_t n_val_ind = n_dynamic_ind + n_variable_ind;
   //
}
// --------------------------------------------------------------------------
# endif
