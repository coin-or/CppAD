/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_sparse_hessian.cpp$$
$spell
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
depends on the vectors $italic i$$ and $italic j$$.
Hence we use a different $cref/ADFun/$$ object for 
each choice of $italic i$$ and $italic j$$.

$head link_sparse_hessian$$
$index link_sparse_hessian$$
Routine that computes the gradient of determinant using Adolc:
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/track_new_del.hpp>
# include <cppad/speed/sparse_evaluate.hpp>

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/drivers/drivers.h>

bool link_sparse_hessian(
	size_t                     repeat   , 
	CppAD::vector<double>     &x_arg    ,
	CppAD::vector<size_t>     &i        ,
	CppAD::vector<size_t>     &j        ,
	CppAD::vector<double>     &h        )
{
	// -----------------------------------------------------
	// setup
	size_t k, m;
	size_t order = 0;         // derivative order corresponding to function
	size_t tag  = 0;          // tape identifier
	size_t keep = 1;          // keep forward mode results in buffer
	size_t n = x_arg.size();  // number of independent variables
	size_t ell = i.size();    // number of indices in i and j
	double f;                 // function value

	typedef CppAD::vector<double>  DblVector;
	typedef CppAD::vector<adouble> ADVector;
	typedef CppAD::vector<size_t>  SizeVector;

	ADVector   X(n);    // AD domain space vector
	double       *x;    // double domain space vector
	double      **H;    // Hessian 
	ADVector   Y(1);    // AD range space value
	DblVector tmp(2 * ell);       // double temporary vector

	x = 0;
	x = CPPAD_TRACK_NEW_VEC(n, x);
	H = 0;
	H = CPPAD_TRACK_NEW_VEC(n, H);
	for(k = 0; k < n; k++)
	{	H[k] = 0;
		H[k] = CPPAD_TRACK_NEW_VEC(n, H[k]);
	}

	// choose a value for x 
	CppAD::uniform_01(n, x);
	for(k = 0; k < n; k++)
		x_arg[k] = x[k];

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
		trace_on(tag, keep);
		for(k = 0; k < n; k++)
			X[k] <<= x[k];

		// AD computation of f(x)
		CppAD::sparse_evaluate(X, i, j, order, Y);

		// create function object f : X -> Y
		Y[0] >>= f;
		trace_off();

		// evaluate and return the hessian of f
		hessian(int(tag), int(n), x, H);
	}
	for(k = 0; k < n; k++)
	{	for(m = 0; m <= k; m++)
		{	h[ k * n + m] = H[k][m];
			h[ m * n + k] = H[k][m];
		}
		CPPAD_TRACK_DEL_VEC(H[k]);
	}
	CPPAD_TRACK_DEL_VEC(H);
	CPPAD_TRACK_DEL_VEC(x);
	return true;
}
/* $$
$end
*/
