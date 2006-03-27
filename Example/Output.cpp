/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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

/*
$begin Output.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Output Operator: Example and Test$$

$index <<, AD example$$
$index output, AD example$$
$index example, AD output$$
$index test, AD output$$

$code
$verbatim%Example/Output.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

# include <sstream>  // std::ostringstream
# include <string>   // std::string
# include <iomanip>  // std::setprecision, setw, setfill, right

namespace {
	template <class S>
	void set_ostream(S &os)
	{	os 
		<< std::setprecision(4) // 4 digits of precision
		<< std::setw(6)         // 6 characters per field
		<< std::setfill(' ')    // fill with spaces
		<< std::right;          // adjust value to the right
	}
}

bool Output(void)
{	bool ok = true;

	// This output stream is an ostringstream for testing purposes.
	// You can use << with other types of streams; i.e., std::cout.
	std::ostringstream stream;

	// ouput an AD<double> object
	CppAD::AD<double>  pi = 4. * atan(1.); // 3.1415926536
	set_ostream(stream);
	stream << pi;

	// ouput a VecAD<double>::reference object
	CppAD::VecAD<double> v(1);
	CppAD::AD<double> zero(0);
	v[zero]   = exp(1.);                  // 2.7182818285
	set_ostream(stream); 
	stream << v[zero];

	// convert output from stream to string
	std::string str = stream.str();

	// check the output
	ok      &= (str == " 3.142 2.718");

	return ok;
}
// END PROGRAM
