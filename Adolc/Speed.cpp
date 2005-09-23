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
$begin SpeedAdolc.cpp$$
$spell
	Adolc
	Cpp
$$

$mindex run all Adolc speed test$$
$section Run All The Adolc Speed Comparison Tests$$

$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%Adolc/Speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cstddef>


std::string DetMinorCp  (size_t size, size_t repeat);
std::string DetMinorAd  (size_t size, size_t repeat);
std::string DetLuCp     (size_t size, size_t repeat);
std::string DetLuAd     (size_t size, size_t repeat);

int main()
{	using CppAD::SpeedTest;

	SpeedTest(DetMinorCp,    1, 2, 7);
	SpeedTest(DetMinorAd,    1, 2, 7);
	SpeedTest(DetLuCp,       1, 2, 7);
	SpeedTest(DetLuAd,       1, 2, 7);

	return 0;
}

// END PROGRAM
