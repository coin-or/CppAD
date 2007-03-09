# ifndef CPPAD_ACOS_OP_INCLUDED
# define CPPAD_ACOS_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForAcosOp$$ $comment CppAD Developer Documentation$$
$spell
	sqrt
	acos
	Taylor
	const
	inline
	Op
$$

$index acos, forward$$
$index forward, acos$$
$index ForAcosOp$$

$section Forward Mode Acos Function$$

$head Syntax$$

$syntax%inline void ForAcosOp(size_t %d%,
	%Base% *%z%, %Base% *%b%, const %Base% *%x%)%$$

$head Description$$
Computes the $italic d$$ order Taylor coefficient for 
$latex Z$$ and $latex B$$ where
$syntax%
	%Z% = acos(%X%)
	%B% = \sqrt{ 1 - %X% * %X% }
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
$begin RevAcosOp$$ $comment CppAD Developer Documentation$$
$spell
	ps
	Acos
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


$index acos, reverse$$
$index reverse, acos$$
$index RevAcosOp$$

$section Reverse Mode Acos Function$$

$head Syntax$$

$syntax%inline void RevAcosOp(size_t %d%,
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
$th d$$ order Taylor coefficient row vector for $latex \arccos(x)$$
and $latex 1 + x * x$$ 
as a function of the corresponding row vector for $italic X$$; i.e.,
$latex \[
\begin{array}{rcl}
	Z & = & \arccos(X) \\
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
inline void ForAcosOp(size_t j, 
	Base *z, Base *b, const Base *x)
{	size_t k;
	Base qj;

	if( j == 0 )
	{	z[j] = acos( x[0] );
		qj   = Base(1) - x[0] * x[0];
		b[j] = sqrt( qj );
	}
	else
	{	qj = 0.;
		for(k = 0; k <= j; k++)
			qj -= x[k] * x[j-k];
		b[j] = Base(0);
		z[j] = Base(0);
		for(k = 1; k < j; k++)
		{	b[j] -= Base(k) * b[k] * b[j-k];
			z[j] -= Base(k) * z[k] * b[j-k];
		}
		b[j] /= Base(j);
		z[j] /= Base(j);
		//
		b[j] += qj / Base(2);
		z[j] -= x[j];
		//
		b[j] /= b[0];
		z[j] /= b[0];
	}
}

template <class Base>
inline void RevAcosOp(size_t d, 
	const Base  *z, const Base  *b, const Base *x,
	      Base *pz,       Base *pb,       Base *px)
{	size_t k;

	// number of indices to access
	size_t j = d;

	while(j)
	{
		// scale partials w.r.t b[j] by 1 / b[0]
		pb[j] /= b[0];

		// scale partials w.r.t z[j] by 1 / b[0]
		pz[j] /= b[0];

		// update partials w.r.t b^0 
		pb[0] -= pz[j] * z[j] + pb[j] * b[j]; 

		// update partial w.r.t. x^0
		px[0] -= pb[j] * x[j];

		// update partial w.r.t. x^j
		px[j] -= pz[j] + pb[j] * x[0];

		// further scale partial w.r.t. z[j] by 1 / j
		pz[j] /= Base(j);

		for(k = 1; k < j; k++)
		{	// update partials w.r.t b^(j-k)
			pb[j-k] -= Base(k) * pz[j] * z[k] + pb[j] * b[k];

			// update partials w.r.t. x^k 
			px[k]   -= pb[j] * x[j-k];

			// update partials w.r.t. z^k
			pz[k]   -= pz[j] * Base(k) * b[j-k];
		}
		--j;
	}

	// j == 0 case
	px[0] -= ( pz[0] + pb[0] * x[0]) / b[0];
}

} // END CppAD namespace

# endif
