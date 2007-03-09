# ifndef CPPAD_ABS_OP_INCLUDED
# define CPPAD_ABS_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForAbsOp$$ $comment CppAD Developer Documentation$$
$spell
	Abs
	Taylor
	const
	inline
	Op
$$

$index abs, forward$$
$index forward, abs$$
$index ForAbsOp$$

$section Forward Mode Absolute Value Function$$

$head Syntax$$

$syntax%inline void ForAbsOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%)%$$

$head Description$$
Computes the $italic d$$ order Taylor coefficient for $latex Z$$ where
$syntax%
	%Z% = Abs(%X%)
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
$begin RevAbsOp$$ $comment CppAD Developer Documentation$$
$spell
	Abs
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$

$index abs, reverse$$
$index reverse, abs$$
$index RevAbsOp$$

$section Reverse Mode Absolute Value Function$$

$head Syntax$$

$syntax%inline void RevAbsOp(size_t %d%,
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
	Z = Abs(X)
\]$$
Note that $italic Z$$ has been used both the original absolute value 
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
with respect to the $th j$$ order Taylor coefficient for $italic Y$$.

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
inline void ForAbsOp(size_t j, 
	Base *z, const Base *x)
{	size_t k;

	// order that decides positive, negative or zero
	k = 0;
	while( (k < j) & (x[k] == Base(0)) )
		k++; 

	if( GreaterThanZero(x[k]) )
		z[j]  = x[j];
	else if( LessThanZero(x[k]) )
		z[j] = -x[j]; 
	else	z[j] = Base(0);
}

template <class Base>
inline void RevAbsOp(size_t d, 
	const Base  *z, const Base *x,
	      Base *pz,      Base *px)
{	size_t j, k;

	// order that decides positive, negative or zero
	k = 0;
	while( (k < d) & (x[k] == Base(0)) )
		k++; 

	if( GreaterThanZero(x[k]) )
	{	for(j = k; j <= d; j++)
			px[j] += pz[j];
	}
	else if( LessThanZero(x[k]) )
	{	for(j = k; j <= d; j++)
			px[j] -= pz[j];
	}

}

} // END CppAD namespace

# endif
