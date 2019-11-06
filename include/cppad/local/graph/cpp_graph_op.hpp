# ifndef CPPAD_LOCAL_GRAPH_CPP_GRAPH_OP_HPP
# define CPPAD_LOCAL_GRAPH_CPP_GRAPH_OP_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <cstddef>
# include <string>
# include <map>

# include <cppad/utility/vector.hpp>
# include <cppad/configure.hpp>
# include <cppad/local/is_pod.hpp>

namespace CppAD { namespace local { namespace graph {
/*
$begin cpp_graph_op$$
$spell
    vec
    asinh
    acosh
    atanh
    erf
    erfc
    expm
    namespace
    enum
    struct
    op
    arg
    CppAD
    addr_t
$$

$section C++ AD Graph Operators$$

$head Namespace$$
All of these definitions,
expect $cref/is_pod/cpp_graph_op/is_pod/$$,
are in the $code CppAD::local::graph$$ namespace.

$head addr_t$$
$srccode%hpp% */
    typedef CPPAD_TAPE_ADDR_TYPE addr_t;
/* %$$

$head graph_op_enum$$
$srccode%hpp% BEGIN_SORT_THIS_LINE_PLUS_2 */
    enum graph_op_enum {
        abs_graph_op,      // 1 result, 1 argument
        acos_graph_op,     // 1 result, 1 argument
        acosh_graph_op,    // 1 result, 1 argument
        add_graph_op,      // 1 result, 2 arguments
        asin_graph_op,     // 1 result, 1 argument
        asinh_graph_op,    // 1 result, 1 argument
        atan_graph_op,     // 1 result, 1 argument
        atanh_graph_op,    // 1 result, 1 argument
        atom_graph_op,     // variable number of results and arguments
        cexp_eq_graph_op,  // 1 result, 4 arguments
        cexp_le_graph_op,  // 1 result, 4 arguments
        cexp_lt_graph_op,  // 1 result, 4 arguments
        comp_eq_graph_op,  // 0 result, 2 arguments
        comp_le_graph_op,  // 0 result, 2 arguments
        comp_lt_graph_op,  // 0 result, 2 arguments
        comp_ne_graph_op,  // 0 result, 2 arguments
        cos_graph_op,      // 1 result, 1 argument
        cosh_graph_op,     // 1 result, 1 argument
        div_graph_op,      // 1 result, 2 arguments
        erf_graph_op,      // 1 result, 1 argument
        erfc_graph_op,     // 1 result, 1 argument
        exp_graph_op,      // 1 result, 1 argument
        expm1_graph_op,    // 1 result, 1 argument
        log1p_graph_op,    // 1 result, 1 argument
        log_graph_op,      // 1 result, 1 argument
        mul_graph_op,      // 1 result, 2 arguments
        sign_graph_op,     // 1 result, 1 argument
        sin_graph_op,      // 1 result, 1 argument
        sinh_graph_op,     // 1 result, 1 argument
        sqrt_graph_op,     // 1 result, 1 argument
        sub_graph_op,      // 1 result, 2 arguments
        sum_graph_op,      // 1 result, variable number of arguments
        tan_graph_op,      // 1 result, 1 argument
        tanh_graph_op,     // 1 result, 1 argument
        n_graph_op         // number of operators defined so far
    };
/* END_SORT_THIS_LINE_MINUS_3 %$$

$subhead Require C++11$$
The following operators require a compiler that supports c++11:
$code asinh$$, $code acosh$$, $code atanh$$,
$code erf$$, $code erfc$$,
$code expm1$$, $code log1p$$.

$head graph_op_struct$$
$srccode%hpp% */
    typedef graph_op_enum graph_op_struct;
/* %$$

$subhead op_enum$$
is the operator being used.

$head op_name2enum$$
This is a mapping from the operator name to its enum value.
The name is the operator enum without the $code _operator$$ at the end.
$srccode%hpp% */
    extern std::map< std::string, graph_op_enum > op_name2enum;
/* %$$

$head op_enum2fixed_n_arg$$
This is the number of arguments for the operators that have
a fixed number of arguments and one result.
For other operators, this value is zero.
$srccode%hpp% */
    extern size_t op_enum2fixed_n_arg[];
/* %$$

$head op_enum2name$$
This is mapping from operator enum value to its name.
In the $code local::graph$$ namespace:
$srccode%hpp% */
    extern const char* op_enum2name[];
/* %$$

$head set_operator_info$$
This routine sets the values in
$code op_enum2fixed_n_arg$$,
$code op_enum2name$$, and
$code op_name2enum$$.
$srccode%hpp% */
    extern void set_operator_info(void);
/* %$$

$comment */
} } } // END_CPPAD_LOCAL_GRAPH_NAMESPACE
namespace CppAD { namespace local {
/* %$$

$head is_pod$$
Inform $code local::is_pod$$ that this is plain old data.
All the definitions above are in this $code CppAD::local::graph$$ namespace
while this definition is in the $code CppAD::local$$ namespace.
$srccode%hpp% */
        template <> inline bool
        is_pod<graph::graph_op_struct>(void) { return true; }
/* %$$
$end
*/

} }

# endif
