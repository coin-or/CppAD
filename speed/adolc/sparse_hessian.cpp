/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_sparse_hessian.cpp$$
$spell
	const
	hes
	thread_alloc
	arg
	cppad
	adouble
	CppAD
	adolc
	hpp
	bool
	typedef
	endif
	tmp
	std
$$

$section Adolc Speed: Sparse Hessian$$

$index adolc, speed sparse Hessian$$
$index speed, adolc sparse Hessian$$
$index Hessian, sparse speed adolc$$
$index sparse, Hessian speed adolc$$

$head Operation Sequence$$
Note that the 
$cref/operation sequence/glossary/Operation/Sequence/$$
depends on the vectors $icode i$$ and $icode j$$.
Hence we use a different $cref ADFun$$ object for 
each choice of $icode i$$ and $icode j$$.

$head link_sparse_hessian$$
$index link_sparse_hessian$$
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/thread_alloc.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/drivers/drivers.h>

bool link_sparse_hessian(
	size_t                           repeat   , 
	CppAD::vector<double>           &x_arg    ,
	const CppAD::vector<size_t>     &row      ,
	const CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>           &hes      )
{
	// -----------------------------------------------------
	// setup
	size_t i, j;
	size_t order = 0;         // derivative order corresponding to function
	size_t tag  = 0;          // tape identifier
	size_t keep = 1;          // keep forward mode results in buffer
	size_t n = x_arg.size();  // number of independent variables
	size_t m = 1;             // number of dependent variables
	double f;                 // function value

	// use thread_alloc memory allocator (fast and checks for leaks)
	using CppAD::thread_alloc; // the allocator
	size_t capacity;           // capacity of an allocation

	typedef CppAD::vector<double>  DblVector;
	typedef CppAD::vector<adouble> ADVector;
	typedef CppAD::vector<size_t>  SizeVector;

	ADVector   a_x(n);      // AD domain space vector
	ADVector   a_y(m);      // AD range space value

	// double version of domain space vector
	double* x  = thread_alloc::create_array<double>(n, capacity);
	// Hessian as computed by adolc
	double** H = thread_alloc::create_array<double*>(n, capacity);
	for(i = 0; i < n; i++)
		H[i] = thread_alloc::create_array<double>(n, capacity);

	// choose a value for x 
	CppAD::uniform_01(n, x);
	for(j = 0; j < n; j++)
		x_arg[j] = x[j];

	// ------------------------------------------------------
	while(repeat--)
	{
		// declare independent variables
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			a_x[j] <<= x[j];

		// AD computation of f(x)
		CppAD::sparse_hes_fun(a_x, row, col, order, a_y);

		// create function object f : X -> Y
		a_y[0] >>= f;
		trace_off();

		// evaluate and return the hessian of f
		hessian(int(tag), int(n), x, H);
	}
	for(i = 0; i < n; i++)
	{	for(j = 0; j <= i; j++)
		{	hes[ i * n + j] = H[i][j];
			hes[ j * n + i] = H[i][j];
		}
		thread_alloc::delete_array(H[i]);
	}
	thread_alloc::delete_array(H);
	thread_alloc::delete_array(x);
	return true;
}
/* $$
$end
*/
