/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_ode.cpp$$
$spell
	cstring
	cppad
	hpp
	retape
        Fadbad
        bool
        CppAD
$$

$section Fadbad Speed: Ode$$

Indicate that this test is not available:
$codep */

// The fadbad version of this test is not yet available

# include <cstring>
# include <cppad/vector.hpp>

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	bool                       retape     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &gradient
)
{
	return false;
}
/* $$
$end
*/
