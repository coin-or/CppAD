/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_sparse_jacobian.cpp$$
$spell
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
depends on the vectors $italic i$$ and $italic j$$.
Hence we use a different $cref/ADFun/$$ object for 
each choice of $italic i$$ and $italic j$$.

$head Sparse Jacobian$$
If the preprocessor symbol $code CPPAD_USE_SPARSE_JACOBIAN$$ is 
true, the routine $cref/SparseJacobian/sparse_jacobian/$$ 
is used for the calculation.
Otherwise, the routine $cref/Jacobian/$$ is used.

$head link_sparse_jacobian$$
$index link_sparse_jacobian$$
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_evaluate.hpp>

// value can be true or false
# define CPPAD_USE_SPARSE_JACOBIAN  1

bool link_sparse_jacobian(
	size_t                     repeat   , 
	CppAD::vector<double>     &x        ,
	CppAD::vector<size_t>     &i        ,
	CppAD::vector<size_t>     &j        ,
	CppAD::vector<double>     &jacobian )
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;
	typedef CppAD::vector<double>       DblVector;
	typedef CppAD::vector< AD<double> > ADVector;
	typedef CppAD::vector<size_t>       SizeVector;

	size_t order = 1;         // derivative order for f'(x)
	size_t ell   = i.size();  // number of indices in i and j
	size_t m     = ell;       // number of dependent variables
	size_t n     = x.size();  // number of independent variables
	ADVector   X(n);          // AD domain space vector
	ADVector   Fp(n);         // AD vector to hold f'(x)
	ADVector   Y(m);          // AD range space vector y = g(x)
	DblVector tmp(2 * ell);   // double temporary vector
	CppAD::ADFun<double> g;   // AD function object

	// choose a value for x 
	CppAD::uniform_01(n, x);
	size_t k;
	for(k = 0; k < n; k++)
		X[k] = x[k];

	// used to display results of optimizing the operation sequence
        static bool printed = false;
        bool print_this_time = (! printed) & (repeat > 1) & (n >= 30);

	// ------------------------------------------------------
	while(repeat--)
	{
		// get the next set of indices
		CppAD::uniform_01(2 * ell, tmp);
		for(k = 0; k < ell; k++)
		{	i[k] = size_t( n * tmp[k] );
			i[k] = std::min(n-1, i[k]);
			//
			j[k] = size_t( n * tmp[k + ell] );
			j[k] = std::min(n-1, j[k]);
		}

		// declare independent variables
		Independent(X);	

		// AD computation of g_k (x) = f'_{i[k]} (x)
		CppAD::sparse_evaluate< AD<double> >(X, i, j, order, Fp);
		for(k = 0; k < ell; k++)
			Y[k] = Fp[ i[k] ];

		// create function object g : X -> Y
		g.Dependent(X, Y);

		extern bool global_optimize;
		if( global_optimize )
		{	size_t before, after;
			before = g.size_var();
			g.optimize();
			if( print_this_time ) 
			{	after = g.size_var();
				std::cout << "cppad_sparse_jacobian_optimize_size_" 
				          << int(n) << " = [ " << int(before) 
				          << ", " << int(after) << "]" << std::endl;
				printed         = true;
				print_this_time = false;
			}
		}

		// evaluate and return the jacobian of f
# if CPPAD_USE_SPARSE_JACOBIAN
		jacobian = g.SparseJacobian(x);
# else
		jacobian = g.Jacobian(x);
# endif
	}
	return true;
}
/* $$
$end
*/
