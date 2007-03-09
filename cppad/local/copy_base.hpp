# ifndef CPPAD_COPY_BASE_INCLUDED
# define CPPAD_COPY_BASE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------

$begin CopyBase$$
$spell
	const
	Var
	const
$$

$section AD From Base Type Constructor$$

$index construct, AD from Base$$
$index Base, construct AD from$$
$index AD, construct from Base$$

$head Syntax$$
$syntax%AD<%Base%> %x%(%b%)%$$
$pre
$$
$syntax%AD<%Base%> %x% = %b%$$


$head Purpose$$
Constructs an $syntax%AD<%Base%>%$$ object from an object of
type $italic Base$$. 
Directly after this construction, the object $italic x$$ is
a $xref/glossary/Parameter/parameter/$$.

$head b$$
The argument $italic b$$ has prototype
$syntax%
	const %Type% &%b%
%$$
where $italic Type$$ is $italic Base$$,
or is any is any type that can be converted to $italic Base$$;
i.e., any type for which the syntax 
$syntax%
	%Base%(%b%)
%$$
is valid.

$head Example$$
$children%
	example/copy_base.cpp
%$$
The file
$xref/CopyBase.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// conversion from Base to AD<Base>
// (if id_ is zero, taddr_ is not used, set anyway to avoid compile warning)
template <class Base>
inline AD<Base>::AD(const Base &b) : value_(b), id_(0), taddr_(0)
{ }	

// conversion from other types to AD<Base>
// (if id_ is zero, taddr_ is not used, set anyway to avoid compile warning)
template <class Base>
template <class T>
inline AD<Base>::AD(const T &t) : value_(Base(t)), id_(0), taddr_(0)
{ }

} // END CppAD namespace

# endif
