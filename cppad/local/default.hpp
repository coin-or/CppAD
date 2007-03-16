# ifndef CPPAD_DEFAULT_INCLUDED
# define CPPAD_DEFAULT_INCLUDED

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

$begin Default$$
$spell
$$

$section AD Default Constructor$$

$index AD, default construct$$
$index construct, default$$
$index default, constructor$$

$head Syntax$$
$syntax%AD<%Base%> %x%;%$$


$head Purpose$$
Constructs an AD object with an unspecified value.
Directly after this construction,
the object is a $xref/glossary/Parameter/parameter/$$.


$head Example$$
$children%
	example/default.cpp
%$$
The file
$xref/Default.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// default constructor
// (if id_ is 1, taddr_ is not used, set anyway to avoid compile warning)
template <class Base>
inline AD<Base>::AD(void) : id_(1), taddr_(0)
{ }	

} // END CppAD namespace

# endif
