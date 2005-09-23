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
