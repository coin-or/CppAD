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
$begin cppad_link_unary$$

$section Extern C Links To Unary Functions Needed by llvm_link Objects$$

$head Syntax$$
$icode%y% = cppad_link_%unary_fun%(%x%)
%$$

$head Prototype$$
$codei%
    extern "C" double cppad_link_%unary_fun%(double %x%)
%$$

$head x$$
This argument has prototype $codei%double %x%$$.

$head z$$
The result has prototype $codei%double %z%$$ and is the value of
of the function $codei%std::%unary_fun%$$ at the argument $icode x$$.

$head unary_fun$$
Only the following functions need linking (so far):
$code fabs$$.

$end
*/
# include <cmath>

extern "C" double cppad_link_fabs(double x)
{   return std::fabs(x); }
