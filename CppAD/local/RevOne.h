# ifndef CppADRevOneIncluded
# define CppADRevOneIncluded

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
$begin RevOne$$
$spell
	Taylor
	const
$$


$index easy, derivative$$
$index driver, easy derivative$$
$index derivative, easy$$

$section Derivative of One Range Component: Reverse Mode Easy Driver$$

$table
$bold Syntax$$
$syntax%%dF% = %F%.RevOne(%x%, %i%)%$$
$tend

$fend 25$$

$head Description$$
Given an $xref/ADFun//ADFun<Base>/$$ object 
$latex F : B^n \rightarrow B^m$$,
this routine computes the partial derivatives
$latex \[
F_i^{(1)} (x) = \left[ 
	\D{ F_i }{ x_0 } (x) , \cdots , \D{ F_i }{ x_{n-1} } (x) 
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

$head i$$
The index $italic i$$ has prototype
$syntax%
	size_t %i%
%$$
It specifies the
component of $italic F$$ that we are computing the 
derivative of, $latex i  < m$$.

$head dF$$
The vector $italic dF$$ has prototype
$syntax%
	%VectorBase% %dF%
%$$
and must have length $syntax%%n%$$.
After the assignment to $italic dF$$, 
for $latex j = 0 , \ldots , n - 1 $$ 
$latex \[.
	dF[ j ] = \D{ F_i }{ x_j } ( x )
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
	Example/RevOne.cpp
%$$
The routine 
$xref/RevOne.cpp//RevOne/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::RevOne(
	const VectorBase   &x ,
	size_t              i )
{	size_t i1;

	size_t n = Domain();
	size_t m = Range();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		x.size() == n,
		"RevOne: Length of x not equal domain dimension for F"
	); 
	CppADUsageError(
		i < m,
		"RevOne: the index i is not less than range dimension for F"
	);

	// point at which we are evaluating the second partials
	Forward(0, x);

	// component which are are taking the derivative of
	VectorBase w(m);
	for(i1 = 0; i1 < m; i1++)
		w[i1] = 0.;
	w[i] = 1.;

	// dimension the return value
	VectorBase dF(n);

	// compute the return value
	dF = Reverse(1, w);

	return dF;
}

} // END CppAD namespace

# endif
