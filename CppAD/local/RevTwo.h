# ifndef CppADRevTwoIncluded
# define CppADRevTwoIncluded

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
$begin RevTwo$$
$spell
	ddw
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


$section Reverse Mode Second Partial Derivative Driver$$

$head Syntax$$
$syntax%%ddw% = %f%.RevTwo(%x%, %i%, %j%)%$$


$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
The syntax above sets 
$latex \[
	ddw [ k * p + \ell ]
	=
	\DD{ F_{i[ \ell ]} }{ x_{j[ \ell ]} }{ x_k } (x) 
\] $$
for $latex k = 0 , \ldots , n-1$$
and $latex \ell = 0 , \ldots , p$$,
where $latex p$$ is the size of the vectors $italic i$$ and $italic j$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
Note that the $xref/ADFun/$$ object $italic f$$ is not $code const$$
(see $xref/RevTwo/RevTwo Uses Forward/RevTwo Uses Forward/$$ below).

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %VectorBase% &%x%
%$$
(see $xref/RevTwo/VectorBase/VectorBase/$$ below)
and its size 
must be equal to $italic n$$, the dimension of the
$xref/SeqProperty/Domain/domain/$$ space for $italic f$$.
It specifies
that point at which to evaluate the partial derivatives listed above.

$head i$$
The argument $italic i$$ has prototype
$syntax%
	const %VectorSize_t% &%i%
%$$
(see $xref/RevTwo/VectorSize_t/VectorSize_t/$$ below)
We use $italic p$$ to denote the size of the vector $italic i$$.
All of the indices in $italic i$$ 
must be less than $italic m$$, the dimension of the
$xref/SeqProperty/Range/range/$$ space for $italic f$$; i.e.,
for $latex \ell = 0 , \ldots , p-1$$, $latex i[ \ell ]  < m$$.

$head j$$
The argument $italic j$$ has prototype
$syntax%
	const %VectorSize_t% &%j%
%$$
(see $xref/RevTwo/VectorSize_t/VectorSize_t/$$ below)
and its size must be equal to $italic p$$,
the size of the vector $italic i$$.
All of the indices in $italic j$$ 
must be less than $italic n$$; i.e.,
for $latex \ell = 0 , \ldots , p-1$$, $latex j[ \ell ]  < n$$.

$head ddw$$
The result $italic ddw$$ has prototype
$syntax%
	%VectorBase% %ddw%
%$$
(see $xref/RevTwo/VectorBase/VectorBase/$$ below)
and its size is $latex n * p$$.
It contains the requested partial derivatives; to be specific,
for $latex k = 0 , \ldots , n - 1 $$ 
and $latex \ell = 0 , \ldots , p - 1$$
$latex \[
	ddw [ k * p + \ell ]
	=
	\DD{ F_{i[ \ell ]} }{ x_{j[ \ell ]} }{ x_k } (x) 
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

$head RevTwo Uses Forward$$
After each call to $xref/Forward/$$,
the object $italic f$$ contains the corresponding 
$xref/glossary/Taylor Coefficient/Taylor coefficients/$$.
After $code RevTwo$$,
the previous calls to $xref/Forward/$$ are undefined.

$head Examples$$
$children%
	Example/RevTwo.cpp
%$$
The routine 
$xref/RevTwo.cpp//RevTwo/$$ is both an example and test.
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
	const VectorSize_t &i,
	const VectorSize_t &j)
{	size_t i1;
	size_t j1;
	size_t k;
	size_t l;

	size_t n = Domain();
	size_t m = Range();
	size_t p = i.size();

	// check VectorBase is Simple Vector class with Base elements
	CheckSimpleVector<Base, VectorBase>();

	// check VectorSize_t is Simple Vector class with size_t elements
	CheckSimpleVector<size_t, VectorSize_t>();

	CppADUsageError(
		x.size() == n,
		"RevTwo: Length of x not equal domain dimension for f."
	); 
	CppADUsageError(
		i.size() == j.size(),
		"RevTwo: Lenght of the i and j vectors are not equal."
	);
	// point at which we are evaluating the second partials
	Forward(0, x);

	// dimension the return value
	VectorBase ddw(n * p);

	// direction vector in argument space
	VectorBase dx(n);
	for(j1 = 0; j1 < n; j1++)
		dx[j1] = Base(0);

	// direction vector in range space
	VectorBase w(m);
	for(i1 = 0; i1 < m; i1++)
		w[i1] = Base(0);

	// place to hold the results of a reverse calculation
	VectorBase r(n * 2);

	// check the indices in i and j
	for(l = 0; l < p; l++)
	{	i1 = i[l];
		j1 = j[l];
		CppADUsageError(
		i1 < m,
		"RevTwo: an eleemnt of i not less than range dimension for f."
		);
		CppADUsageError(
		j1 < n,
		"RevTwo: an element of j not less than domain dimension for f."
		);
	}

	// loop over all forward directions 
	for(j1 = 0; j1 < n; j1++) 
	{	// first order forward mode calculation done 
		bool first_done = false;
		for(l = 0; l < p; l++) if( j[l] == j1 )
		{	if( ! first_done )
			{	first_done = true;

				// first order forward mode in j1 direction
				dx[j1] = Base(1);
				Forward(1, dx);
				dx[j1] = Base(0);
			}
			// execute a reverse in this component direction
			i1    = i[l];
			w[i1] = Base(1);
			r     = Reverse(2, w);
			w[i1] = Base(0);

			// place the reverse result in return value
			for(k = 0; k < n; k++)
				ddw[k * p + l] = r[k * 2 + 1];
		}
	}
	return ddw;
}

} // END CppAD namespace

# endif
