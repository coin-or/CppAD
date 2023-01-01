# ifndef CPPAD_LOCAL_GRAPH_CSRC_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_CSRC_WRITER_HPP

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
$begin csrc_writer$$
$spell
    CppAD
    obj
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io%current documentation%$$.
$$
$section C Source Code Writer$$

$head Syntax$$
$codei%csrc_writer( %csrc%, %graph_obj% )%$$

$head os$$
The C source code representation of an AD function
is written to $icode os$$.

$head graph_obj$$
This is a $code cpp_graph$$ representation of the function.

$head Prototype$$
$srccode%hpp% */
namespace CppAD { namespace local { namespace graph {
    CPPAD_LIB_EXPORT void csrc_writer(
        std::ostream&       os          ,
        const cpp_graph&    graph_obj   ,
        const std::string&  type
    );
} } }
/* %$$
$end
*/


# endif
