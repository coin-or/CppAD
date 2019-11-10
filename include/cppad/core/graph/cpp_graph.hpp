# ifndef CPPAD_CORE_GRAPH_CPP_GRAPH_HPP
# define CPPAD_CORE_GRAPH_CPP_GRAPH_HPP
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

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

class cpp_graph { // BEGIN_CPP_GRAPH_CLASS
public:
    typedef CppAD::graph::graph_op_enum graph_op_enum;
private:
    //
    std::string                   function_name_;
    vector<std::string>           atomic_name_vec_;
    size_t                        n_dynamic_ind_;
    size_t                        n_independent_;
    vector<double>                constant_vec_;
    vector<graph_op_enum>         operator_vec_;
    vector<size_t>                operator_arg_;
    vector<size_t>                dependent_vec_;
public:
    typedef local::graph::cpp_graph_itr const_iterator;
    //
    const_iterator begin(void) const
    {   size_t op_index = 0;
        return const_iterator(operator_vec_, operator_arg_, op_index);
    }
    const_iterator end(void)
    {   size_t op_index = operator_vec_.size();
        return const_iterator(operator_vec_, operator_arg_, op_index);
    }
/*
-------------------------------------------------------------------------------
$begin cpp_graph_ctor$$
$spell
    obj
    cpp
    ind
    vec
    arg
$$

$section C++ AD Graph Constructor$$

$head Syntax$$
$codei%cpp_graph %graph_obj%
%$$
$icode%graph_obj%.initialize()
%$$

$head Scalars$$
The default constructor and $code initialize$$ function set
$cref/function_name/cpp_ad_graph/function_name/$$ to the empty string.
They also set
$cref/n_dynamic_ind/cpp_ad_graph/n_dynamic_ind/$$ and
$cref/n_independent/cpp_ad_graph/n_independent/$$ to zero.

$head Vectors$$
The vectors
$cref/constant_vec/cpp_ad_graph/constant_vec/$$,
$cref/operator_vec/cpp_ad_graph/operator_vec/$$,
$cref/operator_arg/cpp_ad_graph/operator_arg/$$, and
$cref/dependent_vec/cpp_ad_graph/dependent_vec/$$ are set to empty vectors.

$end
--------------------------------------------------------------------------------
*/
public:
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
    cpp_graph(void)
    {   initialize(); }
/*
---------------------------------------------------------------------------------
$begin cpp_graph_scalar$$
$spell
    obj
    cpp
    ind
    std
    const
$$

$section C++ AD Graph Scalar Values$$

$head Syntax$$

$subhead Get$$
The object $icode graph_obj$$ is const durning the get functions.
$codei%
%function_name% = %graph_obj%.function_name_get()
%$$
$icode%n_dynamic_ind% = %graph_obj%.n_dynamic_ind_get()
%$$
$icode%n_independent% = %graph_obj%.n_independent_get()
%$$

$subhead Set$$
THe arguments to the set functions are const
and $icode graph_obj$$ is not const.
$codei%
%graph_obj%.function_name_set(%function_name%)
%$$
$icode%graph_obj%.n_dynamic_ind_set(%n_dynamic_ind%)
%$$
$icode%graph_obj%.n_independent_set(%n_independent%)
%$$

$head graph_obj$$
is an $code cpp_graph$$ object.

$head function_name$$
is a $code std::string$$ specifying the name of the function
for this graph.

$head n_dynamic_ind$$
is a $code size_t$$ specifying the number of independent dynamic parameters.

$head n_independent$$
is a $code size_t$$ specifying the number of independent variables.

$end
*/
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
/*
---------------------------------------------------------------------------------
$begin cpp_graph_vector$$
$spell
    obj
    cpp
    ind
    std
    const
    vec
    arg
    op_enum
$$

$section C++ AD Graph Vector Values$$

$head Syntax$$

$subhead Size$$
The return value $icode size$$ is the
number of elements currently in the corresponding vector.
The object $icode graph_obj$$ is const durning these operations:
$codei%
%size%       = %graph_obj%.atomic_name_vec_size()
%$$
$icode%size% = %graph_obj%.constant_vec_size()
%$$
$icode%size% = %graph_obj%.operator_vec_size()
%$$
$icode%size% = %graph_obj%.operator_arg_size()
%$$
$icode%size% = %graph_obj%.dependent_vec_size()
%$$

$subhead Get$$
The argument $icode index$$ below is a const $code size_t$$ specifying the
index in the corresponding vector.
It must be less than the size of the vector being indexed.
The object $icode graph_obj$$ is const durning these operations:
$codei%
%atomic_name%     = %graph_obj%.atomic_name_vec_get(%index%)
%$$
$icode%constant%  = %graph_obj%.constant_vec_get(%index%)
%$$
$icode%operator%  = %graph_obj%.operator_vec_get(%index%)
%$$
$icode%argument%  = %graph_obj%.operator_arg_get(%index%)
%$$
$icode%dependent% = %graph_obj%.dependent_vec_get(%index%)
%$$

$subhead Push Back$$
These functions add an element at the back of the corresponding vector.
The element index is the size of the vector just prior to the push back.
The arguments to the push back calls are const,
but the $icode graph_obj$$ is not const.
$codei%
%graph_obj%.atomic_name_vec_push_back(%atomic_name%)
%$$
$icode%graph_obj%.constant_vec_push_back(%constant%)
%$$
$icode%graph_obj%.operator_vec_push_back(%operator%)
%$$
$icode%graph_obj%.operator_arg_push_back(%argument%)
%$$
$icode%graph_obj%.dependent_vec_get(%dependent%)
%$$

$head graph_obj$$
is an $code cpp_graph$$ object.

$head atomic_name$$
is a $code std::string$$ equal to the name of an $cref atomic_three$$ function.

$head constant$$
is a $code double$$ equal to the constant with the corresponding
index in $code constant_vec$$.

$head operator$$
is the $cref graph_op_enum$$ for corresponding operator.

$head argument$$
is the $code size_t$$ value for corresponding operator argument.

$end
*/
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
    const graph_op_enum& operator_vec_get(size_t index) const
    {   return operator_vec_[index]; }
    size_t operator_vec_size(void) const
    {   return operator_vec_.size(); }
    void operator_vec_push_back(const graph_op_enum& op_usage)
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

}; // END CPP_GRAPH_CLASS

} // END_CPPAD_NAMESPACE

# endif
