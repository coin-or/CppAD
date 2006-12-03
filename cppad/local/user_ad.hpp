# ifndef CPPAD_USER_AD_INCLUDED
# define CPPAD_USER_AD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
---------------------------------------------------------------------------

$begin AD$$
$spell
	bool
	cos
	Cpp
$$

$section AD Objects$$

$index AD, object$$

$head Purpose$$
The sections listed below describe the operations 
that are available to $xref/glossary/AD of Base/AD of Base/$$ objects.
These objects are used to
$xref/glossary/Tape State/Recording/record/1/$$
an AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.
This operation sequence can
be transferred to an $xref/ADFun/$$ object where it
can be used to evaluate the corresponding 
function and derivative values.

$childtable%
	CppAD/local/constructor.hpp%
	CppAD/local/convert.hpp%
	CppAD/local/eq.hpp%
	CppAD/local/ad_valued.hpp%
	CppAD/local/bool_valued.hpp%
	CppAD/local/vec_ad.hpp
%$$

$end
---------------------------------------------------------------------------
*/

# include <CppAD/local/constructor.hpp>
# include <CppAD/local/convert.hpp>
# include <CppAD/local/eq.hpp>
# include <CppAD/local/ad_valued.hpp>
# include <CppAD/local/bool_valued.hpp>
# include <CppAD/local/vec_ad.hpp>

# endif
