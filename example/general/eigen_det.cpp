/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin eigen_det.cpp$$
$spell
	Eigen
$$

$section Using Eigen To Compute Determinant: Example and Test$$
$mindex determinant$$


$code
$srcfile%example/general/eigen_det.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/example/cppad_eigen.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <Eigen/Dense>

bool eigen_det(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using Eigen::Matrix;
	using Eigen::Dynamic;
	//
	typedef Matrix< double     , Dynamic, Dynamic > matrix;
	typedef Matrix< AD<double> , Dynamic, Dynamic > a_matrix;
	//
	typedef Matrix< double ,     Dynamic , 1>       vector;
	typedef Matrix< AD<double> , Dynamic , 1>       a_vector;
	// some temporary indices
	size_t i, j;

	// domain and range space vectors
	size_t size = 3, n  = size * size, m = 1;
	a_vector a_x(n), a_y(m);
	vector x(n);

	// set and declare independent variables and start tape recording
	for(i = 0; i < size; i++)
	{	for(j = 0; j < size; j++)
		{	// lower triangular matrix
			a_x[i * size + j] = x[i * size + j] = 0.0;
			if( j <= i )
				a_x[i * size + j] = x[i * size + j] = double(1 + i + j);
		}
	}
	CppAD::Independent(a_x);

	// copy independent variable vector to a matrix
	a_matrix a_X(size, size);
	matrix X(size, size);
	for(i = 0; i < size; i++)
	{	for(j = 0; j < size; j++)
		{	X(i, j)   = x[i * size + j];
			// If we used a_X(i, j) = X(i, j), a_X would not depend on a_x.
			a_X(i, j) = a_x[i * size + j];
		}
	}

	// Compute the log of determinant of X
	a_y[0] = log( a_X.determinant() );

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(a_x, a_y);

	// check function value
	double eps = 100. * CppAD::numeric_limits<double>::epsilon();
	CppAD::det_by_minor<double> det(size);
	ok &= NearEqual(Value(a_y[0]) , log(det(x)), eps, eps);

	// compute the derivative of y w.r.t x using CppAD
	vector jac = f.Jacobian(x);

	// check the derivative using the formula
	// d/dX log(det(X)) = transpose( inv(X) )
	matrix inv_X = X.inverse();
	for(i = 0; i < size; i++)
	{	for(j = 0; j < size; j++)
			ok &= NearEqual(jac[i * size + j], inv_X(j, i), eps, eps);
	}

	return ok;
}

// END C++
