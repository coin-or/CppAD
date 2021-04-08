/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cmath>
/*
$begin cppad_link_fabs$$
$spell
    cppad
    fabs
$$

$section The cppad_link_fabs Function$$

$head Source$$
$srccode%cpp% */
extern "C" double cppad_link_fabs(double x)
{   return std::fabs(x); }
/* %$$
$end
------------------------------------------------------------------------------

$begin cppad_link_sign$$
$spell
    cppad
$$

$section The cppad_link_sign Function$$

$head Source$$
$srccode%cpp% */
extern "C" double cppad_link_sign(double x)
{   return double(x > 0.0) - double(x < 0.0); }
/* %$$
$end
------------------------------------------------------------------------------
*/
