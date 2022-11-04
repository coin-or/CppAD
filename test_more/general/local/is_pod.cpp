// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstddef>
# include <cppad/cppad.hpp>

bool is_pod(void)
{  bool ok = true;
   using CppAD::local::is_pod;
   //
   // Check all the cases that are the same as short int on
   // https://en.cppreference.com/w/cpp/language/types on 2020-12-04.
   ok &= is_pod<short>();
   ok &= is_pod<short int>();
   ok &= is_pod<signed short>();
   ok &= is_pod<signed short int>();
   //
   return ok;
}
