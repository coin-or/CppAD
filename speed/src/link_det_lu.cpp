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
$begin link_det_lu$$
$spell
	det_lu
	bool
	CppAD
$$


$section Speed Testing Gradient of Determinant Using Lu Factorization$$
$mindex link_det_lu test$$

$head Prototype$$
$codei%extern bool link_det_lu(
	size_t                 %size%      ,
	size_t                 %repeat%    ,
	CppAD::vector<double> &%matrix%    ,
	CppAD::vector<double> &%gradient%
);
%$$

$head Purpose$$
Each $cref/package/speed_main/package/$$
must define a version of this routine as specified below.
This is used by the $cref speed_main$$ program
to run the corresponding speed and correctness tests.

$head Method$$
The same template routine $cref det_by_lu$$ is used
by the different AD packages.

$head Return Value$$
If this speed test is not yet
supported by a particular $icode package$$,
the corresponding return value for $code link_det_lu$$
should be $code false$$.

$head size$$
The argument $icode size$$
is the number of rows and columns in the matrix.

$head repeat$$
The argument $icode repeat$$ is the number of different matrices
that the gradient (or determinant) is computed for.

$head matrix$$
The argument $icode matrix$$ is a vector with $icode%size%*%size%$$ elements.
The input value of its elements does not matter.
The output value of its elements is the last matrix that the
gradient (or determinant) is computed for.

$head gradient$$
The argument $icode gradient$$ is a vector with $icode%size%*%size%$$ elements.
The input value of its elements does not matter.
The output value of its elements is the gradient of the
determinant of $icode matrix$$ with respect to its elements.

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode gradient$$ is used and it is actually
the determinant value (the gradient value is not computed).

$end
-----------------------------------------------------------------------------
*/

# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>

extern bool link_det_lu(
	size_t                     size      ,
	size_t                     repeat    ,
	CppAD::vector<double>      &matrix   ,
	CppAD::vector<double>      &gradient
);


bool available_det_lu(void)
{	size_t size   = 3;
	size_t repeat = 1;
	CppAD::vector<double> matrix(size * size);
	CppAD::vector<double> gradient(size * size);

	return link_det_lu(size, repeat, matrix, gradient);
}
bool correct_det_lu(bool is_package_double)
{	size_t size   = 3;
	size_t repeat = 1;
	CppAD::vector<double> matrix(size * size);
	CppAD::vector<double> gradient(size * size);

	link_det_lu(size, repeat, matrix, gradient);
	bool ok;
	if( is_package_double )
		ok = CppAD::det_33(matrix, gradient);
	else	ok = CppAD::det_grad_33(matrix, gradient);
	return ok;
}
void speed_det_lu(size_t size, size_t repeat)
{	// free statically allocated memory
	if( size == 0 && repeat == 0 )
		return;
	//
	CppAD::vector<double> matrix(size * size);
	CppAD::vector<double> gradient(size * size);

	link_det_lu(size, repeat, matrix, gradient);
	return;
}
