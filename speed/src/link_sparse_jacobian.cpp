/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_sparse_jacobian$$
$spell
	colpack
	cppad
	const
	bool
	CppAD
	Jacobian
$$


$section Speed Testing Sparse Jacobian$$
$mindex link_sparse_jacobian test$$

$head Prototype$$
$codei%extern bool link_sparse_jacobian(
	size_t                       %size%      ,
	size_t                       %repeat%    ,
	size_t                       %m%         ,
	const CppAD::vector<size_t>& %row%       ,
	const CppAD::vector<size_t>& %col%       ,
	      CppAD::vector<double>& %x%         ,
	      CppAD::vector<double>& %jacobian%  ,
	      size_t&                %n_sweep%
);
%$$

$head Method$$
Given a range space dimension $icode m$$
the row index vector $latex row$$, and column index vector $latex col$$,
a corresponding function $latex f : \B{R}^n \rightarrow \B{R}^m $$
is defined by $cref sparse_jac_fun$$.
The non-zero entries in the Jacobian of this function have the form
$latex \[
	\D{f[row[k]]}{x[col[k]]]}
\] $$
for some $latex k$$ between zero and $icode%K% = %row%.size()-1%$$.
All the other terms of the Jacobian are zero.


$head size$$
The argument $icode size$$, referred to as $latex n$$ below,
is the dimension of the domain space for $latex f(x)$$.

$head repeat$$
The argument $icode repeat$$ is the number of times
to repeat the test
(with a different value for $icode x$$ corresponding to
each repetition).

$head m$$
Is the dimension of the range space for the function $latex f(x)$$.

$head row$$
The size of the vector $icode row$$ defines the value $latex K$$.
All the elements of $icode row$$ are between zero and $latex m-1$$.

$head col$$
The argument $icode col$$ is a vector with size $latex K$$.
The input value of its elements does not matter.
On output, it has been set the column index vector
for the last repetition.
All the elements of $icode col$$ are between zero and $latex n-1$$.
There are no duplicate row and column entires; i.e., if $icode%j% != %k%$$,
$codei%
	%row%[%j%] != %row%[%k%] || %col%[%j%] != %col%[%k%]
%$$

$head x$$
The argument $icode x$$ has prototype
$codei%
        CppAD::vector<double>& %x%
%$$
and its size is $latex n$$; i.e., $icode%x%.size() == %size%$$.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated and placed in $icode jacobian$$.
The value of this vector need not change with each repetition.

$head jacobian$$
The argument $icode jacobian$$ has prototype
$codei%
        CppAD::vector<double>& %jacobian%
%$$
and its size is $icode K$$.
The input value of its elements does not matter.
The output value of its elements is the Jacobian of the function $latex f(x)$$.
To be more specific, for
$latex k = 0 , \ldots , K - 1$$,
$latex \[
	\D{f[ \R{row}[k] ]}{x[ \R{col}[k] ]} (x) = \R{jacobian} [k]
\] $$

$head n_sweep$$
The input value of $icode n_sweep$$ does not matter. On output,
it is the value $cref/n_sweep/sparse_jacobian/n_sweep/$$ corresponding
to the evaluation of $icode jacobian$$.
This is also the number of colors corresponding to the
$cref/coloring method/sparse_jacobian/work/color_method/$$,
which can be set to $cref/colpack/speed_main/Sparsity Options/colpack/$$,
and is otherwise $code cppad$$.

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first $latex m$$
elements of $icode jacobian$$ are used and they are set to
the value of $latex f(x)$$.

$end
-----------------------------------------------------------------------------
*/
# include <cppad/utility/vector.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/index_sort.hpp>

/*!
\{
\file link_sparse_jacobian.cpp
Defines and implement sparse Jacobian speed link to package specific code.
*/
namespace {
	using CppAD::vector;

	/*!
	Class used by choose_row_col to determine order of row and column indices
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
	is the dimension of the domain space for the function f(x).

	\param m [in]
	is the dimension of the range space for the function f(x).

	\param row [out]
	the input size and elements of \c row do not matter.
	Upon return it is the chosen row indices.

	\param col [out]
	the input size and elements of \c col do not matter.
	Upon return it is the chosen column indices.
	*/
	void choose_row_col(
		size_t          n   ,
		size_t          m   ,
		vector<size_t>& row ,
		vector<size_t>& col )
	{	size_t r, c, k, K = 5 * std::max(m, n);

		// get the random indices
		vector<double>  random(2 * K);
		CppAD::uniform_01(2 * K, random);

		// sort the temporary row and colunn choices
		vector<Key>   keys(K);
		vector<size_t> ind(K);
		for(k = 0; k < K; k++)
		{	// avoid warning when converting double to size_t
			r = size_t( float( double(m) * random[k] ) );
			r = std::min(m-1, r);
			//
			c = size_t( float( double(n) * random[k + K] ) );
			c = std::min(n-1, c);
			//
			keys[k] = Key(r, c);
		}
		CppAD::index_sort(keys, ind);

		// remove duplicates while setting the return value for row and col
		row.resize(0);
		col.resize(0);
		size_t r_previous = keys[ ind[0] ].row_;
		size_t c_previous = keys[ ind[0] ].col_;
		CPPAD_ASSERT_UNKNOWN( r_previous < m && c_previous < n );
		row.push_back(r_previous);
		col.push_back(c_previous);
		for(k = 1; k < K; k++)
		{	r = keys[ ind[k] ].row_;
			c = keys[ ind[k] ].col_;
			CPPAD_ASSERT_UNKNOWN( r < m && c < n );
			if( r != r_previous || c != c_previous)
			{	row.push_back(r);
				col.push_back(c);
			}
			r_previous = r;
			c_previous = c;
		}
	}
}

/*!
Package specific implementation of a sparse Jacobian claculation.

\param size [in]
is the size of the domain space; i.e. specifies \c n.

\param repeat [in]
number of times tha the test is repeated.

\param m [in]
is the dimension of the range space for f(x).

\param row [in]
is the row indices correpsonding to non-zero Jacobian entries.

\param col [in]
is the column indices corresponding to non-zero Jacobian entries.

\param x [out]
is a vector of size \c n containing
the argument at which the Jacobian was evaluated during the last repetition.

\param jacobian [out]
is a vector with size <code>row.size()</code>
containing the value of the Jacobian of f(x)
corresponding to the last repetition.

\param n_sweep [out]
The input value of this parameter does not matter.
Upon return, it is the number of sweeps (colors) corresponding
to the sparse jacobian claculation.

\return
is true, if the sparse Jacobian speed test is implemented for this package,
and false otherwise.
*/
extern bool link_sparse_jacobian(
	size_t                            size      ,
	size_t                            repeat    ,
	size_t                            m         ,
	const CppAD::vector<size_t>&      row       ,
	const CppAD::vector<size_t>&      col       ,
	      CppAD::vector<double>&      x         ,
	      CppAD::vector<double>&      jacobian  ,
	      size_t&                     n_sweep
);

/*!
Is sparse Jacobian test avaialable.

\return
true, if spare Jacobian available for this package, and false otherwise.
*/
bool available_sparse_jacobian(void)
{	size_t n      = 10;
	size_t m      = 2 * n;
	size_t repeat = 1;
	vector<size_t> row, col;
	choose_row_col(n, m, row, col);

	vector<double> x(n);
	size_t K = row.size();
	vector<double> jacobian(K);
	size_t         n_sweep;
	return link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_sweep);
}
/*!
Does final sparse Jacobian value pass correctness test.

\param is_package_double [in]
if true, we are checking function values instead of derivatives.

\return
true, if correctness test passes, and false otherwise.
*/
bool correct_sparse_jacobian(bool is_package_double)
{	size_t i, k;
	bool ok       = true;
	double eps    = 10. * CppAD::numeric_limits<double>::epsilon();
	size_t n      = 5;
	size_t m      = 2 * n;
	size_t repeat = 1;
	vector<size_t> row, col;
	choose_row_col(n, m, row, col);

	size_t K = row.size();
	// The double package assumes jacobian.size() >= m
	CPPAD_ASSERT_UNKNOWN( K >= m );
	vector<double> x(n);
	vector<double> jacobian(K);
	size_t         n_sweep;
	link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_sweep);

	if( is_package_double)
	{
		// check f(x)
		size_t order = 0;
		vector<double> check(m);
		CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, check);
		for(i = 0; i < m; i++)
			ok &= CppAD::NearEqual(check[i], jacobian[i], eps, eps);

		return ok;
	}
    // check f'(x)
	size_t order = 1;
	vector<double> check(K);
	CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, check);
	for(k = 0; k < K; k++)
		ok &= CppAD::NearEqual(check[k], jacobian[k], eps, eps);

	return ok;
}
/*!
Sparse Jacobian speed test.

\param size [in]
is the dimension of the argument space for this speed test.

\param repeat [in]
is the number of times to repeate the speed test.
*/
void speed_sparse_jacobian(size_t size, size_t repeat)
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

	size_t n   = size;
	size_t m   = 2 * n;
	if( size != previous_size)
	{	choose_row_col(n, m, row, col);
		previous_size = size;
	}

	// note that cppad/sparse_jacobian.cpp assumes that x.size()
	// is the size corresponding to this test
	vector<double> x(n);
	size_t K = row.size();
	vector<double> jacobian(K);
	size_t         n_sweep;
	link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_sweep);
	return;
}
/*!
Sparse Jacobian speed test information.

\param size [in]
is the \c size parameter in the corresponding call to speed_sparse_jacobian.

\param n_sweep [out]
The input value of this parameter does not matter.
Upon return, it is the value \c n_sweep retruned by the corresponding
call to \c link_sparse_jacobian.
*/
void info_sparse_jacobian(size_t size, size_t& n_sweep)
{	size_t n      = size;
	size_t m      = 2 * n;
	size_t repeat = 1;
	vector<size_t> row, col;
	choose_row_col(n, m, row, col);

	// note that cppad/sparse_jacobian.cpp assumes that x.size()
	// is the size corresponding to this test
	vector<double> x(n);
	size_t K = row.size();
	vector<double> jacobian(K);
	link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_sweep);
	return;
}
