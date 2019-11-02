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
$begin cpp_graph_itr_data$$

$section C++ AD Graph Iterator Private Member Data$$
$srccode%hpp% */
private:
    // valuse set by constructor
    const vector<graph_op_struct>* operator_vec_;
    const vector<size_t>*          operator_arg_;
    //
    // set by constructor and ++
    size_t                         op_index_;
    //
    // set by get_op_info
    graph_op_enum                  op_enum_;
    size_t                         name_index_;
    size_t                         n_result_;
    vector<size_t>                 arg_node_;
/* %$$
$end
------------------------------------------------------------------------------
$begin cpp_graph_itr_get_op_info$$
$spell
    obj
    op
    arg
    vec
    enum
$$

$section C++ AD Graph Get Information for One Operator$$

$head Syntax$$
$icode%graph_itr%.get_op_info()%$$

$head op_enum_$$
This is set to the
$cref/graph_op_enum/cpp_graph_op/graph_op_enum/$$ for
the operator usage corresponding to $icode op_index_$$.

$head name_index_$$
If $icode op_enum_$$ is $code atom_graph_op$$,
this is set to the index in
$cref/atomic_name_vec/cpp_ad_graph/atomic_name_vec/$$
for the function called by this operator usage.

$head n_result_$$
This is set to the number of result nodes for this operator usage.

$head arg_node_$$
Upon return, its size is the number of arguments,
that are node indices, for this operator usage.
The value of the elements are the node indices.

$head Prototype$$
$srccode%hpp% */
    void get_op_info(void)
/* %$$
$enc
*/
{   // initialize output values
    size_t invalid_index   = std::numeric_limits<size_t>::max();
    size_t n_arg = invalid_index;
    name_index_  = invalid_index;
    n_result_    = invalid_index;
    arg_node_.resize(0);
    //
    // op_enum, first_node
    op_enum_          = (*operator_vec_)[op_index_].op_enum;
    size_t first_node = (*operator_vec_)[op_index_].first_node;
    //
    // n_result_, n_arg (name_index if op_enum is atom_graph_op)
    switch( op_enum_ )
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
        n_result_  = 1;
        n_arg      = 1;
        break;

        // binary operators
        case add_graph_op:
        case div_graph_op:
        case mul_graph_op:
        case sub_graph_op:
        n_result_  = 1;
        n_arg      = 2;
        break;

        // atom_graph_op
        case atom_graph_op:
        name_index_ = (*operator_arg_)[first_node- 3];
        n_result_   = (*operator_arg_)[first_node- 2];
        n_arg       = (*operator_arg_)[first_node- 1];
        break;

        // conditional expressions
        case cexp_eq_graph_op:
        case cexp_le_graph_op:
        case cexp_lt_graph_op:
        n_result_  = 1;
        n_arg      = 4;
        break;

        // comparison operators
        case comp_eq_graph_op:
        case comp_le_graph_op:
        case comp_lt_graph_op:
        case comp_ne_graph_op:
        n_result_  = 0;
        n_arg      = 2;
        break;

        // sum_graph_op
        case sum_graph_op:
        n_result_  = 1;
        n_arg      = (*operator_arg_)[first_node- 1];
        break;

        default:
        CPPAD_ASSERT_UNKNOWN(false);
        break;
    }
    // set arg_node
    arg_node_.resize(n_arg);
    for(size_t i = 0; i < n_arg; i++)
        arg_node_[i] = (*operator_arg_)[first_node+ i];

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
    // default ctor
    cpp_graph_itr(void) :
    operator_vec_(CPPAD_NULL) ,
    operator_arg_(CPPAD_NULL) ,
    op_index_(0)
    { }
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
    {   CPPAD_ASSERT_KNOWN( operator_vec_ != CPPAD_NULL,
            "cpp_graph_itr: attempt to dereference default iterator"
        );
        get_op_info();
        value_type ret( {op_enum_, name_index_, n_result_, &arg_node_} );
        return ret;
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
