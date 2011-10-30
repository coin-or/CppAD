/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin bthread.cpp$$
$spell
	inv
	mega
	cpp
	num
	bthread
$$
$index OpenMP, example$$
$index OpenMP, speed$$
$index OpenMP, test$$
$index example, OpenMP$$
$index speed, OpenMP$$
$index test, OpenMP$$


$section Run OpenMP Examples and Speed Tests$$

$head Syntax$$
$codei%./bthread a11c
%$$
$codei%./bthread simple_ad
%$$

$head Running Tests$$
You can build this program and run the default version of its test
parameters by executing the following commands:
$codep
	cd bthread
	make test
$$

$head Purpose$$
Runs either of the examples
$cref bthread_a11c.cpp$$, and
$cref bthread_simple_ad.cpp$$.

$head Subroutines$$
$childtable%
	multi_thread/bthread/a11c.cpp%
	multi_thread/bthread/simple_ad.cpp
%$$

$head Source$$
$code
$verbatim%multi_thread/bthread/bthread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>

extern bool a11c(void);
extern bool simple_ad(void);

namespace {
	size_t arg2size_t(
		const char* arg       , 
		int limit             , 
		const char* error_msg )
	{	int i = std::atoi(arg);
		if( i >= limit )
			return size_t(i);
		std::cerr << "value = " << i << std::endl;
		std::cerr << error_msg << std::endl;
		exit(1);
	}
}

int main(int argc, char *argv[])
{	using CppAD::thread_alloc;
	bool ok         = true;
	using std::cout;

	// commnd line usage message
	const char *usage = 
	"usage: ./bthread a11c\n"
	"       ./bthread simple_ad\n";

	const char* test_name = *++argv;
	bool run_a11c      = std::strcmp(test_name, "a11c") == 0;
	bool run_simple_ad = std::strcmp(test_name, "simple_ad") == 0;
	ok = false;
	if( run_a11c || run_simple_ad )
		ok = (argc == 2);
	//
	if( ! ok )
	{	std::cerr << "test_name = " << test_name << std::endl;	
		std::cerr << "argc      = " << argc      << std::endl;	
		std::cerr << usage << std::endl;
		exit(1);
	}
	if( run_a11c || run_simple_ad )
	{	if( run_a11c )
			ok        = a11c();
		else ok        = simple_ad();
		if( ok )
		{	cout << "OK:    " << test_name << std::endl;
			exit(0);
		}
		else
		{	 cout << "Error: " << test_name << std::endl;
			exit(1);
		}
	}
	// should not reach here
	return 1;
}

// END PROGRAM
