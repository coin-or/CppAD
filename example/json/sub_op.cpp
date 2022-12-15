// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin json_sub_op.cpp}

Json sub Operator: Example and Test
###################################

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end json_sub_op.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool sub_op(void)
{  bool ok = true;
   using CppAD::vector;
   using CppAD::AD;
   //
   // AD graph example
   // node_1 : p[0]
   // node_2 : p[1]
   // node_3 : x[0]
   // node_4 : p[0] - p[1]
   // node_5 : x[0] - ( p[0] - p[1] )
   // y[0]   = x[0] - ( p[0] - p[1] )
   // use single quote to avoid having to escape double quote
   std::string json =
      "{\n"
      "   'function_name'  : 'sub_op example',\n"
      "   'op_define_vec'  : [ 2, [\n"
      "       { 'op_code':1, 'name':'add', 'n_arg':2 } ,\n"
      "       { 'op_code':2, 'name':'sub', 'n_arg':2 } ]\n"
      "   ],\n"
      "   'n_dynamic_ind'  : 2,\n"
      "   'n_variable_ind' : 1,\n"
      "   'constant_vec'   : [ 0, [ ] ],\n"
      "   'op_usage_vec'   : [ 2, [\n"
      "       [ 2, 1, 2 ] ,\n" // p[0] - p[1]
      "       [ 2, 3, 4 ] ]\n" // x[0] - ( p[0] - p[1] )
      "   ],\n"
      "   'dependent_vec' : [ 1, [5] ] \n"
      "}\n";
   // Convert the single quote to double quote
   for(size_t i = 0; i < json.size(); ++i)
      if( json[i] == '\'' ) json[i] = '"';
   //
   // f(x, p) = x_0 - ( p_0 - p_1 )
   CppAD::ADFun<double> f;
   f.from_json(json);
   //
   ok &= f.Domain() == 1;
   ok &= f.Range() == 1;
   ok &= f.size_dyn_ind() == 2;
   //
   // set independent variables and parameters
   vector<double> p(2), x(1);
   p[0] = 2.0;
   p[1] = 3.0;
   x[0] = 4.0;
   //
   // compute y = f(x, p)
   f.new_dynamic(p);
   vector<double> y = f.Forward(0, x);
   //
   // check result
   ok &= y[0] == x[0] - ( p[0] - p[1] );
   // -----------------------------------------------------------------------
   // Convert to Json graph and back again
   json = f.to_json();
   // std::cout << "json = " << json;
   f.from_json(json);
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
   ok &= y[0] == x[0] - ( p[0] - p[1] );
   //
   return ok;
}
// END C++
