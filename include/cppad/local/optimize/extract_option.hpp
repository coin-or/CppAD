# ifndef CPPAD_LOCAL_OPTIMIZE_EXTRACT_OPTION_HPP
# define CPPAD_LOCAL_OPTIMIZE_EXTRACT_OPTION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*!
{xrst_begin optimize_extract_option dev}
{xrst_spell
   struct
}

Convert Optimizer Options From String to Struct
###############################################

Prototype
*********
{xrst_literal
   // BEGIN_OPTIMIZE_OPTIONS
   // END_OPTIMIZE_OPTIONS
}

result
******
The return value, *result* below,  has the following type
{xrst_literal
   // BEGIN_OPTIONS_T
   // END_OPTIONS_T
}


options
*******

no_compare_op
=============
If this sub-string is (is not) present,
*result*.compare_op is false (true).
In this case comparison operators are (are not) removed from the optimized tape.
These operators are necessary for the :ref:`compare_change-name` feature to be
meaningful in the resulting recording.
On the other hand, they are otherwise not necessary and take extra time
when this feature is not needed.

no_conditional_skip
===================
If this sub-string is (is not) present,
*result*.conditional_skip is false (true).
In this case conditional skip operations are not (are) generated.
This may make the optimize routine use significantly less (more) memory
and take significantly less (more) time.

no_cumulative_sum_op
====================
If this sub-string is (is not) present,
*result*.cumulative_sum_op is false (true).
In this case cumulative sum operations will not (will)
be generated during the optimization; see
:ref:`optimize_cumulative_sum.cpp-name` .

no_print_for_op
===============
If this sub-string is (is not) present,
*result*.print_for_op is false (true).
In this case then :ref:`printfor-name` operators ``PriOp``
are (are not) removed from the optimized tape.
These operators are useful for reporting problems evaluating derivatives
at independent variable values different from those used to record a function.

collision_limit=value
=====================
If this sub-string is (is not) present,
the optimizer's hash code collision limit will be *value* (10).
When the collision limit is exceeded, the expressions with that hash code
are removed and a new lists of expressions with that has code is started.
The larger *value* , the more identical expressions the optimizer
can recognize, but the slower the optimizer may run.
The default for *value* is ``10`` .

yes_val_graph
=============
If this sub-string is present, the value graph optimizer is used.
This is a new CppAD optimizer that has a much simpler implementation.
It is expected to be much better in the long run
(because it is easier to improve).

no_conditional_skip
   If val_graph is present, no_conditional_skip must also be present.

collision_limit=value
   If val_graph is present, the collision_limit=value must not be present
   (there is currently no collision limit for the val_graph optimizer).

{xrst_end optimize_extract_option}
*/

# include <cppad/core/cppad_assert.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {

// BEGIN_SORT_THIS_LINE_PLUS_3
// BEGIN_OPTIONS_T
struct options_t {
   bool   compare_op;
   bool   conditional_skip;
   bool   cumulative_sum_op;
   bool   print_for_op;
   bool   val_graph;
   size_t collision_limit;
};
// END_OPTIONS_T
// END_SORT_THIS_LINE_MINUS_3

// BEGIN_OPTIMIZE_OPTIONS
inline options_t extract_option(const std::string& options)
// END_OPTIMIZE_OPTIONS
{  //
   // result: defualt value
   struct options_t result = {
      true,  // compare_op
      true,  // conditional_skip
      true,  // cumulative_sum_op
      true,  // print_for_op
      false, // val_graph
      10     // collision_limit
   };
   size_t index = 0;
   while( index < options.size() )
   {  while( index < options.size() && options[index] == ' ' )
         ++index;
      std::string option;
      while( index < options.size() && options[index] != ' ' )
         option += options[index++];
      if( option != "" )
      {
         if( option == "no_compare_op" )
            result.compare_op = false;
         else if( option == "no_conditional_skip" )
            result.conditional_skip = false;
         else if( option == "no_cumulative_sum_op" )
            result.cumulative_sum_op = false;
         else if( option == "no_print_for_op" )
            result.print_for_op = false;
         else if( option == "val_graph" )
            result.val_graph = true;
         else if( option.substr(0, 16)  == "collision_limit=" )
         {  std::string value = option.substr(16, option.size());
            bool value_ok = value.size() > 0;
            for(size_t i = 0; i < value.size(); ++i)
            {  value_ok &= '0' <= value[i];
               value_ok &= value[i] <= '9';
            }
            if( ! value_ok )
            {  option += " value is not a sequence of decimal digits";
               CPPAD_ASSERT_KNOWN( false , option.c_str() );
            }
            result.collision_limit = size_t( std::atoi( value.c_str() ) );
            if( result.collision_limit < 1 )
            {  option += " value must be greater than zero";
               CPPAD_ASSERT_KNOWN( false , option.c_str() );
            }
         }
         else
         {  option += " is not a valid optimize option";
            CPPAD_ASSERT_KNOWN( false , option.c_str() );
         }
      }
   }
   return result;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
