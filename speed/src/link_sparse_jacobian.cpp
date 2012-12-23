/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin link_sparse_jacobian$$
$spell
	bool
	CppAD
	Jacobian
$$

$index link_sparse_jacobian$$
$index sparse, speed test$$
$index speed, test sparse$$
$index test, sparse speed$$

$section Speed Testing Sparse Jacobian$$

$head Prototype$$
$codei%extern bool link_sparse_jacobian(
	size_t                 %size%      ,
	size_t                 %repeat%    ,
	size_t                 %m%         ,
	CppAD::vector<double> &%x%         ,
	CppAD::vector<size_t> &%row%       ,
	CppAD::vector<size_t> &%col%       , 
	CppAD::vector<double> &%jacobian%
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
The argument $icode repeat$$ is the number of different functions
$latex f(x)$$ for which the Jacobian is computed for.
Each function corresponds to a randomly chosen index vectors, i.e.,
for each repetition a random choice is made for
$latex row[k]$$ and $latex col[k]$$ for $latex k = 0 , \ldots , K-1$$.

$head m$$
Is the dimension of the range space for the function $latex f(x)$$.

$head x$$
The argument $icode x$$ has prototype
$codei%
        CppAD::vector<double> &%x%
%$$
and its size is $latex n$$; i.e., $icode%x%.size() == %size%$$.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector need not change with each repetition.

$head row$$
The size of the vector $icode row$$ defines the value $latex K$$.
The input value of the elements of $icode row$$ does not matter.
On output, it has been set the row index vector
for the last repetition.
All the elements of $icode row$$ must are between zero and $latex m-1$$.

$head col$$
The argument $icode col$$ is a vector with size $latex K$$.
The input value of its elements does not matter.
On output, it has been set the column index vector
for the last repetition.
All the elements of $icode col$$ must are between zero and $latex n-1$$.
$pre

$$
There are no duplicate row and column entires; i.e., if $icode%j% != %k%$$,
$codei%
	%row%[%j%] != %row%[%k%] || %col%[%j%] != %col%[%k%]
%$$

$head jacobian$$
The argument $icode jacobian$$ is a vector with 
$latex m \times n$$ elements.
The input value of its elements does not matter. 
The output value of its elements is the Jacobian of the function $latex f(x)$$
that corresponds to output values of 
$icode row$$, $icode col$$, and $icode x$$.
To be more specific, for
$latex i = 0 , \ldots , m - 1$$,
$latex j = 0 , \ldots , n-1$$,
$latex \[
	\D{f[i]}{x[j]} (x) = jacobian [ i * n + j ]
\] $$

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first $latex m$$ 
elements of $icode jacobian$$ are used and they are set to
the value of $latex f(x)$$.

$end 
-----------------------------------------------------------------------------
*/
# include <cppad/vector.hpp>
# include <cppad/near_equal.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/index_sort.hpp>

/*!
\defgroup link_sparse_jacobian_cpp link_sparse_jacobian.cpp
\{
\file link_sparse_jacobian.cpp
Defines and implement sparse Jacobian speed link to package specific code.
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
		{	r = size_t( m * random[k] );
			r = std::min(m-1, r);
			//
			c = size_t( n * random[k + K] );
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

\param x [out]
is a vector of size \c n containing
the argument at which the Jacobian was evaluated during the last repetition.

\param row [in]
is the row indices correpsonding to non-zero Jacobian entries.

\param col [in]
is the column indices corresponding to non-zero Jacobian entries.

\param jacobian [out]
is a vector with size <code>m * n</code> 
containing the value of the Jacobian of f(x) 
corresponding to the last repetition.

\return
is true, if the sparse Jacobian speed test is implemented for this package,
and false otherwise.
*/
extern bool link_sparse_jacobian(
	size_t                     size       ,
	size_t                     repeat     ,
	size_t                     m          ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<size_t>      &row       ,
	CppAD::vector<size_t>      &col       , 
	CppAD::vector<double>      &jacobian
);

/*!
Is sparse Jacobian test avaialable.

\return
true, if spare Jacobian available for this package, and false otherwise.
*/
bool available_sparse_jacobian(void)
{	size_t n      = 10;
	size_t m      = 3 * n;
	size_t repeat = 1;
	vector<double> x(n);
	vector<double> jacobian(m * n);
	vector<size_t> row, col; 
	choose_row_col(n, m, row, col);

	return link_sparse_jacobian(n, repeat, m, x, row, col, jacobian);
}
/*!
Does final sparse Jacobian value pass correctness test.

\return
true, if correctness test passes, and false otherwise.
*/
bool correct_sparse_jacobian(bool is_package_double)
{	size_t i, j;
	bool ok       = true;
	double eps    = 10. * CppAD::numeric_limits<double>::epsilon();
	size_t n      = 5;
	size_t m      = 3 * n;
	size_t repeat = 1;
	vector<double> x(n);
	vector<double> jacobian(m * n);
	vector<size_t> row, col;
	choose_row_col(n, m, row, col);

	link_sparse_jacobian(n, repeat, m, x, row, col, jacobian);

	if( is_package_double)
	{	// check f(x)
		size_t order = 0;
		vector<double> check(m);
		CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, check);
		for(i = 0; i < m; i++)
		{	double u = check[i];
			double v = jacobian[i];
			ok &= CppAD::NearEqual(u, v, eps, eps);
		}
		return ok;
	}
     // check f'(x) 
	size_t order = 1;
	size_t size  = m * n;
	vector<double> check(size);
	CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, check);
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
		{	double u = check[ i * n + j ];
			double v = jacobian[ i * n + j ];
			ok &= CppAD::NearEqual(u, v, eps, eps);
		}
	}
	return ok;
}
/*!
Sparse Jacobian speed test.

\param size
is the dimension of the argument space for this speed test.

\param repeat
is the number of times to repeate the speed test.
*/
void speed_sparse_jacobian(size_t size, size_t repeat)
{	size_t n   = size;	
	size_t m   = 3 * n;
	vector<double> x(n);
	vector<double> jacobian(m * n);
	vector<size_t> row, col;
	choose_row_col(n, m, row, col);

	// note that cppad/sparse_jacobian.cpp assumes that x.size()
	// is the size corresponding to this test
	link_sparse_jacobian(n, repeat, m, x, row, col, jacobian);
	return;
}
/*! \} */
