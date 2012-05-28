/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_sparse_hessian$$
$spell
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
	CppAD::vector<size_t> &%row%       ,
	CppAD::vector<size_t> &%col%       , 
	CppAD::vector<double> &%hessian%
);
%$$

$head f$$
Given a row index vector $latex row$$
and a second column vector $latex col$$,
the corresponding function 
$latex f : \B{R}^n \rightarrow \B{R} $$ 
is defined by $cref sparse_hes_fun$$.
The non-zero entries in the Hessian of this function have 
one of the following forms:
$latex \[
	\DD{f}{x[row[k]]}{x[row[k]]}
	\; , \;
	\DD{f}{x[row[k]]}{x[col[k]]}
	\; , \;
	\DD{f}{x[col[k]]}{x[row[k]]}
	\; , \;
	\DD{f}{x[col[k]]}{x[col[k]]}
\] $$
for some $latex k $$ between zero and $latex K-1 $$.
All the other terms of the Hessian are zero.

$head repeat$$
The argument $icode repeat$$ is the number of different functions
$latex f(x)$$ that the Hessian is computed for.
Each function corresponds to a randomly chosen index vectors, i.e.,
for each repetition a random choice is made for
$latex row[k]$$ and $latex col[k]$$ for $latex k = 0 , \ldots , K-1$$.

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

$head row$$
The size of the vector $icode row$$ defines the value $latex K$$.
The input value of the elements of $icode row$$ does not matter.
On output, it has been set to the row index vector
for the last repetition.
All the elements of $icode row$$ must are between zero and $latex n-1$$.

$head col$$
The argument $icode col$$ is a vector with size $latex K$$.
The input value of its elements does not matter.
On output, it has been set to the column index vector
for the last repetition.
All the elements of $icode col$$ must are between zero and $latex n-1$$.

$head hessian$$
The argument $icode hessian$$ is a vector with $latex n \times n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Hessian of the function $latex f(x)$$
that corresponds to output values of 
$icode row$$, $icode col$$, and $icode x$$.
To be more specific, for
$latex i = 0 , \ldots , n-1$$,
$latex j = 0 , \ldots , n-1$$,
$latex \[
	\DD{f}{x[i]}{x[j]} (x) = hessian [ i * n + j ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode hessian$$ is used and it is actually 
the value of $latex f(x)$$ (derivatives are not computed).

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/near_equal.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

// Link std::exp to spare_hessian_evaluate.hpp for doubles.
# include <math.h>

extern bool link_sparse_hessian(
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<size_t>      &row       ,
	CppAD::vector<size_t>      &col       , 
	CppAD::vector<double>      &hessian
);
bool available_sparse_hessian(void)
{	size_t n      = 10;
	size_t repeat = 1;
	size_t K      = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> row(K), col(K); 
	CppAD::vector<double> hessian(n * n);

	return link_sparse_hessian(repeat, x, row, col, hessian);
}
bool correct_sparse_hessian(bool is_package_double)
{	size_t n      = 10;
	size_t repeat = 1;
	size_t K      = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> row(K), col(K);
	CppAD::vector<double> hessian(n * n);

	link_sparse_hessian(repeat, x, row, col, hessian);

	size_t order, size;
	if( is_package_double)
	{	order = 0;  // check function value
		size  = 1;
	}
	else
	{	order = 2;     // check hessian value
		size  = n * n;
	}
	CppAD::vector<double> check(size);
	CppAD::sparse_hes_fun(x, row, col, order, check);
	bool ok = true;
	size_t k;
	for( k = 0; k < size; k++)
		ok &= CppAD::NearEqual(check[k], hessian[k], 1e-10, 1e-10);

	return ok;
}
void speed_sparse_hessian(size_t n, size_t repeat)
{
	size_t K   = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> row(K), col(K);
	CppAD::vector<double> hessian(n * n);

	// note that cppad/sparse_hessian.cpp assumes that x.size() == size
	link_sparse_hessian(repeat, x, row, col, hessian);
	return;
}
