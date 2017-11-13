/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_ode.cpp$$
$spell
	jacobian jacobian
	endif
	var
	Jacobian
	std
	cout
	endl
	CppAD
	cppad
	hpp
	bool
	onetape
	typedef
	cassert
$$

$section CppAD Speed: Gradient of Ode Solution$$
$mindex link_ode speed$$


$head Specifications$$
See $cref link_ode$$.

$head Implementation$$

$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cassert>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &jacobian
)
{
	// --------------------------------------------------------------------
	// check global options
	const char* valid[] = { "memory", "onetape", "optimize"};
	size_t n_valid = sizeof(valid) / sizeof(valid[0]);
	typedef std::map<std::string, bool>::iterator iterator;
	//
	for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
	{	if( itr->second )
		{	bool ok = false;
			for(size_t i = 0; i < n_valid; i++)
				ok |= itr->first == valid[i];
			if( ! ok )
				return false;
		}
	}
	// --------------------------------------------------------------------
	// optimization options: no conditional skips or compare operators
	std::string options="no_compare_op";
	// --------------------------------------------------------------------
	// setup
	assert( x.size() == size );
	assert( jacobian.size() == size * size );

	typedef CppAD::AD<double>       ADScalar;
	typedef CppAD::vector<ADScalar> ADVector;

	size_t j;
	size_t p = 0;              // use ode to calculate function values
	size_t n = size;           // number of independent variables
	size_t m = n;              // number of dependent variables
	ADVector  X(n), Y(m);      // independent and dependent variables
	CppAD::ADFun<double>  f;   // AD function

	// -------------------------------------------------------------
	if( ! global_option["onetape"] ) while(repeat--)
	{	// choose next x value
		uniform_01(n, x);
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// declare the independent variable vector
		Independent(X);

		// evaluate function
		CppAD::ode_evaluate(X, p, Y);

		// create function object f : X -> Y
		f.Dependent(X, Y);

		if( global_option["optimize"] )
			f.optimize(options);

		// skip comparison operators
		f.compare_change_count(0);

		jacobian = f.Jacobian(x);
	}
	else
	{	// an x value
		uniform_01(n, x);
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// declare the independent variable vector
		Independent(X);

		// evaluate function
		CppAD::ode_evaluate(X, p, Y);

		// create function object f : X -> Y
		f.Dependent(X, Y);

		if( global_option["optimize"] )
			f.optimize(options);

		// skip comparison operators
		f.compare_change_count(0);

		while(repeat--)
		{	// get next argument value
			uniform_01(n, x);

			// evaluate jacobian
			jacobian = f.Jacobian(x);
		}
	}
	return true;
}
/* %$$
$end
*/
