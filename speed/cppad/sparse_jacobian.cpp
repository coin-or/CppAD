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
$begin cppad_sparse_jacobian.cpp$$
$spell
	jac
	CppAD
	cppad
	hpp
	bool
	typedef
	endif
	tmp
	std
	var
	cout
	endl
	Jacobian
	Fp
$$

$section CppAD Speed: Sparse Jacobian$$

$index cppad, speed sparse Jacobian$$
$index speed, cppad sparse Jacobian$$
$index Jacobian, sparse speed cppad$$
$index sparse, Jacobian speed cppad$$

$head Operation Sequence$$
Note that the 
$cref/operation sequence/glossary/Operation/Sequence/$$
depends on the vectors $icode i$$ and $icode j$$.
Hence we use a different $cref ADFun$$ object for 
each choice of $icode i$$ and $icode j$$.

$head Sparse Jacobian$$
If the preprocessor symbol $code CPPAD_USE_SPARSE_JACOBIAN$$ is 
true, the routine $cref/SparseJacobian/sparse_jacobian/$$ 
is used for the calculation.
Otherwise, the routine $cref Jacobian$$ is used.

$head link_sparse_jacobian$$
$index link_sparse_jacobian$$
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

// value can be true or false
# define CPPAD_USE_SPARSE_JACOBIAN  1

bool link_sparse_jacobian(
	size_t                     repeat   , 
	size_t                     m        ,
	CppAD::vector<double>     &x        ,
	CppAD::vector<size_t>     &row      ,
	CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>     &jacobian )
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;
	typedef CppAD::vector<double>       DblVector;
	typedef CppAD::vector< AD<double> > ADVector;
	typedef CppAD::vector<size_t>       SizeVector;

	size_t order = 0;         // derivative order corresponding to function 
	size_t K     = row.size();// number of row and column indices 
	size_t n     = x.size();  // number of independent variables

	ADVector   a_x(n);        // AD domain space vector
	ADVector   a_y(m);        // AD range space vector y = g(x)
	DblVector tmp(2 * K);     // double temporary vector
	CppAD::ADFun<double> f;   // AD function object

	// choose a value for x 
	CppAD::uniform_01(n, x);
	size_t k;
	for(k = 0; k < n; k++)
		a_x[k] = x[k];

	// used to display results of optimizing the operation sequence
	static bool printed = false;
	bool print_this_time = (! printed) & (repeat > 1) & (n >= 30);

	// ------------------------------------------------------
	while(repeat--)
	{
		// get the next set of indices
		CppAD::uniform_01(2 * K, tmp);
		for(k = 0; k < K; k++)
		{	row[k] = size_t( m * tmp[k] );
			row[k] = std::min(m-1, row[k]);
			//
			col[k] = size_t( n * tmp[k + K] );
			col[k] = std::min(n-1, col[k]);
		}

		// declare independent variables
		Independent(a_x);	

		// AD computation of f (x) 
		CppAD::sparse_jac_fun(m, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		extern bool global_optimize;
		if( global_optimize )
		{	size_t before, after;
			before = f.size_var();
			f.optimize();
			if( print_this_time ) 
			{	after = f.size_var();
				std::cout << "cppad_sparse_jacobian_optimize_size_" 
				          << int(n) << " = [ " << int(before) 
				          << ", " << int(after) << "]" << std::endl;
				printed         = true;
				print_this_time = false;
			}
		}

		// evaluate and return the jacobian of f
# if CPPAD_USE_SPARSE_JACOBIAN
		jacobian = f.SparseJacobian(x);
# else
		jacobian = f.Jacobian(x);
# endif
	}
	return true;
}
/* $$
$end
*/
