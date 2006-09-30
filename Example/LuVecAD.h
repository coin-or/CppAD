# ifndef CppADLuVecADIncluded
# define CppADLuVecADIncluded

// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT

# include <CppAD/CppAD.h>

namespace CppAD {
	extern CppAD::AD<double> LuVecAD(
		size_t                           n,
		size_t                           m,
		CppAD::VecAD<double>             &Matrix,
		CppAD::VecAD<double>             &Rhs,
		CppAD::VecAD<double>             &Result,
		CppAD::AD<double>                &logdet
	);
}

# endif
