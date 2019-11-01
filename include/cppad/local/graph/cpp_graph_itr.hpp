# ifndef CPPAD_LOCAL_GRAPH_CPP_GRAPH_ITR_HPP
# define CPPAD_LOCAL_GRAPH_CPP_GRAPH_ITR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/utility/vector.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>

// BEGIN_CPPAD_LOCAL_GRAPH_NAMESPACE
namespace CppAD { namespace local { namespace graph {

class cpp_graph_itr {
/*
$begin cpp_graph_itr_private$$

$section C++ AD Graph Iterator Private Data and Functions$$
$srccode%hpp% */
private:
    const vector<graph_op_struct>* operator_vec_;
    const vector<size_t>*          operator_arg_;
    size_t                         op_index_;
    vector<size_t>                 arg_node_;
    //
    void get_op_info(
        size_t                    op_index     ,
        graph_op_enum&            op_enum      ,
        size_t&                   name_index   ,
        size_t&                   n_result     ,
        vector<size_t>&           arg_node     )
{   // initialize output values
    size_t invalid_index   = std::numeric_limits<size_t>::max();
    size_t n_arg = invalid_index;
    name_index   = invalid_index;
    n_result     = invalid_index;
    arg_node.resize(0);
    //
    // op_enum, start_arg
    op_enum          = (*operator_vec_)[op_index].op_enum;
    size_t start_arg = (*operator_vec_)[op_index].start_arg;
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
        name_index = (*operator_arg_)[start_arg - 3];
        n_result   = (*operator_arg_)[start_arg - 2];
        n_arg      = (*operator_arg_)[start_arg - 1];
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
        n_arg      = (*operator_arg_)[start_arg - 1];
        break;

        default:
        CPPAD_ASSERT_UNKNOWN(false);
        break;
    }
    // set arg_node
    arg_node.resize(n_arg);
    for(size_t i = 0; i < n_arg; i++)
        arg_node[i] = (*operator_arg_)[start_arg + i];

    return;
}
/* %$$
$end
-------------------------------------------------------------------------------
$begin cpp_graph_itr_public$$

$section C++ AD Graph Iterator Public Functions$$

$srccode%hpp% */
public:
    typedef struct {
        graph_op_enum          op_enum;
        size_t                 name_index;
        size_t                 n_result;
        const vector<size_t>*  arg_node_ptr;
    } value_type;
    typedef std::input_iterator_tag    iterator_category;
    //
    // ctor
    cpp_graph_itr(
        const vector<graph_op_struct>& operator_vec   ,
        const vector<size_t>&          operator_arg   ,
        size_t op_index                               )
    :
    operator_vec_(&operator_vec) ,
    operator_arg_(&operator_arg) ,
    op_index_(op_index)
    { }
    // ==
    bool operator==(const cpp_graph_itr& other) const
    {   return op_index_ == other.op_index_;
    }
    // !=
    bool operator!=(const cpp_graph_itr& other) const
    {   return op_index_ != other.op_index_;
    }
    value_type operator*(void)
    {   value_type result;
        get_op_info(
            op_index_,
            result.op_enum,
            result.name_index,
            result.n_result,
            arg_node_
        );
        result.arg_node_ptr = &arg_node_;
        return result;
    }
    cpp_graph_itr& operator++(void)
    {   ++op_index_;
        return *this;
    }
    cpp_graph_itr operator++(int)
    {   cpp_graph_itr ret(*this);
        ++op_index_;
        return ret;
    }
/* %$$
$end
*/

};

} } } // END_CPPAD_LOCAL_GRAPH_NAMESPACE

# endif
