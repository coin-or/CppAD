/* $Id$ */
# ifndef CPPAD_EPSILON_INCLUDED
# define CPPAD_EPSILON_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------
$begin epsilon$$
$spell
	std
	eps
	CppAD
	namespace
	const
$$
$index AD, epsilon$$
$index epsilon, AD$$

$section Machine Epsilon For AD Types$$

$head Syntax$$
$icode%eps% = epsilon<%Float%>()%$$

$head Purpose$$
Obtain the value of machine epsilon corresponding
to the type $icode%Float%$$.

$head Float$$
this type can either be $codei%AD<%Base%>%$$,
or it can be $icode Base$$ for any $codei%AD<%Base%>%$$ type;
see $cref/base_std_math/base_std_math/epsilon/$$.

$head eps$$
The result $icode eps$$ has prototype
$codei%
	%Float% eps
%$$

$head Example$$
$children%
	example/epsilon.cpp
%$$
The file
$cref epsilon.cpp$$
contains an example and test of this operation.

$end 
------------------------------------------------------------------------------
*/

namespace CppAD {

	template <class ADtype>
	inline ADtype epsilon(void)
	{	typedef typename ADtype::value_type value_type;
		return ADtype ( epsilon<value_type>() ); 
	}

}
# endif
