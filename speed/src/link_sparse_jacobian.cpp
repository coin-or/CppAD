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
$begin link_sparse_jacobian$$
$spell
	bool
	CppAD
	Jacobian
$$

$index link_sparse_jacobian$$
$index sparse, speed test$$
$index speed, test sparse$$
$index test, sparse speed$$

$section Speed Testing Sparse Jacobian$$

$head Prototype$$
$codei%extern bool link_sparse_jacobian(
	size_t                 %repeat%    ,
	size_t                 %m%         ,
	CppAD::vector<double> &%x%         ,
	CppAD::vector<size_t> &%row%       ,
	CppAD::vector<size_t> &%col%       , 
	CppAD::vector<double> &%jacobian%
);
%$$

$head f$$
Given a range space dimension $icode m$$
the row index vector $latex row$$, and column index vector $latex col$$,
a corresponding function $latex f : \B{R}^n \rightarrow \B{R}^m $$ 
is defined by $cref sparse_jac_fun$$.
The non-zero entries in the Jacobian of this function have the form
$latex \[
	\D{f[row[k]]}{x[col[k]]]}
\] $$
for some $latex k$$ between zero and $icode%K% = %row%.size()-1%$$.
All the other terms of the Jacobian are zero.

$head repeat$$
The argument $icode repeat$$ is the number of different functions
$latex f(x)$$ for which the Jacobian is computed for.
Each function corresponds to a randomly chosen index vectors, i.e.,
for each repetition a random choice is made for
$latex row[k]$$ and $latex col[k]$$ for $latex k = 0 , \ldots , K-1$$.

$head x$$
The argument $icode x$$ has prototype
$codei%
        CppAD::vector<double> &%x%
%$$
The size of the vector $icode x$$ determines $latex n$$ the domain space 
dimension for $latex f(x)$$.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector need not change with each repetition.

$head row$$
The size of the vector $icode row$$ defines the value $latex K$$.
The input value of the elements of $icode row$$ does not matter.
On output, it has been set the row index vector
for the last repetition.
All the elements of $icode row$$ must are between zero and $latex m-1$$.

$head col$$
The argument $icode col$$ is a vector with size $latex K$$.
The input value of its elements does not matter.
On output, it has been set the column index vector
for the last repetition.
All the elements of $icode col$$ must are between zero and $latex n-1$$.

$head jacobian$$
The argument $icode jacobian$$ is a vector with 
$latex m \times n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Jacobian of the function $latex f(x)$$
that corresponds to output values of 
$icode row$$, $icode col$$, and $icode x$$.
To be more specific, for
$latex i = 0 , \ldots , m - 1$$,
$latex j = 0 , \ldots , n-1$$,
$latex \[
	\D{f[i]}{x[j]} (x) = jacobian [ i * n + j ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first $latex m$$ 
elements of $icode jacobian$$ are used and they are set to
the value of $latex f(x)$$.

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/near_equal.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

extern bool link_sparse_jacobian(
	size_t                     repeat     ,
	size_t                     m          ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<size_t>      &row       ,
	CppAD::vector<size_t>      &col       , 
	CppAD::vector<double>      &jacobian
);
bool available_sparse_jacobian(void)
{	size_t n      = 10;
	size_t m      =  n;
	size_t repeat = 1;
	size_t K      = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> row(K), col(K); 
	CppAD::vector<double> jacobian(m * n);

	return link_sparse_jacobian(repeat, m, x, row, col, jacobian);
}
bool correct_sparse_jacobian(bool is_package_double)
{	size_t i, j;
	bool ok       = true;
	double eps    = 10. * CppAD::epsilon<double>();
	size_t n      = 10;
	size_t m      = n;
	size_t repeat = 1;
	size_t K      = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> row(K), col(K);
	CppAD::vector<double> jacobian(m * n);

	link_sparse_jacobian(repeat, m, x, row, col, jacobian);

	if( is_package_double)
	{	// check f(x)
		size_t order = 0;
		CppAD::vector<double> check(m);
		CppAD::sparse_jac_fun(m, x, row, col, order, check);
		for( i = 0; i < m; i++)
		{	double u = check[i];
			double v = jacobian[i];
			ok &= CppAD::NearEqual(u, v, eps, eps);
		}
		return ok;
	}
     // check f'(x) 
	size_t order = 1;
	size_t size  = m * n;
	CppAD::vector<double> check(size);
	CppAD::sparse_jac_fun(m, x, row, col, order, check);
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	double u = check[ i * n + j ];
			double v = jacobian[ i * n + j ];
			ok &= CppAD::NearEqual(u, v, eps, eps);
		}
	}
	return ok;
}
void speed_sparse_jacobian(size_t n, size_t repeat)
{	size_t m   = 3 * n;
	size_t K   = 10 * m;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> row(K), col(K);
	CppAD::vector<double> jacobian(m * n);

	// note that cppad/sparse_jacobian.cpp assumes that x.size()
	// is the size corresponding to this test
	link_sparse_jacobian(repeat, m, x, row, col, jacobian);
	return;
}
