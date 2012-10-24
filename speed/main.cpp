/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cstring>
# include <cstdlib>
# include <cassert>
# include <cstddef>
# include <iostream>
# include <iomanip>
# include <cppad/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>
# include <cppad/time_test.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/poly.hpp>
# include <cppad/track_new_del.hpp>
# include <cppad/thread_alloc.hpp>

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
	optionlist
	underbar
	alloc
	mat_mul
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
$codei%speed/%package%/%package% %test% %seed% %option_list%$$

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
$cref/mat_mul/link_mat_mul/$$,
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
The random number simulator $cref uniform_01$$ is initialized with
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
every test must retape the 
$cref/operation sequence/glossary/Operation/Sequence/$$
for each test repetition.
If it is false,
the AD package is allowed to use one taping of the operation
sequence for all the repetitions of that speed test.
$pre

$$
All of the tests, except $cref/det_lu/link_det_lu/$$,
have a fixed operations sequence.
The operation sequence for $code det_lu$$ 
may be different for each repetition of the test because it
depends on the matrix for which the determinant is being calculated. 
For this reason,
$cref cppad_det_lu.cpp$$ returns false 
(for test not implemented)
when $code global_retape$$ is false.

$subhead optimize$$
If the option $code optimize$$ is present, the symbol
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

$subhead atomic$$
If the option $code atomic$$ is present, the symbol
$codep
	extern bool global_atomic
$$
is true and otherwise it is false.
If this external symbol is true,
and the AD package has a way to speed up the processing
by adding $cref user_atomic$$ operations,
this should be included in computations.
If it is false, user defined atomic operations should not be done.

$subhead memory$$
If the option $code memory$$ is present, the symbol
$codep
	extern bool global_memory
$$
is true and otherwise it is false.
If it is true, the CppAD
$cref/hold_memory/ta_hold_memory/$$ routine will be called by 
the main program before any of the tests are executed.
This should make the CppAD $code thread_alloc$$ allocator faster.
If it is false, standard memory allocation should be done by.
Otherwise the test can use special memory allocation to try 
and improve speed.

$head Correctness Results$$
An output line of the following form:
$codei%
	%package%_%test%_%optionlist%_ok = %flag%
%$$
is generated for each correctness test where
$icode package$$ and $icode test$$ are as above,
$icode optionlist$$ are the options (in $icode option_list$$) 
separated by the underbar $code _$$ character
(whereas they are separated by spaces in $icode option_list$$),
and $icode flag$$ is $code true$$ or $code false$$.

$head Speed Results$$
For each speed test, corresponds to three lines of the
following form are generated:
$codei%
	%package%_%test%_%optionlist%_ok   = %flag%
	%package%_%test%_%optionlist%_size = [ %size_1%, %...%, %size_n% ]
	%package%_%test%_%optionlist%_rate = [ %rate_1%, %...%, %rate_n% ]
%$$
The values $icode package$$, $icode test$$, $icode optionlist$$,
and $icode flag$$ are as in the correctness results above.
The values $icode size_1$$, ..., $icode size_n$$ are the
size arguments used for the corresponding tests.
The values $icode rate_1$$, ..., $icode rate_n$$ are the number of times
per second that the corresponding size problem executed.

$children%
	speed/src/link_det_lu.cpp%
	speed/src/link_det_minor.cpp%
	speed/src/link_mat_mul.cpp%
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
$rref link_mat_mul$$
$rref link_ode$$
$rref link_poly$$
$rref link_sparse_hessian$$
$rref link_sparse_jacobian$$
$tend


$end 
-----------------------------------------------------------------------------
*/

# define CPPAD_DECLARE_SPEED(name)                       \
     extern bool available_##name(void);                 \
     extern bool correct_##name(bool is_package_double); \
     extern void speed_##name(size_t size, size_t repeat)

CPPAD_DECLARE_SPEED(det_lu);
CPPAD_DECLARE_SPEED(det_minor);
CPPAD_DECLARE_SPEED(mat_mul);
CPPAD_DECLARE_SPEED(ode);
CPPAD_DECLARE_SPEED(poly);
CPPAD_DECLARE_SPEED(sparse_hessian);
CPPAD_DECLARE_SPEED(sparse_jacobian);

bool   global_retape;
bool   global_optimize;
bool   global_atomic;
bool   global_memory;

namespace {
	using std::cout;
	using std::endl;
	// ----------------------------------------------------------------
	// not available test message
	void not_available_message(const char* test_name)
	{	cout << AD_PACKAGE << ": " << test_name;
		cout << " is not availabe with " << endl;
		cout << "global_retape = " << global_retape;
		cout << ", global_optimize = " << global_optimize;
		cout << ", global_atomic = " << global_atomic;
		cout << ", global_memory = " << global_memory;
		cout << endl;
	}

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
# ifdef SPEED_DOUBLE
		ok = correct_case(true);
# else
		ok = correct_case(false);
# endif
		cout << AD_PACKAGE << "_" << case_name;
		if( global_retape )
			cout << "_retape";
		if( global_optimize )
			cout << "_optimize";
		if( global_atomic )
			cout << "_atomic";
		if( global_memory )
			cout << "_memory";
		cout << "_ok = ";
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
	void run_speed(
		void speed_case(size_t size, size_t repeat) , 
		const CppAD::vector<size_t>&       size_vec ,
		const std::string&                case_name )
	{	double time_min = 1.;
		cout << AD_PACKAGE << "_" << case_name << "_size = ";
		output(size_vec);
		cout << endl;
		cout << AD_PACKAGE << "_" << case_name;
		if( global_retape )
			cout << "_retape";
		if( global_optimize )
			cout << "_optimize";
		if( global_atomic )
			cout << "_atomic";
		if( global_memory )
			cout << "_memory";
		cout << "_rate = ";

		cout << std::fixed;
		for(size_t i = 0; i < size_vec.size(); i++)
		{	if( i == 0 )
				cout << "[ ";
			else	cout << ", ";	
			cout << std::flush;
			size_t size = size_vec[i];
			double time = CppAD::time_test(speed_case, time_min, size);
			double rate = 1. / time;
			if( rate >= 1000 )
				cout << std::setprecision(0) << rate;
			else cout << std::setprecision(2) << rate;
		}
		cout << " ]" << endl;
		return;
	}
}

// main program that runs all the tests
int main(int argc, char *argv[])
{	bool ok = true;
	enum test_enum {
		test_correct,
		test_speed,
		test_det_lu,
		test_det_minor,
		test_mat_mul,
		test_ode,
		test_poly,
		test_sparse_hessian,
		test_sparse_jacobian,
		test_error
	};
	struct test_struct {
		const char       *name;
		const test_enum  index;
	}; 
	const test_struct test_list[]= {
		{ "correct",            test_correct         }, 
		{ "speed",              test_speed           },
		{ "det_lu",             test_det_lu          },
		{ "det_minor",          test_det_minor       },
		{ "mat_mul",            test_mat_mul         },
		{ "ode",                test_ode             },
		{ "poly",               test_poly            },
		{ "sparse_hessian",     test_sparse_hessian  },
		{ "sparse_jacobian",    test_sparse_jacobian }
	};
	const size_t n_test  = sizeof(test_list) / sizeof(test_list[0]);

	size_t i;
	test_enum match = test_error;
	int    iseed = 0;
	bool   error = argc < 3;
	if( ! error )
	{	for(i = 0; i < n_test; i++)
			if( strcmp(test_list[i].name, argv[1]) == 0 )
				match = test_list[i].index;
		error = match == test_error;
		iseed = std::atoi( argv[2] );
		error |= iseed < 0;
		global_retape   = false;
		global_optimize = false;
		global_atomic   = false;
		global_memory   = false;
		for(i = 3; i < size_t(argc); i++)
		{	if( strcmp(argv[i], "retape") == 0 )
				global_retape = true;
			else if( strcmp(argv[i], "optimize") == 0 )
				global_optimize = true;
			else if( strcmp(argv[i], "atomic") == 0 )
				global_atomic = true;
			else if( strcmp(argv[i], "memory") == 0 )
				global_memory = true;
			else
				error = true;
		}
	}
	if( error )
	{	cout << "usage: ./" 
		     << AD_PACKAGE << " test seed option_list" << endl;
		cout << "test choices: " << endl;
		for(i = 0; i < n_test; i++)
			cout << "\t" << test_list[i].name << endl;
		cout << "seed choices: ";
		cout << "a positive integer used as a random seed." << endl;
		cout << "option choices: ";
		cout << " \"retape\",";
		cout << " \"optimize\",";
		cout << " \"atomic\",";
		cout << " \"memory\"." << endl << endl;
		return 1;
	}
	if( global_memory )
		CppAD::thread_alloc::hold_memory(true);

	// initialize the random number simulator
	CppAD::uniform_01(size_t(iseed));

	// arguments needed for speed tests
	size_t n_size   = 5;
	CppAD::vector<size_t> size_det_lu(n_size);
	CppAD::vector<size_t> size_det_minor(n_size);
	CppAD::vector<size_t> size_mat_mul(n_size);
	CppAD::vector<size_t> size_ode(n_size);
	CppAD::vector<size_t> size_poly(n_size);
	CppAD::vector<size_t> size_sparse_hessian(n_size);
	CppAD::vector<size_t> size_sparse_jacobian(n_size);
	for(i = 0; i < n_size; i++) 
	{	size_det_lu[i]      = 3 * i + 1;
		size_det_minor[i]   = i + 1;
		size_mat_mul[i]     = 10 * (i + 1);
		size_ode[i]         = 3 * i + 1;
		size_poly[i]        = 8 * i + 1;
		size_sparse_hessian[i]  = 100 * (i + 1) * (i + 1);
		size_sparse_jacobian[i] = 100 * (i + 1) * (i + 1);
	}

	switch(match)
	{
		// run all the correctness tests
		case test_correct:
		if( available_det_lu() ) ok &= run_correct(
			correct_det_lu,           "det_lu"       
		);
		if( available_det_minor() ) ok &= run_correct(
			correct_det_minor,        "det_minor"    
		);
		if( available_mat_mul() ) ok &= run_correct(
			correct_mat_mul,          "mat_mul"    
		);
		if( available_ode() ) ok &= run_correct(
			correct_ode,             "ode"         
		);
		if( available_poly() ) ok &= run_correct(
			correct_poly,            "poly"         
		);
		if( available_sparse_hessian() ) ok &= run_correct(
			correct_sparse_hessian,  "sparse_hessian"         
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
		if( available_det_lu() ) run_speed(
		speed_det_lu,          size_det_lu,          "det_lu"
		);
		if( available_det_minor() ) run_speed(
		speed_det_minor,       size_det_minor,       "det_minor"
		);
		if( available_mat_mul() ) run_speed(
		speed_mat_mul,           size_mat_mul,       "mat_mul"
		);
		if( available_ode() ) run_speed(
		speed_ode,             size_ode,             "ode"
		);
		if( available_poly() ) run_speed(
		speed_poly,            size_poly,            "poly"
		);
		if( available_sparse_hessian() ) run_speed(
		speed_sparse_hessian,  size_sparse_hessian,  "sparse_hessian"
		);
		if( available_sparse_jacobian() ) run_speed(
		speed_sparse_jacobian, size_sparse_jacobian, "sparse_jacobian"
		);
		ok = true;
		break;
		// ---------------------------------------------------------

		case test_det_lu:
		if( ! available_det_lu() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_det_lu,           "det_lu");
		run_speed(speed_det_lu,    size_det_lu,     "det_lu");
		break;
		// ---------------------------------------------------------

		case test_det_minor:
		if( ! available_det_minor() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_det_minor,       "det_minor");
		run_speed(speed_det_minor, size_det_minor, "det_minor");
		break;
		// ---------------------------------------------------------

		case test_mat_mul:
		if( ! available_mat_mul() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_mat_mul,     "mat_mul");
		run_speed(speed_mat_mul, size_mat_mul, "mat_mul");
		break;
		// ---------------------------------------------------------

		case test_ode:
		if( ! available_ode() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_ode,           "ode");
		run_speed(speed_ode,      size_ode,      "ode");
		break;
		// ---------------------------------------------------------

		case test_poly:
		if( ! available_poly() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_poly,            "poly");
		run_speed(speed_poly,      size_poly,      "poly");
		break;
		// ---------------------------------------------------------

		case test_sparse_hessian:
		if( ! available_sparse_hessian() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_sparse_hessian, "sparse_hessian");
		run_speed(
		speed_sparse_hessian, size_sparse_hessian,  "sparse_hessian");
		break;
		// ---------------------------------------------------------

		case test_sparse_jacobian:
		if( ! available_sparse_jacobian() )
		{	not_available_message( argv[1] ); 
			exit(1);
		}
		ok &= run_correct(correct_sparse_jacobian, "sparse_jacobian");
		run_speed(
		speed_sparse_jacobian, size_sparse_jacobian, "sparse_jacobian"
		);
		break;
		// ---------------------------------------------------------
		
		default:
		assert(0);
	}
# ifndef NDEBUG
	// return memory for vectors that are still in scope
	size_det_lu.clear();
	size_det_minor.clear();
	size_mat_mul.clear();
	size_ode.clear();
	size_poly.clear();
	size_sparse_hessian.clear();
	size_sparse_jacobian.clear();
	// check for memory leak
	if( CppAD::thread_alloc::free_all() )
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
