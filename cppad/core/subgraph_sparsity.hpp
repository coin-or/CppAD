# ifndef CPPAD_CORE_SUBGRAPH_SPARSITY_HPP
# define CPPAD_CORE_SUBGRAPH_SPARSITY_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin subgraph_sparsity$$
$spell
	subgraph
	rc
	Jacobian
	bool
$$

$section Subgraph Dependency Sparsity Patterns$$

$head Syntax$$
$icode%f%.subgraph_sparsity(%pattern_out%, %transpose%)%$$

$head Purpose$$
We use $latex F : \B{R}^n \rightarrow \B{R}^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to
the operation sequence stored in $icode f$$.

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
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$

$head pattern_out$$
This argument has prototype
$codei%
	sparse_rc<%SizeVector%>& %pattern_out%
%$$
This input value of $icode pattern_out$$ does not matter.
Upon return $icode pattern_out$$ is a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
for $latex F(x)$$; i.e., the pattern has $latex m$$ rows, $latex n$$ columns
and if $latex F_i (x)$$ depends on the value of $latex x_j$$,
the pair $latex (i, j)$$ is in $icode pattern_out$$.
Not that this is also a sparsity pattern for the Jacobian
$latex F^{(1)} (x)$$.

$head transpose$$
This argument has prototype
$codei%
	bool %transpose%
%$$
If $icode transpose$$ it is false (true),
upon return $icode pattern_out$$ is a sparsity pattern for
$latex F^{(1)} (x)$$ ($latex F^{(1)} (x)^\R{T}$$).

$head Example$$
$children%
	example/sparse/subgraph_sparsity.cpp
%$$
The file
$cref subgraph_sparsity.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/rev_jac_subgraph.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Subgraph sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\param pattern_out
The input value does not matter.
The return value is a dependency sparsity pattern for  \f$ F^{(1)} (x) \f$
where F is the function corresponding to the operation sequence
and x is any argument value.

\param transpose
is the sparsity pattern transposed.
*/
template <typename Base>
template <typename SizeVector>
void ADFun<Base>::subgraph_sparsity(
	sparse_rc<SizeVector>&       pattern_out      ,
	bool                         transpose        )
{
	// compute the sparsity pattern
    local::pod_vector<size_t> row_out;
    local::pod_vector<size_t> col_out;
	rev_jac_subgraph(&play_, ind_taddr_, dep_taddr_, row_out, col_out );
	CPPAD_ASSERT_UNKNOWN( row_out.size() == col_out.size() );

	// return the sparsity pattern
	size_t nr  = dep_taddr_.size();
	size_t nc  = ind_taddr_.size();
	size_t nnz = row_out.size();
	if( transpose )
	{	pattern_out.resize(nc, nr, nnz);
		for(size_t k = 0; k < nnz; k++)
			pattern_out.set(k, col_out[k], row_out[k]);
	}
	else
	{	pattern_out.resize(nr, nc, nnz);
		for(size_t k = 0; k < nnz; k++)
			pattern_out.set(k, row_out[k], col_out[k]);
	}
	return;
}
} // END_CPPAD_NAMESPACE
# endif
