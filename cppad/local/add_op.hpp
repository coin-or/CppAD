# ifndef CPPAD_ADD_OP_INCLUDED
# define CPPAD_ADD_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForAddOp$$ $comment CppAD Developer Documentation$$
$spell
	Addpv
	Addvp
	Addvv
	Taylor
	const
	inline
	Op
$$

$index add, forward operator$$
$index forward, add operator$$
$index operator, add forward$$
$index ForAdd$$

$section Forward Mode Addition Operators$$

$head Syntax$$

$syntax%inline void ForAddvvOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%y%)%$$
$pre
$$
$syntax%inline void ForAddpvOp(size_t %d%,
	%Base% *%z%, const %Base% *%p%, const %Base% *%x%)%$$
$pre
$$
$syntax%inline void ForAddvpOp(size_t %d%,
	%Base% *%z%, const %Base% *%x%, const %Base% *%p%)%$$


$head Description$$
Computes the $italic d$$ order Taylor coefficient for $latex Z$$ where
$table
Operation  $cnext Value  $rnext
Addvv      $cnext $latex Z = X + Y$$ $rnext
Addpv      $cnext $latex Z = P + Y$$ $rnext
Addvp      $cnext $latex Z = X + P$$ 
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
$begin RevAddOp$$ $comment CppAD Developer Documentation$$
$spell
	Addpv
	Addvp
	Addvv
	Taylor
	const
	inline
	Op
	px
	py
	pz
$$


$index add, reverse operator$$
$index reverse, add operator$$
$index operator, add reverse$$
$index RevAdd$$
$section Reverse Mode Addition Operator$$

$head Syntax$$

$syntax%inline void RevAddvvOp(size_t %d%,
	const %Base% *%pz%, %Base% *%px%, %Base% *%py%)%$$
$pre
$$
$syntax%inline void RevAddpvOp(size_t %d%,
	const %Base% *%pz%, %Base% *%px%)%$$
$pre
$$
$syntax%inline void RevAddvpOp(size_t %d%,
	const %Base% *%pz%, %Base% *%py%)%$$

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
Addvv      $cnext $latex Z = X + Y$$ $rnext
Addpv      $cnext $latex Z = P + Y$$ $rnext
Addvp      $cnext $latex Z = X + P$$ 
$tend

Note that $italic Z$$ has been used both the original addition 
function and for the corresponding mapping of Taylor coefficients.

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

// --------------------------- Addvv -----------------------------------------

template <class Base>
inline void ForAddvvOp(size_t d, 
	Base *z, const Base *x, const Base *y)
{
	z[d] = x[d] + y[d];
}

template <class Base>
inline void RevAddvvOp(size_t d,
	const Base *pz, Base *px, Base *py)
{
	// number of indices to access
	size_t i = d + 1;

	while(i)
	{	--i;
		px[i] += pz[i];
		py[i] += pz[i];
	}
}

// --------------------------- Addpv -----------------------------------------

template <class Base>
inline void ForAddpvOp(size_t d,
	Base *z, const Base *p, const Base *y)
{
	if( d == 0 )
		z[d] = (*p) + y[d];
	else	z[d] = y[d];

}

template <class Base>
inline void RevAddpvOp(size_t d, 
	const Base *pz, Base *py)
{
	// number of indices to access
	size_t i = d + 1;

	while(i)
	{	--i;
		py[i] += pz[i];
	}
}

// --------------------------- Addvp -----------------------------------------

template <class Base>
inline void ForAddvpOp(size_t d, 
	Base *z, const Base *x, const Base *p)
{
	if( d == 0 )
		z[d] = x[d] + (*p);
	else	z[d] = x[d];

}

template <class Base>
inline void RevAddvpOp(size_t d, 
	const Base *pz, Base *px)
{
	// number of indices to access
	size_t i = d + 1;

	while(i)
	{	--i;
		px[i] += pz[i];
	}
}

} // END CppAD namespace

# endif
