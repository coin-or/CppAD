/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin cppad_link$$
$spell
    llvm
$$

$section Extern C Links To Functions Needed by llvm_link Objects$$

$srcthisfile%
    0%// BEGIN_C++%// END_C++%1
%$$

$end
*/
# include <cmath>
// BEGIN_C++
//
// fabs
extern "C" double cppad_link_fabs(double x)
{   return std::fabs(x); }
//
// sign
extern "C" double cppad_link_sign(double x)
{   return double(x > 0.0) - double(x < 0.0); }
//
// END_C++
