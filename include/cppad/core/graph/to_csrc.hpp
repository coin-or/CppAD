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
$$

$section C Source Code Corresponding to an ADFun Object$$

$head Syntax$$
$codei%
    %csrc% = %fun%.to_csrc()
%$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head fun$$
is the $cref/ADFun/adfun/$$ object.

$head csrc$$
The return value of $icode csrc$$ is a
C source code representation of the corresponding function.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$head Restrictions$$
So far,
the $code to_csrc$$ routine is only implemented for the following operatoions:

$children%
    example/csrc/to_csrc.cpp
%$$
$head Example$$
The file $cref to_csrc.cpp$$ is an example and test of this operation.

$end
*/
# include <cppad/local/graph/csrc_writer.hpp>

// BEGIN_PROTOTYPE
template <class Base, class RecBase>
std::string CppAD::ADFun<Base,RecBase>::to_csrc(void)
// END_PROTOTYPE
{   //
    // to_graph return values
    cpp_graph graph_obj;
    //
    // graph corresponding to this function
    to_graph(graph_obj);
    //
    // convert to csrc
    std::string csrc;
    local::graph::csrc_writer(csrc, graph_obj);
    //
    return csrc;
}

# endif
