// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_det_lu.cpp$$
$spell
	onetape
	bool
	CppAD
	vector Vector
	typedef
	Lu
	cppad
	det
	hpp
	const
	srand
	var
	std
	cout
	endl
$$

$section CppAD Speed: Gradient of Determinant Using Lu Factorization$$
$mindex link_det_lu speed matrix factor$$


$head Specifications$$
See $cref link_det_lu$$.

$head Implementation$$
$srccode%cpp% */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_det_lu(
	size_t                           size     ,
	size_t                           repeat   ,
	CppAD::vector<double>           &matrix   ,
	CppAD::vector<double>           &gradient )
{
	// speed test global option values
	if( global_option["onetape"] || global_option["atomic"] )
		return false;

	// -----------------------------------------------------
	// setup
	typedef CppAD::AD<double>           ADScalar;
	typedef CppAD::vector<ADScalar>     ADVector;
	CppAD::det_by_lu<ADScalar>          Det(size);

	size_t i;               // temporary index
	size_t m = 1;           // number of dependent variables
	size_t n = size * size; // number of independent variables
	ADVector   A(n);        // AD domain space vector
	ADVector   detA(m);     // AD range space vector
	CppAD::ADFun<double> f; // AD function object

	// vectors of reverse mode weights
	CppAD::vector<double> w(1);
	w[0] = 1.;

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, matrix);
		for( i = 0; i < n; i++)
			A[i] = matrix[i];

		// declare independent variables
		Independent(A);

		// AD computation of the determinant
		detA[0] = Det(A);

		// create function object f : A -> detA
		f.Dependent(A, detA);
		if( global_option["optimize"] )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// evaluate and return gradient using reverse mode
		f.Forward(0, matrix);
		gradient = f.Reverse(1, w);
	}
	return true;
}
/* %$$
$end
*/
