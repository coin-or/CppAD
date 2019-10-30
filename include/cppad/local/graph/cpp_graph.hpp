# ifndef CPPAD_LOCAL_GRAPH_CPP_GRAPH_HPP
# define CPPAD_LOCAL_GRAPH_CPP_GRAPH_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <string>
# include <cppad/utility/vector.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>

// BEGIN_CPPAD_LOCAL_GRAPH_NAMESPACE
namespace CppAD { namespace local { namespace graph {

// BEGIN_CPP_GRAPH
class cpp_graph {
/*
-------------------------------------------------------------------------------
$begin cpp_graph_private_data$$
$spell
    ind
    vec
    arg
    op
    struct
$$

$section C++ AD Graph Private Data$$

$table
$cref/graph_op_struct/cpp_graph_op/graph_op_struct/$$ $rnext
$cref/function_name/cpp_ad_graph/function_name/$$ $rnext
$cref/n_dynamic_ind/cpp_ad_graph/n_dynamic_ind/$$ $rnext
$cref/n_independent/cpp_ad_graph/n_independent/$$ $rnext
$cref/constant_vec/cpp_ad_graph/constant_vec/$$ $rnext
$cref/operator_vec/cpp_ad_graph/operator_vec/$$ $rnext
$cref/operator_arg/cpp_ad_graph/operator_arg/$$ $rnext
$cref/dependent_vec/cpp_ad_graph/dependent_vec/$$
$tend

$srccode%hpp% */
private:
    typedef local::graph::graph_op_struct graph_op_struct;
    //
    std::string                   function_name_;
    vector<std::string>           atomic_name_vec_;
    size_t                        n_dynamic_ind_;
    size_t                        n_independent_;
    vector<double>                constant_vec_;
    vector<graph_op_struct>       operator_vec_;
    vector<size_t>                operator_arg_;
    vector<size_t>                dependent_vec_;
/* %$$
$end
------------------------------------------------------------------------------
$begin cpp_graph_data_access$$

$section C++ AD Graph Private Data Access$$

$srccode%hpp% */
public:
    // const
    const std::string& function_name(void) const
    {   return function_name_; }
    const vector<std::string>& atomic_name_vec(void) const
    {   return atomic_name_vec_; }
    const size_t& n_dynamic_ind(void) const
    {   return n_dynamic_ind_; }
    const size_t& n_independent(void) const
    {   return n_independent_; }
    const vector<double>& constant_vec(void) const
    {   return constant_vec_; }
    const vector<graph_op_struct>& operator_vec(void) const
    {   return operator_vec_; }
    const vector<size_t>& operator_arg(void) const
    {   return operator_arg_; }
    const vector<size_t>& dependent_vec(void) const
    {   return dependent_vec_; }
    //
    // not const
    std::string& function_name(void)
    {   return function_name_; }
    vector<std::string>& atomic_name_vec(void)
    {   return atomic_name_vec_; }
    size_t& n_dynamic_ind(void)
    {   return n_dynamic_ind_; }
    size_t& n_independent(void)
    {   return n_independent_; }
    vector<double>& constant_vec(void)
    {   return constant_vec_; }
    vector<graph_op_struct>& operator_vec(void)
    {   return operator_vec_; }
    vector<size_t>& operator_arg(void)
    {   return operator_arg_; }
    vector<size_t>& dependent_vec(void)
    {   return dependent_vec_; }
/* %$$
$end
------------------------------------------------------------------------------
*/
};
// END CPP_GRAPH

} } }
// END_CPPAD_LOCAL_GRAPH_NAMESPACE

# endif
