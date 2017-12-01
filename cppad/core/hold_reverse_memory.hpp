# ifndef CPPAD_CORE_HOLD_REVERSE_MEMORY_HPP
# define CPPAD_CORE_HOLD_REVERSE_MEMORY_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin hold_reverse_memory$$
$spell
	Taylor
	const
	bool
$$
$section Hold onto Reverse Mode Memory for Future Use$$

$head Syntax$$
$icode%f%.hold_reverse_memory(%b%)
%$$
$icode%b% = %f%.hold_reverse_memory()
%$$

$head Purpose$$
The $cref reverse$$ mode routines compute a derivatives for a number
of Taylor coefficients and every variable in the operation sequence.
This routine allows one to decide if that memory should be held onto
by $icode f$$ between calls these routines to avoid repeated memory allocation.
Setting this value to true
may speed up calculations and require more memory.

$head f$$
For the syntax where $icode b$$ is an argument,
$icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
(see $codei%ADFun<%Base%>%$$ $cref/constructor/FunConstruct/$$).
For the syntax where $icode b$$ is the result,
$icode f$$ has prototype
$codei%
	const ADFun<%Base%> %f%
%$$

$head b$$
This argument or result has prototype
$codei%
	bool %b%
%$$
Future calls to $icode%f%.Forward%$$ will (will not) hold onto memory
between calls to reverse mode routines
depending on if $icode b$$ is true (false).

$head Default$$
The value for this setting after construction of $icode f$$) is false.
The value of this setting is not affected by calling
$cref Dependent$$ for this function object.

$head Example$$
$children%
	example/general/hold_reverse_memory.cpp
%$$
The file
$cref hold_reverse_memory.cpp$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
*/

# include <cppad/utility/vector.hpp>
# include <cppad/configure.hpp>
# include <fstream>

# if CPPAD_HAS_MKSTEMP
# include <stdlib.h>
# include <unistd.h>
# else
# if CPPAD_HAS_TMPNAM_S
# include <stdio.h>
# else
# include <stdlib.h>
# endif
# endif


namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Set hold_reverse_memory

\param value
new value for this flag.
*/
template <typename Base>
void ADFun<Base>::hold_reverse_memory(bool value)
{	hold_reverse_memory_ = value;
	if( ! hold_reverse_memory_ )
		partial_.clear();
}

/*!
Get hold_reverse_memory

\return
current value of hold_reverse_memory.
*/
template <typename Base>
bool ADFun<Base>::hold_reverse_memory(void) const
{	return hold_reverse_memory_; }

} // END_CPPAD_NAMESPACE

# endif
