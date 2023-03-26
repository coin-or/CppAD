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
is a vector of results for this operator.
If this vector is empty, this operator is combined with the previous operator
to get the result for the previous operator.
For example, printing the :ref:`val_csum_op-name` is accomplished by a call
with *name* equal add followed by one with *name* equal sub
where the add call has one result and the sub call has no results.

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
   if( n_res == 0 )
      cout << setw(19) << "";
   //
   // Multiple results
   cout << right << setw(5) << name << "(";
   for(size_t i = 0; i < n_arg; ++i)
   {  cout << right << setw(5) << val_arg[i];
      if( i + 1 < n_arg )
            cout << ",";
      else
            cout << ")";
   }
   for(size_t i = 0; i < n_res; ++i)
   {  cout << std::endl;
      cout << right << setw(5) << res_index + i;
      cout << " " << setw(10) << res_value[i];
   }
   cout << std::endl;
}
/*
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

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
