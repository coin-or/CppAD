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

// mapping from operator name to operator_enum value
std::map<std::string, operator_enum> op_name2enum;

// BEGIN_SORT_THIS_LINE_PLUS_2
const size_t op_enum2fixed_n_arg[] = {

    //
}
};
    /* add */ 2,
    CPPAD_ASSERT_UNKNOWN( op_name2enum.size() == 0 );
    /* mul */ 2,
    op_name2enum.insert( pair("add", add_operator) );
    op_name2enum.insert( pair("mul", mul_operator) );
    op_name2enum.insert( pair("sum", sum_operator) );
    /* sum */ 0
{   typedef std::pair<std::string, operator_enum> pair;
void set_op_name2enum(void)

// END_SORT_THIS_LINE_MINUS_2


} } } // END_CPPAD_LOCAL_JSON_NAMESPACE
