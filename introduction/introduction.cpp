/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin exp_apx.cpp$$
$spell
	Cpp
	exp_eps
	apx
$$

$section Correctness Tests For Exponential Approximation in Introduction$$
$mindex exp_apx main run$$


$head Running Tests$$
To build this program and run its correctness tests see $cref cmake_check$$.

$head Source$$
$code
$srcfile%introduction/introduction.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

// system include files used for I/O
# include <iostream>

// memory allocation routine
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool exp_2(void);
extern bool exp_2_cppad(void);
extern bool exp_2_for1(void);
extern bool exp_2_for2(void);
extern bool exp_2_rev1(void);
extern bool exp_2_rev2(void);
extern bool exp_2_for0(void);
extern bool exp_eps(void);
extern bool exp_eps_cppad(void);
extern bool exp_eps_for1(void);
extern bool exp_eps_for2(void);
extern bool exp_eps_for0(void);
extern bool exp_eps_rev1(void);
extern bool exp_eps_rev2(void);

// main program that runs all the tests
int main(void)
{	std::string group = "introduction";
	size_t      width = 20;
	CppAD::test_boolofvoid Run(group, width);

	// This comment is used by OneTest

	// external compiled tests
	Run( exp_2,           "exp_2"          );
	Run( exp_2_cppad,     "exp_2_cppad"    );
	Run( exp_2_for0,      "exp_2_for0"     );
	Run( exp_2_for1,      "exp_2_for1"     );
	Run( exp_2_for2,      "exp_2_for2"     );
	Run( exp_2_rev1,      "exp_2_rev1"     );
	Run( exp_2_rev2,      "exp_2_rev2"     );
	Run( exp_eps,         "exp_eps"        );
	Run( exp_eps_cppad,   "exp_eps_cppad"  );
	Run( exp_eps_for0,    "exp_eps_for0"   );
	Run( exp_eps_for1,    "exp_eps_for1"   );
	Run( exp_eps_for2,    "exp_eps_for2"   );
	Run( exp_eps_rev1,    "exp_eps_rev1"   );
	Run( exp_eps_rev2,    "exp_eps_rev2"   );
	//
	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
// END C++
