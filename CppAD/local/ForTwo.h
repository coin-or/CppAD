# ifndef CppADForTwoIncluded
# define CppADForTwoIncluded

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
$begin ForTwo$$
$spell
	typename
	Taylor
	const
$$


$index easy, second derivative$$
$index second, derivative easy$$
$index driver, easy second derivative$$
$index derivative, second easy$$

$section Subset of Second Partial Derivatives: Forward Mode Easy Driver$$

$table
$bold Syntax$$
$syntax%%dF% = %F%.ForTwo(%x%, %J%, %K%)%$$
$tend

$fend 25$$

$head Description$$
Given an $xref/ADFun//ADFun<Base>/$$ object 
$latex F : B^n \rightarrow B^m$$,
this routine computes a subset of
the second order partial derivatives of the form
$latex \[
	\DD{ F_i }{ x_j } { x_k } (x)
\] $$
where $italic i$$ ranges over all possible values
and specific values are chosen for 
$italic x$$, $italic j$$ and $italic k$$. 

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

$head VectorSize_t$$
The type $italic VectorSize_t$$ must have all the operations
specified by 
$syntax%
	%SimpleVector%<size_t>
%$$
where $italic SimpleVector$$ is a 
$xref/SimpleVector//Simple Vector/$$ template class. 
Note that the type $code size_t$$ must be equal to
$syntax%%VectorSize_t%::value_type%$$.

$head x$$
The vector $italic x$$ has prototype
$syntax%
	const %VectorBase% &%x% 
%$$
It must have length $italic n$$ and specifies
that point at which to evaluate the partial derivatives.

$head J$$
The vector $italic J$$ has prototype
$syntax%
	const %VectorSize_t% &%J%
%$$
We use $latex L$$ to denote the length of the vector $italic J$$.
For $latex \ell = 0 , \ldots , L-1$$, $latex J[ \ell ]  < n$$.

$head K$$
The vector $italic K$$ has prototype
$syntax%
	const %VectorSize_t% &%K%
%$$
It must have length $italic L$$ and 
for $latex \ell = 0 , \ldots , L-1$$,
$latex K[ \ell ]  < n$$.

$head dF$$
The vector $italic dF$$ has prototype
$syntax%
	%VectorBase% %dF% 
%$$
It must have length $syntax%%m% * %L%$$.
After the assignment to $italic dF$$, 
for $latex i = 0 , \ldots , m - 1 $$ and $latex \ell = 0 , \ldots , L - 1$$
$latex \[.
	dF[ i * L +  \ell  ] = \DD{ F_i }{ x_j }{ x_k } ( x )
\] $$
where $latex j = J[ \ell ]$$ and $latex k = K[ \ell ]$$.


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
	Example/ForTwo.cpp
%$$
The routine 
$xref/ForTwo.cpp//ForTwo/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase, typename VectorSize_t>
VectorBase ADFun<Base>::ForTwo(
	const VectorBase   &x, 
	const VectorSize_t &J,
	const VectorSize_t &K)
{	size_t i;
	size_t j;
	size_t k;
	size_t l;

	size_t m = Domain();
	size_t n = Range();
	size_t L = J.size();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		x.size() == m,
		"ForTwo: Length of x not equal domain dimension for F"
	); 
	CppADUsageError(
		J.size() == K.size(),
		"ForTwo: Lenght of the J and K vectors are not equal"
	);
	// point at which we are evaluating the second partials
	Forward(0, x);


	// dimension the return value
	VectorBase S(n * L);

	// space to hold diagonal Taylor coefficients
	VectorBase D(n * m);

	// boolean flag for which diagonal coefficients are computed
	CppAD::vector<bool> c(m);
	for(j = 0; j < m; j++)
		c[j] = false;

	// direction vector in argument space
	VectorBase u(m);
	for(j = 0; j < m; j++)
		u[j] = Base(0);

	// result vector in range space
	VectorBase v(n);

	// compute the diagonal coefficients that are needed
	for(l = 0; l < L; l++)
	{	j = J[l];
		CppADUsageError(
		j < m,
		"ForTwo: an element of J not less than domain dimension for F"
		);
		k = K[l];
		CppADUsageError(
		k < m,
		"ForTwo: an element of K not less than domain dimension for F"
		);
		size_t p;
		for(p = 0; p < 2; p++)
		{	if( ! c[j] )
			{	c[j] = true;
				u[j] = Base(1);
				Forward(1, u);
				u[j] = Base(0);
				v = Forward(2, u);
				for(i = 0; i < n; i++)
					D[i * m + j ] = v[i];
			} 
			j = k;
		}
	}
	// compute all the requested cross partials
	for(l = 0; l < L; l++)
	{	j = J[l];
		k = K[l];
		if( j == k )
		{	for(i = 0; i < n; i++)
				S[i * L + l] = Base(2) * D[i * m + j];
		}
		else
		{
			u[j] = Base(1);
			u[k] = Base(1);

			// cross term in j and k directions
			Forward(1, u);
			u[j] = Base(0);
			u[k] = Base(0);
			v = Forward(2, u);

			// place result in return value
			for(i = 0; i < n; i++)
				S[i * L + l] = v[i] - D[i*m+j] - D[i*m+k];

		}
	}

	return S;
}

} // END CppAD namespace

# endif
