# ifndef CPPAD_SIN_COS_INCLUDED
# define CPPAD_SIN_COS_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForSinCos$$
$spell
	Hyp
	Sin
	Cos
	Taylor
	const
	inline
	Op
	ps
$$

$index forward, cos$$
$index forward, cosh$$
$index forward, sin$$
$index forward, sinh$$
$index cos, forward$$
$index cosh, forward$$
$index sin, forward$$
$index sinh, forward$$
$index ForSinCos$$

$section Forward Mode Trigonometric and Hyperbolic Sine and Cosine$$

$head Syntax$$

$syntax%inline void For%Type%SinCos(size_t %d%,
	%Base% *%s%, %Base% *%c%, const %Base% *%x%)%$$

$head Description$$
Computes the $italic d$$ order Taylor coefficient for $latex S$$
and $latex C$$ where  are given by

$table
$italic Type$$
	$cnext $latex S$$              
	$cnext $latex C$$         
$rnext
$code Trig$$   
	$cnext $latex \sin [ X(t) ]$$  
	$cnext $latex \cos [ X(t) ]$$    
$rnext
$code Hyp$$    
	$cnext $latex \sinh[  X(t) ]$$ 
	$cnext $latex \cosh [ X(t) ]$$ 
$tend

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient matrix for $italic X$$.

$head s$$
The vector $italic s$$ has length $latex d+1$$.
On input it contains the
$th d-1$$ order Taylor coefficient matrix for $italic S$$.
On output it contains the
$th d$$ order Taylor coefficient matrix for $italic S$$; i.e.,
$syntax%%s%[%d%]%$$ is set equal to the $th d$$ Taylor coefficient for
the function $latex S$$.

$head c$$
The vector $italic c$$ has length $latex d+1$$.
On input it contains the
$th d-1$$ order Taylor coefficient matrix for $italic C$$.
On output it contains the
$th d$$ order Taylor coefficient matrix for $italic C$$; i.e.,
$syntax%%c%[%d%]%$$ is set equal to the $th d$$ Taylor coefficient for
the function $latex C$$.

$end
------------------------------------------------------------------------------
$begin RevSinCos$$
$spell
	Hyp
	Cos
	Cosh
	ps
	Sin
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$

$index reverse, cos$$
$index reverse, cosh$$
$index reverse, sin$$
$index reverse, sinh$$
$index cos, reverse$$
$index cosh, reverse$$
$index sin, reverse$$
$index sinh, reverse$$
$index RevSinCos$$

$section Reverse Mode Sine and Cosine Functions$$

$head Syntax$$

$syntax%inline void Rev%Type%SinCos(size_t %d%,
	const %Base% *%s%, const %Base% *%c%, const %Base% *%x%,
	 %Base% *%ps%, %Base% *%pc%, %Base% *%px%)%$$

$head Description$$
We are given the partial derivatives for a function
$latex G(s, c, x)$$ and we wish to compute the partial derivatives for
the function
$latex \[
	H(x) = G [ S(x) , C(x) , x ]
\]$$
where $latex S(x)$$ and $latex C(x)$$ are defined as the 
$th d$$ order Taylor coefficient matrix for sine and cosine
as a function of the corresponding matrix for $italic X$$; i.e.,

$table
$italic Type$$
	$cnext $latex S$$              
	$cnext $latex C$$         
$rnext
$code Trig$$   
	$cnext $latex \sin [ X(t) ]$$  
	$cnext $latex \cos [ X(t) ]$$    
$rnext
$code Hyp$$    
	$cnext $latex \sinh[  X(t) ]$$ 
	$cnext $latex \cosh [ X(t) ]$$ 
$tend

Note that $italic S$$ and $latex C$$ have
been used both the original 
functions and for the corresponding mapping of Taylor coefficients.

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient matrix for $italic X$$.


$head s$$
The vector $italic s$$ has length $latex d+1$$ and contains
$th d$$ order Taylor coefficient matrix for $italic S$$.

$head c$$
The vector $italic c$$ has length $latex d+1$$ and contains
$th d$$ order Taylor coefficient matrix for $italic C$$.


$head On Input$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead ps$$
The vector $italic ps$$ has length $latex d+1$$ and 
$syntax%%ps%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic S$$.

$subhead pc$$
The vector $italic pc$$ has length $latex d+1$$ and 
$syntax%%pc%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic C$$.

$head On Output$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic H$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead ps$$
The vector $italic ps$$ has length $latex d+1$$ and 
its contents are no longer specified; i.e., it has
been used for work space.

$subhead pc$$
The vector $italic pc$$ has length $latex d+1$$ and 
its contents are no longer specified; i.e., it has
been used for work space.

$end
------------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline void ForTrigSinCos(size_t j, 
	Base *s, Base *c, const Base *x)
{	size_t k;

	if( j == 0 )
	{	s[j] = sin( x[0] );
		c[j] = cos( x[0] );
	}
	else
	{
		s[j] = Base(0);
		c[j] = Base(0);
		for(k = 1; k <= j; k++)
		{	s[j] += Base(k) * x[k] * c[j-k];
			c[j] -= Base(k) * x[k] * s[j-k];
		}
		s[j] /= Base(j);
		c[j] /= Base(j);
	}
}

template <class Base>
inline void ForHypSinCos(size_t j, 
	Base *s, Base *c, const Base *x)
{	size_t k;

	if( j == 0 )
	{	s[j] = sinh( x[0] );
		c[j] = cosh( x[0] );
	}
	else
	{
		s[j] = Base(0);
		c[j] = Base(0);
		for(k = 1; k <= j; k++)
		{	s[j] += Base(k) * x[k] * c[j-k];
			c[j] += Base(k) * x[k] * s[j-k];
		}
		s[j] /= Base(j);
		c[j] /= Base(j);
	}
}

template <class Base>
inline void RevTrigSinCos(size_t d, 
	const Base  *s, const Base  *c, const Base *x,
	      Base *ps,       Base *pc,       Base *px)
{	size_t k;

	// number of indices to access
	size_t j = d;

	while(j)
	{
		ps[j]   /= Base(j);
		pc[j]   /= Base(j);
		for(k = 1; k <= j; k++)
		{
			px[k]   += ps[j] * Base(k) * c[j-k];
			px[k]   -= pc[j] * Base(k) * s[j-k];
	
			ps[j-k] -= pc[j] * Base(k) * x[k];
			pc[j-k] += ps[j] * Base(k) * x[k];

		}
		--j;
	}
	px[0] += ps[0] * c[0];
	px[0] -= pc[0] * s[0];
}

template <class Base>
inline void RevHypSinCos(size_t d, 
	const Base  *s, const Base  *c, const Base *x,
	      Base *ps,       Base *pc,       Base *px)
{	size_t k;

	// number of indices to access
	size_t j = d;

	while(j)
	{
		ps[j]   /= Base(j);
		pc[j]   /= Base(j);
		for(k = 1; k <= j; k++)
		{
			px[k]   += ps[j] * Base(k) * c[j-k];
			px[k]   += pc[j] * Base(k) * s[j-k];
	
			ps[j-k] += pc[j] * Base(k) * x[k];
			pc[j-k] += ps[j] * Base(k) * x[k];

		}
		--j;
	}
	px[0] += ps[0] * c[0];
	px[0] += pc[0] * s[0];
}

} // END CppAD namespace

# endif
