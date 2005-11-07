# ifndef CppADRevTwoIncluded
# define CppADRevTwoIncluded

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
$begin RevTwo$$
$spell
	Taylor
	const
$$

$index easy, second derivative$$
$index second, derivative easy$$
$index driver, easy second derivative$$
$index derivative, second easy$$

$section Subset of Second Partial Derivatives: Reverse Mode Easy Driver$$


$table
$bold Syntax$$ $cnext
$syntax%%dF% = %F%.RevTwo(%x%, %I%, %J%)%$$
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
where $italic k$$ ranges over all possible values
and specific values are chosen for 
$italic x$$, $italic i$$ and $italic j$$.

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

$head I$$
The vector $italic I$$ has prototype
$syntax%
	const %VectorSize_t% &%I%
%$$
We user $italic L$$ to denote the length of $italic I$$.
for $latex \ell = 0 , \ldots , L-1$$, $latex I[ \ell ]  < m$$.

$head J$$
The vector $italic J$$ has prototype
$syntax%
	const %VectorSize_t% &%J%
%$$
It must have length $italic L$$ and 
for $latex \ell = 0 , \ldots , L-1$$,
$latex J[ \ell ]  < n$$.

$head dF$$
The vector $italic dF$$ has prototype
$syntax%
	%VectorBase% %dF%
%$$
and must have length $syntax%%n% * %L%$$.
After the assignment to $italic dF$$, 
for $latex k = 0 , \ldots , n - 1 $$ and $latex \ell = 0 , \ldots , L - 1$$
$latex \[.
	dF[ k * L + \ell ] = \DD{ F_i }{ x_j }{ x_k } ( x )
\] $$
where $latex i = I[ \ell ]$$ and $latex j = J[ \ell ]$$.

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
	Example/RevTwo.cpp
%$$
The routine 
$xref/RevTwo.cpp//RevTwo/$$ is both an example and a test.
It returns $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase, typename VectorSize_t>
VectorBase ADFun<Base>::RevTwo(
	const VectorBase   &x,
	const VectorSize_t &I,
	const VectorSize_t &J)
{	size_t i;
	size_t j;
	size_t k;
	size_t l;

	size_t m = Domain();
	size_t n = Range();
	size_t L = I.size();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		x.size() == m,
		"RevTwo: Length of x not equal domain dimension for F"
	); 
	CppADUsageError(
		I.size() == J.size(),
		"RevTwo: Lenght of the I and J vectors are not equal"
	);
	// point at which we are evaluating the second partials
	Forward(0, x);

	// dimension the return value
	VectorBase S(m * L);

	// boolean flag for which forward components are computed
	VectorSize_t c(m);
	for(j = 0; j < m; j++)
		c[j] = false;

	// direction vector in argument space
	VectorBase u(m);
	for(j = 0; j < m; j++)
		u[j] = Base(0);

	// direction vector in range space
	VectorBase v(n);
	for(i = 0; i < n; i++)
		v[i] = Base(0);

	// place to hold the results of a reverse calculation
	VectorBase r(m * 2);

	// determine which forward directions need to be computed
	for(l = 0; l < L; l++)
	{	i = I[l];
		CppADUsageError(
		i < n,
		"RevTwo: an eleemnt of I not less than range diemnsion for F"
		);
		j = J[l];
		CppADUsageError(
		j < m,
		"RevTwo: an element of J not less than domain dimension for F"
		);
		c[j] = true;
	}

	// loop over all forward directions that need to be computed
	for(j = 0; j < L; j++) if( c[j] )
	{	// execute a forward mode for this component direction	
		u[j] = Base(1);
		Forward(1, u);
		u[j] = Base(0);

		// loop over all reverse computations that use this forward
		for(l = 0; l < L; l++) if( J[l] == j )
		{	i = I[l];
			// execute a reverse in this component direction
			v[i] = Base(1);
			r = Reverse(2, v);
			v[i] = Base(0);

			// place the reverse result in return value
			for(k = 0; k < m; k++)
				S[k * L + l] = r[k * 2 + 1];
		}
	}
	return S;
}

} // END CppAD namespace

# endif
