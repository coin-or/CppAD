# ifndef CPPAD_QUADRATIC_OK_INCLUDED
# define CPPAD_QUADRATIC_OK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin quadratic_ok$$
$spell
	cppad
	hpp
	fm
	CppAD
	namespace
	const
	bool
$$

$section Check Sparse Quadratic Function and Derivatives$$ 

$index quadratic_ok$$
$index quadratic, check$$
$index correct, quadratic$$
$index check, quadratic$$

$head Syntax$$
$syntax%# include <cppad/speed/quadratic_ok.hpp>
%$$
$syntax%%ok% = quadratic_ok(%n%, %i%, %j%, %x%, %m%, %fm%)%$$

$head Purpose$$
We define the function
$latex f : \R^n \rightarrow \R $$ defined by
$latex \[
        f(x) = \sum_{k=1}^\ell \exp ( x_{i[k]} x_{j[k]} )
\] $$
This routine can be used to check a method for computing
$latex f(x)$$, $latex f^{(1)} (x)$$, or $latex f^{(2)} (x)$$.


$head Inclusion$$
The template function $code quadratic_ok$$ is defined in the $code CppAD$$
namespace by including 
the file $code cppad/speed/quadratic_ok.hpp$$ 
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$cref/cppad.hpp/cppad/$$.

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$
and is the dimension of the argument space for the function $latex f$$.

$head i$$
The argument $italic i$$ has prototype
$syntax%
	 const CppAD::vector<size_t> &%i%
%$$
and is a vector with size $latex \ell$$.
It specifies one of the first 
index of $latex x$$ for each quadratic term in $latex f(x)$$.
All the elements of $italic i$$ must be between zero and $syntax%%n%-1%$$.

$head j$$
The argument $italic j$$ has prototype
$syntax%
	 const CppAD::vector<size_t> &%j%
%$$
and is a vector with size $latex \ell$$.
It specifies one of the second 
index of $latex x$$ for each quadratic term in $latex f(x)$$.
All the elements of $italic j$$ must be between zero and $syntax%%n%-1%$$.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const CppAD::vector<double> &%x%
%$$
and its size is $italic n$$.
It contains he argument value for which the derivative is being checked.
Note that if $italic m$$ is zero,
the derivative does not depend on $latex x$$ and you can
use any value for its elements.

$head m$$
The argument $italic m$$ has prototype
$syntax%
	size_t %m%
%$$
It specifies the order of the derivative of $latex f$$
that is being checked; i.e., $latex f^{(m)} (x)$$ is checked.


$head fm$$
The argument $italic fm$$ has prototype
$syntax%
	const CppAD::vector<double> &%fm%
%$$
and contains the value of $latex f^{(m)} (x)$$ that is being checked.

$subhead Function$$
If $italic m$$ is zero, $italic fm$$ has size one and
$syntax%%fm%[0]%$$ is the value of $latex f(x)$$.

$subhead Gradient$$
If $italic m$$ is one, $italic fm$$ has size $italic n$$ and 
for $latex j = 0 , \ldots , n-1$$
$latex \[
	\D{f}{x[j]} = fm [ j ]
\] $$

$subhead Hessian$$
If $italic m$$ is two, $italic fm$$ has size $syntax%%n% * %n%$$ and
for $latex k = 0 , \ldots , n-1$$,
$latex m = 0 , \ldots , n-1$$
$latex \[
	\DD{f}{x[k]}{x[m]} = fm [ k * n + m ]
\] $$


$head ok$$
The return value $italic ok$$ has prototype
$syntax%
	bool %ok%
%$$
It is true, if $italic hessian$$
passes the test and false otherwise.

$children%
	omh/quadratic_ok.omh
%$$

$head Source Code$$
The file 
$cref/quadratic_ok.hpp/$$
contains the source code for this template function.

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/near_equal.hpp>
namespace CppAD {
	inline bool quadratic_ok(
		const size_t                 n  ,
		const CppAD::vector<size_t> &i  , 
		const CppAD::vector<size_t> &j  , 
		const CppAD::vector<double> &x  ,
		const size_t                 m  ,
		const CppAD::vector<double> &fm )
	{	bool ok = true;
		using CppAD::NearEqual;

		size_t k, size = 1;
		for(k = 0; k < m; k++)
			size *= n;

		CppAD::vector<double> check(size);
		for(k = 0; k < size; k++)
			check[k] = 0.;

		size_t ell = i.size();
		for(k = 0; k < ell; k++)
		{	
			switch(m)
			{	case 0:
				check[0] += x[i[k]] * x[j[k]];
				break;

				case 1:
				check[i[k]] += x[j[k]];
				check[j[k]] += x[i[k]];
				break;

				case 2:
				check[i[k] * n + j[k]] += 1.;
				check[j[k] * n + i[k]] += 1.;
				break;
			}
		}
			
		for(k = 0; k < size; k++)
			ok &= NearEqual(fm[k], check[k], 1e-10, 1e-10);
		
		return ok;
	}
}
// END PROGRAM
# endif
