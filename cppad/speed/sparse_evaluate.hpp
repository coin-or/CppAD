# ifndef CPPAD_SPARSE_EVALUATE_INCLUDED
# define CPPAD_SPARSE_EVALUATE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sparse_evaluate$$
$spell
	cppad
	hpp
	fm
	CppAD
	namespace
	const
	bool
	exp
	arg
$$

$section Evaluate a Function That Has a Sparse Hessian$$ 

$index sparse_evaluate, function$$
$index function, sparse_evaluate$$

$head Syntax$$
$codei%# include <cppad/speed/sparse_evaluate.hpp>
%$$
$codei%sparse_evaluate(%x%, %i%, %j%, %m%, %fm%)%$$

$head Purpose$$
This routine evaluates
$latex f(x)$$, $latex f^{(1)} (x)$$, or $latex f^{(2)} (x)$$
where the Hessian $latex f^{(2)} (x)$$ is sparse.
The function $latex f : \R^n \rightarrow \R$$ depends on the 
index vectors $icode i$$ and $icode j$$. 
The only non-zero entries in the Hessian of this function have the form
\[
	\DD{f}{x[k]]}{x[j[k]}
\]
for some \( k \) between zero and \( \ell-1 \).

$head Inclusion$$
The template function $code sparse_evaluate$$ 
is defined in the $code CppAD$$ namespace by including 
the file $code cppad/speed/sparse_evaluate.hpp$$ 
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$cref/cppad.hpp/cppad/$$.

$head Float$$
The type $icode Float$$ must be a $cref/NumericType/$$.
In addition, if $icode y$$ and $icode z$$ are $icode Float$$ objects,
$codei%
	%y% = exp(%z%)
%$$ 
must set the $icode y$$ equal the exponential of $icode z$$, i.e.,
the derivative of $icode y$$ with respect to $icode z$$ is equal to $icode y$$.

$head x$$
The argument $icode x$$ has prototype
$codei%
	const CppAD::vector<%Float%> &%x%
%$$
It contains he argument value for which the function,
or its derivative, is being evaluated.
We use $latex n$$ to denote the size of the vector $icode x$$.

$head i$$
The argument $icode i$$ has prototype
$codei%
	 const CppAD::vector<size_t> &%i%
%$$
It specifies one of the first 
index of $latex x$$ for each non-zero Hessian term
(see $cref/purpose/sparse_evaluate/Purpose/$$ above).
All the elements of $icode i$$ must be between zero and $icode%n%-1%$$.
We use $latex \ell$$ to denote the size of the vector $icode i$$.

$head j$$
The argument $icode j$$ has prototype
$codei%
	 const CppAD::vector<size_t> &%j%
%$$
and is a vector with size $latex \ell$$.
It specifies one of the second 
index of $latex x$$ for each non-zero Hessian term.
All the elements of $icode j$$ must be between zero and $icode%n%-1%$$.

$head m$$
The argument $italic m$$ has prototype
$icode%
	size_t %m%
%$$
It is between zero and two and
specifies the order of the derivative of $latex f$$
that is being evaluated, i.e., $latex f^{(m)} (x)$$ is evaluated.

$head fm$$
The argument $italic fm$$ has prototype
$icode%
	CppAD::vector<%Float%> &%fm%
%$$
The input value of the elements of $icode fm$$ does not matter.

$subhead Function$$
If $icode m$$ is zero, $italic fm$$ has size one and
$syntax%%fm%[0]%$$ is the value of $latex f(x)$$.

$subhead Gradient$$
If $icode m$$ is one, $italic fm$$ has size $italic n$$ and 
for $latex j = 0 , \ldots , n-1$$
$latex \[
	\D{f}{x[j]} = fm [ j ]
\] $$

$subhead Hessian$$
If $icode m$$ is two, $italic fm$$ has size $syntax%%n% * %n%$$ and
for $latex k = 0 , \ldots , n-1$$,
$latex m = 0 , \ldots , n-1$$
$latex \[
	\DD{f}{x[k]}{x[m]} = fm [ k * n + m ]
\] $$

$children%
	speed/example/sparse_evaluate.cpp%
	omh/sparse_evaluate.omh
%$$

$head Example$$
The file
$cref/sparse_evaluate.cpp/$$
contains an example and test  of $code sparse_evaluate.hpp$$.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The file 
$cref/sparse_evaluate.hpp/$$
contains the source code for this template function.

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/local/cppad_assert.hpp>
# include <cppad/check_numeric_type.hpp>
# include <cppad/vector.hpp>

namespace CppAD {
	template <class Float>
	void sparse_evaluate(
		const CppAD::vector<Float>  &x  ,
		const CppAD::vector<size_t> &i  , 
		const CppAD::vector<size_t> &j  , 
		size_t                       m  ,
		CppAD::vector<Float>       &fm  )
	{
		// check numeric type specifications
		CheckNumericType<Float>();

		size_t k;
		size_t n    = x.size();
		size_t size = 1;
		for(k = 0; k < m; k++)
			size *= n;
		CPPAD_ASSERT_KNOWN(
			fm.size() == size,
			"sparse_evaluate: size of fm not equal n^m"
		);
		for(k = 0; k < size; k++)
			fm[k] = Float(0);

		size_t ell = i.size();
		Float t;
		Float dt_i;
		Float dt_j;
		for(k = 0; k < ell; k++)
		{	t    = exp( x[i[k]] * x[j[k]] );	
			dt_i = t * x[j[k]];
			dt_j = t * x[i[k]];
			switch(m)
			{	case 0:
				fm[0] += t;
				break;

				case 1:
				fm[i[k]] += dt_i;
				fm[j[k]] += dt_j;
				break;

				case 2:
				fm[i[k] * n + i[k]] += dt_i * x[j[k]];
				fm[i[k] * n + j[k]] += t + dt_j * x[j[k]];
				fm[j[k] * n + i[k]] += t + dt_i * x[i[k]];
				fm[j[k] * n + j[k]] += dt_j * x[i[k]];
				break;
			}
		}
			
	}
}
// END PROGRAM
# endif
