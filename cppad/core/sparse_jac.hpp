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
	std
	Cppad
	Colpack
	cmake
$$

$section Computing Sparse Jacobians$$

$head Syntax$$
$icode%n_sweep% = %f%.sparse_jac_for(%x%, %pattern%, %subset%, %work%)
%$$
$icode%n_sweep% = %f%.sparse_jac_rev(%x%, %pattern%, %subset%, %work%)
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
This argument is not used (and need not satisfy any conditions),
when $cref/work/sparse_jac/work/$$ is non-empty.

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
	sparse_jac_work& %work%
%$$
We refer to its initial value,
and its value after $icode%work%.clear()%$$, as empty.
If it is empty, information is stored in $icode work$$.
This can be used to reduce computation when
a future call is for the same object $icode f$$,
the same member function $code sparse_jac_for$$ or $code sparse_jac_rev$$,
and the same subset of the Jacobian.
If any of these values change, use $icode%work%.clear()%$$ to
empty this structure.

$subhead color_method$$
The coloring algorithm determines which rows (reverse) or columns (forward)
can be computed during the same sweep.
This field has prototype
$codei%
	std::string %work%.color_method
%$$
This value only matters when work is empty; i.e.,
after the $icode work$$ constructor or $icode%work%.clear()%$$.
$codei%

"cppad"
%$$
This is the default value for $icode%work%.color_method%$$; i.e.,
its value after the constructor or $icode%work%.clear()%$$.
This uses a general purpose coloring algorithm written for Cppad.
$codei%

"colpack"
%$$
If $cref colpack_prefix$$ is specified on the
$cref/cmake command/cmake/CMake Command/$$ line,
you can set $icode%work%.method%$$ to $code "colpack"$$.
This uses a general purpose coloring algorithm that is part of Colpack.

$head n_sweep$$
The return value $icode n_sweep$$ has prototype
$codei%
	size_t %n_sweep%
%$$
If $code sparse_jac_for$$ ($code sparse_jac_rev$$) is used,
$icode n_sweep$$ is the number of first order forward (reverse) sweeps
used to compute the requested Jacobian values.
It is also the number of colors determined by the coloring method
mentioned above.
It is proportional to the total computational work,
not counting the zero order forward sweep,
or combining multiple columns (rows) into a single sweep.

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
	example/sparse/sparse_jac_for.cpp%
	example/sparse/sparse_jac_rev.cpp
%$$
The files $cref sparse_jac_for.cpp$$ and $cref sparse_jac_rev.cpp$$
are examples and tests of $code sparse_jac_for$$ and $code sparse_jac_rev$$.
They return $code true$$, if they succeed, and $code false$$ otherwise.

$end
*/
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/sparse_internal.hpp>
# include <cppad/local/color_general.hpp>
# include <cppad/utility/vector.hpp>

/*!
\file sparse_jac.hpp
Sparse Jacobian calculation routines.
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
Class used to hold information used by Sparse Jacobian routines in this file,
so they do not need to be recomputed every time.
*/
	class sparse_jac_work {
		public:
			/// Coloring method: "cppad", or "colpack"
			/// (this field is set by user)
			std::string color_method;
			/// indices that sort the user row and col arrays by color
			CppAD::vector<size_t> order;
			/// results of the coloring algorithm
			CppAD::vector<size_t> color;
			//
			/// constructor
			sparse_jac_work(void) : color_method("cppad")
			{ }
			/// reset work to empty.
			/// This informs CppAD that color and order need to be recomputed
			void clear(void)
			{	color_method = "cppad";
				order.clear();
				color.clear();
			}
	};

// ----------------------------------------------------------------------------
/*!
Calculate sparse Jacobains using forward mode

\tparam Base
the base type for the recording that is stored in the ADFun object.

\tparam SizeVector
a simple vector class with elements of type size_t.

\tparam BaseVector
a simple vector class with elements of type Base.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param pattern
is a sparsity pattern for the Jacobian of f;
pattern.nr() == m,
pattern.nc() == n,
where m is number of dependent variables in f.

\param subset
specifices the subset of the sparsity pattern where the Jacobian is evaluated.
subset.nr() == m,
subset.nc() == n.

\param work
this structure must be empty, or contain the information stored
by a previous call to sparse_jac_for.
The previous call must be for the same ADFun object f
and the same subset.
*/
template <class Base>
template <class SizeVector, class BaseVector>
size_t ADFun<Base>::sparse_jac_for(
	const BaseVector&                    x       ,
	const sparse_rc<SizeVector>&         pattern ,
	sparse_rcv<SizeVector, BaseVector>&  subset  ,
	sparse_jac_work&                     work    )
{	size_t m = Range();
	size_t n = Domain();
	//
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
		CPPAD_ASSERT_KNOWN(
			pattern.nr() == m,
			"sparse_jac_for: pattern.nr() not equal range dimension for f"
		);
		CPPAD_ASSERT_KNOWN(
			pattern.nc() == n,
			"sparse_jac_for: pattern.nc() not equal domain dimension for f"
		);
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
	// direction vector and return values for calls to Forward
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
// ----------------------------------------------------------------------------
/*!
Calculate sparse Jacobains using reverse mode

\tparam Base
the base type for the recording that is stored in the ADFun object.

\tparam SizeVector
a simple vector class with elements of type size_t.

\tparam BaseVector
a simple vector class with elements of type Base.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param pattern
is a sparsity pattern for the Jacobian of f;
pattern.nr() == m,
pattern.nc() == n,
where m is number of dependent variables in f.

\param subset
specifices the subset of the sparsity pattern where the Jacobian is evaluated.
subset.nr() == m,
subset.nc() == n.

\param work
this structure must be empty, or contain the information stored
by a previous call to sparse_jac_rev.
The previous call must be for the same ADFun object f
and the same subset.
*/
template <class Base>
template <class SizeVector, class BaseVector>
size_t ADFun<Base>::sparse_jac_rev(
	const BaseVector&                    x       ,
	const sparse_rc<SizeVector>&         pattern ,
	sparse_rcv<SizeVector, BaseVector>&  subset  ,
	sparse_jac_work&                     work    )
{	size_t m = Range();
	size_t n = Domain();
	//
	CPPAD_ASSERT_KNOWN(
		subset.nr() == m,
		"sparse_jac_rev: subset.nr() not equal range dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		subset.nc() == n,
		"sparse_jac_rev: subset.nc() not equal domain dimension for f"
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
		"sparse_jac_rev: work is non-empty and conditions have changed"
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
		CPPAD_ASSERT_KNOWN(
			pattern.nr() == m,
			"sparse_jac_rev: pattern.nr() not equal range dimension for f"
		);
		CPPAD_ASSERT_KNOWN(
			pattern.nc() == n,
			"sparse_jac_rev: pattern.nc() not equal domain dimension for f"
		);
		//
		// convert pattern to an internal version
		vector<size_t> internal_index(n);
		for(size_t j = 0; j < n; j++)
			internal_index[j] = j;
		bool transpose = false;
		local::sparse_list internal_pattern;
		internal_pattern.resize(m, n);
		local::set_internal_sparsity(
			transpose, internal_index, pattern, internal_pattern
		);
		//
		// execute coloring algorithm
		color.resize(m);
		if(	work.color_method == "cppad" )
			local::color_general_cppad(internal_pattern, row, col, color);
		else if( work.color_method == "colpack" )
		{
# if CPPAD_HAS_COLPACK
			local::color_general_colpack(internal_pattern, row, col, color);
# else
			CPPAD_ASSERT_KNOWN(
				false,
				"sparse_jac_rev: work.color_method = colpack "
				"and colpack_prefix missing from cmake command line."
			);
# endif
		}
		else CPPAD_ASSERT_KNOWN(
			false,
			"sparse_jac_rev: work.color_method is not valid."
		);
		//
		// put sorting indices in color order
		SizeVector key(K);
		order.resize(K);
		for(size_t k = 0; k < K; k++)
			key[k] = color[ row[k] ];
		index_sort(key, order);
	}
	// Base versions of zero and one
	Base one(1.0);
	Base zero(0.0);
	//
	size_t n_color = 1;
	for(size_t i = 0; i < m; i++) if( color[i] < m )
		n_color = std::max(n_color, color[i] + 1);
	//
	// initialize the return Jacobian values as zero
	for(size_t k = 0; k < K; k++)
		subset.set(k, zero);
	//
	// weighting vector and return values for calls to Reverse
	BaseVector w(m), dw(n);
	//
	// loop over colors
	size_t k = 0;
	for(size_t ell = 0; ell < n_color; ell++)
	{	CPPAD_ASSERT_UNKNOWN( color[ row[ order[k] ] ] == ell );
		//
		// combine all rows with this color
		for(size_t i = 0; i < m; i++)
		{	w[i] = zero;
			if( color[i] == ell )
				w[i] = one;
		}
		// call reverse mode for all these rows at once
		dw = Reverse(1, w);
		//
		// set the corresponding components of the result
		while( k < K && color[ row[order[k]] ] == ell )
		{	subset.set(order[k], dw[col[order[k]]] );
			k++;
		}
	}
	return n_color;
}

} // END_CPPAD_NAMESPACE
# endif
