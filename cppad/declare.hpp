# ifndef CPPAD_DECLARE_INCLUDED
# define CPPAD_DECLARE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/* 
$begin declare.hpp$$
$spell
	hpp
	enum
	namespace
	Op
	Lt
	Le
	Eq
	Ge
	Gt
	Ne
$$

$section declare.hpp$$

$head Purpose$$
This enum type is used to define the $cref/CondExp/$$ operation:

$codep */

namespace CppAD {

	// The conditional expression operator enum type
	enum CompareOp 
	{	CompareLt, // less than
		CompareLe, // less than or equal
		CompareEq, // equal
		CompareGe, // greater than or equal
		CompareGt, // greater than
		CompareNe  // not equal
	};

}
/* $$
$end
*/

# endif
