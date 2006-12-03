# ifndef CPPAD_ARITHMETIC_INCLUDED
# define CPPAD_ARITHMETIC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin Arithmetic$$
$spell
	Op
	const
$$

$index operator, AD arithmetic$$
$index AD, arithmetic operator$$
$index arithmetic, AD operator$$

$index computed, AD assignment$$
$index assignment, AD computed$$
$index AD, computed assignment$$

$section AD Arithmetic Operators and Computed Assignments$$

$childtable%
	cppad/local/unary_plus.hpp%
	cppad/local/unary_minus.hpp%
	cppad/local/add.hpp%
	cppad/local/sub.hpp%
	cppad/local/mul.hpp%
	cppad/local/div.hpp%
	cppad/local/add_eq.hpp%
	cppad/local/sub_eq.hpp%
	cppad/local/mul_eq.hpp%
	cppad/local/div_eq.hpp
%$$

$end
-------------------------------------------------------------------------------
*/
# include <cppad/local/unary_plus.hpp>
# include <cppad/local/unary_minus.hpp>
# include <cppad/local/add.hpp>
# include <cppad/local/sub.hpp>
# include <cppad/local/mul.hpp>
# include <cppad/local/div.hpp>
# include <cppad/local/add_eq.hpp>
# include <cppad/local/sub_eq.hpp>
# include <cppad/local/mul_eq.hpp>
# include <cppad/local/div_eq.hpp>

# endif
