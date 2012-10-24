/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_mat_mul.cpp$$
$spell
	sq
	retape
	typedef
	ADvar
	Gradcomp
	Sacado
	cppad.hpp
	bool
	mul
	dz
	CppAD
$$

$section Sacado Speed: Matrix Multiplication$$

$index link_mat_mul, sacado$$
$index sacado, link_mat_mul$$
$index speed, sacado$$
$index sacado, speed$$
$index matrix, speed sacado$$
$index multiply, speed sacado$$

$head Specifications$$
See $cref link_mat_mul$$.

$head Implementation$$

$codep */
# include <Sacado.hpp>
# include <cppad/vector.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>
bool link_mat_mul(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           z        ,
	CppAD::vector<double>&           dz       )
{
	// speed test global option values
	extern bool global_retape, global_atomic, global_optimize;
	if( ! global_retape || global_atomic || global_optimize )
		return false;

	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef Sacado::Rad::ADvar<double>    ADScalar; 
	typedef CppAD::vector<ADScalar>       ADVector; 

	size_t j;                // temporary index
	size_t m = 1;            // number of dependent variables
	size_t n = size * size;  // number of independent variables
	ADVector   X(n);         // AD domain space vector
	ADVector   Y(n);         // Store product matrix
	ADVector   Z(m);         // AD range space vector
	ADScalar   f;
	
	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, x);

		// set independent variable values
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// do the computation
		mat_sum_sq(size, X, Y, Z);

		// create function object f : X -> Z
		f = Z[0];

		// reverse mode gradient of last ADvar computed value; i.e., f
		ADScalar::Gradcomp();

		// return gradient
		for(j = 0; j < n; j++)
			dz[j] = X[j].adj(); // partial f w.r.t X[j]
	}
	// return function value
	z[0] = f.val();

	// ---------------------------------------------------------
	return true;
}
/* $$
$end
*/
