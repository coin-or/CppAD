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
$begin adolc_poly.cpp$$
$spell
	alloc
	retape
	coef
	cppad
	hos
	Taylor
	std
	ddp
	cppad
	adouble
	std
	vector Vector
	typedef
	adolc
	Lu
	CppAD
	det
	hpp
	const
	bool
$$

$section Adolc Speed: Second Derivative of a Polynomial$$

$index adolc, speed polynomial$$
$index speed, adolc polynomial$$
$index polynomial, speed adolc$$

$head link_poly$$
$index link_poly$$
$codep */
# include <vector>

# include <cppad/speed/uniform_01.hpp>
# include <cppad/poly.hpp>
# include <cppad/vector.hpp>
# include <cppad/thread_alloc.hpp>

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

bool link_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &a        ,  // coefficients of polynomial
	CppAD::vector<double>     &z        ,  // polynomial argument value
	CppAD::vector<double>     &ddp      )  // second derivative w.r.t z  
{
	// -----------------------------------------------------
	// setup
	int tag  = 0;  // tape identifier
	int keep = 1;  // keep forward mode results in buffer
	int m    = 1;  // number of dependent variables
	int n    = 1;  // number of independent variables
	int d    = 2;  // order of the derivative
	double f;      // function value
	int i;         // temporary index

	// Use thread_alloc memory allocator (fast and checks for leaks)
	using CppAD::thread_alloc; // the allocator
	size_t capacity;           // capacity of an allocation

	// choose a vector of polynomial coefficients
	CppAD::uniform_01(size, a);

	// AD copy of the polynomial coefficients
	std::vector<adouble> A(size);
	for(i = 0; i < int(size); i++)
		A[i] = a[i];

	// domain and range space AD values
	adouble Z, P;

	// allocate arguments to hos_forward
	double* x0 = thread_alloc::create_array<double>(n, capacity);
	double* y0 = thread_alloc::create_array<double>(m, capacity);
	double** x = thread_alloc::create_array<double*>(n, capacity);
	double** y = thread_alloc::create_array<double*>(m, capacity);
	for(i = 0; i < n; i++)
		x[i] = thread_alloc::create_array<double>(d, capacity);
	for(i = 0; i < m; i++)
		y[i] = thread_alloc::create_array<double>(d, capacity);

	// Taylor coefficient for argument
	x[0][0] = 1.;  // first order
	x[0][1] = 0.;  // second order
	

	extern bool global_retape;
	if( global_retape ) while(repeat--)
	{	// choose an argument value
		CppAD::uniform_01(1, z);

		// declare independent variables
		trace_on(tag, keep);
		Z <<= z[0]; 

		// AD computation of the function value 
		P = CppAD::Poly(0, A, Z);

		// create function object f : Z -> P
		P >>= f;
		trace_off();

		// get the next argument value
		CppAD::uniform_01(1, z);
		x0[0] = z[0];

		// evaluate the polynomial at the new argument value
		hos_forward(tag, m, n, d, keep, x0, x, y0, y);

		// second derivative is twice second order Taylor coef
		ddp[0] = 2. * y[0][1];
	}
	else
	{
		// choose an argument value
		CppAD::uniform_01(1, z);

		// declare independent variables
		trace_on(tag, keep);
		Z <<= z[0]; 

		// AD computation of the function value 
		P = CppAD::Poly(0, A, Z);

		// create function object f : Z -> P
		P >>= f;
		trace_off();
		while(repeat--)
		{	// get the next argument value
			CppAD::uniform_01(1, z);
			x0[0] = z[0];

			// evaluate the polynomial at the new argument value
			hos_forward(tag, m, n, d, keep, x0, x, y0, y);

			// second derivative is twice second order Taylor coef
			ddp[0] = 2. * y[0][1];
		}
	}
	// ------------------------------------------------------
	// tear down
	thread_alloc::delete_array(x0);
	thread_alloc::delete_array(y0);
	for(i = 0; i < n; i++)
		thread_alloc::delete_array(x[i]);
	for(i = 0; i < m; i++)
		thread_alloc::delete_array(y[i]);
	thread_alloc::delete_array(x);
	thread_alloc::delete_array(y);

	return true;
}
/* $$
$end
*/
