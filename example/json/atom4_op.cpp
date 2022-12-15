// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin json_atom4_op.cpp}

Json Atomic Function Operator: Example and Test
###############################################

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end json_atom4_op.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
   class atomic_avg : public CppAD::atomic_four<double> {
   public:
      atomic_avg(void) : CppAD::atomic_four<double>("avg")
      { }
   private:
      // for_type
      bool for_type(
         size_t                                     call_id     ,
         const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
         CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
      {  type_y[0] = type_x[0];
         return true;
      }
      // forward
      bool forward(
         size_t                              call_id      ,
         const CppAD::vector<bool>&          select_y     ,
         size_t                              order_low    ,
         size_t                              order_up     ,
         const CppAD::vector<double>&        taylor_x     ,
         CppAD::vector<double>&              taylor_y     ) override
      {
         // order_up
         if( order_up != 0 )
            return false;
         //
         // n
         size_t n = taylor_x.size();
         if( n == 0 )
            return false;
         //
         // taylor_y
         double sum = 0.0;
         for(size_t j = 0; j < n; ++j)
            sum += taylor_x[j];
         taylor_y[0] = sum / double(n);
         //
         return true;
      }
   };
}

bool atom4_op(void)
{  bool ok = true;
   using CppAD::vector;
   using CppAD::AD;
   //
   // avg
   atomic_avg avg;
   //
   // -----------------------------------------------------------------------
   // g (p; x) = avg( [ p_0 * x_0, p_0 * x_1 ] )
   //          = p_0 * (x_0 + x_1) / 2
   //
   // This function has an atomic function operator with name avg
   // node_1 : p[0]
   // node_2 : x[0]
   // node_3 : x[1]
   // node_4 : p[0] * x[0]
   // node_5 : p[0] * x[1]
   // node_6 : avg( p[0] * x[0], p[0] * x[1] )
   // y[0]   = p[0] * ( x[0] + x[1] ) / 2
   std::string json =
      "{\n"
      "   'function_name'  : 'g(p; x)',\n"
      "   'op_define_vec'  : [ 2, [\n"
      "       { 'op_code':1, 'name':'atom4'          } ,\n"
      "       { 'op_code':2, 'name':'mul', 'n_arg':2 } ]\n"
      "   ],\n"
      "   'n_dynamic_ind'  : 1,\n"              // p[0]
      "   'n_variable_ind' : 2,\n"              // x[0], x[1]
      "   'constant_vec'   : [ 0, [ ] ],\n"
      "   'op_usage_vec'   : [ 3, [\n"
      "       [ 2, 1, 2 ]                      ,\n" // p[0] * x[0]
      "       [ 2, 1, 3 ]                      ,\n" // p[0] * x[1]
            // avg( p[0] * x[0], p[0] * x[1] )
      "       [ 1, 'avg', 0, 1, 2, [ 4, 5 ] ]  ]\n"
      "   ],\n"
      "   'dependent_vec' : [ 1, [6] ] \n"
      "}\n";
   // Convert the single quote to double quote
   for(size_t i = 0; i < json.size(); ++i)
      if( json[i] == '\'' ) json[i] = '"';
   // ------------------------------------------------------------------------
   CppAD::ADFun<double> g;
   g.from_json(json);
   // ------------------------------------------------------------------------
   ok &= g.Domain() == 2;
   ok &= g.Range() == 1;
   ok &= g.size_dyn_ind() == 1;
   //
   // set p in g(p; x)
   vector<double> p(1);
   p[0] = 2.0;
   g.new_dynamic(p);
   //
   // evalute g(p; x)
   vector<double> x(2), y(1);
   x[0] = 3.0;
   x[1] = 4.0;
   y    = g.Forward(0, x);
   //
   // check value
   ok &= y[0] == p[0] * (x[0] + x[1]) / 2.0;
   // ------------------------------------------------------------------------
   json = g.to_json();
   g.from_json(json);
   // ------------------------------------------------------------------------
   ok &= g.Domain() == 2;
   ok &= g.Range() == 1;
   ok &= g.size_dyn_ind() == 1;
   //
   // set p in g(p; x)
   p[0] = 5.0;
   g.new_dynamic(p);
   //
   // evalute g(p; x)
   x[0] = 6.0;
   x[1] = 7.0;
   y    = g.Forward(0, x);
   //
   // check value
   ok &= y[0] == p[0] * (x[0] + x[1]) / 2.0;
   // ------------------------------------------------------------------------
   return ok;
}
// END C++
