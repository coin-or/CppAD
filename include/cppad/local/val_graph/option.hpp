# ifndef  CPPAD_LOCAL_VAL_GRAPH_OPTION_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OPTION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
-------------------------------------------------------------------------------
{xrst_begin val_tape_option dev}

Set an Option's Value
#####################

Prototype
*********
{xrst_literal
   // BEGIN_SET_OPTION
   // END_SET_OPTION
}

keep_compare
************
If *name* is keep_compare, *value* must be true or false .
If it is false (true), :ref:`val_comp_op-name` operators will (will not)
be removed during :ref:`val_tape_dead_code-name` optimization.

keep_print
**********
If *name* is keep_print, *value* must be true or false .
If it is false (true), :ref:`val_pri_op-name` operators will (will not)
be removed during dead code optimization.

{xrst_end val_tape_option}
*/
// ---------------------------------------------------------------------------
// BEGIN_INITIALIZE_OPTION
template <class Value>
void tape_t<Value>::initialize_option(void)
// END_INITIALIZE_OPTION
{
   option_map_["keep_compare"] = "true";
   option_map_["keep_print"]   = "true";
   //
   return;
}
// ---------------------------------------------------------------------------
// BEGIN_SET_OPTION
template <class Value>
void tape_t<Value>::set_option(
   const std::string& name  ,
   const std::string& value )
// END_SET_OPTION
{  //
   if( option_map_.find(name) == option_map_.end() )
   {  std::string msg = "value tape: There is no option named " + name;
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
   if( value != "true" && value != "false" )
   {  std::string msg = "value tape: option [" + name + "]";
      msg            += " value is not true or false ";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
   option_map_[name] = value;
   //
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
