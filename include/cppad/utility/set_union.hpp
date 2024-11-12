# ifndef CPPAD_UTILITY_SET_UNION_HPP
# define CPPAD_UTILITY_SET_UNION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin set_union}

Union of Standard Sets
######################

Syntax
******
| # ``include <cppad/utility/set_union.hpp>``
| *result* = ``set_union`` ( *left* , *right* )

Purpose
*******
This is a simplified (and restricted) interface to
the ``std::union`` operation.

Element
*******
This is the type of the elements of the sets.

left
****
This argument has prototype

   ``const std::set<`` *Element* >& *left*

right
*****
This argument has prototype

   ``const std::set<`` *Element* >& *right*

result
******
The return value has prototype

   ``std::set<`` *Element* >& *result*

It contains the union of *left* and *right* .
Note that C++11 detects that the return value is a temporary
and uses it for the result instead of making a separate copy.
{xrst_toc_hidden
   example/utility/set_union.cpp
}
Example
*******
The file :ref:`set_union.cpp-name` contains an example and test of this

{xrst_end set_union}
*/

# include <set>
# include <algorithm>
# include <iterator>

namespace CppAD {
   template <class Element>
   std::set<Element> set_union(
      const std::set<Element>&     left   ,
      const std::set<Element>&     right  )
   {  std::set<Element> result;
      std::set_union(
         left.begin()              ,
         left.end()                ,
         right.begin()             ,
         right.end()               ,
         std::inserter(result, result.begin())
      );
      return result;
   }
}

# endif
