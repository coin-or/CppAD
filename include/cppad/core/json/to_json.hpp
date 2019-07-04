# ifndef CPPAD_CORE_JSON_TO_JSON_HPP
# define CPPAD_CORE_JSON_TO_JSON_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>

/*
$begin to_json$$
$spell
    Json
$$

$section Create a Json AD Graph Corresponding to an ADFun Object$$

$head Under Construction$$
This routine is under construction and subject to
change without backward compatibility.

$head Syntax$$
$codei%
    %graph% = %fun%.to_json()
%$$

$head Prototype$$
$srcfile%include/cppad/core/json/to_json.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head fun$$
is the $cref/ADFun/adfun/$$ object.

$head graph$$
The return value of $icode graph$$ is a
$cref json_ad_graph$$ representation of the corresponding function.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$childtable%
    example/json/to_json.cpp
%$$
$head Example$$
The file $cref to_json.cpp$$ is an example and test of this operation.

$end
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
std::string CppAD::ADFun<Base,RecBase>::to_json(void)
// END_PROTOTYPE
{   using local::pod_vector;

    // joint parameter information
    const size_t num_par = play_.num_par_rec();
    const Base* parameter = CPPAD_NULL;
    if( num_par > 0 )
        parameter = play_.GetPar();
    const pod_vector<bool>& dyn_par_is( play_.dyn_par_is() );
    CPPAD_ASSERT_UNKNOWN( dyn_par_is.size() == num_par );
    //
    // dynamic parameter information
    const pod_vector<local::opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
   const pod_vector<addr_t>& dyn_par_arg(           play_.dyn_par_arg() );
    const pod_vector<addr_t>& dyn_ind2par_ind (     play_.dyn_ind2par_ind() );
    size_t n_dynamic     = dyn_ind2par_ind.size();
    size_t n_dynamic_ind = play_.num_dynamic_ind();
    CPPAD_ASSERT_UNKNOWN( n_dynamic_ind <= n_dynamic );

    // start with the opening '{' for this graph
    std::string result = "{\n";

    // op_define_vec
    enum graph_code_enum {
        not_used_graph_code, // 0
        add_graph_code,      // 1
        mul_graph_code,      // 2
        sum_graph_code,      // 3
        number_graph_code    // 4
    };
    result +=
        "'op_define_vec' : [ 3, [\n"
        "{ 'op_code':1, 'name':'add', 'n_arg':2 } ,\n"
        "{ 'op_code':2, 'name':'mul', 'n_arg':2 } ,\n"
        "{ 'op_code':3, 'name':'sum'} ]\n"
        "],\n";
    //
    // initialize index of previous node in the graph
    size_t previous_node = 0;
    //
    // n_dynamic_ind
    result += "'n_dynamic_ind' : " + to_string( n_dynamic_ind ) + ",\n";
    previous_node += n_dynamic_ind;
    //
    // n_independent
    size_t n_independent = ind_taddr_.size();
    result += "'n_independent' : " + to_string( n_independent ) + ",\n";
    previous_node += n_independent;
    //
    // string_vec
    size_t n_string = 0;
    result += "'string_vec' : [ 0, [ ] ],\n";
    previous_node += n_string;
    // ----------------------------------------------------------------------
    // constant and par2node
    size_t dynamic_node  = 0;
    local::pod_vector_maybe<Base> constant;
    pod_vector<size_t> par2node(num_par);
    for(size_t i = 0; i < num_par; ++i)
        par2node[i] = 0; // invalid value
    if( num_par > 0 )
    {   CPPAD_ASSERT_UNKNOWN( isnan( parameter[0] ) );
        CPPAD_ASSERT_UNKNOWN( ! dyn_par_is[0] );
        for(size_t i = 1; i < num_par; ++i)
        {   if( ! dyn_par_is[i] )
            {   // this is a constant node
                par2node[i] = ++previous_node;
                constant.push_back( parameter[i] );
            }
            else
            {   if( dynamic_node < n_dynamic_ind )
                    par2node[i] = ++dynamic_node;
                // other dynamic parameters get set later
            }
        }
    }
    //
    // constant_vec
    size_t n_constant = constant.size();
    result += "'constant_vec' : [ " + to_string(n_constant) + ", [\n";
    for(size_t i = 0; i < n_constant; ++i)
    {   result += to_string( constant[i] );
        if( i + 1 < n_constant )
            result += ",\n";
        else
            result += " ] ],\n";
    }
    // ----------------------------------------------------------------------
    // op_usage_vec
    size_t n_usage = n_dynamic - n_dynamic_ind;
    CPPAD_ASSERT_UNKNOWN( play_.num_op_rec() >= 2 + n_independent );
    // skip BeginOp, EndOp, and independent variable operators
    n_usage += play_.num_op_rec() - 2 - n_independent;
    result += "'op_usage_vec' : [ " + to_string(n_usage) + ", [\n";
    size_t count_usage = 0;
    // ----------------------------------------------------------------------
    // dynamic parameter operations
    CPPAD_ASSERT_UNKNOWN( num_arg_dyn(local::ind_dyn) == 0 );
    CPPAD_ASSERT_UNKNOWN( dynamic_node == n_dynamic_ind );
    size_t i_arg = 0;
    std::string error_message =
    "Json conversion for following dynamic operator not yet implemented: ";
    pod_vector<size_t> node_arg(2);
    for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
    {   // operator for this dynamic parameter
        local::op_code_dyn op = local::op_code_dyn( dyn_par_op[i_dyn] );
        //
        // parameter index for this dynamic parameter
        size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
        CPPAD_ASSERT_UNKNOWN( par2node[i_par] == 0 );
        par2node[i_par] = ++previous_node;
        //
        // number of arguments for operators with fixed number of arguments
        size_t n_arg = size_t( num_arg_dyn(op) );
        CPPAD_ASSERT_UNKNOWN( n_arg <= 2 );
        //
        // arguments in graph node space
        for(size_t i = 0; i < n_arg; ++i)
        {   node_arg[i] = par2node[ dyn_par_arg[i_arg + i] ];
            CPPAD_ASSERT_UNKNOWN( node_arg[i] > 0 );
        }
        //
        switch(op)
        {
            case local::add_dyn:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            result += "[ " + to_string( size_t(add_graph_code) ) + ", ";
            result += to_string(node_arg[0]) + ", ";
            result += to_string(node_arg[1]) + " ]\n";
            i_arg  += n_arg;
            ++count_usage;
            break;

            default:
            error_message += op_name_dyn(op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    // ----------------------------------------------------------------------
    // variable operators
    local::play::const_sequential_iterator itr = play_.begin();
    size_t num_var = play_.num_var_rec();
    pod_vector<size_t> var2node(num_var);
    var2node[0] = 0; // invalide node value
    for(size_t i = 1; i <= n_independent; ++i)
        var2node[i] = n_dynamic_ind + i;
    for(size_t i = n_independent + 1; i < num_var; ++i)
        var2node[i] = 0; // invalid node value
    //
    // op_info
    local::OpCode op;
    const addr_t* arg;
    size_t        i_var;
    itr.op_info(op, arg, i_var);
    CPPAD_ASSERT_UNKNOWN( op == local::BeginOp ); // skip BeginOp
    //
    bool more_operators = true;
    error_message =
    "Json conversion for following variable operator not yet implemented: ";
    while(more_operators)
    {
        // next op
        (++itr).op_info(op, arg, i_var);
        CPPAD_ASSERT_UNKNOWN( itr.op_index() < play_.num_op_rec() );
        switch( op )
        {
            // --------------------------------------------------------------
            // CSumOp
            case local::CSumOp:
            var2node[i_var] = ++previous_node;
            if( (arg[1] != arg[2]) | (arg[3] != arg[4]) )
            {   error_message += "CSumOp with subtraction entries";
                CPPAD_ASSERT_KNOWN(false, error_message.c_str() );
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( arg[4] > 4 );
                size_t n_arg = size_t(arg[4] - 4);
                result += "[ " + to_string(size_t(sum_graph_code)) + ", 1, ";
                result += to_string(n_arg) + ", [ ";
                size_t arg_node  = par2node[ arg[0] ];
                result += to_string(arg_node) + ", ";
                for(addr_t i = 5; i < arg[1]; ++i)
                {   arg_node    = var2node[ arg[i] ];
                    CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                    result += to_string(arg_node);
                    if( i + 1 < arg[3] )
                        result += ", ";
                }
                for(addr_t i = arg[2]; i < arg[3]; ++i)
                {   arg_node  = par2node[ arg[i] ];
                    result   += to_string(arg_node);
                    if( i + 1 < arg[3] )
                        result += ", ";
                }
                result += "] ]";
            }
            itr.correct_before_increment();
            ++count_usage;
            if( count_usage < n_usage )
                result += " ,\n";
            break;
            // --------------------------------------------------------------

            // EndOp:
            case local::EndOp:
            more_operators = false;
            break;

            // InvOp: independent variables
            case local::InvOp:
            break;

            // MulvvOp:
            case local::MulvvOp:
            var2node[i_var] = ++previous_node;
            result += "[ " + to_string( size_t(mul_graph_code) ) + ", ";
            result += to_string( var2node[ arg[0] ] ) + ", ";
            result += to_string( var2node[ arg[1] ] ) + " ]";
            ++count_usage;
            if( count_usage < n_usage )
                result += " ,\n";
            break;

            default:
            error_message += local::OpName(op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    CPPAD_ASSERT_UNKNOWN( count_usage == n_usage );
    result += " ]\n] ,\n";
    // ----------------------------------------------------------------------
    // dependent_vec
    size_t n_dependent = dep_taddr_.size();
    result += "'dependent_vec' : [ " + to_string(n_dependent) + ", [\n";
    for(size_t i = 0; i < n_dependent; ++i)
    {   result += to_string( var2node[ dep_taddr_[i] ] );
        if( i + 1 < n_dependent )
            result += ",\n";
    }
    result += "] ]\n";
    result += "}\n";
    // ----------------------------------------------------------------------
    // Convert the single quote to double quote
    for(size_t i = 0; i < result.size(); ++i)
        if( result[i] == '\'' ) result[i] = '"';
    //
    return result;
}

# endif
