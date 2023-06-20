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
See :ref:`optimize@options`

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
