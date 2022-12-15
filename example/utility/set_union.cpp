// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin set_union.cpp}

Set Union: Example and Test
###########################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end set_union.cpp}
*/
// BEGIN C++
# include <cppad/utility/set_union.hpp>

bool set_union(void)
{  bool ok = true;

   // create empty sets
   std::set<size_t> left, right, result;

   // set left = {1, 2}
   left.insert(1);
   left.insert(2);

   // set right = {2, 3}
   right.insert(2);
   right.insert(3);

   // set result = {1, 2} U {2, 3}
   result = CppAD::set_union(left, right);

   // expected result
   size_t check_vec[] = {1, 2, 3};
   std::set<size_t> check_set(check_vec, check_vec + 3);

   // check result
   ok &= result == check_set;

   return ok;
}

// END C++
