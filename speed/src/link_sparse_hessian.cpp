/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_sparse_hessian$$
$spell
	const
	bool
	CppAD
	cppad
	colpack
$$


$section Speed Testing Sparse Hessian$$
$mindex link_sparse_hessian test$$

$head Prototype$$
$codei%extern bool link_sparse_hessian(
	size_t                        %size%      ,
	size_t                        %repeat%    ,
	CppAD::vector<double>&        %x%         ,
	const CppAD::vector<size_t>&  %row%       ,
	const CppAD::vector<size_t>&  %col%       ,
	CppAD::vector<double>&        %hessian%   ,
	size_t                        %n_sweep%
);
%$$

$head Method$$
Given a row index vector $latex row$$
and a second column vector $latex col$$,
the corresponding function
$latex f : \B{R}^n \rightarrow \B{R} $$
is defined by $cref sparse_hes_fun$$.
The non-zero entries in the Hessian of this function have
one of the following forms:
$latex \[
	\DD{f}{x[row[k]]}{x[row[k]]}
	\; , \;
	\DD{f}{x[row[k]]}{x[col[k]]}
	\; , \;
	\DD{f}{x[col[k]]}{x[row[k]]}
	\; , \;
	\DD{f}{x[col[k]]}{x[col[k]]}
\] $$
for some $latex k $$ between zero and $latex K-1 $$.
All the other terms of the Hessian are zero.

$head size$$
The argument $icode size$$, referred to as $latex n$$ below,
is the dimension of the domain space for $latex f(x)$$.

$head repeat$$
The argument $icode repeat$$ is the number of times
to repeat the test
(with a different value for $icode x$$ corresponding to
each repetition).

$head x$$
The argument $icode x$$ has prototype
$codei%
        CppAD::vector<double>& %x%
%$$
and its size is $latex n$$; i.e., $icode%x%.size() == %size%$$.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector need not change with each repetition.

$head row$$
The argument $icode row$$ has prototype
$codei%
	const CppAD::vector<size_t> %row%
%$$
Its size defines the value $latex K$$.
It contains the row indices for the corresponding function $latex f(x)$$.
All the elements of $icode row$$ are between zero and $latex n-1$$.

$head col$$
The argument $icode col$$ has prototype
$codei%
	const CppAD::vector<size_t> %col%
%$$
Its size must be the same as $icode row$$; i.e., $latex K$$.
It contains the column indices for the corresponding function
$latex f(x)$$.
All the elements of $icode col$$ are between zero and $latex n-1$$.
There are no duplicated entries requested, to be specific,
if $icode%k1% != %k2%$$ then
$codei%
	( %row%[%k1%] , %col%[%k1%] ) != ( %row%[%k2%] , %col%[%k2%] )
%$$
Furthermore, the entries are lower triangular; i.e.,
$codei%
	%col%[%k%] <= %row%[%k%]
%$$.


$head hessian$$
The argument $icode hessian$$ has prototype
$codei%
	CppAD::vector<double>&  hessian
%$$
and its size is $icode K$$.
The input value of its elements does not matter.
The output value of its elements is the Hessian of the function $latex f(x)$$.
To be more specific, for
$latex k = 0 , \ldots , K-1$$,
$latex \[
	\DD{f}{ x[ \R{row}[k] ] }{ x[ \R{col}[k] ]} = \R{hessian} [k]
\] $$

$head n_sweep$$
The input value of $icode n_sweep$$ does not matter. On output,
it is the value $cref/n_sweep/sparse_hessian/n_sweep/$$ corresponding
to the evaluation of $icode hessian$$.
This is also the number of colors corresponding to the
$cref/coloring method/sparse_hessian/work/color_method/$$,
which can be set to $cref/colpack/speed_main/Sparsity Options/colpack/$$,
and is otherwise $code cppad$$.


$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode hessian$$ is used and it is actually
the value of $latex f(x)$$ (derivatives are not computed).

$end
-----------------------------------------------------------------------------
*/
# include <cppad/utility/vector.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/index_sort.hpp>

/*!
\{
\file link_sparse_hessian.cpp
Defines and implement sparse Hessian speed link to package specific code.
*/
namespace {
	using CppAD::vector;

	/*!
	Class used by choose_row_col to determin order of row and column indices
	*/
	class Key {
	public:
		/// row index
		size_t row_;
		/// column index
		size_t col_;
		/// default constructor
		Key(void)
		{ }
		/*!
		Construct from a value for row and col

		\param row
		row value for this key

		\param col
		column value for this key
		*/
		Key(size_t row, size_t col)
		: row_(row), col_(col)
		{ }
		/*!
		Compare this key with another key using < operator

		\param other
		the other key.
		*/
		bool operator<(const Key& other) const
		{	if( row_ == other.row_ )
				return col_ < other.col_;
			return row_ < other.row_;
		}
	};

	/*!
	Function that randomly choose the row and column indices

	\param n [in]
	is the dimension of the argument space for the function f(x).

	\param row [out]
	the input size and elements of \c row do not matter.
	Upon return it is the chosen row indices.

	\param col [out]
	the input size and elements of \c col do not matter.
	Upon return it is the chosen column indices.
	*/
	void choose_row_col(
		size_t          n   ,
		vector<size_t>& row ,
		vector<size_t>& col )
	{	size_t i, j, k, ell;
		size_t max_per_row = 5;

		// generate the row and column indices
		row.resize(0);
		col.resize(0);
		for(i = 0; i < n; i++)
		{	// generate max_per_row random column indices between 0 and i
			vector<double> random(max_per_row);
			CppAD::uniform_01(max_per_row, random);

			// set the indices for this row
			size_t k_start = col.size();
			for(ell = 0; ell < max_per_row; ell++)
			{	// avoid warning when converting double to size_t
				j = std::min(i, size_t( float(random[ell]) * double(i) ) );
				bool ok = true;
				for(k = k_start; k < col.size(); k++)
					ok &= j != col[k];
				if( ok )
				{	row.push_back(i);
					col.push_back(j);
				}
			}
		}
	}
}

/*!
Package specific implementation of a sparse Hessian claculation.

\param size [in]
is the size of the domain space; i.e. specifies \c n.

\param repeat [in]
number of times tha the test is repeated.

\param x [out]
is a vector of size \c n containing
the argument at which the Hessian was evaluated during the last repetition.

\param row [in]
is the row indices correpsonding to non-zero Hessian entries.

\param col [in]
is the column indices corresponding to non-zero Hessian entries;
col.size() == row.size().

\param hessian [out]
is a vector, with hessian.size() == row.size(),
containing the value of the Hessian of f(x)
corresponding to the last repetition.

\param n_sweep [out]
The input value of this parameter does not matter.
Upon return, it is the number of sweeps (colors) corresponding
to the sparse hessian claculation.

\return
is true, if the sparse Hessian speed test is implemented for this package,
and false otherwise.
*/
extern bool link_sparse_hessian(
	size_t                           size      ,
	size_t                           repeat    ,
	const CppAD::vector<size_t>&     row       ,
	const CppAD::vector<size_t>&     col       ,
	CppAD::vector<double>&           x         ,
	CppAD::vector<double>&           hessian   ,
	size_t&                          n_sweep
);

/*!
Is sparse Hessian test avaialable.

\return
true, if spare Hessian available for this package, and false otherwise.
*/
bool available_sparse_hessian(void)
{
	size_t n      = 2;
	size_t repeat = 1;
	vector<double> x(n);
	vector<size_t> row, col;
	choose_row_col(n, row, col);
	size_t K = row.size();
	vector<double> hessian(K);

	size_t n_sweep;
	return link_sparse_hessian(n, repeat, row, col, x, hessian, n_sweep);
}
/*!
Does final sparse Hessian value pass correctness test.

\param is_package_double
if true, we are checking function values instead of derivatives.

\return
true, if correctness test passes, and false otherwise.
*/
bool correct_sparse_hessian(bool is_package_double)
{
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
	size_t n      = 10;
	size_t repeat = 1;
	vector<double> x(n);
	vector<size_t> row, col;
	choose_row_col(n, row, col);
	size_t K = row.size();
	vector<double> hessian(K);
# ifndef NDEBUG
	for(size_t k = 0; k < K; k++)
		CPPAD_ASSERT_UNKNOWN( col[k] <= row[k] );
# endif

	// The double package assumes hessian.size() >= 1
	CPPAD_ASSERT_UNKNOWN( K >= 1 );
	size_t n_sweep;
	link_sparse_hessian(n, repeat, row, col, x, hessian, n_sweep);

	size_t order, size;
	if( is_package_double)
	{	order = 0;  // check function value
		size  = 1;
	}
	else
	{	order = 2;     // check hessian value
		size  = K;
	}
	CppAD::vector<double> check(size);
	CppAD::sparse_hes_fun<double>(n, x, row, col, order, check);
	bool ok = true;
	size_t k;
	for(k = 0; k < size; k++)
		ok &= CppAD::NearEqual(check[k], hessian[k], eps99, eps99);

	return ok;
}

/*!
Sparse Hessian speed test.

\param size
is the dimension of the argument space for this speed test.

\param repeat
is the number of times to repeate the speed test.
*/
void speed_sparse_hessian(size_t size, size_t repeat)
{
	static size_t previous_size = 0;
	static vector<size_t> row, col;
	//
	// free statically allocated memory
	if( size == 0 && repeat == 0 )
	{	row.clear();
		col.clear();
		previous_size = size;
		return;
	}

	size_t n = size;
	vector<double> x(n);
	if( size != previous_size )
	{	choose_row_col(n, row, col);
		previous_size = size;
	}
	size_t K = row.size();
	vector<double> hessian(K);
# ifndef NDEBUG
	for(size_t k = 0; k < K; k++)
		CPPAD_ASSERT_UNKNOWN( col[k] <= row[k] );
# endif

	// note that cppad/sparse_hessian.cpp assumes that x.size() == size
	size_t n_sweep;
	link_sparse_hessian(n, repeat, row, col, x, hessian, n_sweep);
	return;
}

/*!
Sparse Hessian speed test information.

\param size [in]
is the \c size parameter in the corresponding call to speed_sparse_hessian.

\param n_sweep [out]
The input value of this parameter does not matter.
Upon return, it is the value \c n_sweep retruned by the corresponding
call to \c link_sparse_hessian.
*/
void info_sparse_hessian(size_t size, size_t& n_sweep)
{	size_t n      = size;
	size_t repeat = 1;
	vector<size_t> row, col;
	choose_row_col(n, row, col);

	// note that cppad/speed/sparse_hessian.cpp assumes that x.size()
	// is the size corresponding to this test
	vector<double> x(n);
	size_t K = row.size();
	vector<double> hessian(K);
	link_sparse_hessian(n, repeat, row, col, x, hessian, n_sweep);
	return;
}
