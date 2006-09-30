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
/*
$begin SpeedFadbad.cpp$$
$spell
	Fadbad
	Cpp
$$

$mindex run all Fadbad speed test$$
$section Run All The Fadbad Speed Comparison Tests$$

$comment This file is in the Fadbad subdirectory$$ 
$code
$verbatim%Fadbad/Speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cstddef>


std::string DetMinorCp  (size_t size, size_t repeat);
std::string DetMinorFa  (size_t size, size_t repeat);

std::string DetLuCp     (size_t size, size_t repeat);
std::string DetLuFa     (size_t size, size_t repeat);


int main()
{	using CppAD::SpeedTest;

	SpeedTest(DetMinorCp,    1, 2, 7);
	SpeedTest(DetMinorFa,    1, 2, 7);

	SpeedTest(DetLuCp,       1, 2, 7);
	SpeedTest(DetLuFa,       1, 2, 7);

	return 0;
}

// END PROGRAM
