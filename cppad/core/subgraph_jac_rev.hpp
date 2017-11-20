# ifndef CPPAD_CORE_SUBGRAPH_JAC_REV_HPP
# define CPPAD_CORE_SUBGRAPH_JAC_REV_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin subgraph_jac_rev$$
$spell
	Jacobians
	Jacobian
	Subgraphs
	subgraph
	jac
	rcv
	Taylor
	rev
	nr
	nc
$$

$section Compute Sparse Jacobians Using Subgraphs$$

$head Syntax$$
$icode%f%.subgraph_jac_rev(%x%, %subset%)%$$

$head Purpose$$
We use $latex F : \B{R}^n \rightarrow \B{R}^m$$ to denote the
function corresponding to $icode f$$.
Here $icode n$$ is the $cref/domain/seq_property/Domain/$$ size,
and $icode m$$ is the $cref/range/seq_property/Range/$$ size, or $icode f$$.
The syntax above takes advantage of sparsity when computing the Jacobian
$latex \[
	J(x) = F^{(1)} (x)
\] $$
In the sparse case, this should be faster and take less memory than
$cref Jacobian$$.
We use the notation $latex J_{i,j} (x)$$ to denote the partial of
$latex F_i (x)$$ with respect to $latex x_j$$.


$head Method$$
This routine uses a subgraph technique. To be specific,
for each dependent variable,
it a subgraph of the operation sequence
to determine which independent variables affect it.
This avoids to overhead of performing set operations
that is inherent in other methods for computing sparsity patterns.

$head SizeVector$$
The type $icode SizeVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head f$$
This object has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the Taylor coefficients stored in $icode f$$ are affected
by this operation; see
$cref/uses forward/sparse_jac/Uses Forward/$$ below.

$head subset$$
This argument has prototype
$codei%
	sparse_rcv<%SizeVector%, %BaseVector%>& %subset%
%$$
Its row size is $icode%subset%.nr() == %m%$$,
and its column size is $icode%subset%.nc() == %n%$$.
It specifies which elements of the Jacobian are computed.
The input value of its value vector
$icode%subset%.val()%$$ does not matter.
Upon return it contains the value of the corresponding elements
of the Jacobian.

$head Uses Forward$$
After each call to $cref Forward$$,
the object $icode f$$ contains the corresponding
$cref/Taylor coefficients/glossary/Taylor Coefficient/$$.
After a call to $code sparse_jac_forward$$ or $code sparse_jac_rev$$,
the zero order coefficients correspond to
$codei%
	%f%.Forward(0, %x%)
%$$
All the other forward mode coefficients are unspecified.

$head Example$$
$children%
	example/sparse/subgraph_jac_rev.cpp
%$$
The file $cref subgraph_jac_rev.cpp$$
is an example and test of $code subgraph_jac_rev$$.
It returns $code true$$, if it succeeds, and $code false$$ otherwise.


$end
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/subgraph/info.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Subgraph sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\tparam BaseVector
a simple vector class with elements of type Base.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param subset
specifices the subset of the sparsity pattern where the Jacobian is evaluated.
subset.nr() == m,
subset.nc() == n.
*/
template <typename Base>
template <typename SizeVector, typename BaseVector>
void ADFun<Base>::subgraph_jac_rev(
	const BaseVector&                   x      ,
	sparse_rcv<SizeVector, BaseVector>& subset )
{	size_t m = Range();
	size_t n = Domain();
	//
	CPPAD_ASSERT_KNOWN(
		subset.nr() == m,
		"subgraph_jac_rev: subset.nr() not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		subset.nc() == n,
		"subgraph_jac_rev: subset.nc() not equal domain dimension for f"
	);
	//
	// point at which we are evaluating Jacobian
	Forward(0, x);
	//
	// nnz and row, column, and row_major vectors for subset
	size_t nnz = subset.nnz();
	const SizeVector& row( subset.row() );
	const SizeVector& col( subset.col() );
	SizeVector row_major = subset.row_major();
	//
	// determine set of independent variabels
	local::pod_vector<bool> select_domain(n);
	for(size_t j = 0; j < n; j++)
		select_domain[j] = false;
	for(size_t k = 0; k < nnz; k++)
		select_domain[ col[k] ] = true;
	//
	// initialize reverse mode computation on subgraphs
	subgraph_reverse(select_domain);
	//
	// memory used to hold subgraph_reverse results
	BaseVector dw;
	//
	// initialize index in row_major
	size_t k = 0;
	while(k < nnz )
	{	size_t q   = 1;
		size_t i_dep = row[ row_major[k] ];
		size_t i_ind = col[ row_major[k] ];
		size_t ell   = i_dep;
		subgraph_reverse(dw, q, ell);
		while( i_dep == ell )
		{	subset.set( row_major[k], dw[i_ind] );
			++k;
			if( k == nnz )
			{	i_dep = m;
				i_ind = n;
			}
			else
			{	i_dep = row[ row_major[k] ];
				i_ind = col[ row_major[k] ];
			}
		}
	}
	return;
}
} // END_CPPAD_NAMESPACE
# endif
