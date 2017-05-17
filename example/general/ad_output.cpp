/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ad_output.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Output Operator: Example and Test$$
$mindex <<$$


$code
$srcfile%example/general/ad_output.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

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

bool ad_output(void)
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
// END C++
