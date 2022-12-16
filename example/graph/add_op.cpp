// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin graph_add_op.cpp}

C++ AD Graph add Operator: Example and Test
###########################################

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end graph_add_op.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool add_op(void)
{  bool ok = true;
   using std::string;
   //
   // AD graph example
   // node_1 : p[0]
   // node_2 : p[1]
   // node_3 : x[0]
   // node_4 : p[0] + p[1]
   // node_5 : x[0] + ( p[0] + p[1] )
   // y[0]   = x[0] + ( p[0] + p[1] )
   //
   // C++ graph object
   CppAD::cpp_graph graph_obj;
   //
   // operator being used
   CppAD::graph::graph_op_enum op_enum;
   //
   // set scalars
   graph_obj.function_name_set("add_op example");
   size_t n_dynamic_ind = 2;
   graph_obj.n_dynamic_ind_set(n_dynamic_ind);
   size_t n_variable_ind = 1;
   graph_obj.n_variable_ind_set(n_variable_ind);
   //
   // node_4 : p[0] + p[1]
   op_enum = CppAD::graph::add_graph_op;
   graph_obj.operator_vec_push_back(op_enum);
   graph_obj.operator_arg_push_back(1);
   graph_obj.operator_arg_push_back(2);
   //
   // node_5 : x[0] + ( p[0] + p[1] )
   graph_obj.operator_vec_push_back(op_enum);
   graph_obj.operator_arg_push_back(3);
   graph_obj.operator_arg_push_back(4);
   //
   // y[0]   = x[0] + ( p[0] + p[1] )
   graph_obj.dependent_vec_push_back(5);
   //
   // f(x, p) = x_0 + ( p_0 + p_1 )
   CppAD::ADFun<double> f;
   f.from_graph(graph_obj);
   ok &= f.Domain() == 1;
   ok &= f.Range() == 1;
   ok &= f.size_dyn_ind() == 2;
   //
   // set independent variables and parameters
   CPPAD_TESTVECTOR(double) p(2), x(1);
   p[0] = 2.0;
   p[1] = 3.0;
   x[0] = 4.0;
   //
   // compute y = f(x, p)
   f.new_dynamic(p);
   CPPAD_TESTVECTOR(double) y = f.Forward(0, x);
   //
   // check result
   ok &= y[0] == x[0] + ( p[0] + p[1] );
   // -----------------------------------------------------------------------
   // Convert function to graph and back again
   f.to_graph(graph_obj);
   f.from_graph(graph_obj);
   // -----------------------------------------------------------------------
   ok &= f.Domain() == 1;
   ok &= f.Range() == 1;
   ok &= f.size_dyn_ind() == 2;
   //
   // set independent variables and parameters
   p[0] = 2.0;
   p[1] = 3.0;
   x[0] = 4.0;
   //
   // compute y = f(x, p)
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   // check result
   ok &= y[0] == x[0] + ( p[0] + p[1] );
   //
   return ok;
}
// END C++
