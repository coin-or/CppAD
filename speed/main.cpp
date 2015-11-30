// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

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
# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>
# include <cppad/utility/time_test.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/poly.hpp>
# include <cppad/utility/track_new_del.hpp>
# include <cppad/utility/thread_alloc.hpp>

# ifdef CPPAD_ADOLC_SPEED
# define AD_PACKAGE "adolc"
# endif
# ifdef CPPAD_CPPAD_SPEED
# define AD_PACKAGE "cppad"
# endif
# ifdef CPPAD_DOUBLE_SPEED
# define AD_PACKAGE "double"
# endif
# ifdef CPPAD_FADBAD_SPEED
# define AD_PACKAGE "fadbad"
# endif
# ifdef CPPAD_PROFILE_SPEED
# define AD_PACKAGE "profile"
# endif
# ifdef CPPAD_SACADO_SPEED
# define AD_PACKAGE "sacado"
# endif

/*
$begin speed_main$$
$spell
	colpack
	onetape
	boolsparsity
	optionlist
	underbar
	alloc
	mat_mul
	retaped
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


$section Running the Speed Test Program$$
$mindex cppad uniform_01$$

$head Syntax$$
$codei%speed/%package%/speed_%package% %test% %seed% %option_list%$$

$head Purpose$$
A version of this program runs the correctness tests
or the speed tests for one AD package identified by $icode package$$.

$head package$$

$subhead AD Package$$
The command line argument
$icode package$$ specifies one of the AD package.
The CppAD distribution comes with support for the following packages:
$cref/adolc/speed_adolc/$$,
$cref/cppad/speed_cppad/$$,
$cref/fadbad/speed_fadbad/$$,
$cref/sacado/speed_sacado/$$.
You can extend this program to include other package.
Such an extension need not include all the tests.
For example,
$cref link_sparse_hessian$$ just returns $code false$$ for the
$cref/fadbad/fadbad_sparse_hessian.cpp/$$ and
$cref/sacado/sacado_sparse_hessian.cpp/$$ packages.


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
You can experiment with changing the implementation of a
particular test for a particular package.

$subhead correct$$
If $icode test$$ is equal to $code correct$$,
all of the correctness tests are run.

$subhead speed$$
If $icode test$$ is equal to $code speed$$,
all of the speed tests are run.

$head seed$$
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
The documentation below specifics how CppAD uses these options,
see the examples in $cref speed_adolc$$ for how another package might
uses these options.

$subhead onetape$$
If the option $code onetape$$ is present, the symbol
$codep
	extern bool global_onetape
$$
is true and otherwise it is false.
If this external symbol is true,
CppAD will use one taping of the operation
sequence for all the repetitions of that speed test.
Otherwise, the
$cref/operation sequence/glossary/Operation/Sequence/$$
will be retaped for each test repetition.
$pre

$$
All of the tests, except $cref/det_lu/link_det_lu/$$,
have a fixed operations sequence.
The operation sequence for $code det_lu$$
may be different for each repetition of the test because it
depends on the matrix for which the determinant is being calculated.
For this reason, the CppAD test
$cref cppad_det_lu.cpp$$ returns false
(indicating that the test not implemented)
when $code global_onetape$$ is true.

$subhead optimize$$
If the option $code optimize$$ is present, the symbol
$codep
	extern bool global_optimize
$$
is true and otherwise it is false.
If this external symbol is true,
CppAD will optimize the operation sequence before doing computations.
If it is false, this optimization will not be done.

$subhead atomic$$
If the option $code atomic$$ is present, the symbol
$codep
	extern bool global_atomic
$$
is true and otherwise it is false.
If this external symbol is true, CppAD will use its user defined
$cref/atomic/atomic_base/$$ operation is used for the test.
If no such atomic operation exists,
and atomic is chosen, CppAD returns false for the test.

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
If it is false, CppAD will used standard memory allocation.

$head Sparsity Options$$
The following options only apply to the
$cref/sparse_jacobian/link_sparse_jacobian/$$ and
$cref/sparse_hessian/link_sparse_hessian/$$ tests.
The other tests will ignore these options:

$subhead boolsparsity$$
If the option $code boolsparsity$$ is present, the symbol
$codep
	extern bool global_boolsparsity
$$
is true and otherwise it is false.
If it is true, CppAD will use a
$cref/vector of bool/glossary/Sparsity Pattern/Vector of Boolean/$$
for its sparsity patterns.
Otherwise it will use a
$cref/vector of sets/glossary/Sparsity Pattern/Vector of Sets/$$.

$subhead colpack$$
If the option $code colpack$$ is present, the symbol
$codep
	extern bool global_colpack
$$
is true and otherwise it is false.
If this external symbol is true,
CppAD will use $cref/colpack/colpack_prefix/$$ to do the coloring
for its
Otherwise, it will use it's own coloring algorithm.

$head Correctness Results$$
One, but not both, of the following two output lines
$codei%
	%package%_%test%_%optionlist%_available = false
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
	%package%_%test%_size = [ %size_1%, %...%, %size_n% ]
	%package%_%test%_rate = [ %rate_1%, %...%, %rate_n% ]
%$$
The values $icode package$$, $icode test$$, $icode optionlist$$,
and $icode flag$$ are as in the correctness results above.
The values $icode size_1$$, ..., $icode size_n$$ are the
size arguments used for the corresponding tests.
The values $icode rate_1$$, ..., $icode rate_n$$ are the number of times
per second that the corresponding size problem executed.

$subhead n_sweep$$
The $cref/sparse_jacobian/link_sparse_jacobian/$$
and $cref/sparse_hessian/link_sparse_hessian/$$ tests has an extra output
line with the following form
$codei%
	%package%_sparse_%test%_n_sweep = [ %n_sweep_1%, %...%, %n_sweep_n% ]
%$$
were $icode test$$ is $code jacobian$$ ($code hessian$$).
The values $icode n_sweep_1$$, ..., $icode n_sweep_n$$ are the number of
sweeps (colors) used for each sparse Jacobian (Hessian) calculation; see
$icode n_sweep$$ for
$cref/sparse_jacobian/sparse_jacobian/n_sweep/$$ and
$cref/sparse_hessian/sparse_hessian/n_sweep/$$.


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
Each $cref/package/speed_main/package/$$
defines it's own version of one of the link functions listed below.
Each of these functions links this main program to the corresponding test:
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
// external routines

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

// info is different for each test
extern void info_sparse_jacobian(size_t size, size_t& n_sweep);
extern void info_sparse_hessian(size_t size, size_t& n_sweep);

// --------------------------------------------------------------------------

bool   global_onetape;
bool   global_colpack;
bool   global_optimize;
bool   global_atomic;
bool   global_memory;
bool   global_boolsparsity;

namespace {
	using std::cout;
	using std::endl;
	// ----------------------------------------------------------------
	// not available test message
	void not_available_message(const char* test_name)
	{	cout << AD_PACKAGE << ": " << test_name;
		cout << " is not availabe with " << endl;
		cout << "onetape = " << global_onetape << endl;
		cout << "colpack = " << global_colpack << endl;
		cout << "optimize = " << global_optimize << endl;
		cout << "atomic = " << global_atomic << endl;
		cout << "memory = " << global_memory << endl;
		cout << "boolsparsity = " << global_boolsparsity << endl;
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
	bool run_correct(
		bool available_case(void) ,
		bool correct_case(bool)   ,
		const char *case_name     )
	{	bool available = available_case();
		bool ok        = true;
		if( available )
		{
# ifdef CPPAD_DOUBLE_SPEED
			ok = correct_case(true);
# else
			ok = correct_case(false);
# endif
		}
		cout << AD_PACKAGE << "_" << case_name;
		if( global_onetape )
			cout << "_onetape";
		if( global_colpack )
			cout << "_colpack";
		if( global_optimize )
			cout << "_optimize";
		if( global_atomic )
			cout << "_atomic";
		if( global_memory )
			cout << "_memory";
		if( global_boolsparsity )
			cout << "_boolsparsity";
		if( ! available )
		{	cout << "_available = false" << endl;
			return ok;
		}
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
		cout << AD_PACKAGE << "_" << case_name << "_rate = ";
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
		global_onetape      = false;
		global_colpack      = false;
		global_optimize     = false;
		global_atomic       = false;
		global_memory       = false;
		global_boolsparsity = false;
		for(i = 3; i < size_t(argc); i++)
		{	if( strcmp(argv[i], "onetape") == 0 )
				global_onetape = true;
			else if( strcmp(argv[i], "colpack") == 0 )
				global_colpack = true;
			else if( strcmp(argv[i], "optimize") == 0 )
				global_optimize = true;
			else if( strcmp(argv[i], "atomic") == 0 )
				global_atomic = true;
			else if( strcmp(argv[i], "memory") == 0 )
				global_memory = true;
			else if( strcmp(argv[i], "boolsparsity") == 0 )
				global_boolsparsity = true;
			else
				error = true;
		}
	}
	if( error )
	{	cout << "usage: ./speed_"
		     << AD_PACKAGE << " test seed option_list" << endl;
		cout << "test choices: " << endl;
		for(i = 0; i < n_test; i++)
			cout << "\t" << test_list[i].name << endl;
		cout << "seed choices: ";
		cout << "a positive integer used as a random seed." << endl;
		cout << "option choices: ";
		cout << " \"onetape\",";
		cout << " \"colpack\",";
		cout << " \"optimize\",";
		cout << " \"atomic\",";
		cout << " \"memory\",";
		cout << " \"boolsparsity\"." << endl << endl;
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
	{	size_det_minor[i]   = i + 1;
		size_det_lu[i]      = 10 * i + 1;
		size_mat_mul[i]     = 10 * i + 1;
		size_ode[i]         = 10 * i + 1;
		size_poly[i]        = 10 * i + 1;
		size_sparse_hessian[i]  = 100 * (i + 1) * (i + 1);
		size_sparse_jacobian[i] = 100 * (i + 1) * (i + 1);
	}

	switch(match)
	{
		// run all the correctness tests
		case test_correct:
		ok &= run_correct( available_det_lu, correct_det_lu, "det_lu"
		);
		ok &= run_correct(
			available_det_minor, correct_det_minor, "det_minor"
		);
		ok &= run_correct(
			available_mat_mul, correct_mat_mul, "mat_mul"
		);
		ok &= run_correct(
			available_ode, correct_ode, "ode"
		);
		ok &= run_correct( available_poly, correct_poly, "poly"
		);
		ok &= run_correct(
			available_sparse_hessian,
			correct_sparse_hessian,
			"sparse_hessian"
		);
		ok &= run_correct(
			available_sparse_jacobian,
			correct_sparse_jacobian,
			"sparse_jacobian"
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
		ok &= run_correct(
			available_det_lu, correct_det_lu, "det_lu")
		;
		run_speed(speed_det_lu,    size_det_lu,     "det_lu");
		break;
		// ---------------------------------------------------------

		case test_det_minor:
		if( ! available_det_minor() )
		{	not_available_message( argv[1] );
			exit(1);
		}
		ok &= run_correct(
			available_det_minor, correct_det_minor, "det_minor"
		);
		run_speed(speed_det_minor, size_det_minor, "det_minor");
		break;
		// ---------------------------------------------------------

		case test_mat_mul:
		if( ! available_mat_mul() )
		{	not_available_message( argv[1] );
			exit(1);
		}
		ok &= run_correct(
			available_mat_mul, correct_mat_mul, "mat_mul"
		);
		run_speed(speed_mat_mul, size_mat_mul, "mat_mul");
		break;
		// ---------------------------------------------------------

		case test_ode:
		if( ! available_ode() )
		{	not_available_message( argv[1] );
			exit(1);
		}
		ok &= run_correct(
			available_ode, correct_ode, "ode"
		);
		run_speed(speed_ode,      size_ode,      "ode");
		break;
		// ---------------------------------------------------------

		case test_poly:
		if( ! available_poly() )
		{	not_available_message( argv[1] );
			exit(1);
		}
		ok &= run_correct(
			available_poly, correct_poly, "poly"
		);
		run_speed(speed_poly,      size_poly,      "poly");
		break;
		// ---------------------------------------------------------

		case test_sparse_hessian:
		if( ! available_sparse_hessian() )
		{	not_available_message( argv[1] );
			exit(1);
		}
		ok &= run_correct(
			available_sparse_hessian,
			correct_sparse_hessian,
			"sparse_hessian"
		);
		run_speed(
			speed_sparse_hessian, size_sparse_hessian,  "sparse_hessian"
		);
		cout << AD_PACKAGE << "_sparse_hessian_sweep = ";
		for(i = 0; i < size_sparse_hessian.size(); i++)
		{	if( i == 0 )
				cout << "[ ";
			else	cout << ", ";
			size_t n_sweep;
			info_sparse_hessian(size_sparse_hessian[i], n_sweep);
			cout << n_sweep;
		}
		cout << " ]" << endl;
		break;
		// ---------------------------------------------------------

		case test_sparse_jacobian:
		if( ! available_sparse_jacobian() )
		{	not_available_message( argv[1] );
			exit(1);
		}
		ok &= run_correct(
			available_sparse_jacobian,
			correct_sparse_jacobian,
			"sparse_jacobian"
		);
		run_speed(
			speed_sparse_jacobian, size_sparse_jacobian, "sparse_jacobian"
		);
		cout << AD_PACKAGE << "_sparse_jacobian_n_sweep = ";
		for(i = 0; i < size_sparse_jacobian.size(); i++)
		{	if( i == 0 )
				cout << "[ ";
			else	cout << ", ";
			size_t n_sweep;
			info_sparse_jacobian(size_sparse_jacobian[i], n_sweep);
			cout << n_sweep;
		}
		cout << " ]" << endl;
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
