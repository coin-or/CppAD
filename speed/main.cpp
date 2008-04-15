/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

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
# include <cppad/speed/quadratic_ok.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/poly.hpp>

# ifdef ADOLC
# define AD_PACKAGE "adolc"
# endif
# ifdef CPPAD
# define AD_PACKAGE "cppad"
# endif
# ifdef DOUBLE
# define AD_PACKAGE "double"
# endif
# ifdef FADBAD
# define AD_PACKAGE "fadbad"
# endif
# ifdef PROFILE
# define AD_PACKAGE "profile"
# endif
# ifdef SACADO
# define AD_PACKAGE "sacado"
# endif

/*
$begin speed_main$$
$spell
	bool
	ddp
	cppad
	adolc
	fadbad
	sacado
	CppAD
	det
	lu
$$

$index cppad, speed test$$
$index speed, test cppad$$
$index test, cppad speed$$

$section Speed Testing Main Program$$

$head Syntax$$
$syntax%speed/%package%/%package% %option% %seed%
%$$

$head Purpose$$
A version of this program runs the correctness tests
or the speed tests for one AD package identified by $italic package$$.

$head package$$

$subhead AD Package$$
The value
$italic package$$ can specify one of the following AD packages:
$cref/adolc/speed_adolc/$$,
$cref/cppad/speed_cppad/$$, 
$cref/fadbad/speed_fadbad/$$, 
$cref/sacado/speed_sacado/$$.

$subhead double$$
The value
$italic package$$ can be $code double$$ in which case
the function values (instead of derivatives) are computed
using double precision operations.
This enables one to compare the speed of computing function
values in $code double$$ to the speed of the derivative computations.
(It is often useful to divide the speed of the derivative computation by
the speed of the function evaluation in $code double$$.) 

$subhead profile$$
In the special case where $italic package$$ is $code profile$$,
the CppAD package is compiled and run with profiling to aid in determining
where it is spending most of its time.

$head option$$
It the argument $italic option$$ specifies which test to run
and has the following possible values:
$cref/correct/speed_main/option/correct/$$,
$cref/speed/speed_main/option/speed/$$,
$cref/det_minor/speed_main/det_minor/$$,
$cref/det_lu/speed_main/det_lu/$$,
$cref/poly/speed_main/poly/$$.

$subhead correct$$
If $italic option$$ is equal to $code correct$$,
all of the correctness tests are run.

$subhead speed$$
If $italic option$$ is equal to $code speed$$,
all of the speed tests are run.

$head Return Values$$
Each of the $italic option$$ values specified below
corresponds to a particular test.
The function call corresponding to each test
has a $code bool$$ return value.
If a particular $italic package$$ supports a test,
it should return $code true$$ when the corresponding function is called.
Otherwise (if the return value is $code false$$),
the test is skipped whenever the corresponding
$italic package$$ and $italic option$$ are specified.

$head seed$$
$index uniform_01$$
The random number simulator $cref/uniform_01/$$ is initialized with
the call
$syntax%
	uniform_01(%seed%)
%$$
before any of the testing routines (listed above) are called.

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

$comment -----------------------------------------------------------------$$
$head det_lu$$
$index det_lu, correct$$
$index det_lu, speed$$
$index speed, det_lu$$
$index correct, det_lu$$
If $italic option$$ is equal to $code det_lu$$,
the correctness and speed test for the
gradient of the determinant using LU factorization tests are run.
Each package defines a version of this test with the following prototype:
$codep */
	extern bool compute_det_lu(
		size_t                     size      , 
		size_t                     repeat    , 
		CppAD::vector<double>      &matrix   ,
		CppAD::vector<double>      &gradient 
	);
/* $$

$subhead size$$
The argument $italic size$$
is the number of rows and columns in the matrix.

$subhead repeat$$
The argument $italic repeat$$ is the number of different matrices
that the gradient is computed for.

$subhead matrix$$
The argument $italic matrix$$ is a vector with 
$syntax%%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the last matrix that the
gradient is computed for.

$subhead gradient$$
The argument $italic gradient$$ is a vector with 
$syntax%%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the gradient of the
determinant of $italic matrix$$ with respect to its elements.
gradient is computed for.
(If $italic package$$ is $code double$$, only the first element
of $italic gradient$$ is used and it is actually the determinant value instead
of the gradient value.)


$comment -----------------------------------------------------------------$$
$head det_minor$$
$index det_minor, correct$$
$index det_minor, speed$$
$index speed, det_minor$$
$index correct, det_minor$$
If $italic option$$ is equal to $code det_minor$$,
the correctness and speed test for the
gradient of the determinant using expansion by minors are run.
Each package defines a version of this test with the following prototype:
$codep */
	extern bool compute_det_minor(
		size_t                     size      , 
		size_t                     repeat    , 
		CppAD::vector<double>      &matrix   ,
		CppAD::vector<double>      &gradient 
	);
/* $$
$subhead size$$
The argument $italic size$$
is the number of rows and columns in the matrix.

$subhead repeat$$
The argument $italic repeat$$ is the number of different matrices
that the gradient is computed for.

$subhead matrix$$
The argument $italic matrix$$ is a vector with 
$syntax%%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the last matrix that the
gradient is computed for.

$subhead gradient$$
The argument $italic gradient$$ is a vector with 
$syntax%%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the gradient of the
determinant of $italic matrix$$ with respect to its elements.
gradient is computed for.
(If $italic package$$ is $code double$$, only the first element
of $italic gradient$$ is used and it is actually the determinant value
instead of the gradient value.)


$comment -----------------------------------------------------------------$$
$head poly$$
$index poly, correct$$
$index poly, speed$$
$index speed, poly$$
$index correct, poly$$
If $italic option$$ is equal to $code poly$$,
the correctness and speed test for the derivative of a polynomial are run.
Each package defines a version of this test with the following prototype:
$codep */
	extern bool compute_poly(
		size_t                     size     , 
		size_t                     repeat   , 
		CppAD::vector<double>      &a       ,
		CppAD::vector<double>      &z       ,
		CppAD::vector<double>      &ddp      
	);
/* $$

$subhead size$$
The argument $italic size$$ is the order of the polynomial
(the number of coefficients in the polynomial).

$subhead repeat$$
The argument $italic repeat$$ is the number of different argument
values that the polynomial will be differentiated at.

$subhead a$$
The argument $italic a$$ is a vector with 
$syntax%%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its is the coefficients of the 
last polynomial that is differentiated
($th i$$ element is coefficient of order $latex i$$).

$subhead z$$
The argument $italic z$$ is a vector with one element.
The input value of the element does not matter.
The output its value is the polynomial argument value
were the derivative was computed.

$subhead ddp$$
The argument $italic ddp$$ is a vector with one element.
The input value of the element does not matter.
The output its value is the second derivative of the polynomial value
with respect to it's argument value.
(If $italic package$$ is $code double$$, only the first element
of $italic ddp$$ is used and it is actually the polynomial value instead
of the gradient value.)

$comment -----------------------------------------------------------------$$
$head sparse_hessian$$
$index sparse, quadratic$$
$index quadratic, sparse$$
$index speed, sparse quadratic$$
$index correct, sparse quadratic$$
If $italic option$$ is equal to $code sparse_hessian$$,
the correctness and speed test for computing a sparse Hessian matrix are run.
Each package defines a version of this test with the following prototype:
$codep */
	extern bool compute_sparse_hessian(
		size_t                     size       , 
		size_t                     repeat     ,
		size_t                      ell       ,
		CppAD::vector<size_t>      &i         ,
		CppAD::vector<size_t>      &j         , 
		CppAD::vector<double>      &hessian
	);
/* $$

$subhead f$$
The function $latex f : \R^n \rightarrow \R $$ is defined by
$latex \[
	f(x) = \sum_{k=1}^\ell x_{i[k]} x_{j[k]}
\] $$
Note that different vectors $latex i$$ and $latex j$$ 
correspond to different functions $latex f(x)$$.

$subhead size$$
The argument $italic size$$
is the dimension of the argument space for the function $latex f$$; 
i.e. $italic size$$ is equal to $latex n$$.

$subhead repeat$$
The argument $italic repeat$$ is the number of different functions
$latex f(x)$$
that the Hessian is computed for.

$head ell$$
The argument $italic ell$$ has prototype
$syntax%
        size_t %ell%
%$$
and is the number of terms in the summation that defines $latex f(x)$$; i.e.,
$latex \ell$$.

$subhead i$$
The argument $italic i$$ is a vector of size $italic ell$$.
The input value of its elements does not matter.
On output, it has been set the first index vector
for the last function $latex f(x)$$ (each element of the repeat
loop corresponds to a different choice of $latex i$$ and $latex j$$).
All the elements of $italic i$$ must be between zero and $syntax%%size%-1%$$.

$subhead j$$
The argument $italic j$$ is a vector of size $italic ell$$.
The input value of its elements does not matter.
On output, it has been set the second index vector
for the last function $latex f(x)$$. 
All the elements of $italic j$$ must be between zero and $syntax%%size%-1%$$.

$subhead hessian$$
The argument $italic hessian$$ is a vector with 
$syntax%%n%*%n%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Hessian of the function $latex f(x)$$
corresponding to output values of $italic i$$ and $italic j$$.
To be more specific,
$latex k = 0 , \ldots , n-1$$,
$latex m = 0 , \ldots , n-1$$,
$latex \[
	\DD{f}{x[k]}{x[m]} = hessian [ k * n + m ]
\] $$
(If $italic package$$ is $code double$$, only the first element
of $italic hessian$$ is used and it is actually $latex f(x)$$  value instead
of $latex f^{(2)} (x)$$.)

$end 
-----------------------------------------------------------------------------
*/

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
	// functions that check if a test is available
	bool available_det_lu(void)
	{	size_t size   = 3;
		size_t repeat = 1;
		CppAD::vector<double> matrix(size * size);
		CppAD::vector<double> gradient(size * size);

		return compute_det_lu(size, repeat, matrix, gradient);
	}
	bool available_det_minor(void)
	{	size_t size   = 3;
		size_t repeat = 1;
		CppAD::vector<double> matrix(size * size);
		CppAD::vector<double> gradient(size * size);

		return compute_det_minor(size, repeat, matrix, gradient);
	}
	bool available_poly(void)
	{	size_t size   = 10;
		size_t repeat = 1;
		CppAD::vector<double>  a(size), z(1), ddp(1);

		return compute_poly(size, repeat, a, z, ddp);
	}
	bool available_sparse_hessian(void)
	{	size_t size   = 10;
		size_t repeat = 1;
		size_t ell    = 3 * size;
		CppAD::vector<size_t> i(ell), j(ell); 
		CppAD::vector<double> hessian(size * size);

		return compute_sparse_hessian(
			size, repeat, ell, i, j, hessian
		);
	}
	// ----------------------------------------------------------------
	// function that runs one correctness case
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run_correct(bool correct_case(void), const char *case_name)
	{	bool ok;
		cout << AD_PACKAGE << "_" << case_name << "_ok = ";
		ok = correct_case();
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
	bool correct_det_lu(void)
	{	size_t size   = 3;
		size_t repeat = 1;
		CppAD::vector<double> matrix(size * size);
		CppAD::vector<double> gradient(size * size);

		compute_det_lu(size, repeat, matrix, gradient);
		bool ok = CppAD::det_grad_33(matrix, gradient);
# if DOUBLE
		ok = CppAD::det_33(matrix, gradient);
# endif
		return ok;
	}
	bool correct_det_minor(void)
	{	size_t size   = 3;
		size_t repeat = 1;
		CppAD::vector<double> matrix(size * size);
		CppAD::vector<double> gradient(size * size);

		compute_det_minor(size, repeat, matrix, gradient);
		bool ok = CppAD::det_grad_33(matrix, gradient);
# if DOUBLE
		ok = CppAD::det_33(matrix, gradient);
# endif
		return ok;
	}
	bool correct_poly(void)
	{	size_t size   = 10;
		size_t repeat = 1;
		CppAD::vector<double>  a(size), z(1), ddp(1);

		compute_poly(size, repeat, a, z, ddp);
		double check = CppAD::Poly(2, a, z[0]);
		// use direct evaluation by Poly to check AD evaluation
# if DOUBLE
		check = 0.;
		size_t i = size;
		while(i--)
		{	check *= z[0];
			check += a[i];
		}
# endif
		bool ok = CppAD::NearEqual(check, ddp[0], 1e-10, 1e-10);
		return ok;
	}
	bool correct_sparse_hessian(void)
	{	size_t size   = 10;
		size_t repeat = 1;
		size_t ell    = 3 * size;
		CppAD::vector<size_t> i(ell), j(ell);
		CppAD::vector<double> x(size); // values do not matter
		CppAD::vector<double> hessian(size * size);

		compute_sparse_hessian(size, repeat, ell, i, j, hessian);
		size_t m = 2; // order of derivative
		bool ok = CppAD::quadratic_ok(size, i, j, x, m, hessian);
		return ok;
	}
	// ----------------------------------------------------------------
	// function that runs one speed case
	void Run_speed(
		void speed_case(size_t size, size_t repeat) , 
		CppAD::vector<size_t>              size_vec ,
		std::string                       case_name )
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
	void speed_det_lu(size_t size, size_t repeat)
	{	CppAD::vector<double> matrix(size * size);
		CppAD::vector<double> gradient(size * size);

		compute_det_lu(size, repeat, matrix, gradient);
		return;
	}
	void speed_det_minor(size_t size, size_t repeat)
	{	CppAD::vector<double> matrix(size * size);
		CppAD::vector<double> gradient(size * size);

		compute_det_minor(size, repeat, matrix, gradient);
		return;
	}
	void speed_poly(size_t size, size_t repeat)
	{	CppAD::vector<double>  a(size), z(1), ddp(1);

		compute_poly(size, repeat, a, z, ddp);
		return;
	}
	void speed_sparse_hessian(size_t size, size_t repeat)
	{	CppAD::vector<double> hessian(size * size);

		size_t ell = 3 * size;
		CppAD::vector<size_t> i(ell), j(ell);
		compute_sparse_hessian(size, repeat, ell, i, j, hessian);
		return;
	}
}

// main program that runs all the tests
int main(int argc, char *argv[])
{	bool ok = true;

	const char *option[]= {
		"correct",
		"speed",
		"det_lu",
		"det_minor",
		"poly",
		"sparse_hessian"
	};
	const size_t n_option  = sizeof(option) / sizeof(option[0]);
	const size_t option_correct   = 0;
	const size_t option_speed     = 1;
	const size_t option_det_lu    = 2;
	const size_t option_det_minor = 3;
	const size_t option_poly      = 4;
	const size_t option_sparse_hessian = 5;
	assert( n_option == option_sparse_hessian+1 );

	size_t i;
	size_t match = n_option;
	int    iseed = 0;
	if( argc == 3 )
	{	for(i = 0; i < n_option; i++)
			if( strcmp(option[i], argv[1]) == 0 )
				match = i;
		iseed = std::atoi( argv[2] );
	}
	if( match == n_option  || iseed <= 0 )
	{	cout << "usage: ./" << AD_PACKAGE << " option seed" << endl;
		cout << "option choices: " << option[0];
		for(i = 1; i < n_option; i++)
			cout << ", " << option[i];
		cout << "." << endl << "seed value: ";
		cout << "a positive integer used as a random seed." << endl;
		cout << endl;
		return 1;
	}

	// initialize the random number simulator
	CppAD::uniform_01(size_t(iseed));

	// arguments needed for speed tests
	size_t n_size   = 5;
	CppAD::vector<size_t> size_det_lu(n_size);
	CppAD::vector<size_t> size_det_minor(n_size);
	CppAD::vector<size_t> size_poly(n_size);
	CppAD::vector<size_t> size_sparse_hessian(n_size);
	for(i = 0; i < n_size; i++) 
	{	size_det_lu[i]      = 3 * i + 1;
		size_det_minor[i]   = i + 1;
		size_poly[i]        = 8 * i + 1;
		size_sparse_hessian[i] = 10 * (i + 1);
	}
# ifndef NDEBUG
	size_t base_count = CPPAD_TRACK_COUNT();
# endif

	switch(match)
	{
		// run all the correctness tests
		case option_correct:
		if( available_det_lu() ) ok &= Run_correct(
			correct_det_lu,    "det_lu"       
		);
		if( available_det_minor() ) ok &= Run_correct(
			correct_det_minor, "det_minor"    
		);
		if( available_poly() ) ok &= Run_correct(
			correct_poly,      "poly"         
		);
		if( available_sparse_hessian() ) ok &= Run_correct(
			correct_sparse_hessian, "sparse_hessian"         
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
		case option_speed:
		if( available_det_lu() ) Run_speed(
		speed_det_lu,    size_det_lu,    "det_lu"
		);
		if( available_det_minor() ) Run_speed(
		speed_det_minor, size_det_minor, "det_minor"
		);
		if( available_poly() ) Run_speed(
		speed_poly,      size_poly,      "poly"
		);
		if( available_sparse_hessian() ) Run_speed(
		speed_sparse_hessian,  size_sparse_hessian,   "sparse_hessian"
		);
		ok = true;
		break;
		// ---------------------------------------------------------

		case option_det_lu:
		if( ! available_det_lu() )
		{	cout << AD_PACKAGE << ": option " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= Run_correct(correct_det_lu,          "det_lu");
		Run_speed(speed_det_lu,    size_det_lu,    "det_lu");
		break;
		// ---------------------------------------------------------

		case option_det_minor:
		if( ! available_det_minor() )
		{	cout << AD_PACKAGE << ": option " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= Run_correct(correct_det_minor,       "det_minor");
		Run_speed(speed_det_minor, size_det_minor, "det_minor");
		break;
		// ---------------------------------------------------------

		case option_poly:
		if( ! available_poly() )
		{	cout << AD_PACKAGE << ": option " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= Run_correct(correct_poly,            "poly");
		Run_speed(speed_poly,      size_poly,      "poly");
		break;
		// ---------------------------------------------------------

		case option_sparse_hessian:
		if( ! available_sparse_hessian() )
		{	cout << AD_PACKAGE << ": option " << argv[1] 
			     << " not available" << endl; 
			exit(1);
		}
		ok &= Run_correct(correct_sparse_hessian, "sparse_hessian");
		Run_speed(
		speed_sparse_hessian, size_sparse_hessian,  "sparse_hessian");
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
