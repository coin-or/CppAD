/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cstring>
# include <cstdlib>
# include <cassert>
# include <cstddef>
# include <iostream>
# include <cppad/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>
# include <cppad/speed_test.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/poly.hpp>

# ifdef SPEED_ADOLC
# define AD_PACKAGE "adolc"
# endif
# ifdef SPEED_CPPAD
# define AD_PACKAGE "cppad"
# endif
# ifdef SPEED_DOUBLE
# define AD_PACKAGE "double"
# endif
# ifdef SPEED_FADBAD
# define AD_PACKAGE "fadbad"
# endif
# ifdef SPEED_PROFILE
# define AD_PACKAGE "profile"
# endif
# ifdef SPEED_SACADO
# define AD_PACKAGE "sacado"
# endif

/*
$begin speed_main$$
$spell
	retaped
	retape
	bool
	ddp
	cppad
	adolc
	fadbad
	sacado
	CppAD
	det
	lu
	Jacobian
$$

$index cppad, speed test$$
$index speed, test cppad$$
$index test, cppad speed$$

$section Speed Testing Main Program$$

$head Syntax$$
$codei%speed/%package%/%package% %test% %seed% %option_list% 
%$$

$head Purpose$$
A version of this program runs the correctness tests
or the speed tests for one AD package identified by $icode package$$.

$head package$$

$subhead AD Package$$
The command line argument
$icode package$$ specifies one of the following AD packages:
$cref/adolc/speed_adolc/$$,
$cref/cppad/speed_cppad/$$, 
$cref/fadbad/speed_fadbad/$$, 
$cref/sacado/speed_sacado/$$.

$subhead double$$
The value
$icode package$$ can be $code double$$ in which case
the function values (instead of derivatives) are computed
using double precision operations.
This enables one to compare the speed of computing function
values in $code double$$ to the speed of the derivative computations.
(It is often useful to divide the speed of the derivative computation by
the speed of the function evaluation in $code double$$.) 

$subhead profile$$
In the special case where $icode package$$ is $code profile$$,
the CppAD package is compiled and run with profiling to aid in determining
where it is spending most of its time.

$head test$$
It the argument $icode test$$ specifies which test to run
and has the following possible values:
$cref/correct/speed_main/test/correct/$$,
$cref/speed/speed_main/test/speed/$$,
$cref/det_minor/link_det_minor/$$,
$cref/det_lu/link_det_lu/$$,
$cref/ode/link_ode/$$,
$cref/poly/link_poly/$$,
$cref/sparse_hessian/link_sparse_hessian/$$,
$cref/sparse_jacobian/link_sparse_jacobian/$$.

$subhead correct$$
If $icode test$$ is equal to $code correct$$,
all of the correctness tests are run.

$subhead speed$$
If $icode test$$ is equal to $code speed$$,
all of the speed tests are run.

$head seed$$
$index uniform_01$$
The command line argument $icode seed$$ is a positive integer.
The random number simulator $cref/uniform_01/$$ is initialized with
the call
$codei%
	uniform_01(%seed%)
%$$
before any of the testing routines (listed above) are called.

$head option_list$$
This is a list of options that follow $icode seed$$ value.
The order of the options does not matter and the list can be empty.
Each option in the list, must be separate 
command line argument to the main program. 

$subhead retape$$
If the option $code retape$$ is present, the symbol
$codep
	extern bool global_retape
$$
is true and otherwise it is false.
If this external symbol is true,
every test must retape the AD operation sequence for each test repetition.
If it is false,
and the particular test has a fixed operation sequence,
the AD package is allowed to use one taping of the operation
sequence for all the repetitions of that speed test.
The following tests have a fixed operation sequence:
$code det_minor$$, $code ode$$, $code poly$$.

$subhead optimize$$
If the option code optimize is present, the symbol
$codep
	extern bool global_optimize
$$
is true and otherwise it is false.
If this external symbol is true,
and the AD package has an optional way to spend time optimizing
the operation sequence,
this optimization should be done before doing computations.
If it is false,
this optimization should not be done.

$head Correctness Results$$
An output line is generated for each correctness test
stating of the test passed or failed.

$head Speed Results$$
For each speed test, corresponds to three lines of output.
The name of the package and test are printed on the first line,
the vector of problem sizes are printed on the next line,
and the rates corresponding to the different problem sizes are
printed on the third line.
The rate is the number of times per second that the calculation was repeated.


$children%
	speed/src/link_det_lu.cpp%
	speed/src/link_det_minor.cpp%
	speed/src/link_ode.cpp%
	speed/src/link_poly.cpp%
	speed/src/link_sparse_hessian.cpp%
	speed/src/link_sparse_jacobian.cpp%
	speed/src/microsoft_timer.cpp
%$$

$head Link Functions$$
Each speed test defines it's own version of one of the following 
functions that link the speed test to the main program described above:
$table
$rref link_det_lu$$
$rref link_det_minor$$
$rref link_ode$$
$rref link_poly$$
$rref link_sparse_hessian$$
$rref link_sparse_jacobian$$
$tend


$end 
-----------------------------------------------------------------------------
*/

# define CPPAD_DECLARE_SPEED(name)                          \
	extern bool available_##name(void);                 \
	extern bool correct_##name(bool is_package_double); \
	extern void speed_##name(size_t size, size_t repeat)

CPPAD_DECLARE_SPEED(det_lu);
CPPAD_DECLARE_SPEED(det_minor);
CPPAD_DECLARE_SPEED(ode);
CPPAD_DECLARE_SPEED(poly);
CPPAD_DECLARE_SPEED(sparse_hessian);
CPPAD_DECLARE_SPEED(sparse_jacobian);

bool   global_retape;
bool   global_optimize;

namespace {
	using std::cout;
	using std::endl;

	// ------------------------------------------------------
	// output vector in form readable by octave or matlab
	// convert size_t to int to avoid warning by MS compiler
	void output(const CppAD::vector<size_t> &v)
	{	size_t i= 0, n = v.size();
		cout << "[ ";
		while(i < n)
		{	cout << int(v[i++]);
			if( i < n )
				cout << ", ";
		}
		cout << " ]";
	}

	// ----------------------------------------------------------------
	// function that runs one correctness case
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool run_correct(bool correct_case(bool), const char *case_name)
	{	bool ok;
		cout << AD_PACKAGE << "_" << case_name << "_ok = ";
# ifdef SPEED_DOUBLE
		ok = correct_case(true);
# else
		ok = correct_case(false);
# endif
		if( ok )
		{	cout << " true" << endl;
			Run_ok_count++;
		}
		else
		{	cout << " false" << endl;
			Run_error_count++;
		}
		return ok;
	}
	// ----------------------------------------------------------------
	// function that runs one speed case
	void Run_speed(
		void speed_case(size_t size, size_t repeat) , 
		const CppAD::vector<size_t>&       size_vec ,
		const std::string&                case_name )
	{	double time_min = 1.;
		cout << AD_PACKAGE << "_" << case_name << "_size = ";
		output(size_vec);
		cout << endl;

		CppAD::vector<size_t> rate_vec( size_vec.size() );
		rate_vec = CppAD::speed_test(speed_case, size_vec, time_min);
		cout << AD_PACKAGE << "_" << case_name << "_rate = ";
		output(rate_vec);
		cout << endl;
		return;
	}
}

// main program that runs all the tests
int main(int argc, char *argv[])
{	bool ok = true;

	const char *test[]= {
		"correct",
		"det_lu",
		"det_minor",
		"ode",
		"poly",
		"sparse_hessian",
		"sparse_jacobian",
		"speed"
	};
	const size_t n_test  = sizeof(test) / sizeof(test[0]);
	const size_t test_correct        = 0;
	const size_t test_det_lu         = 1;
	const size_t test_det_minor      = 2;
	const size_t test_ode            = 3;
	const size_t test_poly           = 4;
	const size_t test_sparse_hessian = 5;
	const size_t test_sparse_jacobian= 6;
	const size_t test_speed          = 7;
	assert( n_test == test_speed+1 );

	size_t i;
	size_t match = n_test;
	int    iseed = 0;
	bool   error = argc < 3;
	if( ! error )
	{	for(i = 0; i < n_test; i++)
			if( strcmp(test[i], argv[1]) == 0 )
				match = i;
		error = match == n_test;
		iseed = std::atoi( argv[2] );
		error |= iseed < 0;
		global_retape   = false;
		global_optimize = false;
		for(i = 3; i < size_t(argc); i++)
		{	if( strcmp(argv[i], "retape") == 0 )
				global_retape = true;
			else if( strcmp(argv[i], "optimize") == 0 )
				global_optimize = true;
			else
				error = true;
		}
	}
	if( error )
	{	cout << "usage: ./" 
		     << AD_PACKAGE << " test seed option_list" << endl;
		cout << "test choices: " << endl;
		for(i = 0; i < n_test; i++)
			cout << "\t" << test[i] << endl;
		cout << "seed choices: ";
		cout << "a positive integer used as a random seed." << endl;
		cout << "option choices: ";
		cout << " \"retape\", \"optimize\"." << endl << endl;
		return 1;
	}

	// initialize the random number simulator
	CppAD::uniform_01(size_t(iseed));

	// arguments needed for speed tests
	size_t n_size   = 5;
	CppAD::vector<size_t> size_det_lu(n_size);
	CppAD::vector<size_t> size_det_minor(n_size);
	CppAD::vector<size_t> size_ode(n_size);
	CppAD::vector<size_t> size_poly(n_size);
	CppAD::vector<size_t> size_sparse_hessian(n_size);
	CppAD::vector<size_t> size_sparse_jacobian(n_size);
	for(i = 0; i < n_size; i++) 
	{	size_det_lu[i]      = 3 * i + 1;
		size_det_minor[i]   = i + 1;
		size_ode[i]         = i + 1;
		size_poly[i]        = 8 * i + 1;
		size_sparse_hessian[i]  = 30 * (i + 1);
		size_sparse_jacobian[i] = 30 * (i + 1);
	}
# ifndef NDEBUG
	size_t base_count = CPPAD_TRACK_COUNT();
# endif

	switch(match)
	{
		// run all the correctness tests
		case test_correct:
		if( available_det_lu() ) ok &= run_correct(
			correct_det_lu,    "det_lu"       
		);
		if( available_det_minor() ) ok &= run_correct(
			correct_det_minor, "det_minor"    
		);
		if( available_ode() ) ok &= run_correct(
			correct_ode,      "ode"         
		);
		if( available_poly() ) ok &= run_correct(
			correct_poly,      "poly"         
		);
		if( available_sparse_hessian() ) ok &= run_correct(
			correct_sparse_hessian, "sparse_hessian"         
		);
		if( available_sparse_jacobian() ) ok &= run_correct(
			correct_sparse_jacobian, "sparse_jacobian"         
		);
		// summarize results
		assert( ok || (Run_error_count > 0) );
		if( ok )
		{	cout	<< "All " << int(Run_ok_count) 
				<< " correctness tests passed." << endl;
		}
		else
		{	cout	<< int(Run_error_count) 
				<< " correctness tests failed." << endl;
		}
		break;
		// ---------------------------------------------------------
		// run all the speed tests 
		case test_speed:
		if( available_det_lu() ) Run_speed(
		speed_det_lu,    size_det_lu,    "det_lu"
		);
		if( available_det_minor() ) Run_speed(
		speed_det_minor, size_det_minor, "det_minor"
		);
		if( available_ode() ) Run_speed(
		speed_ode,       size_ode,       "ode"
		);
		if( available_poly() ) Run_speed(
		speed_poly,      size_poly,      "poly"
		);
		if( available_sparse_hessian() ) Run_speed(
		speed_sparse_hessian,  size_sparse_hessian,  "sparse_hessian"
		);
		if( available_sparse_jacobian() ) Run_speed(
		speed_sparse_jacobian, size_sparse_jacobian, "sparse_jacobian"
		);
		ok = true;
		break;
		// ---------------------------------------------------------

		case test_det_lu:
		if( ! available_det_lu() )
		{	cout << AD_PACKAGE << ": test " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= run_correct(correct_det_lu,           "det_lu");
		Run_speed(speed_det_lu,    size_det_lu,     "det_lu");
		break;
		// ---------------------------------------------------------

		case test_det_minor:
		if( ! available_det_minor() )
		{	cout << AD_PACKAGE << ": test " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= run_correct(correct_det_minor,       "det_minor");
		Run_speed(speed_det_minor, size_det_minor, "det_minor");
		break;
		// ---------------------------------------------------------

		case test_ode:
		if( ! available_ode() )
		{	cout << AD_PACKAGE << ": test " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= run_correct(correct_ode,           "ode");
		Run_speed(speed_ode,      size_ode,      "ode");
		break;
		// ---------------------------------------------------------

		case test_poly:
		if( ! available_poly() )
		{	cout << AD_PACKAGE << ": test " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= run_correct(correct_poly,            "poly");
		Run_speed(speed_poly,      size_poly,      "poly");
		break;
		// ---------------------------------------------------------

		case test_sparse_hessian:
		if( ! available_sparse_hessian() )
		{	cout << AD_PACKAGE << ": test " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= run_correct(correct_sparse_hessian, "sparse_hessian");
		Run_speed(
		speed_sparse_hessian, size_sparse_hessian,  "sparse_hessian");
		break;
		// ---------------------------------------------------------

		case test_sparse_jacobian:
		if( ! available_sparse_jacobian() )
		{	cout << AD_PACKAGE << ": test " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= run_correct(correct_sparse_jacobian, "sparse_jacobian");
		Run_speed(
		speed_sparse_jacobian, size_sparse_jacobian, "sparse_jacobian"
		);
		break;
		// ---------------------------------------------------------
		
		default:
		assert(0);
	}
# ifndef NDEBUG
	if( CPPAD_TRACK_COUNT() == base_count )
	{	Run_ok_count++;
		cout << "No memory leak detected" << endl;
	}
	else
	{	ok = false;
		Run_error_count++;
		cout << "Memory leak detected" << endl;
        }
# endif
	return static_cast<int>( ! ok );
}
