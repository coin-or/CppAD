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
$begin print_for.cpp$$
$spell
	av
	num
	inuse
	omp_alloc
	bool
	makefile
	CppAD
	cppad.hpp
	std::cout
	endl
	namespace
	newline
	\nv
	VecAD
$$

$section Printing During Forward Mode: Example and Test$$

$index forward, mode print$$
$index example, print forward mode$$
$index print, example forward mode$$

$head Running$$
To build this program and run its correctness test,
execute the following commands starting in the
$cref/work directory/InstallUnix/Download/Work Directory/$$:
$codei%
	cd print_for
	make test.sh
	./test.sh
%$$

$head Source Code$$
$codep */
# include <cppad/cppad.hpp>

void print_for(void)
{	// This program uses CppAD::vector (not CPPAD_TEST_VECTOR) because it 
	// CppAD::vector uses omp_alloc and we want to check that when 
	// omp_alloc::max_num_threads is one, there is no need to call 
	// omp_alloc::free_available. Note all allocation is in this routine
	using std::cout;
	using std::endl;
	using CppAD::AD;
	using CppAD::vector;
	using CppAD::PrintFor;

	// independent variable vector
	size_t n = 1;
	vector< AD<double> > ax(n);
	ax[0] = 1.;
	Independent(ax);

	// print a VecAD<double>::reference object that is a parameter
	CppAD::VecAD<double> av(1);
	AD<double> Zero(0);
	av[Zero] = 0.;
	PrintFor("v[0] = ", av[Zero]); 

	// Print a newline to separate this from previous output,
	// then print an AD<double> object that is a variable.
	PrintFor("\nv[0] + x[0] = ", av[0] + ax[0]); 

	// A conditional print that will fail condition x[0] <= 0.
	PrintFor(ax[0], "\n  2. + x[0] = ",   2. + ax[0], "\n");

	// A conditional print that will pass condition x[0] - 2. <= 0.
	PrintFor(ax[0] - 2., "\n  3. + x[0] = ",   3. + ax[0], "\n");

	// dependent variable vector 
	size_t m = 2;
	vector< AD<double> > ay(m);
	ay[0] = av[Zero] + ax[0];

	// define f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay); 

	// zero order forward with x[0] = 2 
	vector<double> x(n);
	x[0] = 2.;


	cout << "v[0] = 0" << endl; 
	cout << "v[0] + x[0] = 2" << endl; 
	cout << "  3. + x[0] = 5" << endl; 
	// ./makefile.am expects "Test passes" at beginning of next output line
	cout << "Test passes if three lines above repeat below:" << endl;
	f.Forward(0, x);	

	return;
}
int main(void)
{	bool ok = true;
	print_for();

	size_t thread;
	for(thread = 0; thread < 2; thread++)
	{	ok &= CppAD::omp_alloc::inuse(thread) == 0;
		ok &= CppAD::omp_alloc::available(thread) == 0;
	}
	if( ! ok )
		return 1;
	return 0;
}
/* $$

$head Output$$
Executing the program above generates the following output:
$codep
	v[0] = 0
	v[0] + x[0] = 2
	Test passes if two lines above repeat below:
	v[0] = 0
	v[0] + x[0] = 2
$$
$end
*/
