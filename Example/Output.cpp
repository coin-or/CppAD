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
$begin Output.cpp$$
$spell
	Cpp
	cstddef
$$

$mindex output test example$$
$section The Output Operator for AD Objects: Example and Test$$
$index output$$
$index example, output$$
$index test, output$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/Output.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

# include <sstream>
# include <string>

bool Output(void)
{	using namespace CppAD;
	using namespace std;

	bool ok = true;
	string str;
	size_t n;

	ostringstream buf;

	AD<double>    s = 1.;
	VecAD<double> T(2);
	T[s] = 2.;

	buf << s;
	buf << T[s];
	str = buf.str();
	n   = str.size();

	ok &= (n == 2);
	ok &= (str[0] == '1');
	ok &= (str[1] == '2');


	return ok;
}
// END PROGRAM
