# ifndef CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <string>
# include <cppad/local/graph/cpp_graph_op.hpp>

/*
$begin json_writer$$
$spell
    Json
    CppAD
    obj
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io/en/latest/user_guide.html%current documentation%$$.
$$
$section Json AD Graph Writer$$

$head Syntax$$
$codei%json_writer( %json%, %graph_obj% )%$$

$head json$$
The input value of $icode json$$ does not matter,
upon return it a $cref/json/json_ad_graph/$$ representation of the AD graph.

$head graph_obj$$
This is a $code cpp_graph$$ object.

$head Prototype$$
$srccode%hpp% */
namespace CppAD { namespace local { namespace graph {
    CPPAD_LIB_EXPORT void json_writer(
        std::string&       json        ,
        const cpp_graph&   graph_obj
    );
} } }
/* %$$
$end
*/


# endif
