# ifndef CPPAD_CORE_UNARY_PLUS_HPP
# define CPPAD_CORE_UNARY_PLUS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin UnaryPlus$$
$spell
   Vec
   const
   inline
$$


$section AD Unary Plus Operator$$

$head Syntax$$

$icode%y% = + %x%$$


$head Purpose$$
Performs the unary plus operation
(the result $icode y$$ is equal to the operand $icode x$$).


$head x$$
The operand $icode x$$ has one of the following prototypes
$codei%
   const AD<%Base%>               &%x%
   const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The result $icode y$$ has type
$codei%
   AD<%Base%> %y%
%$$
It is equal to the operand $icode x$$.

$head Operation Sequence$$
This is an AD of $icode Base$$
$cref/atomic operation/glossary/Operation/Atomic/$$
and hence is part of the current
AD of $icode Base$$
$cref/operation sequence/glossary/Operation/Sequence/$$.

$head Derivative$$
If $latex f$$ is a
$cref/Base function/glossary/Base Function/$$,
$latex \[
   \D{[ + f(x) ]}{x} = \D{f(x)}{x}
\] $$



$head Example$$
$children%
   example/general/unary_plus.cpp
%$$
The file
$cref unary_plus.cpp$$
contains an example and test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::operator + (void) const
{  AD<Base> result(*this);

   return result;
}


template <class Base>
AD<Base> operator + (const VecAD_reference<Base> &right)
{  return right.ADBase(); }

}
//  END CppAD namespace


# endif
