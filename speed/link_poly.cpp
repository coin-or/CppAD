/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_poly$$
$spell
	retape
	poly
	bool
	CppAD
	ddp
$$

$index link_poly$$
$index polynomial, speed test$$
$index speed, test polynomial$$
$index test, polynomial speed$$

$section Speed Testing Second Derivative of a Polynomial$$

$head Prototype$$
$codei%extern bool link_poly(
	size_t                 %size%    , 
	size_t                 %repeat%  , 
	bool                   %retape%  ,
	CppAD::vector<double> &%a%       ,
	CppAD::vector<double> &%z%       ,
	CppAD::vector<double> &%ddp      
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
the corresponding return value for $code link_poly$$ 
should be $code false$$.

$head size$$
The argument $icode size$$ is the order of the polynomial
(the number of coefficients in the polynomial).

$head repeat$$
The argument $icode repeat$$ is the number of different argument values
that the second derivative (or just the polynomial) will be computed at. 

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

$head a$$
The argument $icode a$$ is a vector with $icode%size%*%size%$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the coefficients of the 
polynomial that is differentiated
($th i$$ element is coefficient of order $icode i$$).

$head z$$
The argument $icode z$$ is a vector with one element.
The input value of the element does not matter.
The output of its element is the polynomial argument value
were the last second derivative (or polynomial value) was computed.

$head ddp$$
The argument $icode ddp$$ is a vector with one element.
The input value of its element does not matter.
The output value of its element is the 
second derivative of the polynomial with respect to it's argument value.

$subhead double$$
In the case where $icode package$$ is $code double$$,
the output value of the element of $icode ddp$$ 
is the polynomial value (the second derivative is not computed).

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/poly.hpp>
# include <cppad/near_equal.hpp>

// command line argument
extern bool main_retape;

extern bool link_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	bool                       retape   ,
	CppAD::vector<double>      &a       ,
	CppAD::vector<double>      &z       ,
	CppAD::vector<double>      &ddp      
);
bool available_poly(void)
{	size_t size   = 10;
	size_t repeat = 1;
	bool   retape = main_retape;
	CppAD::vector<double>  a(size), z(1), ddp(1);

	return link_poly(size, repeat, retape, a, z, ddp);
}
bool correct_poly(bool is_package_double)
{	size_t size   = 10;
	size_t repeat = 1;
	bool   retape = main_retape;
	CppAD::vector<double>  a(size), z(1), ddp(1);

	link_poly(size, repeat, retape, a, z, ddp);

	size_t k;
	double check;
	if( is_package_double )
		k = 0;
	else	k = 2;
	check = CppAD::Poly(k, a, z[0]);

	bool ok = CppAD::NearEqual(check, ddp[0], 1e-10, 1e-10);
	return ok;
}
void speed_poly(size_t size, size_t repeat)
{	CppAD::vector<double>  a(size), z(1), ddp(1);
	bool   retape = main_retape;

	link_poly(size, repeat, retape, a, z, ddp);
	return;
}
