/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_ode.cpp$$
$spell
	Jacobian
	Sacado
	cstring
	cppad
	hpp
	bool
	retape
	CppAD
	typedef
	ADvar
	Gradcomp
	endif
	cassert
$$

$section Sacado Speed: Gradient of Ode Solution$$

$index sacado, speed ode gradient$$
$index speed, sacado ode gradient$$
$index gradient, ode speed sacado$$
$index ode, gradient speed sacado$$

$head link_ode$$
$index link_ode$$
$codep */

# include <cstring>
# include <cppad/vector.hpp>

// The sacado version of this test is not yet available
bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &jacobian
)
{
	return false;
}

/* $$
$end
*/
