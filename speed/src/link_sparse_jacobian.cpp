/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

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
	CppAD::vector<double> &%x%         ,
	CppAD::vector<size_t> &%i%         ,
	CppAD::vector<size_t> &%j%         , 
	CppAD::vector<double> &%jacobian%
);
%$$

$head f$$
Given a first index vector $latex i$$ and a second index vector $latex j$$,
the corresponding function 
$latex g : \R^n \rightarrow \R^\ell $$ is defined by $cref/sparse_evaluate/$$
and the index vectors $icode i$$ and $icode j$$.
The function $latex g(x)$$ is defined by
$latex \[
	g_k (x) = \D{f}{x_{i[k]}}
\] $$
The non-zero entries in the Jacobian of this function have the form
$latex \[
	\D{g_k}{x[j[k]]}
\] $$
or
$latex \[
	\D{g_k}{x[i[k]]}
\] $$
for some $latex k$$ between zero and $latex \ell-1 $$.
All the other terms of the Jacobian are zero.

$head repeat$$
The argument $icode repeat$$ is the number of different functions
$latex g(x)$$ that the Jacobian is computed for.
Each function corresponds to a randomly chosen index vectors, i.e.,
for each repetition a random choice is made for
$latex i[k]$$ and $latex j[k]$$ for $latex k = 0 , \ldots , \ell-1$$.

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

$head jacobian$$
The argument $icode jacobian$$ is a vector with $latex \ell \times n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Jacobian of the function $latex g(x)$$
that corresponds to output values of $icode i$$, $icode j$$, and $icode x$$.
To be more specific, for
$latex k = 0 , \ldots , \ell - 1$$,
$latex m = 0 , \ldots , n-1$$,
$latex \[
	\D{g_k}{x[m]} (x) = jacobian [ k * \ell + m ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first $latex \ell$$ 
elements of $icode jacobian$$ are used and they are set to
the value of $latex g(x)$$ ($latex g'(x)$$ is not computed).

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/speed/sparse_evaluate.hpp>
# include <cppad/near_equal.hpp>


extern bool link_sparse_jacobian(
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<size_t>      &i         ,
	CppAD::vector<size_t>      &j         , 
	CppAD::vector<double>      &jacobian
);
bool available_sparse_jacobian(void)
{	size_t n      = 10;
	size_t repeat = 1;
	size_t ell    = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> i(ell), j(ell); 
	CppAD::vector<double> jacobian(ell * n);

	return link_sparse_jacobian(repeat, x, i, j, jacobian);
}
bool correct_sparse_jacobian(bool is_package_double)
{	size_t n      = 10;
	size_t repeat = 1;
	size_t ell    = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> i(ell), j(ell);
	CppAD::vector<double> jacobian(ell * n);

	link_sparse_jacobian(repeat, x, i, j, jacobian);

	size_t order, size, m;
	if( is_package_double)
	{	order = 1;     // check g(x) using f'(x)
		size  = n;
	}
	else
	{	order = 2;     // check g'(x) using f''(x)
		size  = n * n;
	}
	CppAD::vector<double> check(size);
	CppAD::sparse_evaluate(x, i, j, order, check);
	bool ok = true;
	size_t k;
	if( is_package_double )
	{	for( k = 0; k < ell; k++)
		{	double u = check[ i[k] ];
			double v = jacobian[k];
			ok &= CppAD::NearEqual(u, v, 1e-10, 1e-10);
		}
		return ok;
	}
	for(k = 0; k < ell; k++)
	{	for(m = 0; m < n; m++)
		{	double u = check[ i[k] * n + m ];
			double v = jacobian[ k * n + m ];
			ok &= CppAD::NearEqual(u, v, 1e-10, 1e-10);
		}
	}
	return ok;
}
void speed_sparse_jacobian(size_t n, size_t repeat)
{
	size_t ell = 3 * n;
	CppAD::vector<double> x(n);
	CppAD::vector<size_t> i(ell), j(ell);
	CppAD::vector<double> jacobian(ell * n);

	// note that cppad/sparse_jacobian.cpp assumes that x.size() == size
	link_sparse_jacobian(repeat, x, i, j, jacobian);
	return;
}
