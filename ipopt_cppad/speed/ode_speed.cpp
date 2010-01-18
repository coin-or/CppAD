/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ipopt_ode_speed.cpp$$

$section Speed Test for Both Simple and Fast Representations$$

$code
$verbatim%ipopt_cppad/speed/ode_speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/

// BEGIN PROGRAM
// system include files used for I/O
# include <iostream>
# include "../example/ode_run.hpp"


namespace {
	double current_second(void)
	{	return (double) clock() / (double) CLOCKS_PER_SEC;
	}
	void output(
		bool        simple          , 
		size_t      retape          , 
		double      s0              , 
		double      s1              )
	{	using std::cout;
		if( simple )
			cout << "representation simple";
		else	cout << "representation fast  ";
		if( retape )
			cout << " retape yes,";
		else	cout << " retape no, ";
		cout << " seconds = " << s1 - s0;
		cout << std::endl;
	}
}


int main(void)
{	bool simple, retape;
	size_t i;
        double s0, s1;

	// solution vector
	NumberVector x;

	// number of time grid intervals between measurement values
	size_t n = 0;
	SizeVector N(Nz + 1);
	N[0] = 0;
	for(i = 1; i <= Nz; i++)
	{	N[i] = 7;
		n   += N[i] * Ny;
	}
	n += Na;
	std::cout << "ipopt_ode: number of variables = " << n << std::endl;

	s0              = current_second();
	simple          = false;
	retape          = false;
	ipopt_ode_case<FG_fast>(retape, N, x);
	s1              = current_second();
	output(simple, retape, s0, s1);

	s0              = current_second();
	simple          = false;
	retape          = true;
	ipopt_ode_case<FG_fast>(retape, N, x);
	s1              = current_second();
	output(simple, retape, s0, s1);

	s0              = current_second();
	simple          = true;
	retape          = false;
	ipopt_ode_case<FG_simple>(retape, N, x);
	s1              = current_second();
	output(simple, retape, s0, s1);

	s0              = current_second();
	simple          = true;
	retape          = true;
	ipopt_ode_case<FG_simple>(retape, N, x);
	s1              = current_second();
	output(simple, retape, s0, s1);
 
 
 
	return 0;
}
// END PROGRAM
