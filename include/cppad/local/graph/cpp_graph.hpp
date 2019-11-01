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
# include <cppad/local/graph/cpp_graph_itr.hpp>

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
    typedef cpp_graph_itr const_iterator;
    //
    // initialize
    void initialize(void)
    {   function_name_  = "";
        n_dynamic_ind_  = 0;
        n_independent_  = 0;
        atomic_name_vec_.resize(0);
        constant_vec_.resize(0);
        operator_vec_.resize(0);
        operator_arg_.resize(0);
        dependent_vec_.resize(0);
        return;
    }
    //
    // begin
    const_iterator begin(void)
    {   size_t op_index = 0;
        return cpp_graph_itr(operator_vec_, operator_arg_, op_index);
    }
    //
    // end
    const_iterator end(void)
    {   size_t op_index = operator_vec_.size();
        return cpp_graph_itr(operator_vec_, operator_arg_, op_index);
    }
    // function_name
    const std::string& function_name_get(void) const
    {   return function_name_; }
    void function_name_set(const std::string& function_name)
    {   function_name_ = function_name; }
    //
    // n_dynamic_ind
    const size_t& n_dynamic_ind_get(void) const
    {   return n_dynamic_ind_; }
    void n_dynamic_ind_set(const size_t& n_dynamic_ind)
    {   n_dynamic_ind_ = n_dynamic_ind; }
    //
    // n_independent
    const size_t& n_independent_get(void) const
    {   return n_independent_; }
    void n_independent_set(const size_t& n_independent)
    {   n_independent_ = n_independent; }
    //
    // atomic_name_vec
    const std::string& atomic_name_vec_get(size_t index) const
    {   return atomic_name_vec_[index]; }
    size_t atomic_name_vec_size(void) const
    {   return atomic_name_vec_.size(); }
    void atomic_name_vec_push_back(const std::string& atomic_name)
    {   atomic_name_vec_.push_back(atomic_name); }
    //
    // constant_vec
    const double& constant_vec_get(size_t index) const
    {   return constant_vec_[index]; }
    size_t constant_vec_size(void) const
    {   return constant_vec_.size(); }
    void constant_vec_push_back(const double& constant)
    {   constant_vec_.push_back(constant); }
    //
    // oerator_vec
    const graph_op_struct& operator_vec_get(size_t index) const
    {   return operator_vec_[index]; }
    size_t operator_vec_size(void) const
    {   return operator_vec_.size(); }
    void operator_vec_push_back(const graph_op_struct& op_usage)
    {   operator_vec_.push_back(op_usage); }
    //
    // operator_arg
    const size_t& operator_arg_get(size_t index) const
    {   return operator_arg_[index]; }
    size_t operator_arg_size(void) const
    {   return operator_arg_.size(); }
    void operator_arg_push_back(size_t value)
    {   operator_arg_.push_back(value); }
    //
    // dependent_vec
    const size_t& dependent_vec_get(size_t index) const
    {   return dependent_vec_[index]; }
    size_t dependent_vec_size(void) const
    {   return dependent_vec_.size(); }
    void dependent_vec_push_back(size_t node_index)
    {   dependent_vec_.push_back(node_index); }
/* %$$
$end
------------------------------------------------------------------------------
$begin cpp_graph_get_op_info$$
$spell
    obj
    op
    arg
    vec
    enum
$$

$section C++ AD Graph Get Information for One Operator$$

$head Syntax$$
$icode%graph_obj%.get_op_info(
    %op_index%, %name_index%, %n_result%, %node_arg%
)%$$

$head op_index$$
Is the index of this operator in the $code operator_vec$$ vector.

$subhead op_enum$$
The input value of $icode op_enum$$ does not matter.
Upon return it is the
$cref/graph_op_enum/cpp_graph_op/graph_op_enum/$$ for this
operator usage.

$subhead name_index$$
The input value of $icode name_index$$ does not matter.
If $icode op_enum$$ is $code atom_graph_op$$,
upon return $icode name_index$$ is the index in
$cref/atomic_name_vec/cpp_ad_graph/atomic_name_vec/$$
for the function called by this operator usage.

$subhead n_result$$
The input value of $icode n_result$$ does not matter.
Upon return it is the number of result nodes for this operator usage.

$subhead arg_node$$
The input size and elements of this vector do not matter.
Upon return, it size is the number of arguments,
that are node indices, for this operator usage.
The value of the elements are the node indices.

$head Prototype$$
$srccode%hpp% */
public:
    void get_op_info(
        size_t                    op_index     ,
        graph_op_enum&            op_enum      ,
        size_t&                   name_index   ,
        size_t&                   n_result     ,
        CppAD::vector<size_t>&    arg_node     ) const
/* %$$
$end
*/
{   // initialize output values
    size_t invalid_index   = std::numeric_limits<size_t>::max();
    size_t n_arg = invalid_index;
    name_index   = invalid_index;
    n_result     = invalid_index;
    arg_node.resize(0);
    //
    // op_enum, start_arg
    op_enum          = operator_vec_[op_index].op_enum;
    size_t start_arg = operator_vec_[op_index].start_arg;
    //
    // n_result, n_arg (name_index if op_enum is atom_graph_op)
    switch( op_enum )
    {
        // unary operators
        case abs_graph_op:
        case acos_graph_op:
        case acosh_graph_op:
        case asin_graph_op:
        case asinh_graph_op:
        case atan_graph_op:
        case atanh_graph_op:
        case cos_graph_op:
        case cosh_graph_op:
        case erf_graph_op:
        case erfc_graph_op:
        case exp_graph_op:
        case expm1_graph_op:
        case log1p_graph_op:
        case log_graph_op:
        case sign_graph_op:
        case sin_graph_op:
        case sinh_graph_op:
        case sqrt_graph_op:
        case tan_graph_op:
        case tanh_graph_op:
        n_result   = 1;
        n_arg      = 1;
        break;

        // binary operators
        case add_graph_op:
        case div_graph_op:
        case mul_graph_op:
        case sub_graph_op:
        n_result   = 1;
        n_arg      = 2;
        break;

        // atom_graph_op
        case atom_graph_op:
        name_index = operator_arg_[start_arg - 3];
        n_result   = operator_arg_[start_arg - 2];
        n_arg      = operator_arg_[start_arg - 1];
        break;

        // conditional expressions
        case cexp_eq_graph_op:
        case cexp_le_graph_op:
        case cexp_lt_graph_op:
        n_result   = 1;
        n_arg      = 4;
        break;

        // comparison operators
        case comp_eq_graph_op:
        case comp_le_graph_op:
        case comp_lt_graph_op:
        case comp_ne_graph_op:
        n_result   = 0;
        n_arg      = 2;
        break;

        // sum_graph_op
        case sum_graph_op:
        n_result   = 1;
        n_arg      = operator_arg_[start_arg - 1];
        break;

        default:
        CPPAD_ASSERT_UNKNOWN(false);
        break;
    }
    // set arg_node
    arg_node.resize(n_arg);
    for(size_t i = 0; i < n_arg; i++)
        arg_node[i] = operator_arg_[start_arg + i];

    return;
}
};
// END CPP_GRAPH

} } }
// END_CPPAD_LOCAL_GRAPH_NAMESPACE

# endif
