# ifndef CPPAD_SQRT_OP_INCLUDED
# define CPPAD_SQRT_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForSqrtOp$$ $comment CppAD Developer Documentation$$
$spell
	Sqrt
	Taylor
	const
	inline
	Op
$$

$index forward, sqrt$$
$index sqrt, forward$$
$index ForSqrtOp$$

$section Forward Mode Square Root Function$$

$head Syntax$$

$syntax%inline void ForSqrtOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%)%$$

$head Description$$
Computes the $italic d$$ order Taylor coefficient for $latex Z$$ where
$syntax%
	%Z% = Sqrt(%X%)
%$$

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.

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
$begin RevSqrtOp$$ $comment CppAD Developer Documentation$$
$spell
	Sqrt
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$

$index forward, sqrt$$
$index sqrt, forward$$
$index ForSqrtOp$$

$section Reverse Mode Square Root Function$$

$head Syntax$$

$syntax%inline void RevSqrtOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%,
	 %Base% *%pz%, %Base% *%px%)%$$

$head Description$$
We are given the partial derivatives for a function
$latex G(z, x)$$ and we wish to compute the partial derivatives for
the function
$latex \[
	H(x) = G [ Z(x) , x ]
\]$$
where $latex Z(x)$$ is defined as the 
$th d$$ order Taylor coefficient row vector for $italic Z$$ as
a function of the corresponding row vector for $italic X$$ 
and
$latex \[
	Z = Sqrt(X)
\]$$
Note that $italic Z$$ has been used both the original square root 
function and for the corresponding mapping of Taylor coefficients.

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.


$head z$$
The vector $italic z$$ has length $latex d+1$$ and contains
$th d$$ order Taylor coefficient row vector for $italic Z$$.


$head On Input$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
$syntax%%pz%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Z$$.

$head On Output$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic H$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
its contents are no longer specified; i.e., it has
been used for work space.

$end
------------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline void ForSqrtOp(size_t j, 
	Base *z, const Base *x)
{	size_t k;

	if( j == 0 )
		z[j] = sqrt( x[0] );
	else
	{
		z[j] = Base(0);
		for(k = 1; k < j; k++)
			z[j] -= Base(k) * z[k] * z[j-k];
		z[j] /= Base(j);
		z[j] += x[j] / Base(2);
		z[j] /= z[0];
	}
}

template <class Base>
inline void RevSqrtOp(size_t d, 
	const Base  *z, const Base *x,
	      Base *pz,      Base *px)
{	size_t k;

	// number of indices to access
	size_t j = d;

	while(j)
	{	// scale partial w.r.t. z[j]
		pz[j]   /= z[0];

		pz[0]   -= pz[j] * z[j];
		px[j]   += pz[j] / Base(2);
		for(k = 1; k < j; k++)
			pz[k]   -= pz[j] * z[j-k];
		--j;
	}
	px[0] += pz[0] / (Base(2) * z[0]);
}

} // END CppAD namespace

# endif
