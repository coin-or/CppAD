# ifndef CPPAD_DET_BY_MINOR_INCLUDED
# define CPPAD_DET_BY_MINOR_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin DetByMinor$$
$spell
	hpp
	Det
	CppADvector
	namespace
$$

$section Determinant Using Expansion by Minors$$

$table
$bold Syntax$$
$rnext $cnext $syntax%# include "example/det_by_minor.hpp"%$$
$rnext $cnext $syntax%DetByMinor<%Type%> %Det%(size_t %n%)%$$
$rnext $cnext $syntax%%Type% %Det%(CppADvector<%Type%> &%A%)%$$
$tend

$fend 25$$

$head Inclusion$$
The template function $code DetByMinor$$ is defined in the $code CppAD$$
namespace by including 
the file $code example/det_by_minor.hpp$$.
It is only intended for example and testing purposes, 
so it is not automatically included by
$xref/CppAD//CppAD.h/$$.

$head Constructor$$
The syntax
$syntax%
	DetByMinor<%Type%> %Det%(size_t %n%)
%$$
constructs the object $italic Det$$ which can be used for 
evaluating the determinant of $italic n$$ by $italic n$$ matrices
using expansion by minors.

$head Evaluation$$
The syntax
$syntax%
	%Type% %Det%(CppADvector<%Type%> &%A%)
%$$
returns the determinant of $italic A$$ using expansion by minors.

$children%
	example/det_by_minor.cpp
%$$


$head Example$$
The file
$xref/DetByMinor.cpp/$$ 
contains an example and test of $code det_by_minor.hpp$$.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The file
$xref/DetByMinor.h/$$ 
contains the source for this template function.


$end
---------------------------------------------------------------------------
$begin DetByMinor.h$$
$spell
	Cpp
	ifndef
	endif
	Det
	const
$$

$index DetByMinor$$
$mindex determinant minor matrix$$
$section Determinant using Expansion by Minors: Source Code$$

$code
# ifndef CPPAD_DET_BY_MINOR_INCLUDED
$pre
$$
# define CPPAD_DET_BY_MINOR_INCLUDED

$verbatim%example/det_by_minor.hpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$

# endif
$$

$end
---------------------------------------------------------------------------

*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "det_of_minor.hpp"

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
class DetByMinor {
public:
	DetByMinor(size_t m) : m_(m) , r_(m + 1) , c_(m + 1), a_(m * m)
	{
		size_t i;

		// values for r and c that correspond to entire matrix
		for(i = 0; i < m; i++)
		{	r_[i] = i+1;
			c_[i] = i+1;
		}
		r_[m] = 0;
		c_[m] = 0;
	}

	inline Type operator()(const CppADvector<Type> &x) const
	{	size_t i = m_ * m_;
		while(i--)
			a_[i] = x[i];
		return det_of_minor(a_, m_, m_, r_, c_); 
	}

private:
	size_t              m_;

	// made mutable because modified and then restored
	mutable std::vector<size_t> r_;
	mutable std::vector<size_t> c_;
	// make mutable because its value does not matter
	mutable std::vector<Type>   a_;
};

} // END CppAD namespace

// END PROGRAM
# endif
