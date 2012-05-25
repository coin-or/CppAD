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
$begin cppad_mat_mul.cpp$$
$spell
	info info
	nr
	nc
	cppad
	mul
	hpp
	bool
	retape
	sq
	var
	std::cout
	endl
	CppAD
	dz
	typedef
$$

$section CppAD Speed: Matrix Multiplication$$

$index cppad, speed matrix multiply$$
$index speed, cppad matrix multiply$$
$index matrix, multiply speed cppad$$
$index multiply, matrix speed cppad$$

$head Specifications$$
See $cref link_mat_mul$$.

$head Implementation$$
$index cppad, link_mat_mul$$
$index link_mat_mul, cppad$$
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>
# include "../../example/mat_mul.hpp"

bool link_mat_mul(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           z        ,
	CppAD::vector<double>&           dz
)
{
	// -----------------------------------------------------
	// setup
	typedef CppAD::AD<double>           ADScalar; 
	typedef CppAD::vector<ADScalar>     ADVector; 

	size_t j;               // temporary index
	size_t m = 1;           // number of dependent variables
	size_t n = size * size; // number of independent variables
	ADVector   X(n);        // AD domain space vector
	ADVector   Z(m);        // AD range space vector
	CppAD::ADFun<double> f; // AD function object
	
	// vectors of reverse mode weights 
	CppAD::vector<double> w(1);
	w[0] = 1.;

	// ------------------------------------------------------
	ADVector Y(n);          // Store product matrix
	static bool printed = false;
	bool print_this_time = (! printed) & (repeat > 1) & (size >= 10);

	// user atomic information
	extern bool global_atomic;
	CPPAD_TEST_VECTOR<ADScalar> ax(2 * n), ay(n);
	call_info info;
	info.nr_result = size;
	info.n_middle  = size;
	info.nc_result = size;
	size_t   info_id = info_.size();  
	

	extern bool global_retape;
	if( global_retape ) while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, x);
		for( j = 0; j < n; j++)
			X[j] = x[j];

		// declare independent variables
		Independent(X);

		// do computations
		if( ! global_atomic )
			mat_sum_sq(size, X, Y, Z);
		else
		{	info_.push_back(info);
			for(j = 0; j < n; j++)
			{	ax[j]   = X[j];
				ax[j+n] = X[j];
			}
			// Y = X * X
			mat_mul(info_id, ax, ay);
			Z[0] = 0.;
			for(j = 0; j < n; j++)
				Z[0] += ay[j];
		} 
		// create function object f : X -> Z
		f.Dependent(X, Z);

		extern bool global_optimize;
		if( global_optimize )
		{	size_t before, after;
			before = f.size_var();
			f.optimize();
			if( print_this_time ) 
			{	after = f.size_var();
				std::cout << "cppad_mat_mul_optimize_size_" 
				          << int(size) << " = [ " << int(before) 
				          << ", " << int(after) << "]" << std::endl;
				printed         = true;
				print_this_time = false;
			}
		}

		// evaluate and return gradient using reverse mode
		z  = f.Forward(0, x);
		dz = f.Reverse(1, w);
	}
	else
	{	// get a next matrix
		CppAD::uniform_01(n, x);
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// declare independent variables
		Independent(X);

		// do computations
		if( ! global_atomic )
			mat_sum_sq(size, X, Y, Z);
		else
		{	info_.push_back(info);
			for(j = 0; j < n; j++)
			{	ax[j]   = X[j];
				ax[j+n] = X[j];
			}
			// Y = X * X
			mat_mul(info_id, ax, ay);
			Z[0] = 0.;
			for(j = 0; j < n; j++)
				Z[0] += ay[j];
		} 
	
		// create function object f : X -> Z
		f.Dependent(X, Z);

		extern bool global_optimize;
		if( global_optimize )
		{	size_t before, after;
			before = f.size_var();
			f.optimize();
			if( print_this_time ) 
			{	after = f.size_var();
				std::cout << "cppad_mat_mul_optimize_size_" 
				          << int(size) << " = [ " << int(before) 
				          << ", " << int(after) << "]" << std::endl;
				printed         = true;
				print_this_time = false;
			}
		}
		while(repeat--)
		{	// get a next matrix
			CppAD::uniform_01(n, x);

			// evaluate and return gradient using reverse mode
			z  = f.Forward(0, x);
			dz = f.Reverse(1, w);
		}
	}
	return true;
}
/* $$
$end
*/
