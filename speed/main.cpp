/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin speed_main$$
$spell
	hpp
	iostream
	const
	argc
	cppad
	CppAD
	adolc
	fadbad
	cstddef
	bool
	det
	lu
	namespace
	std
	cout
	endl
	vec
	strcmp
	argv
	cerr
$$

$index cppad, speed test$$
$index speed, test cppad$$
$index test, cppad speed$$

$section Run All the Speed Tests$$

$head Syntax$$
$syntax%speed/%name%/%name% correct
%$$
$syntax%speed/%name%/%name% speed
%$$

$head Purpose$$
This main program combines the
functions corresponding to each speed test $italic case$$
into a single program called $italic name$$.

$head name$$
The file $code speed/speed.cpp$$ is the main routine that can 
is linked with a different set of speed tests to create the 
following programs:
$code speed/adolc/adolc$$, 
$code speed/cppad/cppad$$, and 
$code speed/fadbad/fadbad$$.

$head correct$$
If the text $code correct$$ is specified on the program command line,
the main routine runs the correctness tests and reports the results
on standard output.

$head speed$$
If the text $code speed$$ is specified on the program command line,
the main routine runs the speed tests and reports the results
on standard output.

$codep */

# include <cstddef>
# include <iostream>
# include <cppad/vector.hpp>
# include <cppad/speed_test.hpp>

// external routines that are used for correctness testing
extern bool    correct_det_lu(void);
extern bool correct_det_minor(void);

// external routines that are used for speed testing
extern void    speed_det_lu(size_t size, size_t repeat);
extern void speed_det_minor(size_t size, size_t repeat);

namespace {
	// function that runs one correctness case
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run_correct(bool correct_case(void), const char *name)
	{	bool ok;
		ok = correct_case();
		if( ok )
		{	std::cout << "Ok:    " << name << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << name << std::endl;
			Run_error_count++;
		}
		return ok;
	}
	// function that runs one speed case
	void Run_speed(
		void speed_case(size_t size, size_t repeat) , 
		CppAD::vector<size_t>              size_vec ,
		double                             time_min ,
		std::string                       case_name )
	{	using std::cout;
		using std::endl;
		CppAD::vector<size_t> rate_vec( size_vec.size() );

		rate_vec = CppAD::speed_test(speed_case, size_vec, time_min);
		cout << case_name << endl;
		cout << "size = " << size_vec << endl;
		cout << "rate = " << rate_vec << endl;
		return;
	}
}
// main program that runs all the tests
int main(int argc, char *argv[])
{	bool ok = true;
	using std::cout;
	using std::endl;

	if( argc != 2 )
	{	std::cerr << "usage: run (correct|speed)" << std::endl;
		return 1;
	}
	bool correct = strcmp(argv[1], "correct") == 0;
	bool speed   = strcmp(argv[1], "speed") == 0;
	if( ! (correct || speed) )
	{	std::cerr << "usage: run (correct|speed)" << std::endl;
		return 1;
	}
	if( correct )
	{	// run the correctness tests
		ok &= Run_correct(correct_det_lu,           "det_lu"       );
		ok &= Run_correct(correct_det_minor,        "det_minor"    );

		// summarize results
		assert( ok || (Run_error_count > 0) );
		if( ok )
		{	cout	<< "All " << Run_ok_count 
				<< " correctness tests passed." << endl;
		}
		else
		{	cout	<< Run_error_count 
				<< " correctness tests failed." << endl;
		}
		return static_cast<int>( ! ok );
	}

	// speed test arguments
	double time_min = 1.;
	size_t n_size   = 4;
	CppAD::vector<size_t> size_vec(n_size);
	size_t i;
	for(i = 0; i < n_size; i++) 
		size_vec[i] = 2 * i + 1;

	// run the speed tests
	Run_speed(speed_det_lu,    size_vec, time_min, "det_lu");
	Run_speed(speed_det_minor, size_vec, time_min, "det_minor");

	return 0;
}
/* $$
$end
*/
