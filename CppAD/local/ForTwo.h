# ifndef CppADForTwoIncluded
# define CppADForTwoIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
	ddy
	typename
	Taylor
	const
$$


$index partial, second order driver$$
$index second, order partial driver$$
$index driver, second order partial$$

$index easy, partial$$
$index driver, easy partial$$
$index partial, easy$$


$section Forward Mode Second Partial Derivative Driver$$

$head Syntax$$
$syntax%%ddy% = %f%.ForTwo(%x%, %j%, %k%)%$$


$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
The syntax above sets 
$latex \[
	ddy [ i * p + \ell ]
	=
	\DD{ F_i }{ x_{j[ \ell ]} }{ x_{k[ \ell ]} } (x) 
\] $$
for $latex i = 0 , \ldots , m-1$$
and $latex \ell = 0 , \ldots , p$$,
where $latex p$$ is the size of the vectors $italic j$$ and $italic k$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
Note that the $xref/ADFun/$$ object $italic f$$ is not $code const$$
(see $xref/ForTwo/ForTwo Uses Forward/ForTwo Uses Forward/$$ below).

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %VectorBase% &%x%
%$$
(see $xref/ForTwo/VectorBase/VectorBase/$$ below)
and its size 
must be equal to $italic n$$, the dimension of the
$xref/SeqProperty/Domain/domain/$$ space for $italic f$$.
It specifies
that point at which to evaluate the partial derivatives listed above.

$head j$$
The argument $italic j$$ has prototype
$syntax%
	const %VectorSize_t% &%j%
%$$
(see $xref/ForTwo/VectorSize_t/VectorSize_t/$$ below)
We use $italic p$$ to denote the size of the vector $italic j$$.
All of the indices in $italic j$$ 
must be less than $italic n$$; i.e.,
for $latex \ell = 0 , \ldots , p-1$$, $latex j[ \ell ]  < n$$.

$head k$$
The argument $italic k$$ has prototype
$syntax%
	const %VectorSize_t% &%k%
%$$
(see $xref/ForTwo/VectorSize_t/VectorSize_t/$$ below)
and its size must be equal to $italic p$$,
the size of the vector $italic j$$.
All of the indices in $italic k$$ 
must be less than $italic n$$; i.e.,
for $latex \ell = 0 , \ldots , p-1$$, $latex k[ \ell ]  < n$$.

$head ddy$$
The result $italic ddy$$ has prototype
$syntax%
	%VectorBase% %ddy%
%$$
(see $xref/ForTwo/VectorBase/VectorBase/$$ below)
and its size is $latex m * p$$.
It contains the requested partial derivatives; to be specific,
for $latex i = 0 , \ldots , m - 1 $$ 
and $latex \ell = 0 , \ldots , p - 1$$
$latex \[
	ddy [ i * p + \ell ]
	=
	\DD{ F_i }{ x_{j[ \ell ]} }{ x_{k[ \ell ]} } (x) 
\] $$

$head VectorBase$$
The type $italic VectorBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Base/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head VectorSize_t$$
The type $italic VectorSize_t$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type size_t/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head ForTwo Uses Forward$$
After each call to $xref/Forward/$$,
the object $italic f$$ contains the corresponding 
$xref/glossary/Taylor Coefficient/Taylor coefficients/$$.
After $code ForTwo$$,
the previous calls to $xref/Forward/$$ are undefined.


$head Examples$$
$children%
	Example/ForTwo.cpp
%$$
The routine 
$xref/ForTwo.cpp//ForTwo/$$ is both an example and test.
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
	const VectorSize_t &j,
	const VectorSize_t &k)
{	size_t i;
	size_t j1;
	size_t k1;
	size_t l;

	size_t n = Domain();
	size_t m = Range();
	size_t p = j.size();

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	// check VectorSize_t is Simple Vector class with size_t elements
	CheckSimpleVector<size_t, VectorSize_t>();

	CppADUsageError(
		x.size() == n,
		"ForTwo: Length of x not equal domain dimension for f."
	); 
	CppADUsageError(
		j.size() == k.size(),
		"ForTwo: Lenght of the j and k vectors are not equal."
	);
	// point at which we are evaluating the second partials
	Forward(0, x);


	// dimension the return value
	VectorBase ddy(m * p);

	// allocate memory to hold all possible diagonal Taylor coefficients
	// (for large sparse cases, this is not efficient)
	VectorBase D(m * n);

	// boolean flag for which diagonal coefficients are computed
	CppAD::vector<bool> c(n);
	for(j1 = 0; j1 < n; j1++)
		c[j1] = false;

	// direction vector in argument space
	VectorBase dx(n);
	for(j1 = 0; j1 < n; j1++)
		dx[j1] = Base(0);

	// result vector in range space
	VectorBase dy(m);

	// compute the diagonal coefficients that are needed
	for(l = 0; l < p; l++)
	{	j1 = j[l];
		k1 = k[l];
		CppADUsageError(
		j1 < n,
		"ForTwo: an element of j not less than domain dimension for f."
		);
		CppADUsageError(
		k1 < n,
		"ForTwo: an element of k not less than domain dimension for f."
		);
		size_t count = 2;
		while(count)
		{	count--;
			if( ! c[j1] )
			{	// diagonal term in j1 direction
				c[j1]  = true;
				dx[j1] = Base(1);
				Forward(1, dx);

				dx[j1] = Base(0);
				dy     = Forward(2, dx);
				for(i = 0; i < m; i++)
					D[i * n + j1 ] = dy[i];
			} 
			j1 = k1;
		}
	}
	// compute all the requested cross partials
	for(l = 0; l < p; l++)
	{	j1 = j[l];
		k1 = k[l];
		if( j1 == k1 )
		{	for(i = 0; i < m; i++)
				ddy[i * p + l] = Base(2) * D[i * n + j1];
		}
		else
		{
			// cross term in j1 and k1 directions
			dx[j1] = Base(1);
			dx[k1] = Base(1);
			Forward(1, dx);

			dx[j1] = Base(0);
			dx[k1] = Base(0);
			dy = Forward(2, dx);

			// place result in return value
			for(i = 0; i < m; i++)
				ddy[i * p + l] = dy[i] - D[i*n+j1] - D[i*n+k1];

		}
	}
	return ddy;
}

} // END CppAD namespace

# endif
