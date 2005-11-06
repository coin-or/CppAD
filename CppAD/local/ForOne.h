# ifndef CppADForOneIncluded
# define CppADForOneIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin ForOne$$
$spell
	typename
	Taylor
	const
$$


$index easy, partial$$
$index driver, easy partial$$
$index partial, easy$$

$section Partial w.r.t One Domain Component: Forward Mode Easy Driver$$

$table
$bold Syntax$$
$syntax%%dF% = %F%.ForOne(%x%, %j%)%$$
$tend

$fend 25$$

$head Description$$
Given an $xref/ADFun//ADFun<Base>/$$ object 
$latex F : B^n \rightarrow B^m$$,
this routine computes the partial derivative
$latex \[
\D{F}{ x_j } = \left[ 
	\D{ F_0 }{ x_j } (x) , \cdots , \D{ F_{m-1} }{ x_j } (x) 
\right]
\] $$

$head F$$
The object $italic F$$ has prototype
$syntax%
	ADFun<%Base%> %F%
%$$

$head VectorBase$$
The type $italic VectorBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Base/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head x$$
The vector $italic x$$ has prototype
$syntax%
	const %VectorBase% &%x%
%$$
It must have length $italic n$$ and specifies
that point at which to evaluate the partial derivatives.

$head j$$
Is the argument has prototype
$syntax%
	size_t %j%
%$$
It is the component for which we are computing the partial
derivatives, $latex j  < n$$.

$head dF$$
The vector $italic dF$$ has prototype
$syntax%
	%VectorBase% %dF%
%$$
It must have length $syntax%%m%$$.
After the assignment to $italic dF$$, 
for $latex i = 0 , \ldots , m - 1 $$ 
$latex \[.
	dF[ i ] = \D{ F_i }{ x_j } ( x )
\] $$

$head Forward$$
The object $italic F$$ stores information related to previous
calls to $syntax%%F%.Forward%$$.
After this operation,
the previous calls to $xref/Forward/$$ are undefined
with the exception of the zero order call which has the form
$syntax%
	%F%.Forward(0, %x%)
%$$.

$head Examples$$
$children%
	Example/ForOne.cpp
%$$
The routine 
$xref/ForOne.cpp//ForOne/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::ForOne(const VectorBase &x, size_t j)
{	size_t j1;

	size_t n = Domain();
	size_t m = Range();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		x.size() == n,
		"ForOne: Length of x not equal domain dimension for F"
	); 
	CppADUsageError(
		j < n,
		"ForOne: the index j is not less than domain dimension for F"
	);

	// point at which we are evaluating the second partials
	Forward(0, x);

	// direction in which are are taking the derivative
	VectorBase dx(n);
	for(j1 = 0; j1 < n; j1++)
		dx[j1] = 0.;
	dx[j] = 1.;

	// dimension the return value
	VectorBase dF(m);

	// compute the return value
	dF = Forward(1, dx);

	return dF;
}

} // END CppAD namespace

# endif
