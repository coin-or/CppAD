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
$begin xpackage_det_lu.cpp$$
$spell
    xpackage
    Lu
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io/en/latest/user_guide.html%current documentation%$$.
$$
$section Xpackage Speed: Gradient of Determinant Using Lu Factorization$$


$head Specifications$$
$cref link_det_lu$$

$head Implementation$$
A xpackage version of this test is not yet implemented
$srccode%cpp% */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_det_lu(
    size_t                     size     ,
    size_t                     repeat   ,
    CppAD::vector<double>     &matrix   ,
    CppAD::vector<double>     &gradient )
{   return false; }
/* %$$
$end
*/
