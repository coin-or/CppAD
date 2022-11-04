// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>

// implicit constructor from double
bool implicit_ctor(void)
{  using CppAD::AD;
   bool ok = true;
   //
   AD< AD<double> > x = 5.0;
   ok &= Value(x) == 5.0;
   //
   return ok;
}
