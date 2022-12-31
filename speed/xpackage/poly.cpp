/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin xpackage_poly.cpp$$
$spell
    xpackage
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io/en/latest/user_guide.html%current documentation%$$.
$$
$section Xpackage Speed: Second Derivative of a Polynomial$$


$head Specifications$$
$cref link_poly$$

$head Implementation$$
// a xpackage version of this test is not yet implemented
$srccode%cpp% */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_poly(
    size_t                     size     ,
    size_t                     repeat   ,
    CppAD::vector<double>     &a        ,  // coefficients of polynomial
    CppAD::vector<double>     &z        ,  // polynomial argument value
    CppAD::vector<double>     &ddp      )  // second derivative w.r.t z
{   return false; }
/* %$$
$end
*/
