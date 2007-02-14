# ifndef CPPAD_BOOL_FUN_LINK_INCLUDED
# define CPPAD_BOOL_FUN_LINK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin BoolFunLink$$ $comment CppAD Developer Documentation$$

$spell
	inline
	bool
	Cpp
	const
$$

$section CppAD Boolean Valued User Defined Functions Implementation$$

$head UnaryBool$$
$index UnaryBool$$
The procedure call
$syntax%
inline bool UnaryBool( 
	bool %FunName%(const %Base% &%x%),
	const AD<%Base%> &%x%)
%$$
uses a call to the procedure $italic FunName$$ to evaluate a unary 
boolean valued function of a base type argument
and returns it as the value of the corresponding AD argument.


$head BinaryBool$$
$index BinaryBool$$
The procedure call
$syntax%
inline bool BinaryBool( 
	bool %FunName%(const %Base% &%x%, const %Base% &%y%),
	const AD<%Base%> &%x%, const AD<%Base%> &%y%)
%$$
uses a call to the procedure $italic FunName$$ to evaluate a binary 
boolean valued function of two base type arguments
and returns it as the value of the corresponding AD arguments.



$end
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline bool AD<Base>::UnaryBool(
	bool FunName(const Base &x),
	const AD<Base> &x
) 
{
	return FunName(x.value_);
}

template <class Base>
inline bool AD<Base>::BinaryBool(
	bool FunName(const Base &x, const Base &y),
	const AD<Base> &x, const AD<Base> &y
) 
{
	return FunName(x.value_, y.value_);
}

} // END CppAD namespace

# endif
