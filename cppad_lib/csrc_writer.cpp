// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cpp_csrc_writer dev}

Converts Cpp Graph to C Source
##############################

Syntax
******
``csrc_writer(%os%, %graph_obj%, %c_type%)``
``%``

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

os
**
The C source code corresponding to the function is written to *os* .

graph
*****
is the C++ graph representation of the function.

c_type
******
this specifies the type corresponding to the C source code and must
be one of the following:
``float`` , ``double`` , or ``long_double`` .

{xrst_end cpp_csrc_writer}
*/

# include <cppad/local/pod_vector.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/core/graph/cpp_graph.hpp>
# include <cppad/utility/to_string.hpp>

// documentation for this routine is in the file below
# include <cppad/local/graph/csrc_writer.hpp>

/* Optimizations 2DO:
1. Reduce size of v by removing x and y from the v vector.
2. Reduce size of v by reusing elemnents when they are no longer needed.
*/


namespace {
   //
   // element
   std::string element(const std::string& array_name, size_t array_index)
   {  return array_name + "[" + CppAD::to_string(array_index) + "]"; }
   //
   // binary_function
   void binary_function(
      std::ostream&  os           ,
      const char*    op_csrc      ,
      size_t         result_node  ,
      size_t         left_node    ,
      size_t         right_node   )
   {  os << "\t" + element("v", result_node) + " = ";
      os << op_csrc;
      os << "( " + element("v", left_node);
      os << ", " + element("v", right_node) + " );\n";
   }
   //
   // binary_operator
   void binary_operator(
      std::ostream&  os           ,
      const char*    op_csrc      ,
      size_t         result_node  ,
      size_t         left_node    ,
      size_t         right_node   )
   {  os << "\t" + element("v", result_node) + " = ";
      os << element("v", left_node) + " " + op_csrc + " ";
      os << element("v", right_node) + ";\n";
   }
   //
   // compare_operator
   void compare_operator(
      std::ostream&  os           ,
      const char*    op_csrc      ,
      size_t         left_node    ,
      size_t         right_node   )
   {  os << "\tif( " + element("v", left_node) + " " + op_csrc + " ";
      os << element("v", right_node) + " )\n";
      os << "\t\t++(*compare_change);\n";
   }
   //
   // unary_function
   void unary_function(
      std::ostream&  os           ,
      const char*    op_csrc      ,
      size_t         result_node  ,
      size_t         arg_node     )
   {  os << "\t" + element("v", result_node) + " = ";
      os << op_csrc;
      os << "( " + element("v", arg_node) + " );\n";
   }
   //
   // sum_operator
   void sum_operator(
      std::ostream&                os                  ,
      size_t                       result_node         ,
      const CppAD::vector<size_t>& arg_node            )
   {  std::string rhs = "\t" + element("v", result_node) + " = ";
      os << rhs;
      if( arg_node.size() == 1 )
      {  // can have subraction terms with no addition terms
         os << "(float_point_t) 0;\n";
         return;
      }
      for(size_t i = 0; i < arg_node.size(); ++i)
      {  if( i % 5 == 0 && i != 0 )
         {  os << "\n\t";
            for(size_t j = 0;  j < rhs.size() - 4; ++j)
               os << ' ';
         }
         if( 0 < i )
            os << " + ";
         os << element("v", arg_node[i]);
      }
      os << ";\n";
   }
   //
   // atomic_function
   void atomic_function(
      std::ostream&                os                  ,
      size_t                       result_node         ,
      const std::string&           atomic_name         ,
      size_t                       call_id             ,
      size_t                       n_result            ,
      const CppAD::vector<size_t>& arg_node            )
   {  using CppAD::to_string;
      std::string complete_name = "cppad_atomic_" + atomic_name;
      size_t nu = arg_node.size();
      size_t nw = n_result;
      os << "\t{\t// call " + atomic_name + "\n";
      os << "\t\tint flag;\n";
      os << "\t\tfloat_point_t " + element("u", nu) + ";\n";
      os << "\t\tfloat_point_t* w = v + " + to_string(result_node) + ";\n";
      for(size_t j = 0; j < nu; ++j)
      {  size_t i = arg_node[j];
         os << "\t\t" + element("u",j) + " = " + element("v",i) + ";\n";
      }
      //
      os << "\t\tflag = " + complete_name + "(";
      os << to_string(call_id) + ", ";
      os << to_string(nu) + ", u, ";
      os << to_string(nw) + ", w, ";
      os << "compare_change);\n";
      os << "\t\tif( flag == 1 || flag == 2 ) return 3;\n";
      os << "\t\tif( flag != 0 ) return flag;\n";
      //
      os << "\t}\n";
   }
   //
   // discrete_function
   void discrete_function(
      std::ostream&                os                  ,
      size_t                       result_node         ,
      const std::string&           discrete_name       ,
      size_t                       arg_node            )
   {  using CppAD::to_string;
      std::string complete_name = "cppad_discrete_" + discrete_name;
      os << "\t{\t// call " + discrete_name + "\n";
      os << "\t\t" + element("v", result_node) + " = ";
      os << complete_name + "( " + element("v", arg_node) + " );\n";
      os << "\t}\n";
   }

}

// BEGIN_PROTOTYPE
void CppAD::local::graph::csrc_writer(
   std::ostream&                             os                     ,
   const cpp_graph&                          graph_obj              ,
   const std::string&                        c_type                 )
// END_PROTOTYPE
{  using std::string;
   using CppAD::to_string;
   //
   // --------------------------------------------------------------------
   string function_name  = graph_obj.function_name_get();
   size_t n_dynamic_ind  = graph_obj.n_dynamic_ind_get();
   size_t n_variable_ind = graph_obj.n_variable_ind_get();
   size_t n_constant     = graph_obj.constant_vec_size();
   size_t n_dependent    = graph_obj.dependent_vec_size();
   size_t n_usage        = graph_obj.operator_vec_size();
   // --------------------------------------------------------------------
   CPPAD_ASSERT_KNOWN( function_name != "" ,
      "to_csrc: Cannot convert a function with no name"
   );
   //
   // graph_itr
   // defined here because not using as loop index
   cpp_graph::const_iterator graph_itr;
   //
   // first_result_node
   size_t first_result_node = 1 + n_dynamic_ind + n_variable_ind + n_constant;
   //
   // n_node
   size_t n_node = first_result_node;
   for(size_t op_index = 0; op_index < n_usage; ++op_index)
   {  // graph_itr
      if( op_index == 0 )
         graph_itr = graph_obj.begin();
      else
         ++graph_itr;
      //
      // nv
      cpp_graph::const_iterator::value_type itr_value = *graph_itr;
      n_node += itr_value.n_result;
   }
   //
   // includes
   os <<
      "// includes\n"
      "# include <stddef.h>\n"
      "# include <math.h>\n"
      "\n"
   ;
   //
   // typedefs
   string tmp_type = c_type;
   if( c_type == "long_double" )
      tmp_type = "long double";
   os <<
      "// typedefs\n"
      "typedef " + tmp_type + " float_point_t;\n"
      "\n"
   ;
   //
   // externals
   os << "// externals\n";
   size_t n_atomic = graph_obj.atomic_name_vec_size();
   for(size_t i_atomic = 0; i_atomic < n_atomic; ++i_atomic)
   {  string atomic_name = graph_obj.atomic_name_vec_get(i_atomic);
      os << "extern int cppad_atomic_" + atomic_name + "(\n";
      os <<
         "\tsize_t               call_id           ,\n"
         "\tsize_t               nx                ,\n"
         "\tconst float_point_t* x                 ,\n"
         "\tsize_t               ny                ,\n"
         "\tfloat_point_t*       y                 ,\n"
         "\tsize_t*              compare_change\n"
         ");\n"
      ;
   }
   size_t n_discrete = graph_obj.discrete_name_vec_size();
   for(size_t i_discrete = 0; i_discrete < n_discrete; ++i_discrete)
   {  string discrete_name = graph_obj.discrete_name_vec_get(i_discrete);
      os << "extern float_point_t cppad_discrete_" + discrete_name;
      os << "( float_point_t x );\n";
   }
   //
   // azmul
   os <<
      "// azmul\n"
      "static float_point_t azmul(float_point_t x, float_point_t y)\n"
      "{\tif( x == 0.0 ) return 0.0;\n"
      "\treturn x * y;\n"
      "}\n\n"
   ;
   //
   // sign
   os <<
      "// sign\n"
      "static float_point_t sign(float_point_t x)\n"
      "{\tif( x > 0.0 ) return 1.0;\n"
      "\tif( x == 0.0 ) return 0.0;\n"
      "\treturn -1.0;\n"
      "}\n\n"
   ;
   //
   // This JIT function
   os <<
      "// This JIT function\n"
# ifdef _MSC_VER
      "__declspec(dllexport) int __cdecl "
# else
      "int "
# endif
      "cppad_jit_" + function_name + "(\n"
      "\tsize_t               nx              ,\n"
      "\tconst float_point_t* x               ,\n"
      "\tsize_t               ny              ,\n"
      "\tfloat_point_t*       y               ,\n"
      "\tsize_t*              compare_change  )\n"
   ;
   //
   // begin function body
   os <<
      "{\t// begin function body \n"
      "\n"
   ;
   //
   // declare variables
   // v, i, nan
   os <<
      "\t// declare variables\n"
      "\tfloat_point_t v[" + to_string(n_node) + "];\n"
      "\tsize_t i;\n"
      "\n"
      "\t// check nx, ny\n"
   ;
   //
   // nx
   size_t nx = n_dynamic_ind + n_variable_ind;
   os << "\tif( nx != " + to_string(nx) + ") return 1;\n";
   //
   // ny
   size_t ny = n_dependent;
   os << "\tif( ny != " + to_string(ny) + ") return 2;\n";
   //
   // initialize
   // compare_change, v[0]
   os <<
      "\n"
      "\t// initialize\n"
      "\tv[0]            = NAN; // const \n"
   ;
   //
   // independent variables
   // set v[1+i] for i = 0, ..., nx-1"
   os <<
      "\n"
      "\t// independent variables\n"
      "\t// set v[1+i] for i = 0, ..., nx-1\n"
      "\tfor(i = 0; i < nx; ++i)\n"
      "\t\tv[1+i] = x[i];\n"
   ;
   //
   // cosntants
   // set v[1+nx+i] for i = 0, ..., nc-1
   size_t nc = n_constant;
   os <<
      "\n"
      "\t// constants\n"
      "\t// set v[1+nx+i] for i = 0, ..., nc-1\n"
      "\t// nc = " + to_string(nc) + "\n"
   ;
   for(size_t i = 0; i < nc; ++i)
   {  double c_i = graph_obj.constant_vec_get(i);
      os <<
         "\tv[1+nx+" + to_string(i) + "] = " + to_string(c_i) + ";\n"
      ;
   }
   //
   // result nodes
   // set v[1+nx+nc+i] for i = 0, ..., n_result_node-1
   size_t n_result_node = n_node - first_result_node;
   os <<
      "\n"
      "\t// result nodes\n"
      "\t// set v[1+nx+nc+i] for i = 0, ..., n_result_node-1\n"
      "\t// n_result_node = " + to_string(n_result_node) + "\n"
   ;
   //
   // result_node
   size_t result_node = first_result_node;
   //
   // op_index
   for(size_t op_index = 0; op_index < n_usage; ++op_index)
   {  //
      // graph_itr
      if( op_index == 0 )
         graph_itr = graph_obj.begin();
      else
         ++graph_itr;
      //
      // str_index, op_enum, call_id, n_result, arg_node
      cpp_graph::const_iterator::value_type itr_value = *graph_itr;
      const vector<size_t>& str_index( *itr_value.str_index_ptr );
      const vector<size_t>& arg_node(  *itr_value.arg_node_ptr  );
      graph_op_enum op_enum    = itr_value.op_enum;
      size_t        call_id    = itr_value.call_id;
      size_t        n_result   = itr_value.n_result;
      CPPAD_ASSERT_UNKNOWN( arg_node.size() > 0 );
      //
      // op_csrc
      const char* op_csrc = nullptr;
      switch( op_enum )
      {
         // -------------------------------------------------------------
         // binary functions
         // -------------------------------------------------------------
         case azmul_graph_op:
         case pow_graph_op:
         op_csrc = op_enum2name[op_enum];
         break;
         // -------------------------------------------------------------
         // binary operators
         // -------------------------------------------------------------
         case add_graph_op:
         op_csrc = "+";
         break;
         case div_graph_op:
         op_csrc = "/";
         break;
         case mul_graph_op:
         op_csrc = "*";
         break;
         case sub_graph_op:
         op_csrc = "-";
         break;
         // -------------------------------------------------------------
         // comparison operators
         // -------------------------------------------------------------
         case comp_eq_graph_op:
         op_csrc = "!="; // not eq
         break;
         case comp_le_graph_op:
         op_csrc = ">";  // not le
         break;
         case comp_lt_graph_op:
         op_csrc = ">="; // not lt
         break;
         case comp_ne_graph_op:
         op_csrc = "=="; // not ne
         break;
         // -------------------------------------------------------------
         // unary functions
         // -------------------------------------------------------------
         case abs_graph_op:
         op_csrc = "fabs";
         break;
         //
         case acos_graph_op:
         case acosh_graph_op:
         case asin_graph_op:
         case asinh_graph_op:
         case atan_graph_op:
         case atanh_graph_op:
         case cos_graph_op:
         case cosh_graph_op:
         case erf_graph_op:
         case erfc_graph_op:
         case exp_graph_op:
         case expm1_graph_op:
         case log1p_graph_op:
         case log_graph_op:
         case sign_graph_op:
         case sin_graph_op:
         case sinh_graph_op:
         case sqrt_graph_op:
         case tan_graph_op:
         case tanh_graph_op:
         op_csrc = op_enum2name[op_enum];
         break;

         // ---------------------------------------------------------------
         // operators that do not use op_csrc
         // ---------------------------------------------------------------
         case atom4_graph_op:
         case discrete_graph_op:
         case sum_graph_op:
         op_csrc = "";
         break;

         default:
         {  string msg = op_enum2name[op_enum];
            msg = "f.to_csrc: The " + msg + " is not yet implemented.";
            CPPAD_ASSERT_KNOWN(false, msg.c_str() );
         }
         break;
      }
      //
      // csrc
      switch( op_enum )
      {  //
         // binary functions
         case azmul_graph_op:
         case pow_graph_op:
         CPPAD_ASSERT_UNKNOWN( arg_node.size() == 2 );
         CPPAD_ASSERT_UNKNOWN( n_result == 1 );
         binary_function(
            os, op_csrc, result_node, arg_node[0], arg_node[1]
         );
         break;
         //
         // binary operators
         case add_graph_op:
         case div_graph_op:
         case mul_graph_op:
         case sub_graph_op:
         CPPAD_ASSERT_UNKNOWN( arg_node.size() == 2 );
         CPPAD_ASSERT_UNKNOWN( n_result == 1 );
         binary_operator(
            os, op_csrc, result_node, arg_node[0], arg_node[1]
         );
         break;
         //
         // comparison operators
         case comp_eq_graph_op:
         case comp_le_graph_op:
         case comp_lt_graph_op:
         case comp_ne_graph_op:
         CPPAD_ASSERT_UNKNOWN( arg_node.size() == 2 );
         CPPAD_ASSERT_UNKNOWN( n_result == 0 );
         compare_operator(
            os, op_csrc, arg_node[0], arg_node[1])
         ;
         break;
         //
         // unary functions
         case abs_graph_op:
         case acos_graph_op:
         case acosh_graph_op:
         case asin_graph_op:
         case asinh_graph_op:
         case atan_graph_op:
         case atanh_graph_op:
         case cos_graph_op:
         case cosh_graph_op:
         case erf_graph_op:
         case erfc_graph_op:
         case exp_graph_op:
         case expm1_graph_op:
         case log1p_graph_op:
         case log_graph_op:
         case sign_graph_op:
         case sin_graph_op:
         case sinh_graph_op:
         case sqrt_graph_op:
         case tan_graph_op:
         case tanh_graph_op:
         CPPAD_ASSERT_UNKNOWN( arg_node.size() == 1 );
         CPPAD_ASSERT_UNKNOWN( n_result == 1 );
         unary_function(
            os, op_csrc, result_node, arg_node[0]
         );
         break;
         //
         // atom4
         case atom4_graph_op:
         {  size_t index       = str_index[0];
            string atomic_name = graph_obj.atomic_name_vec_get(index);
            atomic_function(os,
               result_node, atomic_name, call_id, n_result, arg_node
            );
         }
         break;
         //
         // discrete
         case discrete_graph_op:
         CPPAD_ASSERT_UNKNOWN( arg_node.size() == 1 );
         CPPAD_ASSERT_UNKNOWN( n_result == 1 );
         {  size_t index         = str_index[0];
            string discrete_name = graph_obj.discrete_name_vec_get(index);
            discrete_function(os,
               result_node, discrete_name, arg_node[0]
            );
         }
         break;
         //
         // sum
         case sum_graph_op:
         CPPAD_ASSERT_UNKNOWN( n_result == 1 );
         sum_operator(os, result_node, arg_node);
         break;
         //
         // default
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
      }
      //
      // result_node
      result_node += n_result;
   }
   // ----------------------------------------------------------------------
   // dependent
   os <<
      "\n"
      "\t// dependent variables\n"
      "\t// set y[i] for i = 0, ny-1\n"
   ;
   for(size_t i = 0; i < ny; ++i)
   {  size_t node = graph_obj.dependent_vec_get(i);
      os << "\t" + element("y", i) + " = " + element("v", node) + ";\n";
   }
   // ----------------------------------------------------------------------
   // end function body
   os << "\n";
   os << "\treturn 0;\n";
   os << "}\n";
   //
   return;
}
