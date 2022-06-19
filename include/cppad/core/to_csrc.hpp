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
    typedef
    const
    namespace
$$

$section C Source Code Corresponding to an ADFun Object$$

$head Syntax$$
$codei%
    %fun%.to_csrc(%os%, %options%)
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
$icode%flag% = cppad_forward_zero_%function_name%(
    %call_id%, %nx%, %x%, %ny%, %y%, %compare_change%
)%$$

$subhead Function Type$$
The syntax above corresponds to the following function type,
which is defined in the CppAD namespace:
$codei%
typedef int (*%type%_forward_zero)(
    size_t, size_t, const %type%*, size_t, %type%*, size_t*
)%$$
In the case of the Visual C++ compiler ($code _MSC_VER$$ is defined),
$code __cdecl$$ and $code __declspec(dllimport)$$ are added this
type definition.

$subhead type$$
In the function type above $icode type$$ is the same as
$icode type$$ in the $icode options$$ argument except in the
long double case where it is $code long_double$$.

$subhead call_id$$
This argument is only used during atomic four function callbacks,
in which case it is the corresponding
$cref/call_id/atomic_four_call/call_id/$$.

$subhead nx$$
is the number of independent variables plus number of independent dynamic
parameters for this function.

$subhead x$$
is a C vector of size $icode nx$$ containing the independent variables
and independent dynamic parameters.
The dynamic parameter come first and then the variables.

$subhead ny$$
is the number of dependent values for this function
(a dependent value can be a variable, dynamic parameter, or constant parameter).

$subhead y$$
is a C vector of size $icode ny$$.
This input values of its elements do not matter.
Upon return, it contains the function value correspond to $icode x$$.

$subhead compare_change$$
This argument is both an input and an output.
The number of comparison operators that change their bool result value
is added to $icode compare_change$$. This way, $icode compare_change$$
can be used to accumulate the number of changes between multiplier calls.

$subhead flag$$
If this is zero, no error was detected.
If it is one (two), $icode nx$$ ($icode ny$$) does not have its expected value.

$head Restrictions$$
The $code to_csrc$$ routine is not implemented for
$cref/VecAD/vecad/$$ operations.

$children%
    example/jit/jit.omh
%$$
$head Example$$
The section $cref example_jit$$ contains examples and tests
that use $code to_csrc$$.


$end
*/
# include <cppad/local/graph/csrc_writer.hpp>

# ifdef _MSC_VER
# define CPPAD_FUN_TYPE __cdecl
# define CPPAD_IMPORT   __declspec(dllimport)
# else
# define CPPAD_FUN_TYPE
# define CPPAD_IMPORT
# endif


namespace CppAD {
    extern "C" {
        CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *float_forward_zero)(
            size_t, size_t, const float*, size_t, float*, size_t*
        );
        CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *double_forward_zero)(
            size_t, size_t, const double*, size_t, double*, size_t*
        );
        CPPAD_IMPORT typedef int (CPPAD_FUN_TYPE *long_double_forward_zero)(
            size_t, size_t, const long double*, size_t, long double*, size_t*
        );
    }
}

# undef CPPAD_FUN_TYPE
# undef CPPAD_IMPORT

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::to_csrc(
    std::ostream&                             os      ,
    const std::map<std::string, std::string>& options )
// END_PROTOTYPE
{   //
# ifndef NDEBUG
    // valid_key
    std::set<std::string> valid_key = { "type" };
    for( const auto& pair : options )
    {   const std::string& key = pair.first;
        CPPAD_ASSERT_KNOWN( valid_key.find(key) != valid_key.end(),
            "f.to_csrc(options): options contains an invalid key"
        );
    }
# endif
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
