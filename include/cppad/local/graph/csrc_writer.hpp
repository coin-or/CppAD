# ifndef CPPAD_LOCAL_GRAPH_CSRC_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_CSRC_WRITER_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/local/graph/cpp_graph_op.hpp>

/*
{xrst_begin csrc_writer dev}

Prototype for csrc_writer
#########################

Syntax
******
| ``csrc_writer`` ( *csrc* , *graph_obj* , *type*  )

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
namespace CppAD { namespace local { namespace graph {
   CPPAD_LIB_EXPORT void csrc_writer(
      std::ostream&       os          ,
      const cpp_graph&    graph_obj   ,
      const std::string&  type
   );
} } }
/* {xrst_code}
{xrst_spell_on}

See
***
:ref:`cpp_csrc_writer-name`

{xrst_end csrc_writer}
*/


# endif
