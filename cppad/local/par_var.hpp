# ifndef CPPAD_PAR_VAR_INCLUDED
# define CPPAD_PAR_VAR_INCLUDED

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

$begin ParVar$$
$spell
	const
	bool
$$

$index Parameter$$
$index Variable$$
$section Is an AD Object a Parameter or Variable$$

$head Syntax$$
$syntax%%b% = Parameter(%x%)%$$
$pre
$$
$syntax%%b% = Variable(%x%)%$$


$head Purpose$$
Determine if $italic x$$ is a 
$xref/glossary/Parameter/parameter/$$ or 
$xref/glossary/Variable/variable/$$. 

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	bool %b%
%$$
The return value for $code Parameter$$ ($code Variable$$)
is true if and only if $italic x$$ is a parameter (variable).

$head Operation Sequence$$
The result of this operation is not an
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ object.
Thus it will not be recorded as part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
(even if the current $xref/glossary/Tape State/tape state/$$ is Recording).

$head Example$$
$children%
	example/par_var.cpp
%$$
The file
$xref/ParVar.cpp/$$
contains an example and test of these functions.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

namespace CppAD {
	template <class Base>
	inline bool Parameter(const AD<Base> &x)
	{	CppADUnknownError( x.id_ != *ADTape<Base>::Id() || x.taddr_ > 0 );
		return ( x.id_ != *ADTape<Base>::Id() ); 
	}
	template <class Base>
	inline bool Variable(const AD<Base> &x)
	{	CppADUnknownError( x.id_ != *ADTape<Base>::Id() || x.taddr_ > 0 );
		return ( x.id_ == *ADTape<Base>::Id() ); 
	}
} 
// END CppAD namespace


# endif
