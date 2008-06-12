# ifndef CPPAD_SPARSE_HESSIAN_INCLUDED
# define CPPAD_SPARSE_HESSIAN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_hessian$$
$spell
	Bool
	hes
	const
	Taylor
$$

$section Sparse Hessian: Easy Driver$$
$index SparseHessian$$
$index hessian, sparse$$

$head Syntax$$
$codei%%hes% = %f%.SparseHessian(%x%, %w%)
%$$
$codei%%hes% = %f%.SparseHessian(%x%, %w%, %p%)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ do denote the
$cref/AD function/glossary/AD Function/$$
corresponding to $icode f$$. 
The syntax above sets $icode hes$$ to the Hessian 
$latex \[
	hes = \dpow{2}{x} \sum_{i=1}^m w_i F_i (x) 
\] $$
This is a preliminary implementation of a method for using the fact
that the matrix is sparse to reduce the amount of computation necessary.
One should use speed tests to verify that results are computed faster
than when using the routine $cref/Hessian/$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the $cref/ADFun/$$ object $icode f$$ is not $code const$$
(see $cref/Uses Forward/sparse_hessian/Uses Forward/$$ below).

$head x$$
The argument $icode x$$ has prototype
$codei%
	const %BaseVector% &%x%
%$$
(see $cref/BaseVector/sparse_hessian/BaseVector/$$ below)
and its size 
must be equal to $icode n$$, the dimension of the
$cref/domain/SeqProperty/Domain/$$ space for $icode f$$.
It specifies
that point at which to evaluate the Hessian.

$head w$$
The argument $icode w$$ has prototype
$codei%
	const %BaseVector% &%w%
%$$
and size $latex m$$.
It specifies the value of $latex w_i$$ in the expression 
for $icode hes$$.
The more components of $latex w$$ that are identically zero,
the more spares the resulting Hessian may be (and hence the more efficient
the calculation of $icode hes$$ may be).

$head p$$
The argument $icode p$$ is optional and has prototype
$syntax%
	const %BoolVector% &%p%
%$$
(see $cref/BoolVector/sparse_hessian/BoolVector/$$ below)
and its size is $latex n * n$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the Hessian; i.e.,
for $latex j = 0 , \ldots , n-1$$ and $latex k = 0 , \ldots , n-1$$.
$latex \[
	\sum_i w_i \DD{ F_i }{ x_j }{ x_k } \neq 0 
		; \Rightarrow \; p [ j * n + k ] = {\rm true}
\] $$
$pre

$$
If this sparsity pattern does not change between calls to 
$codei SparseHessian$$, it should be faster to calculate $icode p$$ once and
pass this argument to $codei SparseHessian$$.

$head hes$$
The result $icode hes$$ has prototype
$codei%
	%BaseVector% %hes%
%$$
and its size is $latex n * n$$.
For $latex j = 0 , \ldots , n - 1 $$ 
and $latex \ell = 0 , \ldots , n - 1$$
$latex \[
	hes [ j * n + \ell ] = \DD{ w^{\rm T} F }{ x_j }{ x_\ell } ( x )
\] $$

$head BaseVector$$
The type $icode BaseVector$$ must be a $cref/SimpleVector/$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$icode Base$$.
The routine $cref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head BoolVector$$
The type $icode BoolVector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type bool/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.
In order to save memory, 
you may want to use a class that packs multiple elements into one
storage location; for example,
$cref/vectorBool/CppAD_vector/vectorBool/$$.

$head Uses Forward$$
After each call to $cref/Forward/$$,
the object $icode f$$ contains the corresponding 
$cref/Taylor coefficients/glossary/Taylor Coefficient/$$.
After $code SparseHessian$$,
the previous calls to $xref/Forward/$$ are undefined.

$head Example$$
$children%
	example/sparse_hessian.cpp
%$$
The routine
$cref/sparse_hessian.cpp/$$ 
is examples and tests of $code sparse_hessian$$.
It return $code true$$, if it succeeds and $code false$$ otherwise.


$end
-----------------------------------------------------------------------------
*/




//  BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename BaseVector>
BaseVector ADFun<Base>::SparseHessian(const BaseVector &x, const BaseVector &w)
{	size_t i, j, k;

	typedef CppAD::vector<bool>   BoolVector;

	size_t m = Range();
	size_t n = Domain();

	// determine the sparsity pattern p for Hessian of w^T F
	BoolVector r(n * n);
	for(j = 0; j < n; j++)
	{	for(k = 0; k < n; k++)
			r[j * n + k] = false;
		r[j * n + j] = true;
	}
	ForSparseJac(n, r);
	//
	BoolVector s(m);
	for(i = 0; i < m; i++)
		s[i] = w[i] != 0;
	BoolVector p = RevSparseHes(n, s);

	// compute sparse Hessian
	return SparseHessian(x, w, p);
}

template <typename Base>
template <typename BaseVector, typename BoolVector>
BaseVector ADFun<Base>::SparseHessian(
	const BaseVector &x ,
	const BaseVector &w ,
	const BoolVector &p )
{
	typedef CppAD::vector<size_t> SizeVector;
	size_t j, k, l;

	size_t n = Domain();

	// check Vector is Simple Vector class with bool elements
	CheckSimpleVector<bool, BoolVector>();

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, BaseVector>();

	CPPAD_ASSERT_KNOWN(
		x.size() == n,
		"SparseHessian: size of x not equal domain dimension for f"
	); 
	CPPAD_ASSERT_KNOWN(
		w.size() == Range(),
		"SparseHessian: size of w not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		p.size() == n * n,
		"SparseHessian: size of p not equal square of"
		" domain dimension for f"
	);
	
	// initial coloring
	SizeVector color(n);
	for(j = 0; j < n; j++)
		color[j] = j;

	// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
	// Graph Coloring in Optimization Revisited by
	// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
	BoolVector    forbidden(n);
	for(j = 0; j < n; j++)
	{	// initial all colors as ok for this column
		for(k = 0; k < n; k++)
			forbidden[k] = false;
		// for each row that is connected to column j
		for(l = 0; l < n; l++) if( p[l * n + j] )
		{	// for each column that is connected to row l
			for(k = 0; k < n; k++) if( p[l * n + k] & (j != k) )	
				forbidden[ color[k] ] = true;
		}
		k = 0;
		while( forbidden[k] && k < n )
		{	k++;
			CPPAD_ASSERT_UNKNOWN( k < n );
		}
		color[j] = k;
	}
	size_t n_color = 1;
	for(k = 0; k < n; k++) n_color = std::max(n_color, color[k] + 1);

	// some values
	const Base zero(0);
	const Base one(1);

	// point at which we are evaluating the Hessian
	Forward(0, x);

	// define the return value
	BaseVector h(n * n);
	for(j = 0; j < n; j++)
		for(k = 0; k < n; k++)
			h[j * n + k] = zero;

	// direction vector for calls to forward
	BaseVector u(n);

	// location for return values from Reverse
	BaseVector ddw(n * 2);

	// loop over colors
	size_t c;
	for(c = 0; c < n_color; c++)
	{	// determine all the colums with this color
		for(j = 0; j < n; j++)
		{	if( color[j] == c )
				u[j] = one;
			else	u[j] = zero;
		}
		// call forward mode for all these columns at once
		Forward(1, u);

		// evaluate derivative of w^T * F'(x) * u
		ddw = Reverse(2, w);

		// set the corresponding components of the result
		for(j = 0; j < n; j++) if( color[j] == c )
		{	for(l = 0; l < n; l++) 
				if( p[ l * n + j ] )
					h[l * n + j] = ddw[l * 2 + 1];
		}
	}
	return h;
}

} // END CppAD namespace

# endif
