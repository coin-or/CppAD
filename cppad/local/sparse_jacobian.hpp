# ifndef CPPAD_SPARSE_JACOBIAN_INCLUDED
# define CPPAD_SPARSE_JACOBIAN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_jacobian$$
$spell
	Bool
	jac
	Jacobian
	const
	Taylor
$$

$section Sparse Jacobian: Easy Driver$$
$index SparseJacobian$$
$index jacobian, sparse$$

$head Syntax$$
$codei%%jac% = %f%.SparseJacobian(%x%)
%$$
$codei%%jac% = %f%.SparseJacobian(%x%, %p%)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ do denote the
$cref/AD function/glossary/AD Function/$$
corresponding to $icode f$$. 
The syntax above sets $icode jac$$ to the Jacobian 
$latex \[
	jac = F^{(1)} (x) 
\] $$
This is a preliminary implementation of a method for using the fact
that the matrix is sparse to reduce the amount of computation necessary.
One should use speed tests to verify that results are computed faster
than when using the routine $cref/Jacobian/$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the $cref/ADFun/$$ object $icode f$$ is not $code const$$
(see $cref/Uses Forward/sparse_jacobian/Uses Forward/$$ below).

$head x$$
The argument $icode x$$ has prototype
$codei%
	const %BaseVector% &%x%
%$$
(see $cref/BaseVector/sparse_jacobian/BaseVector/$$ below)
and its size 
must be equal to $icode n$$, the dimension of the
$cref/domain/SeqProperty/Domain/$$ space for $icode f$$.
It specifies
that point at which to evaluate the Jacobian.

$head p$$
The argument $icode p$$ is optional and has prototype
$syntax%
	const %BoolVector% &%p%
%$$
(see $cref/BoolVector/sparse_jacobian/BoolVector/$$ below)
and its size is $latex m * n$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the Jacobian; i.e.,
for $latex i = 0 , \ldots , m-1$$ and $latex j = 0 , \ldots , n-1$$.
$latex \[
	\D{ F_i }{ x_j } \neq 0 ; \Rightarrow \; p [ i * n + j ] = {\rm true}
\] $$
$pre

$$
If this sparsity pattern does not change between calls to 
$codei SparseJacobian$$, it should be faster to calculate $icode p$$ once and
pass this argument to $codei SparseJacobian$$.

$head jac$$
The result $icode jac$$ has prototype
$codei%
	%BaseVector% %jac%
%$$
and its size is $latex m * n$$.
For $latex i = 0 , \ldots , m - 1$$,
and $latex j = 0 , \ldots , n - 1 $$ 
$latex \[
	jac [ i * n + j ] = \D{ F_i }{ x_j }
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
After $code SparseJacobian$$,
the previous calls to $xref/Forward/$$ are undefined.

$head Example$$
$children%
	example/sparse_jacobian.cpp
%$$
The routine
$cref/sparse_jacobian.cpp/$$
is examples and tests of $code sparse_jacobian$$.
It return $code true$$, if it succeeds and $code false$$ otherwise.

$end
-----------------------------------------------------------------------------
*/




//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class BaseVector>
BaseVector ADFun<Base>::SparseJacobian(const BaseVector &x)
{	typedef CppAD::vector<bool>   BoolVector;

	size_t m = Range();
	size_t n = Domain();

	// sparsity pattern for Jacobian
	BoolVector p(n * m);

	if( n <= m )
	{	size_t j, k;

		// use forward mode 
		BoolVector r(n * n);
		for(j = 0; j < n; j++)
		{	for(k = 0; k < n; k++)
				r[j * n + k] = false;
			r[j * n + j] = true;
		}
		p = ForSparseJac(n, r);
	}
	else
	{	size_t i, k;

		// use reverse mode 
		BoolVector s(m * m);
		for(i = 0; i < m; i++)
		{	for(k = 0; k < m; k++)
				s[i * m + k] = false;
			s[i * m + i] = true;
		}
		p = RevSparseJac(m, s);
	}
	return SparseJacobian(x, p);
}

template <class Base>
template <class BaseVector, class BoolVector>
BaseVector ADFun<Base>::SparseJacobian(const BaseVector &x, const BoolVector &p)
{
	typedef CppAD::vector<size_t> SizeVector;
	size_t i, j, k;

	size_t m = Range();
	size_t n = Domain();

	// some values
	const Base zero(0);
	const Base one(1);

	// check BoolVector is Simple Vector class with bool elements
	CheckSimpleVector<bool, BoolVector>();

	// check BaseVector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, BaseVector>();

	CPPAD_ASSERT_KNOWN(
		x.size() == n,
		"SparseJacobian: size of x not equal domain dimension for f"
	); 

	CPPAD_ASSERT_KNOWN(
		p.size() == m * n,
		"SparseJacobian: size of p not equal domain dimension times "
		" range dimension for f"
	); 

	// point at which we are evaluating the Jacobian
	Forward(0, x);

	// initialize the return value
	BaseVector jac(m * n);
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			jac[i * n + j] = zero;

	if( n <= m )
	{	// use forward mode ----------------------------------------
	
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
			for(i = 0; i < m; i++) if( p[i * n + j] )
			{	// for each column that is connected to row i
				for(k = 0; k < n; k++)
					if( p[i * n + k] & (j != k) )	
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
		for(k = 0; k < n; k++) 
			n_color = std::max(n_color, color[k] + 1);

		// direction vector for calls to forward
		BaseVector dx(n);

		// location for return values from Reverse
		BaseVector dy(m);

		// loop over colors
		size_t c;
		for(c = 0; c < n_color; c++)
		{	// determine all the colums with this color
			for(j = 0; j < n; j++)
			{	if( color[j] == c )
					dx[j] = one;
				else	dx[j] = zero;
			}
			// call forward mode for all these columns at once
			dy = Forward(1, dx);

			// set the corresponding components of the result
			for(j = 0; j < n; j++) if( color[j] == c )
			{	for(i = 0; i < m; i++) 
					if( p[ i * n + j ] )
						jac[i * n + j] = dy[i];
			}
		}
	}
	else
	{	// use reverse mode ----------------------------------------
	
		// initial coloring
		SizeVector color(m);
		for(i = 0; i < m; i++)
			color[i] = i;

		// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
		// Graph Coloring in Optimization Revisited by
		// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
		BoolVector    forbidden(m);
		for(i = 0; i < m; i++)
		{	// initial all colors as ok for this row
			for(k = 0; k < m; k++)
				forbidden[k] = false;
			// for each column that is connected to row i
			for(j = 0; j < n; j++) if( p[i * n + j] )
			{	// for each row that is connected to column j
				for(k = 0; k < m; k++)
					if( p[k * n + j] & (i != k) )	
						forbidden[ color[k] ] = true;
			}
			k = 0;
			while( forbidden[k] && k < m )
			{	k++;
				CPPAD_ASSERT_UNKNOWN( k < n );
			}
			color[i] = k;
		}
		size_t n_color = 1;
		for(k = 0; k < m; k++) 
			n_color = std::max(n_color, color[k] + 1);

		// weight vector for calls to reverse
		BaseVector w(m);

		// location for return values from Reverse
		BaseVector dw(n);

		// loop over colors
		size_t c;
		for(c = 0; c < n_color; c++)
		{	// determine all the rows with this color
			for(i = 0; i < m; i++)
			{	if( color[i] == c )
					w[i] = one;
				else	w[i] = zero;
			}
			// call reverse mode for all these rows at once
			dw = Reverse(1, w);

			// set the corresponding components of the result
			for(i = 0; i < m; i++) if( color[i] == c )
			{	for(j = 0; j < n; j++) 
					if( p[ i * n + j ] )
						jac[i * n + j] = dw[j];
			}
		}
	}
	return jac;
}

} // END CppAD namespace

# endif
