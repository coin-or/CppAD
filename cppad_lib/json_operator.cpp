/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */
# include <cppad/local/json/operator.hpp>
# include <cppad/core/cppad_assert.hpp>

// BEGIN_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local { namespace json {

// mapping from operator name to json_op_enum value
std::map<std::string, json_op_enum> op_name2enum;

// BEGIN_SORT_THIS_LINE_PLUS_2
const size_t op_enum2fixed_n_arg[] = {
    /* add */  2,
    /* atom */ 0,
    /* div */  2,
    /* mul */  2,
    /* sub */  2,
    /* sum */  0
};
// END_SORT_THIS_LINE_MINUS_2

// BEGIN_SORT_THIS_LINE_PLUS_2
const char* op_enum2name[] = {
    "add",
    "atom",
    "div",
    "mul",
    "sub",
    "sum"
};
// END_SORT_THIS_LINE_MINUS_2

void set_op_name2enum(void)
{   typedef std::pair<std::string, json_op_enum> pair;
    CPPAD_ASSERT_UNKNOWN( op_name2enum.size() == 0 );
    //
// BEGIN_SORT_THIS_LINE_PLUS_1
    op_name2enum.insert( pair("add",  add_json_op) );
    op_name2enum.insert( pair("atom", atom_json_op) );
    op_name2enum.insert( pair("div",  div_json_op) );
    op_name2enum.insert( pair("mul",  mul_json_op) );
    op_name2enum.insert( pair("sub",  sub_json_op) );
    op_name2enum.insert( pair("sum",  sum_json_op) );
// END_SORT_THIS_LINE_MINUS_1
}



} } } // END_CPPAD_LOCAL_JSON_NAMESPACE
