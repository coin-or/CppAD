/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/utility/near_equal.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/index_sort.hpp>

# include "link_sparse_jacobian.hpp"

/*!
\{
\file link_sparse_jacobian.cpp
Defines and implement sparse Jacobian speed link to package specific code.
*/
namespace {
    using CppAD::vector;

    /*!
    Class used by choose_row_col to determine
    row major order of row and column indices.
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
        Compare this key with another key with < being row major order

        \param other
        the other key.
        */
        bool operator<(const Key& other) const
        {   if( row_ == other.row_ )
                return col_ < other.col_;
            return row_ < other.row_;
        }
    };

    /*!
    Function that randomly choose the row and column indices
    (and returns them in row major order)

    \param n [in]
    is the dimension of the domain space for the function f(x).

    \param m [in]
    is the dimension of the range space for the function f(x).

    \param row [out]
    the input size and elements of row do not matter.
    Upon return it is the chosen row indices.

    \param col [out]
    the input size and elements of col do not matter.
    Upon return it is the chosen column indices.
    */
    void choose_row_col(
        size_t          n   ,
        size_t          m   ,
        vector<size_t>& row ,
        vector<size_t>& col )
    {   size_t r, c, k, K = 5 * std::max(m, n);

        // get the random indices
        vector<double>  random(2 * K);
        CppAD::uniform_01(2 * K, random);

        // sort the temporary row and colunn choices
        vector<Key>   keys(K);
        vector<size_t> ind(K);
        for(k = 0; k < K; k++)
        {   // avoid warning when converting double to size_t
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
        // in row major order
        row.resize(0);
        col.resize(0);
        size_t r_previous = keys[ ind[0] ].row_;
        size_t c_previous = keys[ ind[0] ].col_;
        CPPAD_ASSERT_UNKNOWN( r_previous < m && c_previous < n );
        row.push_back(r_previous);
        col.push_back(c_previous);
        for(k = 1; k < K; k++)
        {   r = keys[ ind[k] ].row_;
            c = keys[ ind[k] ].col_;
            CPPAD_ASSERT_UNKNOWN( r < m && c < n );
            if( r != r_previous || c != c_previous)
            {   row.push_back(r);
                col.push_back(c);
            }
            r_previous = r;
            c_previous = c;
        }
    }
}


/*!
Is sparse Jacobian test avaialable.

\return
true, if spare Jacobian available for this package, and false otherwise.
*/
bool available_sparse_jacobian(void)
{   size_t n      = 10;
    size_t m      = 2 * n;
    size_t repeat = 1;
    vector<size_t> row, col;
    choose_row_col(n, m, row, col);

    vector<double> x(n);
    size_t K = row.size();
    vector<double> jacobian(K);
    size_t         n_color;
    return link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_color);
}
/*!
Does final sparse Jacobian value pass correctness test.

\param is_package_double [in]
if true, we are checking function values instead of derivatives.

\return
true, if correctness test passes, and false otherwise.
*/
bool correct_sparse_jacobian(bool is_package_double)
{   size_t i, k;
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
    size_t         n_color;
    link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_color);

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
    {   row.clear();
        col.clear();
        previous_size = size;
        return;
    }

    size_t n   = size;
    size_t m   = 2 * n;
    if( size != previous_size)
    {   choose_row_col(n, m, row, col);
        previous_size = size;
    }

    // note that cppad/sparse_jacobian.cpp assumes that x.size()
    // is the size corresponding to this test
    vector<double> x(n);
    size_t K = row.size();
    vector<double> jacobian(K);
    size_t         n_color;
    link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_color);
    return;
}
/*!
Sparse Jacobian speed test information.

\param size [in]
is the size parameter in the corresponding call to speed_sparse_jacobian.

\param n_color [out]
The input value of this parameter does not matter.
Upon return, it is the value n_color retruned by the corresponding
call to link_sparse_jacobian.
*/
void info_sparse_jacobian(size_t size, size_t& n_color)
{   size_t n      = size;
    size_t m      = 2 * n;
    size_t repeat = 1;
    vector<size_t> row, col;
    choose_row_col(n, m, row, col);

    // note that cppad/sparse_jacobian.cpp assumes that x.size()
    // is the size corresponding to this test
    vector<double> x(n);
    size_t K = row.size();
    vector<double> jacobian(K);
    link_sparse_jacobian(n, repeat, m, row, col, x, jacobian, n_color);
    return;
}
