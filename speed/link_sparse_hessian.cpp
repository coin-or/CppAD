/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_sparse_hessian$$
$spell
	retape
	bool
	CppAD
$$

$index link_sparse_hessian$$
$index sparse, speed test$$
$index speed, test sparse$$
$index test, sparse speed$$

$section Speed Testing Sparse Hessian$$

$head Prototype$$
$codei%extern bool link_sparse_hessian(
	size_t                 %repeat%    ,
	CppAD::vector<double> &%x%         ,
	CppAD::vector<size_t> &%i%         ,
	CppAD::vector<size_t> &%j%         , 
	CppAD::vector<double> &%hessian%
);
%$$

$head f$$
Given a first index vector $latex i$$ and a second index vector $latex j$$,
the corresponding function 
$latex f : \R^n \rightarrow \R $$ is defined by $cref/sparse_evaluate/$$
and the index vectors $icode i$$ and $icode j$$.
The only non-zero entries in the Hessian of this function have the form
$latex \[
	\DD{f}{x[[k]]}{x[j[k]]}
\] $$
for some $latex k $$ between zero and $latex \ell-1 $$.

$head repeat$$
The argument $icode repeat$$ is the number of different functions
$latex f(x)$$ that the Hessian is computed for.
Each function corresponds to a randomly chosen index vectors, i.e.,
for each repetition a random choice is made for
$latex i[k]$$ and $latex j[k]$$ for $latex k = 0 , \ldots , \ell-1$$.

$head retape$$
For this test, the operation sequence changes for each repetition.
Thus the argument $cref/retape/speed_main/retape/$$ is not present
because an AD package can not use one recording of the 
operation sequence to compute the gradient for all of the repetitions.

$head x$$
The argument $icode x$$ has prototype
$codei%
        CppAD::vector<double> &%x%
%$$
The size of the vector $icode x$$ determines 
and is equal to the value of $latex n$$.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector need not change with each repetition.

$head i$$
The size of the vector $icode i$$ determines and is equal to
the value of $latex \ell$$.
The input value of the elements of $icode i$$ does not matter.
On output, it has been set the first index vector
for the last repetition.
All the elements of $icode i$$ must are between zero and $latex n-1$$.

$head j$$
The argument $icode j$$ is a vector with size $latex \ell$$.
The input value of its elements does not matter.
On output, it has been set the second index vector
for the last repetition.
All the elements of $icode i$$ must are between zero and $latex n-1$$.

$head hessian$$
The argument $icode hessian$$ is a vector with $latex n \times n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Hessian of the function $latex f(x)$$
that corresponds to output values of $icode i$$, $icode j$$, and $icode x$$.
To be more specific, for
$latex k = 0 , \ldots , n-1$$,
$latex m = 0 , \ldots , n-1$$,
$latex \[
	\DD{f}{x[k]}{x[m]} (x) = hessian [ k * n + m ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode hessian$$ is used and it is actually 
the value of $latex f(x)$$ ($latex f^{(2)} (x)$$ is not computed).

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/speed/sparse_evaluate.hpp>
# include <cppad/near_equal.hpp>


extern bool link_sparse_hessian(
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<size_t>      &i         ,
	CppAD::vector<size_t>      &j         , 
	CppAD::vector<double>      &hessian
);
bool available_sparse_hessian(void)
{	size_t n      = 10;
	size_t repeat = 1;
	size_t ell    = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> i(ell), j(ell); 
	CppAD::vector<double> hessian(n * n);

	return link_sparse_hessian(repeat, x, i, j, hessian);
}
bool correct_sparse_hessian(bool is_package_double)
{	size_t n      = 10;
	size_t repeat = 1;
	size_t ell    = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> i(ell), j(ell);
	CppAD::vector<double> hessian(n * n);

	link_sparse_hessian(repeat, x, i, j, hessian);

	size_t m, size;
	if( is_package_double)
	{	m    = 0;  // check function value
		size = 1;
	}
	else
	{	m = 2;     // check hessian value
		size = n * n;
	}
	CppAD::vector<double> check(size);
	CppAD::sparse_evaluate(x, i, j, m, check);
	bool ok = true;
	size_t k;
	for( k = 0; k < size; k++)
		ok &= CppAD::NearEqual(check[k], hessian[k], 1e-10, 1e-10);

	return ok;
}
void speed_sparse_hessian(size_t n, size_t repeat)
{
	size_t ell = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> i(ell), j(ell);
	CppAD::vector<double> hessian(n * n);
	link_sparse_hessian(repeat, x, i, j, hessian);
	return;
}
