/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <iostream>  // system include files used for I/O
# include <string>    // std::string

// external complied tests
extern double ode_speed(const std::string& name);

// main program that runs all the ipopt_cppad speed tests
int main(void)
{	using std::cout;
	using std::endl;
	std::string name;
	double      seconds;

	seconds = ode_speed("simple_retape_yes");
	cout << "ode simple_retape_yes: seconds = " << seconds << std::endl;

	seconds = ode_speed("simple_retape_no");
	cout << "ode simple_retape_no:  seconds = " << seconds << std::endl;

	seconds = ode_speed("fast_retape_yes");
	cout << "ode fast_retape_yes:   seconds = " << seconds << std::endl;

	seconds = ode_speed("fast_retape_no");
	cout << "ode fast_retape_no:    seconds = " << seconds << std::endl;
	
	return 0;
}
