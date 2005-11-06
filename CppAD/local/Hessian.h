# ifndef CppADHessianIncluded
# define CppADHessianIncluded

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
$begin Hessian$$
$spell
	typename
	Taylor
	Jacobian
	HesLuDet
	const
$$

$index Hessian$$
$index easy, Hessian$$
$index driver, Hessian$$

$section Hessian of One Component: Easy Driver$$

$table
$bold Syntax$$
$syntax%%H% = %F%.Hessian(%x%, %i%)%$$
$tend

$fend 25$$

$head Description$$
Given an $xref/ADFun//ADFun<Base>/$$ object $latex F : B^n \rightarrow B^m$$,
this routine returns the Hessian of $latex F_i$$ at the point 
$italic x$$; i.e.,
$latex \[
	H = F_i^{(2)} (x)
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
that point at which to evaluate the Hessian.

$head i$$
The index $italic i$$ has prototype
$syntax%
	size_t %i%
%$$
It must be between zero and $latex m-1$$
and specifies the component of $italic F$$
for which we are evaluating the Hessian.

$head H$$
The vector $italic H$$ has prototype
$syntax%
	%VectorBase% %H%
%$$
It must have length $syntax%%n% * %n%$$.
After the assignment to $italic H$$, 
for $latex j = 0 , \ldots , n - 1 $$ and $latex \ell = 0 , \ldots , n - 1$$
$latex \[
	H[ j * m + \ell ] = \DD{ F_i }{ x_j }{ x_\ell } ( x )
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
	Example/Hessian.cpp
%$$
The routine 
$xref/Hessian.cpp//Hessian/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.


$subhead Determinant Using Expansion by Minors$$
The routine 
$xref/HesMinorDet.cpp//HesMinorDet/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$subhead Determinant Using Lu Factorization$$
The routine 
$xref/HesLuDet.cpp//HesLuDet/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::Hessian(const VectorBase &x, size_t i)
{	size_t j;
	size_t k;
	size_t l;

	size_t m = Domain();
	size_t n = Range();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		x.size() == m,
		"Hessian: length of x not equal domain dimension for F"
	); 
	CppADUsageError(
		i < n,
		"Hessian: index i is not less than range dimension for F"
	);

	// point at which we are evaluating the Hessian
	Forward(0, x);

	// define the return value
	VectorBase H(m * m);

	// direction vector for calls to forward
	VectorBase u(m);
	for(j = 0; j < m; j++)
		u[j] = Base(0);

	// direction vector for calls to reverse
	VectorBase v(n);
	for(l = 0; l < n; l++)
		v[l] = Base(0);
	v[i] = Base(1);

	// location for return values from Reverse
	VectorBase r(m * 2);

	// loop over forward direstions
	for(j = 0; j < m; j++)
	{	// evaluate partials of entire function w.r.t. j-th coordinate
		u[j] = Base(1);
		Forward(1, u);
		u[j] = Base(0);

		// evaluate derivative of partial corresponding to F_i
		r = Reverse(2, v);

		// return desired components
		for(k = 0; k < m; k++)
			H[k * m + j] = r[k * 2 + 1];
	}
		
	return H;
}

} // END CppAD namespace

# endif
