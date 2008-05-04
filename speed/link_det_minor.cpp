/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_det_minor$$
$spell
	retape
	det
	bool
	CppAD
$$

$index link_det_minor$$
$index det_minor, speed test$$
$index speed, test det_minor$$
$index test, det_minor speed$$

$section Speed Testing Gradient of Determinant by Minor Expansion$$

$head Prototype$$
$codei%extern bool link_det_minor(
	size_t                 %size%      , 
	size_t                 %repeat%    , 
	bool                   %retape%    ,
	CppAD::vector<double> &%matrix%    ,
	CppAD::vector<double> &%gradient% 
);
%$$

$head Purpose$$
Each $cref/package/speed_main/package/$$
must define a version of this routine as specified below.
This is used by the $cref/speed_main/$$ program 
to run the corresponding speed and correctness tests.

$head Return Value$$
If this speed test is not yet
supported by a particular $icode package$$,
the corresponding return value for $code link_det_minor$$ 
should be $code false$$.

$head size$$
The argument $icode size$$
is the number of rows and columns in the matrix.

$head repeat$$
The argument $icode repeat$$ is the number of different matrices
that the gradient (or determinant) is computed for.

$head retape$$

$subhead true$$
If $icode retape$$ is true, 
the operation sequence is considered to change for each repetition.
Thus an AD package can not use one recording of the 
operation sequence to compute the gradient for all of the repetitions.

$subhead false$$
If $icode retape$$ is false, 
the operation sequence is known to be the same for each repetition.
Thus an AD package may use one recording of the 
operation sequence to compute the gradient for all of the repetitions.

$head matrix$$
The argument $icode matrix$$ is a vector with 
$syntax%%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the last matrix that the
gradient (or determinant) is computed for.

$head gradient$$
The argument $icode gradient$$ is a vector with 
$syntax%%size%*%size%$$ elements.
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

# include <cppad/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>

// command line argument
extern bool main_retape;

extern bool link_det_minor(
	size_t                     size      , 
	size_t                     repeat    , 
	bool                       retape    ,
	CppAD::vector<double>      &matrix   ,
	CppAD::vector<double>      &gradient 
);

bool available_det_minor(void)
{	size_t size   = 3;
	size_t repeat = 1;
	bool   retape = main_retape;
	CppAD::vector<double> matrix(size * size);
	CppAD::vector<double> gradient(size * size);

	return link_det_minor(size, repeat, retape, matrix, gradient);
}
bool correct_det_minor(bool is_package_double)
{	size_t size   = 3;
	size_t repeat = 1;
	bool   retape = main_retape;
	CppAD::vector<double> matrix(size * size);
	CppAD::vector<double> gradient(size * size);

	link_det_minor(size, repeat, retape, matrix, gradient);
	bool ok = CppAD::det_grad_33(matrix, gradient);
	if( is_package_double )
		ok = CppAD::det_33(matrix, gradient);
	else	ok = CppAD::det_grad_33(matrix, gradient);
	return ok;
}
void speed_det_minor(size_t size, size_t repeat)
{	CppAD::vector<double> matrix(size * size);
	CppAD::vector<double> gradient(size * size);
	bool   retape = main_retape;

	link_det_minor(size, repeat, retape, matrix, gradient);
	return;
}
