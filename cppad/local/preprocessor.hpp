/* This file may be inclued multiple times so comment out ifndef and define
# ifndef CPPAD_PREPROCESSOR_INCLUDED
# define CPPAD_PREPROCESSOR_INCLUDED
*/

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin preprocessor$$
$spell
	CppAD
	undef
$$

$section Preprocessor Definitions Used by CppAD$$
$index preprocessor, symbol$$
$index symbol, preprocessor$$
$index PACKAGE, preprocessor symbol$$
$index VERSION, preprocessor symbol$$

$head Rule$$
All of the preprocessor symbols used by CppAD begin either with
$code CppAD$$ or with $code CPPAD_$$.

$head Example$$
For example,
the preprocessor symbol 
$small $cref/CPPAD_TEST_VECTOR/test_vector/$$ $$ 
determines which
$xref/SimpleVector/$$ template class is extensively used 
by the tests in the $code Example$$ and $code TestMore$$ directories.

$head Exceptions$$
The following is a list of exceptions to the rule above.
These preprocessor symbols may be undefined
after you include any CppAD include file.

$codep */

# undef PACKAGE
# undef PACKAGE_BUGREPORT
# undef PACKAGE_NAME
# undef PACKAGE_STRING
# undef PACKAGE_TARNAME
# undef PACKAGE_VERSION
# undef VERSION
/* $$
$end
*/

// other preprocessor symbols defined in config.h
# undef CPPAD_GETTIMEOFDAY
# undef CPPAD_BOOSTVECTOR
# undef CPPAD_CPPADVECTOR
# undef CPPAD_STDVECTOR
