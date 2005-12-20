// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */
// END SHORT COPYRIGHT
/*
$begin Speed.cpp$$
$spell
	Cpp
$$

$mindex run all speed test$$
$section Run All The Speed Tests$$

$comment This file is in the Speed subdirectory$$ 
$code
$verbatim%Speed/Speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

# include <cstddef>
# include <iostream>
# include <complex>

// For an unknown reason, cannot move other includes (using Sun's CC compiler)

typedef std::complex<double> Complex;

std::string doubleDetMinor    (size_t size, size_t repeat);
std::string ADdoubleDetMinor  (size_t size, size_t repeat);
std::string TapeDetMinor      (size_t size, size_t repeat);
std::string JacDetMinor       (size_t size, size_t repeat);
std::string HesDetMinor       (size_t size, size_t repeat);

std::string doubleDetLu       (size_t size, size_t repeat);
std::string ADdoubleDetLu     (size_t size, size_t repeat);
std::string TapeDetLu         (size_t size, size_t repeat);
std::string JacDetLu          (size_t size, size_t repeat);
std::string HesDetLu          (size_t size, size_t repeat);

std::string doubleOde         (size_t size, size_t repeat);
std::string ADdoubleOde       (size_t size, size_t repeat);
std::string TapeOde           (size_t size, size_t repeat);
std::string JacOde            (size_t size, size_t repeat);
std::string HesOde            (size_t size, size_t repeat);

std::string LuSolve           (size_t size, size_t repeat);
std::string LuVecAD           (size_t size, size_t repeat);

int main()
{	using CppAD::SpeedTest;
	std::cout << PACKAGE_STRING << " Speed Test " << std::endl;

	SpeedTest(doubleDetMinor,        5, -1, 1);
	SpeedTest(ADdoubleDetMinor,      5, -1, 1);
	SpeedTest(TapeDetMinor,          5, -1, 1);
	SpeedTest(JacDetMinor,           5, -1, 1);
	SpeedTest(HesDetMinor,           5, -1, 1);

	SpeedTest(doubleDetLu,           9, -2, 1);
	SpeedTest(ADdoubleDetLu,         9, -2, 1);
	SpeedTest(TapeDetLu,             9, -2, 1);
	SpeedTest(JacDetLu,              9, -2, 1);
	SpeedTest(HesDetLu,              9, -2, 1);

	SpeedTest(doubleOde,             5, -1, 1);
	SpeedTest(ADdoubleOde,           5, -1, 1);
	SpeedTest(TapeOde,               5, -1, 1);
	SpeedTest(JacOde,                5, -1, 1);
	SpeedTest(HesOde,                5, -1, 1);

	SpeedTest(LuSolve,               5, -1, 1);
	SpeedTest(LuVecAD,               5, -1, 1);

	return 0;
}

// END PROGRAM
