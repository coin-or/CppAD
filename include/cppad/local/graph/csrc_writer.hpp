# ifndef CPPAD_LOCAL_GRAPH_CSRC_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_CSRC_WRITER_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/local/graph/cpp_graph_op.hpp>

/*
$begin csrc_writer$$
$spell
   CppAD
   obj
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
