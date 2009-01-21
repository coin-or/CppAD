/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin abort_recording.cpp$$
$spell
$$

$section Abort Current Recording: Example and Test$$

$index abort, example$$
$index example, abort$$
$index test, abort$$
$index recording, abort$$
$index abort, recording$$

$code
$verbatim%example/abort_recording.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <limits>

bool abort_recording(void)
{	bool ok = true;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	using CppAD::AD;

	try 
	{	// domain space vector
		size_t n = 1;
		CPPAD_TEST_VECTOR< AD<double> > x(n);
		x[0]     = 0.;

		// declare independent variables and start tape recording
		CppAD::Independent(x);

		// simulate an error during calculation of y and the execution
		// stream was aborted
		throw 1;
	}
	catch (int e)
	{	ok &= (e == 1);

		// do this incase throw occured after the call to Independent
		// (for case above this is known, but in general it is unknown)
		AD<double>::abort_recording();
	}
	/*
 	Now make sure that we can start another recording
	*/

	// declare independent variables and start tape recording
	size_t n  = 1;
	double x0 = 0.5;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0; 
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = 2 * x[0];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// forward computation of partials w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= CppAD::NearEqual(dy[0], 2., eps, eps);

	return ok;
}
// END PROGRAM
