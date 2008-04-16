/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_quadratic$$
$spell
	bool
	CppAD
$$

$index sparse, speed test$$
$index speed, test sparse$$
$index test, sparse speed$$

$section Speed Testing Sparse Hessian of a Quadratic Function$$

$head Prototype$$
$codei%extern bool compute_quadratic(
	size_t                 %size%      , 
	size_t                 %repeat%    ,
	size_t                 %ell%       ,
	CppAD::vector<size_t> &%i%         ,
	CppAD::vector<size_t> &%j%         , 
	CppAD::vector<double> &%hessian%
);
%$$

$head f$$
Given a first index vector $latex i$$ and a second index vector $latex j$$,
there is a corresponding function 
$latex f : \R^n \rightarrow \R $$ defined by
$latex \[
	f(x) = \sum_{k=0}^{\ell-1} x_{i[k]} x_{j[k]}
\] $$

$head size$$
The argument $icode size$$
is the dimension of the argument space for the function $latex f$$; 
i.e. $icode size$$ determines the value of $latex n$$.

$head repeat$$
The argument $icode repeat$$ is the number of different functions
$latex f(x)$$ that the Hessian is computed for.
Each function corresponds to a randomly chosen index vectors, i.e.,
for each repetition a random choice is made for
$latex i[k]$$ and $latex j[k]$$ for $latex k = 0 , \ldots , \ell-1$$.

$subhead Operation Sequence$$
For this test, only the operation sequence changes for each repetition.
Thus an AD package can not use one recording of the 
operation sequence to compute the gradient for all of the repetitions.

$head ell$$
The argument $icode ell$$ 
is the number of terms in the summation that defines $latex f(x)$$, i.e.,
it determines the value of $latex \ell$$.

$head i$$
The argument $icode i$$ is a vector of size $icode ell$$.
The input value of its elements does not matter.
On output, it has been set the first index vector
for the last repetition.
All the elements of $icode i$$ must are between zero and $latex \ell-1$$.

$head j$$
The argument $icode j$$ is a vector of size $icode ell$$.
The input value of its elements does not matter.
On output, it has been set the second index vector
for the last repetition.
All the elements of $icode i$$ must are between zero and $latex \ell-1$$.


$head hessian$$
The argument $icode hessian$$ is a vector with $latex n \times n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Hessian of the function $latex f(x)$$
that corresponds to output values of $icode i$$ and $icode j$$.
To be more specific, for
$latex k = 0 , \ldots , n-1$$,
$latex m = 0 , \ldots , n-1$$,
$latex \[
	\DD{f}{x[k]}{x[m]} = hessian [ k * n + m ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode hessian$$ is used and it is actually 
the value of $latex f(x)$$ ($latex f^{(2)} (x)$$ is not computed).

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/speed/quadratic_ok.hpp>
# include <cassert>


extern bool compute_quadratic(
	size_t                     size       , 
	size_t                     repeat     ,
	size_t                      ell       ,
	CppAD::vector<size_t>      &i         ,
	CppAD::vector<size_t>      &j         , 
	CppAD::vector<double>      &hessian
);
bool available_quadratic(void)
{	size_t size   = 10;
	size_t repeat = 1;
	size_t ell    = 3 * size;
	CppAD::vector<size_t> i(ell), j(ell); 
	CppAD::vector<double> hessian(size * size);

	return compute_quadratic(
		size, repeat, ell, i, j, hessian
	);
}
bool correct_quadratic(bool is_package_double)
{	size_t size   = 10;
	size_t repeat = 1;
	size_t ell    = 3 * size;
	CppAD::vector<size_t> i(ell), j(ell);
	CppAD::vector<double> x(size); // values do not matter
	CppAD::vector<double> hessian(size * size);

	// this case not yet implemented
	if( is_package_double )
		assert(0);

	compute_quadratic(size, repeat, ell, i, j, hessian);
	size_t m = 2; // order of derivative
	bool ok = CppAD::quadratic_ok(size, i, j, x, m, hessian);
	return ok;
}
void speed_quadratic(size_t size, size_t repeat)
{	CppAD::vector<double> hessian(size * size);

	size_t ell = 3 * size;
	CppAD::vector<size_t> i(ell), j(ell);
	compute_quadratic(size, repeat, ell, i, j, hessian);
	return;
}
