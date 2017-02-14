# ifndef CPPAD_CORE_SPARSE_HES_HPP
# define CPPAD_CORE_SPARSE_HES_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_hes$$
$spell
	const
	Taylor
	rc
	rcv
	nr
	nc
	hes
	std
	cppad
	colpack
	cmake
	Jacobian
$$

$section Computing Sparse Hessians$$

$head Syntax$$
$icode%n_sweep% = %f%.sparse_hes(
	%x%, %w%, %subset% %pattern%, %coloring%, %work%
)%$$

$head Purpose$$
We use $latex F : \B{R}^n \rightarrow \B{R}^m$$ to denote the
function corresponding to $icode f$$.
Here $icode n$$ is the $cref/domain/seq_property/Domain/$$ size,
and $icode m$$ is the $cref/range/seq_property/Range/$$ size, or $icode f$$.
The syntax above takes advantage of sparsity when computing the Hessian
$latex \[
	H(x) = \dpow{2}{x} \sum_{i=0}^{m-1} F_i (x)
\] $$
In the sparse case, this should be faster and take less memory than
$cref Hessian$$.
The matrix element $latex H_{i,j} (x)$$ is the second partial of
$latex w^\R{T} F (x)$$ with respect to $latex x_i$$ and $latex x_j$$.

$head SizeVector$$
The type $icode SizeVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head BaseVector$$
The type $icode BaseVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head f$$
This object has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the Taylor coefficients stored in $icode f$$ are affected
by this operation; see
$cref/uses forward/sparse_hes/Uses Forward/$$ below.

$head x$$
This argument has prototype
$codei%
	const %BaseVector%& %x%
%$$
and its size is $icode n$$.
It specifies the point at which to evaluate the Hessian
$latex H(x)$$.

$head w$$
This argument has prototype
$codei%
	const %BaseVector%& %w%
%$$
and its size is $icode m$$.
It specifies the weight for each of the components of $latex F(x)$$;
i.e. $latex w_i$$ is the weight for $latex F_i (x)$$.

$head subset$$
This argument has prototype
$codei%
	sparse_rcv<%SizeVector%, %BaseVector%>& %subset%
%$$
Its row size and column size is $icode n$$; i.e.,
$icode%subset%.nr() == %n%$$ and $icode%subset%.nc() == %n%$$.
It specifies which elements of the Hessian are computed.
The input value of its value vector
$icode%subset%.val()%$$ does not matter.
Upon return it contains the value of the corresponding elements
of the Hessian.
All of the row, column pairs in $icode subset$$ must also appear in
$icode pattern$$; i.e., they must be possible non-zero.

$head pattern$$
This argument has prototype
$codei%
	const sparse_rc<%SizeVector%>& %pattern%
%$$
Its row size and column size is $icode n$$; i.e.,
$icode%pattern%.nr() == %n%$$ and $icode%pattern%.nc() == %n%$$.
It is a sparsity pattern for the Hessian $latex H(x)$$.
This argument is not used (and need not satisfy any conditions),
when $cref/work/sparse_hes/work/$$ is non-empty.

$head coloring$$
The coloring algorithm determines which rows and columns
can be computed during the same sweep.
This field has prototype
$codei%
	const std::string& %coloring%
%$$
This value only matters when work is empty; i.e.,
after the $icode work$$ constructor or $icode%work%.clear()%$$.

$subhead cppad.symmetric$$
This coloring takes advantage of the fact that the Hessian matrix
is symmetric when find a coloring that requires fewer
$cref/sweeps/sparse_hes/n_sweep/$$.

$subhead cppad.general$$
This is the same as the sparse Jacobian
$cref/cppad/sparse_jac/coloring/cppad/$$ method
which does not take advantage of symmetry.

$subhead colpack.star$$
If $cref colpack_prefix$$ was specified on the
$cref/cmake command/cmake/CMake Command/$$ line,
you can set $icode coloring$$ to $code colpack.star$$.
This also takes advantage of the fact that the Hessian matrix is symmetric.

$head work$$
This argument has prototype
$codei%
	sparse_hes_work& %work%
%$$
We refer to its initial value,
and its value after $icode%work%.clear()%$$, as empty.
If it is empty, information is stored in $icode work$$.
This can be used to reduce computation when
a future call is for the same object $icode f$$,
and the same subset of the Hessian.
If either of these values change, use $icode%work%.clear()%$$ to
empty this structure.

$head n_sweep$$
The return value $icode n_sweep$$ has prototype
$codei%
	size_t %n_sweep%
%$$
It is the number of first order forward sweeps
used to compute the requested Hessian values.
Each first forward sweep is followed by a second order reverse sweep
so it is also the number of reverse sweeps.
It is also the number of colors determined by the coloring method
mentioned above.
This is proportional to the total computational work,
not counting the zero order forward sweep,
or combining multiple columns (rows) into a single sweep.

$head Uses Forward$$
After each call to $cref Forward$$,
the object $icode f$$ contains the corresponding
$cref/Taylor coefficients/glossary/Taylor Coefficient/$$.
After a call to $code sparse_hes$$
the zero order coefficients correspond to
$codei%
	%f%.Forward(0, %x%)
%$$
All the other forward mode coefficients are unspecified.

$head Example$$
$children%
	example/sparse/sparse_hes.cpp
%$$
The files $cref sparse_hes.cpp$$
is an example and test of $code sparse_hes$$.
It returns $code true$$, if it succeeds, and $code false$$ otherwise.

$end
*/
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/sparse_internal.hpp>
# include <cppad/local/color_general.hpp>
# include <cppad/local/color_symmetric.hpp>

/*!
\file sparse_hes.hpp
Sparse Hessian calculation routines.
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Class used to hold information used by Sparse Hessian routine in this file,
so it does not need to be recomputed every time.
*/
class sparse_hes_work {
	public:
		/// row and column indicies for return values
		/// (some may be reflected by symmetric coloring algorithms)
		CppAD::vector<size_t> row;
		CppAD::vector<size_t> col;
		/// indices that sort the row and col arrays by color
		CppAD::vector<size_t> order;
		/// results of the coloring algorithm
		CppAD::vector<size_t> color;

		/// constructor
		sparse_hes_work(void)
		{ }
		/// inform CppAD that this information needs to be recomputed
		void clear(void)
		{
			row.clear();
			col.clear();
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

\param w
a vector of length m, the number of dependent variables in f
(this ADFun object).

\param subset
specifices the subset of the sparsity pattern where the Hessian is evaluated.
subset.nr() == n,
subset.nc() == n.

\param pattern
is a sparsity pattern for the Hessian of w^T * f;
pattern.nr() == n,
pattern.nc() == n,
where m is number of dependent variables in f.

\param coloring
determines which coloring algorithm is used.
This must be cppad.symmetric, cppad.general, or colpack.star.

\param work
this structure must be empty, or contain the information stored
by a previous call to sparse_hes.
The previous call must be for the same ADFun object f
and the same subset.

\return
This is the number of first order forward
(and second order reverse) sweeps used to compute thhe Hessian.
*/
template <class Base>
template <class SizeVector, class BaseVector>
size_t ADFun<Base>::sparse_hes(
	const BaseVector&                    x        ,
	const BaseVector&                    w        ,
	sparse_rcv<SizeVector , BaseVector>& subset   ,
	const sparse_rc<SizeVector>&         pattern  ,
	const std::string&                   coloring ,
	sparse_hes_work&                     work     )
{	size_t n = Domain();
	//
	CPPAD_ASSERT_KNOWN(
		subset.nr() == n,
		"sparse_hes: subset.nr() not equal domain dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		subset.nc() == n,
		"sparse_hes: subset.nc() not equal domain dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		size_t( x.size() ) == n,
		"sparse_hes: x.size() not equal domain dimension for f"
	);
	CPPAD_ASSERT_KNOWN(
		size_t( w.size() ) == Range(),
		"sparse_hes: w.size() not equal range dimension for f"
	);
	//
	// work information
	vector<size_t>& row(work.row);
	vector<size_t>& col(work.col);
	vector<size_t>& color(work.color);
	vector<size_t>& order(work.order);
	//
	// subset information
	const SizeVector& subset_row( subset.row() );
	const SizeVector& subset_col( subset.col() );
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
# ifndef NDEBUG
	if( color.size() != 0 )
	{	CPPAD_ASSERT_KNOWN(
			color.size() == n,
			"sparse_hes: work is non-empty and conditions have changed"
		);
		CPPAD_ASSERT_KNOWN(
			row.size() == K,
			"sparse_hes: work is non-empty and conditions have changed"
		);
		CPPAD_ASSERT_KNOWN(
			col.size() == K,
			"sparse_hes: work is non-empty and conditions have changed"
		);
		//
		for(size_t k = 0; k < K; k++)
		{	bool ok = row[k] == subset_row[k] && col[k] == subset_col[k];
			ok     |= row[k] == subset_col[k] && col[k] == subset_row[k];
			CPPAD_ASSERT_KNOWN(
				ok,
				"sparse_hes: work is non-empty and conditions have changed"
			);
		}
	}
# endif
	//
	// check for case where input work is empty
	if( color.size() == 0 )
	{	// compute work color and order vectors
		CPPAD_ASSERT_KNOWN(
			pattern.nr() == n,
			"sparse_hes: pattern.nr() not equal domain dimension for f"
		);
		CPPAD_ASSERT_KNOWN(
			pattern.nc() == n,
			"sparse_hes: pattern.nc() not equal domain dimension for f"
		);
		//
		// initialize work row, col to be same as subset row, col
		row.resize(K);
		col.resize(K);
		// cannot assign vectors becasue may be of different types
		// (SizeVector and CppAD::vector<size_t>)
		for(size_t k = 0; k < K; k++)
		{	row[k] = subset_row[k];
			col[k] = subset_col[k];
		}
		//
		// create an internal version of pattern
		vector<size_t> internal_index(n);
		for(size_t j = 0; j < n; j++)
			internal_index[j] = j;
		bool transpose   = false;
		bool zero_empty  = false;
		bool input_empty = true;
		local::sparse_list internal_pattern;
		internal_pattern.resize(n, n);
		local::set_internal_sparsity(zero_empty, input_empty,
			transpose, internal_index, internal_pattern, pattern
		);
		//
		// execute coloring algorithm
		color.resize(n);
		if( coloring == "cppad.general" )
			local::color_general_cppad(internal_pattern, row, col, color);
		else if( coloring == "cppad.symmetric" )
			local::color_general_cppad(internal_pattern, row, col, color);
		else if( coloring == "colpack.star" )
		{
# if CPPAD_HAS_COLPACK
			local::color_general_colpack(internal_pattern, row, col, color);
# else
			CPPAD_ASSERT_KNOWN(
				false,
				"sparse_hes: coloring = colpack.star "
				"and colpack_prefix not in cmake command line."
			);
# endif
		}
		else CPPAD_ASSERT_KNOWN(
			false,
			"sparse_hes: coloring is not valid."
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
	for(size_t j = 0; j < n; j++) if( color[j] < n )
		n_color = std::max(n_color, color[j] + 1);
	//
	// initialize the return Hessian values as zero
	for(size_t k = 0; k < K; k++)
		subset.set(k, zero);
	//
	// direction vector for calls to first order forward
	BaseVector dx(n);
	//
	// return values for calls to second order reverse
	BaseVector ddw(2 * n);
	//
	// loop over colors
	size_t k = 0;
	for(size_t ell = 0; ell < n_color; ell++)
	{	CPPAD_ASSERT_UNKNOWN( color[ row[ order[k] ] ] == ell );
		//
		// combine all rows with this color
		for(size_t i = 0; i < n; i++)
		{	dx[i] = zero;
			if( color[i] == ell )
				dx[i] = one;
		}
		// call forward mode for all these rows at once
		Forward(1, dx);
		//
		// evaluate derivative of w^T * F'(x) * dx
		ddw = Reverse(2, w);
		//
		// set the corresponding components of the result
		while( k < K && color[ row[order[k]] ] == ell )
		{	size_t index = col[ order[k] ] * 2 + 1;
			subset.set(order[k], ddw[index] );
			k++;
		}
	}
	return n_color;
}

} // END_CPPAD_NAMESPACE

# endif
