# ifndef  CPPAD_LOCAL_VAL_GRAPH_PRINT_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_PRINT_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <iostream>
# include <iomanip>
/*
{xrst_begin_parent val_print_op dev}

Printing Value Operators
########################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_OP
   // END_PRINT_OP
}

Output Notation
***************
#. The first column is a value index and the second column is the
   corresponding value.
#. Values inside of parenthesis are arguments that are result indices
   for the operator name  that comes before the left parenthesis.
#. Values inside of brackets are indices for vector name that
   comes before the left bracket.

name
****
Is the name of this operator. In the case of a function call operator,
it is the name of the function being called.

val_arg
*******
is a vector containing the operator arguments that are value indices
(in order).

res_index
*********
is the index of the first result for this operator.

res_value
*********
is a non-empty vector of results for this operator.

Special Cases
*************
{xrst_toc_table
}

{xrst_end val_print_op}
*/

// BEGIN_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
namespace CppAD { namespace local { namespace val_graph {

// BEGIN_PRINT_OP
template <class Value>
void print_op(
   const std::string&    name            ,
   const Vector<addr_t>& val_arg         ,
   addr_t                res_index       ,
   const Vector<Value>&  res_value       )
// END_PRINT_OP
{  size_t n_arg = val_arg.size();
   size_t n_res = res_value.size();
   using std::setw;
   using std::right;
   using std::cout;
   //
   // One result
   if( n_res == 1 )
   {  cout << right << setw(5) << res_index;
      cout << " " << right << setw(10) << res_value[0];
      cout << " = " << right << setw(5)  << name << "(";
      for(size_t i = 0; i < n_arg; ++i)
      {  cout << right << setw(5) << std::right << val_arg[i];
         if( i + 1 < n_arg )
            cout << ",";
      }
      cout << ")" << std::endl;
      return;
   }
   //
   // Multiple results
   CPPAD_ASSERT_UNKNOWN( n_res > 1 );
   cout << right << setw(5) << name << "(";
   for(size_t i = 0; i < n_arg; ++i)
   {  cout << right << setw(5) << val_arg[i];
      if( i + 1 < n_arg )
            cout << ",";
      else
            cout << ")";
   }
   for(addr_t i = 0; i < addr_t( n_res ); ++i)
   {  cout << std::endl;
      cout << right << setw(5) << res_index + i;
      cout << " " << setw(10) << res_value[i];
   }
   cout << std::endl;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_con_op dev}

Printing Constant Operators
###########################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_CON_OP
   // END_PRINT_CON_OP
}

arg
***
is a vector containing all the operator arguments (must be length one).

res_index
*********
is the index of the result for this operator.

res_value
*********
is a vector of results for this operator (must be length one).

{xrst_end val_print_con_op}
*/
// BEGIN_PRINT_CON_OP
template <class Value>
void print_con_op(
   const Vector<addr_t>& arg             ,
   addr_t                res_index       ,
   const Vector<Value>&  res_value       )
// END_PRINT_CON_OP
{  CPPAD_ASSERT_UNKNOWN( arg.size() == 1 );
   CPPAD_ASSERT_UNKNOWN( res_value.size() == 1 );
   using std::setw;
   using std::right;
   using std::cout;
   {  cout << right << setw(5) << res_index;
      cout << " " << right << setw(10) << res_value[0];
      cout << " = " << right << setw(5)  << "con" << "[";
      cout << right << setw(5) << std::right << arg[0] << "]";
      cout << std::endl;
      return;
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_csum_op dev}

Printing Cumulative Summation Operators
#######################################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_CSUM_OP
   // END_PRINT_CSUM_OP
}

arg
***
is a vector containing all the operator arguments (must be length one).

res_index
*********
is the index of the result for this operator.

res_value
*********
is a vector of results for this operator (must be length one).

{xrst_end val_print_csum_op}
*/
// BEGIN_PRINT_CSUM_OP
template <class Value>
void print_csum_op(
   const Vector<addr_t>& arg             ,
   addr_t                res_index       ,
   const Vector<Value>&  res_value       )
// END_PRINT_CSUM_OP
{  CPPAD_ASSERT_UNKNOWN( res_value.size() == 1);
   //
   using std::setw;
   using std::right;
   using std::cout;
   //
   // n_add, n_sub
   addr_t n_add = arg[0];
   addr_t n_sub = arg[1];
   //
   CPPAD_ASSERT_UNKNOWN( arg.size() == size_t(3 + n_add + n_sub ) );
   //
   cout << right << setw(5) << res_index;
   cout << " " << right << setw(10) << res_value[0] << " = ";
   if( n_add > 0 )
   {  cout << right << setw(5)  << "csum+" << "(";
      for(addr_t i = 0; i < n_add; ++i)
      {  cout << right << setw(5) << arg[2 + i];
         if( i + 1 < n_add )
            cout << ",";
      }
      cout << ")" << std::endl;
      if( n_sub > 0 )
      {  cout << setw(19) << "" << right << setw(5)  << "csum-" << "(";
         for(addr_t i = 0; i < n_sub; ++i)
         {  cout << right << setw(5) << arg[2 + n_add + i];
            if( i + 1 < n_sub )
               cout << ",";
         }
         cout << ")" << std::endl;
      }
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( n_sub > 0 );
      cout << right << setw(5)  << "csum-" << "(";
      for(addr_t i = 0; i < n_sub; ++i)
      {  cout << right << setw(5) << arg[2 + n_add + i];
         if( i + 1 < n_sub )
            cout << ",";
      }
      cout << ")" << std::endl;
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_comp_op dev}

Printing Compare Operators
##########################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_COMP_OP
   // END_PRINT_COMP_OP
}

comp_name
*********
is the name of the comparison operator; e.g., "lt"

left_index
**********
is the value index corresponding to the left operand in the comparison.

right_index
***********
is the value index corresponding to the right operand in the comparison.

result
******
is the result of the comparison.

{xrst_end val_print_comp_op}
*/
// BEGIN_PRINT_COMP_OP
inline void print_comp_op(
   const char* comp_name   ,
   addr_t      left_index  ,
   addr_t      right_index ,
   bool        result      )
// END_PRINT_COMP_OP
{  //
   using std::setw;
   using std::right;
   using std::cout;
   //
   const char* res_str;
   if( result )
      res_str = "true";
   else
      res_str = "false";
   //
   cout << setw(19) << "" << right << setw(5)  << comp_name << "(";
   cout << right << setw(5) << left_index  << ",";
   cout << right << setw(5) << right_index << ") = ";
   cout << right << setw(5) << res_str << std::endl;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_pri_op dev}
{xrst_spell
   str
}

Printing Print Operators
########################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_PRI_OP
   // END_PRINT_PRI_OP
}

before_index
************
is the index in str_vec corresponding to the before text.

after_index
***********
is the index in str_vec corresponding to the after text.

flag_index
**********
is the index in the value vector corresponding to the flag.

value_index
***********
is the index in the value vector corresponding to the value that is printed
if the flag is positive.


{xrst_end val_print_pri_op}
*/
// BEGIN_PRINT_PRI_OP
inline void print_pri_op(
   addr_t       before_index ,
   addr_t       after_index  ,
   addr_t       flag_index   ,
   addr_t       value_index  )
// END_PRINT_PRI_OP
{  //
   using std::setw;
   using std::right;
   using std::cout;
   //
   cout << setw(19) << "" << right << setw(5)  << "pri" << "[";
   cout << right << setw(5) << before_index  << ",";
   cout << right << setw(5) << after_index   << "](";
   cout << right << setw(5) << flag_index    << ",";
   cout << right << setw(5) << value_index << ")" << std::endl;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_store_op dev}

Printing Store Operators
########################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_STORE_OP
   // END_PRINT_STORE_OP
}

which_vector
************
is the index in val_vec_vec corresponding to this vector.

vector_index
************
is the index in val_vec corresponding to the index for this vector element.

value_index
***********
is the index in val_vec corresponding to the new value for this vector element.


{xrst_end val_print_store_op}
*/
// BEGIN_PRINT_STORE_OP
inline void print_store_op(
   addr_t       which_vector ,
   addr_t       vector_index ,
   addr_t       value_index  )
// END_PRINT_STORE_OP
{  //
   using std::setw;
   using std::right;
   using std::cout;
   //
   cout << setw(19) << "" << right << setw(5)  << "store" << "[";
   cout << right << setw(5) << which_vector  << "](";
   cout << right << setw(5) << vector_index  << ",";
   cout << right << setw(5) << value_index << ")" << std::endl;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_load_op dev}

Printing Load Operators
#######################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_LOAD_OP
   // END_PRINT_LOAD_OP
}

which_vector
************
is the index in val_vec_vec corresponding to this vector.

vector_index
************
is the index in val_vec corresponding to the index for this vector element.

res_index
*********
in the index in val_vec corresponding to the result for this operator.

{xrst_end val_print_load_op}
*/
// BEGIN_PRINT_LOAD_OP
template <class Value>
inline void print_load_op(
   addr_t       which_vector ,
   addr_t       vector_index ,
   addr_t       res_index    ,
   const Value& res_value    )
// END_PRINT_LOAD_OP
{  //
   using std::setw;
   using std::right;
   using std::cout;
   //
   cout << right << setw(5) << res_index;
   cout << " " << right << setw(10) << res_value;
   cout << " = " << setw(5)  << "load" << "[";
   cout << right << setw(5) << which_vector  << "](";
   cout << right << setw(5) << vector_index << ")" << std::endl;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_print_vec_op dev}

Printing New Dynamic Vector Operators
#####################################

Prototype
*********
{xrst_literal
   // BEGIN_PRINT_VEC_OP
   // END_PRINT_VEC_OP
}

which_vector
************
is the index in val_vec_vec corresponding to this vector.

initial
*******
is the vector of indices in val_vec (the value vector)
corresponding to the initial values for this dynamic vector.

{xrst_end val_print_vec_op}
*/
// BEGIN_PRINT_VEC_OP
inline void print_vec_op(
   addr_t                which_vector ,
   const Vector<addr_t>& initial      )
// END_PRINT_VEC_OP
{  //
   using std::setw;
   using std::right;
   using std::cout;
   //
   cout << setw(19) << "" << right << setw(5)  << "vec" << "[";
   cout << right << setw(5) << which_vector  << "](";
   for(size_t i = 0; i < initial.size(); ++i)
   {  cout << right << setw(5) << initial[i];
      if( i + 1 < initial.size() )
         cout << ",";
   }
   cout << ")" << std::endl;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
