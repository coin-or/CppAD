/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_ode$$
$spell
	retape
	bool
	CppAD
$$

$index link_ode$$
$index ode, speed test$$
$index speed, test ode$$
$index test, ode speed$$

$section Speed Testing Gradient of Ode Solution$$

$head Prototype$$
$codei%extern bool link_ode(
	size_t                 %size%      ,
	size_t                 %repeat%    ,
	bool                   %retape%    ,
	CppAD::vector<double> &%x%         ,
	CppAD::vector<double> &%gradient%
);
%$$

$head Purpose$$
Each $cref/package/speed_main/package/$$
must define a version of this routine as specified below.
This is used by the $cref/speed_main/$$ program
to run the corresponding speed and correctness tests.


$head f$$
The function 
$latex f : \R^n \rightarrow \R $$ that is define and computed by
evaluating $cref/ode_evaluate/$$.

$head Return Value$$
If this speed test is not yet
supported by a particular $icode package$$,
the corresponding return value for $code link_ode$$
should be $code false$$.

$head size$$
The argument $icode size$$ 
is the number of variables in the ordinary differential equation;
i.e., $latex n = size$$.

$head repeat$$
The argument $icode repeat$$ is the number of different functions
$latex f(x)$$ that the gradient is computed for.

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

$head x$$
The argument $icode x$$ is a vector with $latex n$$ elements.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector must change with each repetition.

$head gradient$$
The argument $icode gradient$$ is a vector with $latex n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the gradient of the function $latex f(x)$$
that corresponds to output values of $icode i$$, $icode j$$ and $icode x$$.
To be more specific, for
$latex j = 0 , \ldots , n-1$$,
$latex \[
	\D{f}{x[j]} (x) = gradient [ j ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode gradient$$
is modified and it is set to the function value.

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/near_equal.hpp>

// command line argument
extern bool main_retape;

extern bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	bool                       retape     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &gradient
);
bool available_ode(void)
{	size_t n      = 10;
	size_t repeat = 1;
	bool retape   = main_retape;
	CppAD::vector<double> x(n);
	CppAD::vector<double> gradient(n);

	return link_ode(n, repeat, retape, x, gradient);
}
bool correct_ode(bool is_package_double)
{	bool ok       = true;

	size_t n      = 5;
	size_t repeat = 1;
	CppAD::vector<double> x(n);
	CppAD::vector<double> gradient(n);

	bool retape   = main_retape;
	link_ode(n, repeat, retape, x, gradient);

	size_t m, size;
	if( is_package_double )
	{	m    = 0;  // check function value
		size = 1;
	}
	else
	{	m    = 1;  // check gradient value
		size = n;
	}
	CppAD::vector<double> check(size);
	CppAD::ode_evaluate(x, m, check);
	size_t k;
	for( k = 0; k < size; k++)
		ok &= CppAD::NearEqual(check[k], gradient[k], 1e-10, 1e-10);

	return ok;
}
void speed_ode(size_t n, size_t repeat)
{
	bool retape   = main_retape;
	CppAD::vector<double> x(n);
	CppAD::vector<double> gradient(n);
	link_ode(n, repeat, retape, x, gradient);
	return;
}
