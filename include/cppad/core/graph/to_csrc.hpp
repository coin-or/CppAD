# ifndef CPPAD_CORE_GRAPH_TO_CSRC_HPP
# define CPPAD_CORE_GRAPH_TO_CSRC_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>
# include <cppad/core/graph/cpp_graph.hpp>
# include <cppad/local/graph/csrc_writer.hpp>
/*
------------------------------------------------------------------------------
$begin to_csrc$$
$spell
    Json
    cpp
    csrc
    cppad
    nx
    bool
    Vec
$$

$section C Source Code Corresponding to an ADFun Object$$

$head Syntax$$
$codei%
    %csrc% = %fun%.to_csrc(%os%, %options%)
%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head fun$$
is the $cref/ADFun/adfun/$$ object.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$head os$$
The C source code representation of the function $icode fun$$
is written to $icode os$$.

$head options$$
This map has the following possible keys:

$subhead type$$
If the key $code type$$ appears in $icode options$$,
the possible values are:
$code float$$, $code double$$, or $code long double$$.
This is the type used for operations in the generated C code.
If this key is not present, the type $code double$$ is used.

$head C Function$$

$subhead Syntax$$
$codei%cppad_forward_zero_%function_name%(
    %call_id%, %nx%, %x%, %ny%, %y%, %compare_change%
)%$$

$subhead call_id$$
is the $cref/call_id/atomic_four_call/call_id/$$ for the function call.

$subhead nx$$
is the number of independent variables plus number of independent dynamic
parameters for this function.

$subhead x$$
is a C vector of size $icode nx$$ and type $icode type$$,
containing the independent variables
and independent dynamic parameters.
The dynamic parameter come first and then the variables.

$subhead ny$$
is the number of dependent values for this function
(a dependent value can be a variable, dynamic parameter, or constant parameter).

$subhead y$$
is a C vector of size $icode ny$$ and type $icode type$$.
This input values of its elements do not matter.
Upon return, it contains the function value correspond to $icode x$$.

$subhead compare_change$$
the number of comparison operators that change their bool result value
is added to $icode compare_change$$. This way, $icode compare_change$$
can be used to accumulate the number of changes between multiplier calls.

$head Restrictions$$
The $code to_csrc$$ routine is not implemented for
$cref/VecAD/vecad/$$ operations.

$comment%
    example/csrc/to_csrc.cpp
%$$
$head Example$$
2DO: the file $code to_csrc.cpp$$ is an example and test of this operation.

$end
*/
# include <cppad/local/graph/csrc_writer.hpp>

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::to_csrc(
    std::ostream&                             os      ,
    const std::map<std::string, std::string>& options )
// END_PROTOTYPE
{   //
    // valid_key
    std::set<std::string> valid_key = { "type" };
    for( const auto& pair : options )
    {   const std::string& key = pair.first;
        CPPAD_ASSERT_KNOWN( valid_key.find(key) != valid_key.end(),
            "f.to_csrc(options): options contains an invalid key"
        );
    }
    // type
    std::string type = "double";
    if( options.find("type") != options.end() )
    {   type = options.at("type");
# ifndef NDEBUG
        bool ok = false;
        ok |= type == "float";
        ok |= type == "double";
        ok |= type == "long double";
        CPPAD_ASSERT_KNOWN(ok,
            "f.to_csrc: options[\"type\"] is not one of the following: "
            "float, double, lont_double"
        );
# endif
    }
    // to_graph return values
    cpp_graph graph_obj;
    //
    // graph corresponding to this function
    to_graph(graph_obj);
    //
    // os
    local::graph::csrc_writer(os, graph_obj, type);
    //
    return;
}

# endif
