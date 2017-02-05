# ifndef CPPAD_CORE_SPARSE_JAC_HPP
# define CPPAD_CORE_SPARSE_JAC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_jac$$
$spell
	Jacobian
	Jacobians
	const
	jac
	Taylor
	rc
	rcv
	nr
	nc
$$

$section Sparse Jacobians$$

$head Syntax$$
$icode%n_sweep% = f%.sparse_jac_for(%x%, %pattern%, %subset%, %work%)
%$$
$icode%n_sweep% = f%.sparse_jac_rev(%x%, %pattern%, %subset%, %work%)
%$$


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
The matrix element $latex J_{i,j} (x)$$ is the partial of
$latex F_i (x)$$ with respect to $latex x_j$$.

$head SizeVector$$
The type $icode SizeVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head BaseVector$$
The type $icode BaseVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head sparse_jac_for$$
This function first order forward mode sweeps $cref forward_one$$
to compute multiple columns of the Jacobian at the same time.

$head sparse_jac_rev$$
This function first order reverse mode sweeps $cref reverse_one$$
to compute multiple rows of the Jacobian at the same time.

$head f$$
This object has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the Taylor coefficients stored in $icode f$$ are affected
by this operation; see
$cref/uses forward/sparse_jac/Uses Forward/$$ below.

$head x$$
This argument has prototype
$codei%
	const %BaseVector%& %x%
%$$
and its size is $icode n$$.
It specifies the point at which to evaluate the Jacobian
$latex J(x)$$.

$head pattern$$
This argument has prototype
$codei%
	const sparse_rc<%SizeVector%>& %pattern%
%$$
Its row size is $icode%pattern%.nr() == %m%$$,
and its column size is $icode%pattern%.nc() == %n%$$.
It is a sparsity pattern for the Jacobian $latex J(x)$$.

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

$head work$$
This argument has prototype
$codei%
	sparse_jacobian_work& %work%
%$$
We refer to its initial value,
and its value after $icode%work%.clear()%$$, as empty.
If it is empty, information is stored in $icode work$$.
This can be used to reduce computation when
a future call is for the same object $icode f$$,
the same member function $code sparse_jac_for$$ or $code sparse_jac_rev$$,
the same sparsity pattern,
and the same subset of the Jacobian.
If any of these values change, use $icode%work%.clear()%$$ to
empty this structure.

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
	example/sparse/sparse_jac_for.cpp
%$$
The file $cref sparse_jac_for.cpp$$
is examples and tests of $code sparse_jac_for$$.
It return $code true$$, if it succeeds and $code false$$ otherwise.

$end
*/
# include <cppad/core/cppad_assert.hpp>
# include <cppad/core/sparse_jac_work.hpp>
# include <cppad/local/sparse_internal.hpp>
# include <cppad/local/color_general.hpp>

/*!
\file sparse_jac.hpp
Sparse Jacobian calculation routines.
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE


template <class Base>
template <class SizeVector, class BaseVector>
size_t ADFun<Base>::sparse_jac_for(
	const BaseVector&                    x       ,
	const sparse_rc<SizeVector>&         pattern ,
	sparse_rcv<SizeVector, BaseVector>&  subset  ,
	sparse_jacobian_work&                work    )
{	size_t m = Range();
	size_t n = Domain();
	//
	CPPAD_ASSERT_KNOWN(
		pattern.nr() == m,
		"sparse_jac_for: pattern.nr() not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		pattern.nc() == n,
		"sparse_jac_for: pattern.nc() not equal domain dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		subset.nr() == m,
		"sparse_jac_for: subset.nr() not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		subset.nc() == n,
		"sparse_jac_for: subset.nc() not equal domain dimension for f"
	);
	//
	// row and column vectors in subset
	const SizeVector& row( subset.row() );
	const SizeVector& col( subset.col() );
	//
	vector<size_t>& color(work.color);
	vector<size_t>& order(work.order);
	CPPAD_ASSERT_KNOWN(
		color.size() == 0 || color.size() == n,
		"sparse_jac_for: work is non-empty and conditions have changed"
	);
	//
	// point at which we are evaluationg the Jacobian
	Forward(0, x);
	//
	// number of elements in the subset
	size_t K = subset.nnz();
	//
	// check for case were there is nothing to do
	// (except for call to Forward(0, x)
	if( K == 0 )
		return 0;
	//
	// check for case where input work is empty
	if( color.size() == 0 )
	{	// compute work color and order vectors
		//
		// convert pattern to an internal version of its transpose
		vector<size_t> internal_index(n);
		for(size_t j = 0; j < n; j++)
			internal_index[j] = j;
		bool transpose = true;
		local::sparse_list pattern_transpose;
		pattern_transpose.resize(n, m);
		local::set_internal_sparsity(
			transpose, internal_index, pattern, pattern_transpose
		);
		//
		// execute coloring algorithm
		color.resize(n);
		if(	work.color_method == "cppad" )
			local::color_general_cppad(pattern_transpose, col, row, color);
		else if( work.color_method == "colpack" )
		{
# if CPPAD_HAS_COLPACK
			local::color_general_colpack(pattern_transpose, col, row, color);
# else
			CPPAD_ASSERT_KNOWN(
				false,
				"sparse_jac_for: work.color_method = colpack "
				"and colpack_prefix missing from cmake command line."
			);
# endif
		}
		else CPPAD_ASSERT_KNOWN(
			false,
			"sparse_jac_for: work.color_method is not valid."
		);
		//
		// put sorting indices in color order
		SizeVector key(K);
		order.resize(K);
		for(size_t k = 0; k < K; k++)
			key[k] = color[ col[k] ];
		index_sort(key, order);
	}
	// Base versions of zero and one
	Base one(1.0);
	Base zero(0.0);
	//
	size_t n_color = 1;
	for(size_t j = 0; j < n; j++) if( color[j] < n )
		n_color = std::max(n_color, color[j] + 1);
	//
	// initialize the return Jacobian values as zero
	for(size_t k = 0; k < K; k++)
		subset.set(k, zero);
	//
	// direction vector and return values for calls to forward
	BaseVector dx(n), dy(m);
	//
	// loop over colors
	size_t k = 0;
	for(size_t ell = 0; ell < n_color; ell++)
	{	CPPAD_ASSERT_UNKNOWN( color[ col[ order[k] ] ] == ell );
		//
		// combine all columns with this color
		for(size_t j = 0; j < n; j++)
		{	dx[j] = zero;
			if( color[j] == ell )
				dx[j] = one;
		}
		// call forward mode for all these columns at once
		dy = Forward(1, dx);
		//
		// set the corresponding components of the result
		while( k < K && color[ col[order[k]] ] == ell )
		{	subset.set(order[k], dy[row[order[k]]] );
			k++;
		}
	}
	return n_color;
}

} // END_CPPAD_NAMESPACE
# endif
