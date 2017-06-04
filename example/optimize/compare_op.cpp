// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin optimize_compare_op.cpp$$

$section Example Optimization and Comparison Operators$$

$head See Also$$
$cref cond_exp.cpp$$

$code
$srcfile%example/optimize/compare_op.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace {
	struct tape_size { size_t n_var; size_t n_op; };

	template <class Vector> void fun(
		const std::string& options ,
		const Vector& x, Vector& y, tape_size& before, tape_size& after
	)
	{	typedef typename Vector::value_type scalar;

		// phantom variable with index 0 and independent variables
		// begin operator, independent variable operators and end operator
		before.n_var = 1 + x.size(); before.n_op  = 2 + x.size();
		after.n_var  = 1 + x.size(); after.n_op   = 2 + x.size();

		// Create a variable that is is only used in the comparision operation
		// It is not used when the comparison operator is not included
		scalar one = 1. / x[0];
		before.n_var += 1; before.n_op += 1;
		after.n_var  += 0; after.n_op  += 0;
		// If we keep comparision operators, we must compute their operands
		if( options.find("no_compare_op") == std::string::npos )
		{	after.n_var += 1;  after.n_op += 1;
		}

		// Create a variable that is used by the result
		scalar two = x[0] * 5.;
		before.n_var += 1; before.n_op += 1;
		after.n_var  += 1; after.n_op += 1;

		// Only one variable created for this comparison operation
		// but the value depends on which branch is taken.
		scalar three;
		if( one < x[0] )        // comparison operator
			three = two / 2.0;  // division operator
		else
			three = 2.0 * two;  // multiplication operator
		// comparison and either division of multiplication operator
		before.n_var += 1; before.n_op += 2;
		// comparison operator depends on optimization options
		after.n_var += 1;  after.n_op += 1;
		// check if we are keeping the comparison operator
		if( options.find("no_compare_op") == std::string::npos )
			after.n_op += 1;

		// results for this operation sequence
		y[0] = three;
		before.n_var += 0; before.n_op  += 0;
		after.n_var  += 0; after.n_op   += 0;
	}
}

bool compare_op(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps10 = 10.0 * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n  = 1;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.5;

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) ay(m);

	for(size_t k = 0; k < 2; k++)
	{	// optimization options
		std::string options = "";
		if( k == 0 )
			options = "no_compare_op";

		// declare independent variables and start tape recording
		CppAD::Independent(ax);

		// compute function value
		tape_size before, after;
		fun(options, ax, ay, before, after);

		// create f: x -> y and stop tape recording
		CppAD::ADFun<double> f(ax, ay);
		ok &= f.size_var() == before.n_var;
		ok &= f.size_op() == before.n_op;

		// Optimize the operation sequence
		f.optimize(options);
		ok &= f.size_var() == after.n_var;
		ok &= f.size_op() == after.n_op;

		// Check result for a zero order calculation for a different x,
		// where the result of the comparison is he same.
		CPPAD_TESTVECTOR(double) x(n), y(m), check(m);
		x[0] = 0.75;
		y    = f.Forward(0, x);
		if ( options == "" )
			ok  &= f.compare_change_number() == 0;
		fun(options, x, check, before, after);
		ok &= NearEqual(y[0], check[0], eps10, eps10);

		// Check case where result of the comparision is differnent
		// (hence one needs to re-tape to get correct result)
		x[0] = 2.0;
		y    = f.Forward(0, x);
		if ( options == "" )
			ok  &= f.compare_change_number() == 1;
		fun(options, x, check, before, after);
		ok  &= std::fabs(y[0] - check[0]) > 0.5;
	}
	return ok;
}

// END C++
