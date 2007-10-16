# ifndef CPPAD_UNIFORM_01_INCLUDED
# define CPPAD_UNIFORM_01_INCLUDED 

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin uniform_01$$
$spell
	CppAD
	cppad.hpp
	namespace
$$

$section Simulate a [0,1] Uniform Random Variate$$ 

$index uniform_01$$
$index random, uniform vector$$
$index uniform, random vector$$
$index vector, uniform random$$

$head Syntax$$
$syntax%# include <cppad/speed/uniform_01.hpp>
%$$
$syntax%uniform_01(%seed%)
%$$
$syntax%uniform_01(%n%, %x%)%$$

$head Purpose$$
This routine is used to create random values for speed testing purposes.

$head Inclusion$$
The template function $code uniform_01$$ is defined in the $code CppAD$$
namespace by including 
the file $code cppad/speed/uniform_01.hpp$$ 
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$cref/cppad.hpp/cppad/$$.

$head seed$$
The argument $italic seed$$ has prototype
$syntax%
	size_t %seed%
%$$
It specifies a seed
for the uniform random number generator.

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$
It specifies the number of elements in the random vector $italic x$$.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	%Vector% &%x%
%$$.
The input value of the elements of $italic x$$ does not matter.
Upon return, the elements of $italic x$$ are set to values
randomly sampled over the interval [0,1].

$head Vector$$
If $italic y$$ is a $code double$$ value,
the object $italic x$$ must support the syntax
$syntax%
	%x%[%i%] = %y%
%$$
where $italic i$$ has type $code size_t$$ with value less than
or equal $latex n-1$$.
This is the only requirement of the type $italic Vector$$.

$children%
	omh/uniform_01_hpp.omh
%$$

$head Source Code$$
The file 
$cref/uniform_01.hpp/$$
constraints the source code for this template function.

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cstdlib>

namespace CppAD {
	inline void uniform_01(size_t seed)
	{	std::srand( (unsigned int) seed); }

	template <class Vector>
	void uniform_01(size_t n, Vector &x)
	{	static double factor = 1. / double(RAND_MAX);
		while(n--)
			x[n] = std::rand() * factor;
	}
}
// END PROGRAM
# endif
