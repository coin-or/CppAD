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
    // function_name
    const std::string& function_name(void) const
    {   return function_name_; }
    void set_function_name(const std::string& function_name)
    {   function_name_ = function_name; }
    //
    // n_dynamic_ind
    const size_t& n_dynamic_ind(void) const
    {   return n_dynamic_ind_; }
    void set_n_dynamic_ind(const size_t& n_dynamic_ind)
    {   n_dynamic_ind_ = n_dynamic_ind; }
    //
    // n_independent
    const size_t& n_independent(void) const
    {   return n_independent_; }
    void set_n_independent(const size_t& n_independent)
    {   n_independent_ = n_independent; }
    //
    // atomic_name_vec
    const std::string& atomic_name_vec_get(size_t index) const
    {   return atomic_name_vec_[index]; }
    size_t atomic_name_vec_size(void) const
    {   return atomic_name_vec_.size(); }
    void atomic_name_vec_clear(void)
    {   atomic_name_vec_.resize(0); }
    void atomic_name_vec_push_back(const std::string& atomic_name)
    {   atomic_name_vec_.push_back(atomic_name); }
    //
    // constant_vec
    const double& constant_vec_get(size_t index) const
    {   return constant_vec_[index]; }
    size_t constant_vec_size(void) const
    {   return constant_vec_.size(); }
    void constant_vec_clear(void)
    {   constant_vec_.resize(0); }
    void constant_vec_push_back(const double& constant)
    {   constant_vec_.push_back(constant); }
    //
    // oerator_vec
    const vector<graph_op_struct>& operator_vec(void) const
    {   return operator_vec_; }
    void operator_vec_clear(void)
    {   operator_vec_.resize(0); }
    void operator_vec_push_back(const graph_op_struct& op_usage)
    {   operator_vec_.push_back(op_usage); }
    //
    // operator_arg
    const vector<size_t>& operator_arg(void) const
    {   return operator_arg_; }
    void operator_arg_clear(void)
    {   operator_arg_.clear(); }
    void operator_arg_push_back(size_t value)
    {   operator_arg_.push_back(value); }
    //
    // dependent_vec
    const vector<size_t>& dependent_vec(void) const
    {   return dependent_vec_; }
    void dependent_vec_clear(void)
    {   dependent_vec_.resize(0); }
    void dependent_vec_push_back(size_t node_index)
    {   dependent_vec_.push_back(node_index); }
/* %$$
$end
------------------------------------------------------------------------------
*/
};
// END CPP_GRAPH

} } }
// END_CPPAD_LOCAL_GRAPH_NAMESPACE

# endif
