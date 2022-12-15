// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin graph_atom4_op.cpp}

C++ AD Graph Atomic Four Functions: Example and Test
####################################################

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end graph_atom4_op.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
   class atomic_int_pow : public CppAD::atomic_four<double> {
   public:
      atomic_int_pow(void) : CppAD::atomic_four<double>("int_pow")
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
         // taylor_y
         taylor_y[0] = 1.0;
         for(size_t i = 0; i < call_id; ++i)
            taylor_y[0] *= taylor_x[0];
         //
         return true;
      }
   };
}

bool atom4_op(void)
{  bool ok = true;
   using std::string;
   //
   // reciprocal
   atomic_int_pow int_pow;
   // -----------------------------------------------------------------------
   //
   // This function has an atomic function operator with name int_pow
   // node_1 : p[0]
   // node_2 : x[0]
   // node_3 : p[0] + x[0]
   // node_4 : int_pow( p[0] + x[0] )
   // y[0]   = ( p[0] + x[0] ) ** call_id
   //
   // call_id
   size_t call_id = 2;
   //
   // C++ graph object
   CppAD::cpp_graph graph_obj;
   graph_obj.initialize();
   //
   // operator being used
   CppAD::graph::graph_op_enum op_enum;
   //
   graph_obj.function_name_set("g(p; x)");
   graph_obj.n_dynamic_ind_set(1);
   graph_obj.n_variable_ind_set(1);
   //
   // node_3 : p[0] + x[0]
   op_enum = CppAD::graph::add_graph_op;
   graph_obj.operator_vec_push_back(op_enum);
   graph_obj.operator_arg_push_back(1);
   graph_obj.operator_arg_push_back(2);
   //
   // node_4 : f( p[0] + x[0] )
   //
   // name_index, n_result, n_arg come before first_node
   size_t name_index = graph_obj.atomic_name_vec_size();
   graph_obj.atomic_name_vec_push_back("int_pow");
   //
   op_enum = CppAD::graph::atom4_graph_op;
   graph_obj.operator_vec_push_back(op_enum);
   graph_obj.operator_arg_push_back(name_index);  // name_index
   graph_obj.operator_arg_push_back(call_id);     // call_id
   graph_obj.operator_arg_push_back(1);           // n_result
   graph_obj.operator_arg_push_back(1);           // n_node_arg
   graph_obj.operator_arg_push_back(3);           // first and last node arg
   //
   // y[0]  = int_pow( p[0] + x[0] ) = ( p[0] + x[0] ) ** call_id
   graph_obj.dependent_vec_push_back(4);
   // ------------------------------------------------------------------------
   CppAD::ADFun<double> g;
   g.from_graph(graph_obj);
   // ------------------------------------------------------------------------
   ok &= g.Domain() == 1;
   ok &= g.Range() == 1;
   ok &= g.size_dyn_ind() == 1;
   //
   // set p in g(p; x)
   CPPAD_TESTVECTOR(double) p(1);
   p[0] = 2.0;
   g.new_dynamic(p);
   //
   // evalute g(p; x)
   CPPAD_TESTVECTOR(double) x(1), y(1);
   x[0] = 3.0;
   y    = g.Forward(0, x);
   //
   // check value
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   double check = std::pow( p[0] + x[0], double(call_id) );
   ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
   // ------------------------------------------------------------------------
   g.to_graph(graph_obj);
   g.from_graph(graph_obj);
   // ------------------------------------------------------------------------
   ok &= g.Domain() == 1;
   ok &= g.Range() == 1;
   ok &= g.size_dyn_ind() == 1;
   //
   // set p in g(p; x)
   p[0] = 4.0;
   g.new_dynamic(p);
   //
   // evalute g(p; x)
   x[0] = 5.0;
   y    = g.Forward(0, x);
   //
   // check value
   check = std::pow( p[0] + x[0], double(call_id) );
   ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
   // ------------------------------------------------------------------------
   return ok;
}
// END C++
