# ifndef CPPAD_HESSIAN_INCLUDED
# define CPPAD_HESSIAN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Hessian$$
$spell
	hes
	typename
	Taylor
	HesLuDet
	const
$$

$index Hessian, driver$$
$index second, derivative$$
$index driver, Hessian$$

$section Hessian: Easy Driver$$

$head Syntax$$
$syntax%%hes% = %f%.Hessian(%x%, %i%)%$$


$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
The syntax above sets $italic hes$$ to the
Hessian of $italic F_i$$ evaluated at $italic x$$; i.e.,
$latex \[
	hes = F^{(2)} (x)
\] $$

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
Note that the $xref/ADFun/$$ object $italic f$$ is not $code const$$
(see $xref/Hessian/Hessian Uses Forward/Hessian Uses Forward/$$ below).

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
(see $xref/Hessian/Vector/Vector/$$ below)
and its size 
must be equal to $italic n$$, the dimension of the
$xref/SeqProperty/Domain/domain/$$ space for $italic f$$.
It specifies
that point at which to evaluate the Hessian.

$head i$$
The index $italic i$$ has prototype
$syntax%
	size_t %i%
%$$
and is less than $italic m$$, the dimension of the
$xref/SeqProperty/Range/range/$$ space for $italic f$$.
It specifies the component of $italic F$$
for which we are evaluating the Hessian.

$head hes$$
The result $italic hes$$ has prototype
$syntax%
	%Vector% %hes%
%$$
(see $xref/Hessian/Vector/Vector/$$ below)
and its size is $latex n * n$$.
For $latex j = 0 , \ldots , n - 1 $$ 
and $latex \ell = 0 , \ldots , n - 1$$
$latex \[
	hes [ j * n + \ell ] = \DD{ F_i }{ x_j }{ x_\ell } ( x )
\] $$

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$italic Base$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Hessian Uses Forward$$
After each call to $xref/Forward/$$,
the object $italic f$$ contains the corresponding 
$xref/glossary/Taylor Coefficient/Taylor coefficients/$$.
After $code Hessian$$,
the previous calls to $xref/Forward/$$ are undefined.

$head Example$$
$children%
	example_/Hessian.cpp%
	example_/HesLagrangian.cpp
%$$
The routines 
$cref/Hessian.cpp/$$ and
$cref/HesLagrangian.cpp/$$
are examples and tests of $code Hessian$$.
They return $code true$$, if they succeed and $code false$$ otherwise.


$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename Vector>
Vector ADFun<Base>::Hessian(const Vector &x, size_t i)
{	size_t j;
	size_t k;
	size_t l;

	size_t n = Domain();
	size_t m = Range();

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, Vector>();

	CppADUsageError(
		x.size() == n,
		"Hessian: length of x not equal domain dimension for f"
	); 
	CppADUsageError(
		i < m,
		"Hessian: index i is not less than range dimension for f"
	);

	// point at which we are evaluating the Hessian
	Forward(0, x);

	// define the return value
	Vector hes(n * n);

	// direction vector for calls to forward
	Vector u(n);
	for(j = 0; j < n; j++)
		u[j] = Base(0);

	// direction vector for calls to reverse
	Vector w(m);
	for(l = 0; l < m; l++)
		w[l] = Base(0);
	w[i] = Base(1);

	// location for return values from Reverse
	Vector ddw(n * 2);

	// loop over forward direstions
	for(j = 0; j < n; j++)
	{	// evaluate partials of entire function w.r.t. j-th coordinate
		u[j] = Base(1);
		Forward(1, u);
		u[j] = Base(0);

		// evaluate derivative of partial corresponding to F_i
		ddw = Reverse(2, w);

		// return desired components
		for(k = 0; k < n; k++)
			hes[k * n + j] = ddw[k * 2 + 1];
	}
		
	return hes;
}

} // END CppAD namespace

# endif
