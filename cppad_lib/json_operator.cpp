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

// BEGIN_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local { namespace json {

// mapping from operator name to operator_enum value
std::map<std::string, operator_enum> op_name2enum = {
    { "add", add_operator },
    { "mul", mul_operator }
};

// BEGIN_SORT_THIS_LINE_PLUS_2
const size_t op_enum2fixed_n_arg[] = {
    /* add */ 2,
    /* mul */ 2
};
// END_SORT_THIS_LINE_MINUS_2


} } } // END_CPPAD_LOCAL_JSON_NAMESPACE
