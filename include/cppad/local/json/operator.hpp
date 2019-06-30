# ifndef CPPAD_LOCAL_JSON_OPERATOR_HPP
# define CPPAD_LOCAL_JSON_OPERATOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <map>

// BEGIN_OPERATOR_ENUM
namespace CppAD { namespace local { namespace json {
    enum operator_enum  {
        add_operator,   // has 2 arguments and 1 result
        mul_operator,   // has 2 arguments and  1 result
        n_operator      // number of operators defined so far
    };
} } }
// END_OPERATOR_ENUM

// BEGIN_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local { namespace json {

struct operator_struct {
    size_t        n_result;
    size_t        n_arg;
    size_t        start_arg;
    // op_enum may be smaller so put it last for alignment
    operator_enum op_enum;

};

struct op_define_struct {
    size_t        n_result;
    size_t        n_arg;
    bool          fixed_size;
};

extern std::map< std::string, operator_enum > op_name2enum;
extern const op_define_struct                 op_enum2define[];

} } } // END_CPPAD_LOCAL_JSON_NAMESPACE


# endif
