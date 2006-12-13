/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin speed_cppad.cpp$$
$spell
	Cpp
$$

$index cppad, speed test$$
$index speed, test cppad$$
$index test, cppad speed$$

$section Run the CppAD Speed Tests$$

$code
$verbatim%speed/adolc/example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

extern bool correct_det_lu(void);
extern void speed_det_lu(size_t size, size_t repeat);

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

		rate_vec = speed_test(speed_case, size_vec, time_min);
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
	{	std::cerr << "usage: run (example|speed)" << std::endl;
		return 1;
	}
	bool example = strcmp(argv[1], "example") == 0;
	bool speed   = strcmp(argv[1], "speed") == 0;
	if( ! (example || speed) )
	{	std::cerr << "usage: run (example|speed)" << std::endl;
		return 1;
	}
	if( example )
	{
		ok &= Run_correct(correct_det_lu,  "det_lu");

		assert( ok || (Run_error_count > 0) );
		if( ok )
		{	cout	<< "All " 
				<< Run_ok_count 
				<< " correctness tests passed." 
				<< endl;
		}
		else
		{	cout	<< Run_error_count 
				<< " correctness tests failed." 
		     		<< endl;
		}
		return static_cast<int>( ! ok );
	}
	double time_min = 1.;
	size_t n_size   = 4;
	CppAD::vector<size_t> size_vec(n_size);
	size_t i;
	for(i = 0; i < n_size; i++) 
		size_vec[i] = 2 * i + 1;
	Run_speed(speed_det_lu, size_vec, time_min, "det_lu");
}

// END PROGRAM
