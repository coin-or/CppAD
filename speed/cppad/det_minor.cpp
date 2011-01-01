/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_det_minor.cpp$$
$spell
	retape
	vector Vector
	typedef
	cppad
	Lu
	CppAD
	det
	hpp
	const
	CPPAD_TEST_VECTOR
	bool
	srand
	var
	std
	cout
	endl
$$

$section CppAD Speed: Gradient of Determinant by Minor Expansion$$

$index cppad, speed minor$$
$index speed, cppad minor$$
$index minor, speed cppad$$

$head link_det_minor$$
$index link_det_minor$$
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

bool link_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef CppAD::AD<double>       ADScalar; 
	typedef CppAD::vector<ADScalar> ADVector; 
	CppAD::det_by_minor<ADScalar>   Det(size);

	size_t i;               // temporary index
	size_t m = 1;           // number of dependent variables
	size_t n = size * size; // number of independent variables
	ADVector   A(n);        // AD domain space vector
	ADVector   detA(m);     // AD range space vector
	
	// vectors of reverse mode weights 
	CppAD::vector<double> w(1);
	w[0] = 1.;

	// the AD function object
	CppAD::ADFun<double> f;

	static bool printed = false;
	bool print_this_time = (! printed) & (repeat > 1) & (size >= 3);

	extern bool global_retape;
	if( global_retape ) while(repeat--)
	{
		// choose a matrix
		CppAD::uniform_01(n, matrix);
		for( i = 0; i < size * size; i++)
			A[i] = matrix[i];
	
		// declare independent variables
		Independent(A);
	
		// AD computation of the determinant
		detA[0] = Det(A);
	
		// create function object f : A -> detA
		f.Dependent(A, detA);

		extern bool global_optimize;
		if( global_optimize )
		{	size_t before, after;
			before = f.size_var();
			f.optimize();
			if( print_this_time ) 
			{	after = f.size_var();
				std::cout << "cppad_det_minor_optimize_size_" 
				          << int(size) << " = [ " << int(before) 
				          << ", " << int(after) << "]" << std::endl;
				printed         = true;
				print_this_time = false;
			}
		}
	
		// get the next matrix
		CppAD::uniform_01(n, matrix);
	
		// evaluate the determinant at the new matrix value
		f.Forward(0, matrix);
	
		// evaluate and return gradient using reverse mode
		gradient = f.Reverse(1, w);
	}
	else
	{
		// choose a matrix
		CppAD::uniform_01(n, matrix);
		for( i = 0; i < size * size; i++)
			A[i] = matrix[i];
	
		// declare independent variables
		Independent(A);
	
		// AD computation of the determinant
		detA[0] = Det(A);
	
		// create function object f : A -> detA
		f.Dependent(A, detA);

		extern bool global_optimize;
		if( global_optimize )
		{	size_t before, after;
			before = f.size_var();
			f.optimize();
			if( print_this_time ) 
			{	after = f.size_var();
				std::cout << "optimize: size = " << size
				          << ": size_var() = "
				          << before << "(before) " 
				          << after << "(after) " 
				          << std::endl;
				printed         = true;
				print_this_time = false;
			}
		}
	
		// ------------------------------------------------------
		while(repeat--)
		{	// get the next matrix
			CppAD::uniform_01(n, matrix);
	
			// evaluate the determinant at the new matrix value
			f.Forward(0, matrix);
	
			// evaluate and return gradient using reverse mode
			gradient = f.Reverse(1, w);
		}
	}
	return true;
}
/* $$
$end
*/
