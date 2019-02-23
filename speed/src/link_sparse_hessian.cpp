/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/utility/vector.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/index_sort.hpp>
// BEGIN PROTOTYPE
extern bool link_sparse_hessian(
    size_t                           size      ,
    size_t                           repeat    ,
    const CppAD::vector<size_t>&     row       ,
    const CppAD::vector<size_t>&     col       ,
    CppAD::vector<double>&           x         ,
    CppAD::vector<double>&           hessian   ,
    size_t&                          n_color
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
$begin link_sparse_hessian$$
$spell
    const
    bool
    CppAD
    cppad
    colpack
$$


$section Speed Testing Sparse Hessian$$

$head Prototype$$
$srcfile%speed/src/link_sparse_hessian.cpp%
    0%// BEGIN PROTOTYPE%// END PROTOTYPE%0
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
The size of $icode x$$ is $latex n$$; i.e., $icode%x%.size() == %size%$$.
The input value of the elements of $icode x$$ does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector need not change with each repetition.

$head row$$
The size of the vector $icode row$$ defines the value $latex K$$.
The input value of its elements does not matter.
On output,
all the elements of $icode row$$ are between zero and $latex n-1$$.

$head col$$
The argument $icode col$$ is a vector with size $latex K$$.
The input value of its elements does not matter.
On output,
all the elements of $icode col$$ are between zero and $latex n-1$$.

$head Row Major$$
The indices $icode row$$ and $icode col$$ are in row major order; i.e.,
for each $icode%k% < %row%.size()-2%$$
$codei%
    %row%[%k%] <= %row%[%k%+1]
%$$
and if $icode%row%[%k%] == %row%[%k%+1]%$$ then
$codei%
    %col%[%k%] < %col%[%k%+1]
%$$

$head Lower Triangular$$
Only the lower triangle of the Hessian is included.
$codei%
    %col%[%k%] <= %row%[%k%]
%$$.


$head hessian$$
The size of $icode hessian$$ is $icode K$$.
The input value of its elements does not matter.
The output value of its elements is the Hessian of the function $latex f(x)$$.
To be more specific, for
$latex k = 0 , \ldots , K-1$$,
$latex \[
    \DD{f}{ x[ \R{row}[k] ] }{ x[ \R{col}[k] ]} = \R{hessian} [k]
\] $$

$head n_color$$
The input value of $icode n_color$$ does not matter. On output,
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

/*!
\{
\file link_sparse_hessian.cpp
Defines and implement sparse Hessian speed link to package specific code.
*/
namespace {
    using CppAD::vector;

    /*!
    Function that randomly choose the row and column indices
    (and returns them in row major order)

    \param n [in]
    is the dimension of the argument space for the function f(x).

    \param row [out]
    the input size and elements of row do not matter.
    Upon return it is the chosen row indices.

    \param col [out]
    the input size and elements of col do not matter.
    Upon return it is the chosen column indices.
    */
    void choose_row_col(
        size_t          n   ,
        vector<size_t>& row ,
        vector<size_t>& col )
    {
        // maximum number of entries per row
        size_t max_per_row = 5;

        // random choices for each row, and correspond sort order
        vector<double> random_01(max_per_row);
        vector<size_t> random_index(max_per_row), order(max_per_row);

        // generate the row and column indices
        row.resize(0);
        col.resize(0);
        for(size_t i = 0; i < n; i++)
        {   // generate max_per_row random values between 0 and 1
            CppAD::uniform_01(max_per_row, random_01);

            // convert to column indices between 0 and i
            for(size_t k = 0; k < max_per_row; ++k)
            {   random_index[k] = size_t( random_01[k] * double(i) );
                random_index[k] = std::min(random_index[k], i);
            }

            // determine the sort order for the indices
            CppAD::index_sort(random_index, order);

            // set the indices for this row
            for(size_t k = 0; k < max_per_row; k++)
            {   size_t j = random_index[ order[k] ];
                bool ok = k == 0;
                if( ! ok )
                    ok = random_index[ order[k-1] ] < j;
                if( ok )
                {   row.push_back(i);
                    col.push_back(j);
                }
            }
        }
    }
}


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

    size_t n_color;
    return link_sparse_hessian(n, repeat, row, col, x, hessian, n_color);
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
    size_t n_color;
    link_sparse_hessian(n, repeat, row, col, x, hessian, n_color);

    size_t order, size;
    if( is_package_double)
    {   order = 0;  // check function value
        size  = 1;
    }
    else
    {   order = 2;     // check hessian value
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
    {   row.clear();
        col.clear();
        previous_size = size;
        return;
    }

    size_t n = size;
    vector<double> x(n);
    if( size != previous_size )
    {   choose_row_col(n, row, col);
        previous_size = size;
    }
    size_t K = row.size();
    vector<double> hessian(K);
# ifndef NDEBUG
    for(size_t k = 0; k < K; k++)
        CPPAD_ASSERT_UNKNOWN( col[k] <= row[k] );
# endif

    // note that cppad/sparse_hessian.cpp assumes that x.size() == size
    size_t n_color;
    link_sparse_hessian(n, repeat, row, col, x, hessian, n_color);
    return;
}

/*!
Sparse Hessian speed test information.

\param size [in]
is the size parameter in the corresponding call to speed_sparse_hessian.

\param n_color [out]
The input value of this parameter does not matter.
Upon return, it is the value n_color retruned by the corresponding
call to link_sparse_hessian.
*/
void info_sparse_hessian(size_t size, size_t& n_color)
{   size_t n      = size;
    size_t repeat = 1;
    vector<size_t> row, col;
    choose_row_col(n, row, col);

    // note that cppad/speed/sparse_hessian.cpp assumes that x.size()
    // is the size corresponding to this test
    vector<double> x(n);
    size_t K = row.size();
    vector<double> hessian(K);
    link_sparse_hessian(n, repeat, row, col, x, hessian, n_color);
    return;
}
