# ifndef CppADDivOpIncluded
# define CppADDivOpIncluded

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
$begin ForDivvvOp$$
$spell
	Div
	Divpv
	Divvp
	Divvv
	Taylor
	const
	inline
	Op
$$

$index divide, forward operator$$
$index forward, divide operator$$
$index operator, divide forward$$
$index ForDiv$$

$section Forward Mode Division Operator$$

$head Syntax$$

$syntax%inline void ForDivvvOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%y%)%$$
$pre
$$
$syntax%inline void ForDivpvOp(size_t %d%,
	%Base% *%z%, const %Base% *%p%, const %Base% *%y%)%$$
$pre
$$
$syntax%inline void ForDivvpOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%p%)%$$


$head Description$$
Computes the $italic d$$ order Taylor coefficient for $latex Z$$ where
$table
Operation  $cnext Value  $rnext
Divvv       $cnext $latex Z = X / Y$$ $rnext
Divpv       $cnext $latex Z = P / Y$$ $rnext
Divvp       $cnext $latex Z = X / P$$ 
$tend

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.

$head y$$
The vector $italic y$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic Y$$.

$head p$$
The scalar $syntax%*%p%$$ contains the value of the parameter $italic P$$.


$head z$$
The vector $italic z$$ has length $latex d+1$$.
On input it contains the
$th d-1$$ order Taylor coefficient row vector for $italic Z$$.
On output it contains the
$th d$$ order Taylor coefficient row vector for $italic Z$$; i.e.,
$syntax%%z%[%d%]%$$ is set equal to the $th d$$ Taylor coefficient for
the function $italic Z$$.

$end
------------------------------------------------------------------------------
$begin RevDivvvOp$$
$spell
	Div
	Divpv
	Divvp
	Divvv
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$


$index divide, reverse operator$$
$index reverse, divide operator$$
$index operator, divide reverse$$
$index ForDiv$$

$section Reverse Mode Division Operator$$

$head Syntax$$

$syntax%inline void RevDivvvOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%, const %Base% *%y%,
	%Base% *%pz%, %Base% *%px%, %Base% *%py%)%$$
$pre
$$
$syntax%inline void RevDivpvOp(size_t %d%,
	const %Base% *%z%, const %Base% *%p%, const %Base% *%y%,
	%Base% *%pz%, %Base% *%py%)%$$
$pre
$$
$syntax%inline void RevDivvpOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%, const %Base% *%p%,
	const %Base% *%pz%, %Base% *%px%)%$$

$head Description$$
We are given the partial derivatives for a function
$latex G(z, x, y)$$ and we wish to compute the partial derivatives for
the function
$latex \[
	H(x, y) = G [ Z(x, y) , x , y ]
\]$$
where $latex Z(x, y)$$ is defined as the 
$th d$$ order Taylor coefficient row vector for $italic Z$$ as
a function of the corresponding vectors for 
$italic X$$ and $italic Y$$ where

$table
Operation  $cnext Value  $rnext
Divvv      $cnext $latex Z = X / Y$$ $rnext
Divvp      $cnext $latex Z = P / Y$$ $rnext
Divpv      $cnext $latex Z = X / P$$ 
$tend

Note that $italic Z$$ has been used both the original division 
function and for the corresponding mapping of Taylor coefficients.

$head z$$
The vector $italic z$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic Z$$.

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.

$head y$$
The vector $italic y$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic Y$$.

$head p$$
The scalar $syntax%*%p%$$ contains the value of the parameter $italic P$$.


$head On Input$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead py$$
The vector $italic py$$ has length $latex d+1$$ and 
$syntax%%py%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Y$$.

$head pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
$syntax%%pz%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Z$$.

$head On Output$$

$subhead px$$
If present,
the vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic H$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead py$$
If present,
the vector $italic py$$ has length $latex d+1$$ and 
$syntax%%py%[%j%]%$$ contains the partial for $italic H$$
with respect to the $th j$$ order Taylor coefficient for $italic Y$$.


$subhead pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
its contents are no longer specified value; i.e., it has been used
for work space.

$end
------------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

// --------------------------- Divvv -----------------------------------------

template <class Base>
inline void ForDivvvOp(size_t j, 
	Base *z, const Base *x, const Base *y)
{
	size_t k;

# if 0
// 05-01-08 removed divide by zero check for better use of CondExp
	CppADUsageError(
		y[0] != Base(0),
		"Attempt to divide by zero"
	);
# endif

	z[j] = x[j];
	for(k = 1; k <= j; k++)
		z[j] -= z[j-k] * y[k];
	z[j] /= y[0];
}

template <class Base>
inline void RevDivvvOp(size_t d, 
	const Base  *z, const Base *x, const Base *y,
	Base       *pz, Base      *px, Base      *py)
{	size_t k;

	// number of indices to access
	size_t j = d + 1;

# if 0
// 05-01-08 removed divide by zero check for better use of CondExp
	CppADUsageError(
		y[0] != Base(0),
		"Attempt to divide by zero"
	);
# endif

	while(j)
	{	--j;
		// scale partial w.r.t. z[j]
		pz[j] /= y[0];

		px[j] += pz[j];
		for(k = 1; k <= j; k++)
		{	pz[j-k] -= pz[j] * y[k];
			py[k]   -= pz[j] * z[j-k];
		}	
		py[0] -= pz[j] * z[j];
	}
}

// --------------------------- Divpv ----------------------------------------

template <class Base>
inline void ForDivpvOp(size_t j, 
	Base *z, const Base *p, const Base *y)
{	size_t k;

# if 0
// 05-01-08 removed divide by zero check for better use of CondExp
	CppADUsageError(
		y[0] != Base(0),
		"Attempt to divide by zero"
	);
# endif

	if( j == 0 )
		z[j] = (*p);
	else	z[j] = Base(0);
	for(k = 1; k <= j; k++)
		z[j] -= z[j-k] * y[k];
	z[j] /= y[0];

}

template <class Base>
inline void RevDivpvOp(size_t d, 
	const Base  *z, const Base *p, const Base *y,
	Base       *pz, Base      *py)
{	size_t k;

	// number of indices to access
	size_t j = d + 1;

# if 0
// 05-01-08 removed divide by zero check for better use of CondExp
	CppADUsageError(
		y[0] != Base(0),
		"Attempt to divide by zero"
	);
# endif

	while(j)
	{	--j;
		// scale partial w.r.t z[j]
		pz[j] /= y[0];

		for(k = 1; k <= j; k++)
		{	pz[j-k] -= pz[j] * y[k];
			py[k]   -= pz[j] * z[j-k];
		}	
		py[0] -= pz[j] * z[j];
	}
}

// --------------------------- Divvp ------------------------------------------

template <class Base>
inline void ForDivvpOp(size_t j, 
	Base *z, const Base *x, const Base *p)
{
	z[j] = x[j] / (*p);
}

template <class Base>
inline void RevDivvpOp(size_t d, 
	const Base  *z, const Base *x, const Base *p,
	const Base *pz, Base *px)
{
	// number of indices to access
	size_t j = d + 1;

# if 0
// 05-01-08 removed divide by zero check for better use of CondExp
	// should catch this case in zero order forward mode
	CppADUnknownError( (*p) != Base(0) );
# endif

	while(j)
	{	--j;
		px[j] += pz[j] / (*p);
	}
}

} // END CppAD namespace

# endif
