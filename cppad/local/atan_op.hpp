# ifndef CPPAD_ATAN_OP_INCLUDED
# define CPPAD_ATAN_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForAtanOp$$ $comment CppAD Developer Documentation$$
$spell
	atan
	Taylor
	const
	inline
	Op
$$

$index forward, atan$$
$index atan, forward$$
$index ForAtanOp$$

$section Forward Mode Atan Function$$

$head Syntax$$

$syntax%inline void ForAtanOp(size_t %d%,
	%Base% *%z%, %Base% *%b%, const %Base% *%x%)%$$

$head Description$$
Computes the $italic d$$ order Taylor coefficient for 
$latex Z$$ and $latex B$$ where
$syntax%
	%Z% = atan(%X%)
	%B% = 1 + %X% * %X%
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
the function $italic S$$.

$head b$$
The vector $italic c$$ has length $latex d+1$$.
On input it contains the
$th d-1$$ order Taylor coefficient row vector for $italic B$$.
On output it contains the
$th d$$ order Taylor coefficient row vector for $italic B$$; i.e.,
$syntax%%b%[%d%]%$$ is set equal to the $th d$$ Taylor coefficient for
the function $italic B$$.

$end
------------------------------------------------------------------------------
$begin RevAtanOp$$ $comment CppAD Developer Documentation$$
$spell
	ps
	Atan
	pb
	Sin
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$


$index reverse, atan$$
$index atan, reverse$$
$index RevAtanOp$$

$section Reverse Mode Atan Function$$

$head Syntax$$

$syntax%inline void RevAtanOp(size_t %d%,
	const %Base% *%z%, const %Base% *%b%, const %Base% *%x%,
	 %Base% *%pz%, %Base% *%pb%, %Base% *%px%)%$$

$head Description$$
We are given the partial derivatives for a function
$latex G(z, b, x)$$ and we wish to compute the partial derivatives for
the function
$latex \[
	H(x) = G [ Z(x) , B(x) , x ]
\] $$
where $latex Z(x)$$ and $latex B(x)$$ are defined as the 
$th d$$ order Taylor coefficient row vector for $latex \arctan(x)$$
and $latex 1 + x * x$$ 
as a function of the corresponding row vector for $italic X$$; i.e.,
$latex \[
\begin{array}{rcl}
	Z & = & \arctan(X) \\
	B & = & 1 + X * X
\end{array}
\]$$
Note that $italic Z$$ and $latex B$$ have
been used both the original 
functions and for the corresponding mapping of Taylor coefficients.

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.


$head z$$
The vector $italic z$$ has length $latex d+1$$ and contains
$th d$$ order Taylor coefficient row vector for $italic z$$.

$head b$$
The vector $italic b$$ has length $latex d+1$$ and contains
$th d$$ order Taylor coefficient row vector for $italic B$$.


$head On Input$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
$syntax%%pz%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Z$$.

$subhead pb$$
The vector $italic pb$$ has length $latex d+1$$ and 
$syntax%%pb%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic B$$.

$head On Output$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic H$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead pz$$
The vector $italic ps$$ has length $latex d+1$$ and 
its contents are no longer specified; i.e., it has
been used for work space.

$subhead pb$$
The vector $italic pb$$ has length $latex d+1$$ and 
its contents are no longer specified; i.e., it has
been used for work space.

$end
------------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline void ForAtanOp(size_t j, 
	Base *z, Base *b, const Base *x)
{	size_t k;

	if( j == 0 )
	{	z[j] = atan( x[0] );
		b[j] = Base(1) + x[0] * x[0];
	}
	else
	{
		b[j] = Base(2) * x[0] * x[j];
		z[j] = Base(0);
		for(k = 1; k < j; k++)
		{	b[j] += x[k] * x[j-k];
			z[j] -= Base(k) * z[k] * b[j-k];
		}
		z[j] /= Base(j);
		z[j] += x[j];
		z[j] /= b[0];
	}
}

template <class Base>
inline void RevAtanOp(size_t d, 
	const Base  *z, const Base  *b, const Base *x,
	      Base *pz,       Base *pb,       Base *px)
{	size_t k;

	// number of indices to access
	size_t j = d;

	while(j)
	{	// scale partials w.r.t z[j] and b[j]
		pz[j] /= b[0];
		pb[j] *= Base(2);

		pb[0] -= pz[j] * z[j]; 
		px[j] += pz[j] + pb[j] * x[0];
		px[0] += pb[j] * x[j];

		// more scaling of partials w.r.t z[j]
		pz[j] /= Base(j);

		for(k = 1; k < j; k++)
		{	pb[j-k] -= pz[j] * Base(k) * z[k];
			pz[k]   -= pz[j] * Base(k) * b[j-k];
			px[k]   += pb[j] * x[j-k];
		}
		--j;
	}
	px[0] += pz[0] / b[0] + pb[0] * Base(2) * x[0];
}

} // END CppAD namespace

# endif
