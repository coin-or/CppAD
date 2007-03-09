# ifndef CPPAD_MUL_OP_INCLUDED
# define CPPAD_MUL_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForMulvvOp$$ $comment CppAD Developer Documentation$$
$spell
	Mul
	Mulpv
	Mulvp
	Mulvv
	Taylor
	const
	inline
	Op
$$

$index multiply, forward operator$$
$index forward, multiply operator$$
$index operator, multiply forward$$
$index ForMul$$


$section Forward Mode Multiplication Operator$$

$head Syntax$$

$syntax%inline void ForMulvvOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%y%)%$$
$pre
$$
$syntax%inline void ForMulpvOp(size_t %d%,
	%Base% *%z%, const %Base% *%p%, const %Base% *%v%)%$$
$pre
$$
$syntax%inline void ForMulvpOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%p%)%$$


$head Description$$
Computes the $italic d$$ order Taylor coefficient for $latex Z$$ where
$table
Operation  $cnext Value  $rnext
Mulvv       $cnext $latex Z = X * Y$$ $rnext
Mulpv       $cnext $latex Z = P * Y$$ $rnext
Mulvp       $cnext $latex Z = X * P$$ 
$tend

$head x$$
The vector $italic x$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic X$$.

$head y$$
The vector $italic y$$ has length $latex d+1$$ and contains the
$th d$$ order Taylor coefficient row vector for $italic Y$$.
The vector $italic y$$ is not present in the MulL and MulR cases.

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
$begin RevMulvvOp$$ $comment CppAD Developer Documentation$$
$spell
	Mul
	Mulpv
	Mulvp
	Mulvv
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$


$index multiply, reverse operator$$
$index reverse, multiply operator$$
$index operator, multiply reverse$$
$index RevMul$$

$section Reverse Mode Multiplication Operator$$

$head Syntax$$

$syntax%inline void RevMulvvOp(size_t %d%,
	const %Base% *%z%, const %Base% *%x%, const %Base% *%y%,
	const %Base% *%pz%, %Base% *%px%, %Base% *%py%)%$$
$pre
$$
$syntax%inline void RevMulpvOp(size_t %d%,
	const %Base% *%z%, const %Base% *%p%, const %Base% *%y%,
	const %Base% *%pz%, %Base% *%py%)%$$
$pre
$$
$syntax%inline void RevMulvpOp(size_t %d%,
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
Mulvv       $cnext $latex Z = X * Y$$ $rnext
Mulpv       $cnext $latex Z = P * Y$$ $rnext
Mulvp       $cnext $latex Z = X * P$$ 
$tend

Note that $italic Z$$ has been used both the original multiplication 
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


$head pz$$
The vector $italic pz$$ has length $latex d+1$$ and 
$syntax%%pz%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Z$$.

$head On Input$$

$subhead px$$
The vector $italic px$$ has length $latex d+1$$ and 
$syntax%%px%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic X$$.

$subhead py$$
The vector $italic py$$ has length $latex d+1$$ and 
$syntax%%py%[%j%]%$$ contains the partial for $italic G$$
with respect to the $th j$$ order Taylor coefficient for $italic Y$$.

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

$end
------------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

// --------------------------- Mulvv -----------------------------------------

template <class Base>
inline void ForMulvvOp(size_t j, 
	Base *z, const Base *x, const Base *y)
{
	size_t k;

	z[j] = Base(0);
	for(k = 0; k <= j; k++)
		z[j] += x[j-k] * y[k];
}

template <class Base>
inline void RevMulvvOp(size_t d, 
	const Base  *z, const Base *x, const Base *y,
	const Base *pz, Base      *px, Base      *py)
{	size_t k;

	// number of indices to access
	size_t j = d + 1;

	while(j)
	{	--j;
		for(k = 0; k <= j; k++)
		{	
			px[j-k] += pz[j] * y[k];
			py[k]   += pz[j] * x[j-k];
		}
	}
}

// --------------------------- Mulpv -----------------------------------------

template <class Base>
inline void ForMulpvOp(size_t j, 
	Base *z, const Base *p, const Base *y)
{
	z[j] = (*p) * y[j];
}

template <class Base>
inline void RevMulpvOp(size_t d, 
	const Base  *z, const Base *p, const Base *y,
	const Base *pz, Base *py)
{
	// number of indices to access
	size_t j = d + 1;

	while(j)
	{	--j;
		py[j] += pz[j] * (*p);

	}
}

// --------------------------- Mulvp -----------------------------------------

template <class Base>
inline void ForMulvpOp(size_t j,
	Base *z, const Base *x, const Base *p)
{
	z[j] = x[j] * (*p);
}

template <class Base>
inline void RevMulvpOp(size_t d,
	const Base  *z, const Base *x, const Base *p,
	const Base *pz, Base *px)
{
	// number of indices to access
	size_t j = d + 1;

	while(j)
	{	--j;
		px[j] += pz[j] * (*p);
	}
}

} // END CppAD namespace

# endif
