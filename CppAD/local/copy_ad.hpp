# ifndef CPPAD_COPY_AD_INCLUDED
# define CPPAD_COPY_AD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------

$begin CopyAD$$
$spell
	Var
	const
$$

$index AD, copy construct$$
$index copy, AD construct$$
$index construct, copy AD$$

$section AD Copy Constructor$$

$head Syntax$$
$syntax%AD<%Base%> %y%(%x%)%$$
$pre
$$
$syntax%AD<%Base%> %y% = %x%$$


$head Purpose$$
Creates a new object $italic y$$ with the same value as $italic x$$.
The object $italic y$$ also has the same dependence on the 
$xref/glossary/Independent Variable/independent variables/$$ as $italic x$$.
($italic y$$ is a 
$xref/glossary/Variable/variable/$$ if and only if $italic x$$ is a variable).

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &x
%$$ 


$head Example$$
$children%
	Example/CopyAD.cpp
%$$
The file
$xref/CopyAD.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

# if 0
// use default copy constructor because it may be optimized better by compiler
template <class Base>
inline AD<Base>::AD(const AD &x) 
{
	value   = x.value;
	id      = x.id;
	taddr   = x.taddr;

	return;
}
# endif

template <class Base>
inline AD<Base>::AD(const VecAD_reference<Base> &x)
{	*this = x.ADBase(); }


} // END CppAD namespace

# endif
