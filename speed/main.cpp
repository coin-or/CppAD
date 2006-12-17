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

$section Speed Test Main Program$$

$head Syntax$$
$syntax%speed/%package%/%package% %option%
%$$

$head Purpose$$
A version of this program runs the correctness tests
or the speed tests for one AD package identified by $italic package$$.

$head package$$
The value
$italic package$$ specifies the AD package that is used for the speed tests 
and is one of the following packages:
$cref/adolc/speed_adolc/$$,
$cref/cppad/speed_cppad/$$, 
$cref/fadbad/speed_fadbad/$$, 
$cref/profile/InstallUnix/--with-profiling/$$.
In the special case where $italic package$$ is $code profile$$,
the CppAD package is compiled and run with profiling to determine
where it is spending most of its time.

$head option$$
It the argument $italic option$$ specifies which test to run
and has the following possible values:
$code correct$$,
$code all$$,
$code det_minor$$,
$code det_lu$$.

$head correct$$
If $italic option$$ is equal to $code correct$$,
all of the correctness tests are run.

$head all$$
If $italic option$$ is equal to $code all$$,
all of the speed tests are run.

$head det_lu$$
If $italic option$$ is equal to $code det_lu$$,
the speed test for the
gradient of the determinant using LU factorization tests is run.
Each package defines a version of this speed test with the prototype
$syntax%
	void speed_det_lu(size_t %size%, size_t %repeat%)
%$$
The argument $italic size$$
is the number of rows and columns in the matrix.
The argument $italic repeat$$ is the number of different matrices
that the gradient is computed for.

$head det_minor$$
If $italic option$$ is equal to $code det_minor$$,
the speed test for 
computing the gradient of the determinant using expansion by minors is run.
Each package defines a version of this speed test with the prototype
$syntax%
	void speed_det_minor(size_t %size%, size_t %repeat%)
%$$
The argument $italic size$$
is the number of rows and columns in the matrix.
The argument $italic repeat$$ is the number of different matrices
that the gradient is computed for.

$head uniform_01$$
The random number simulator $cref/uniform_01/$$ is initialized with
$syntax%
	uniform_01(%seed%)
%$$
before any of the speed testing routines (listed above) are called
by this speed testing main program.

$head Correctness Results$$
An output line is generated for each correctness test
stating of the test passed or failed.

$head Speed Results$$
For each speed test, corresponds to three lines of output.
The name of the test is printed on the first line,
the vector of problem sizes are printed on the next line,
and the rates corresponding to the different problem sizes are
printed on the third line.
The rate is the number of times per second that the calculation was repeated.

$end 
*/

# include <cassert>
# include <cstddef>
# include <iostream>
# include <cppad/vector.hpp>
# include <cppad/speed_test.hpp>
# include <speed/uniform_01.hpp>

# ifdef ADOLC
# define AD_PACKAGE "adolc"
# endif
# ifdef CPPAD
# define AD_PACKAGE "cppad"
# endif
# ifdef FADBAD
# define AD_PACKAGE "fadbad"
# endif
# ifdef PROFILE
# define AD_PACKAGE "profile"
# endif

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
	using std::cerr;

	char *option[]= {
		"correct",
		"all",
		"det_lu",
		"det_minor"
	};
	const size_t n_option  = sizeof(option) / sizeof(option[0]);
	const size_t option_correct   = 0;
	const size_t option_all       = 1;
	const size_t option_det_lu    = 2;
	const size_t option_det_minor = 3;
	assert( n_option == option_det_minor+1 );

	// use preprocessor symbol defined in makefile
	char *ad_package = AD_PACKAGE;

	size_t match = n_option;
	size_t i;
	if( argc == 2 )
	{	for(i = 0; i < n_option; i++)
			if( strcmp(option[i], argv[1]) == 0 )
				match = i;
	}
	if( match == n_option  )
	{	cerr << "usage: " << ad_package << " option" << endl;
		cerr << "where option is one of the following:" << endl;
		for(i = 0; i < n_option; i++)
		{	cerr << option[i];
			if( i == n_option - 1 )
				cerr << ".";
			else	cerr << ", ";
		}
		cerr << endl;
		return 1;
	}

	// initialize the random number simulator
	CppAD::uniform_01(1);

	// arguments needed for speed tests
	double time_min = 1.;
	size_t n_size   = 4;
	CppAD::vector<size_t> size_vec(n_size);
	for(i = 0; i < n_size; i++) 
		size_vec[i] = 2 * i + 1;

	switch(match)
	{	case option_correct:
		// run all the correctness tests
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
		break;
		// ---------------------------------------------------------

		case option_all:
		// run all the speed tests 
		Run_speed(speed_det_lu,    size_vec, time_min, "det_lu");
		Run_speed(speed_det_minor, size_vec, time_min, "det_minor");
		ok = true;
		break;
		// ---------------------------------------------------------

		case option_det_lu:
		Run_speed(speed_det_lu,    size_vec, time_min, "det_lu");
		break;
		// ---------------------------------------------------------

		case option_det_minor:
		Run_speed(speed_det_minor, size_vec, time_min, "det_minor");
		break;
		// ---------------------------------------------------------
		
		default:
		assert(0);
	}
	return static_cast<int>( ! ok );
}
