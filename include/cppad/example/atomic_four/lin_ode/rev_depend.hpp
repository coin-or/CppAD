# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_REV_DEPEND_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_REV_DEPEND_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin atomic_four_lin_ode_rev_depend.hpp$$
$spell
    Jacobian
    jac
    vk
    nnz
$$

$section
Atomic Linear ODE Forward Type Calculation: Example Implementation$$

$head Purpose$$
The $code rev_depend$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/rev_depend/atomic_four_rev_depend/$$.

$head Notation$$
We use the notation:
$cref/call_id/atomic_four_lin_ode/call_id/$$
$cref/r/atomic_four_lin_ode/r/$$
$cref/pattern/atomic_four_lin_ode/pattern/$$
$cref/transpose/atomic_four_lin_ode/transpose/$$
$cref/nnz/atomic_four_lin_ode/pattern/nnz/$$,
$cref/row/atomic_four_lin_ode/pattern/row/$$,
$cref/col/atomic_four_lin_ode/pattern/col/$$,
$cref/x/atomic_four_lin_ode/x/$$,
$cref/n/atomic_four_lin_ode/x/n/$$,
$cref/A(x)/atomic_four_lin_ode/x/A(x)/$$,
$cref/b(x)/atomic_four_lin_ode/x/b(x)/$$,
$cref/y(x)/atomic_four_lin_ode/y(x)/$$,
$cref/m/atomic_four_lin_ode/y(x)/m/$$,
$cref/vk(x)/atomic_four_lin_ode/vk(x)/$$,
and the following additional notation:

$subhead wk(x)$$
Note that the factor $latex r / k$$,
in the definition of $latex v^k (x)$$,
is constant (with respect to the variables).
Hence it suffices to compute the dependency for
$latex \[
h (x) = \sum_{k=0}^4 w^k (x)
\] $$
where $latex w^0 (x) = b(x)$$ and for $latex k = 1, 2, \ldots$$,
$latex w^k (x) = A(x) w^{k-1} (x)$$.


$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// rev_depend override
template <class Base>
bool atomic_lin_ode<Base>::rev_depend(
    size_t                                         call_id,
    CppAD::vector<bool>&                           ident_zero_x,
    CppAD::vector<bool>&                           depend_x,
    const CppAD::vector<bool>&                     depend_y
)
{
    // nnz
    Base      r;
    Base      step;
    sparse_rc pattern;
    bool      transpose;
    get(call_id, r, step, pattern, transpose);
    size_t nnz = pattern.nnz();
    //
    // m
    size_t m = depend_y.size();
    CPPAD_ASSERT_UNKNOWN( ident_zero_x.size() == depend_x.size() );
    CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
    //
    // depend_w
    CppAD::vector<bool> depend_w = depend_y;
    //
    // depend_x
    for(size_t p = 0; p < nnz; ++p)
        depend_x[p] = false;
    for(size_t i = 0; i < m; ++i)
        depend_x[nnz + i] = depend_y[i];
    //
    // change
    // Did depend_w change during the previous iteration of the while loop
    bool change = true;
    while(change)
    {   change = false;
        // we use k = 1, 2, ... to denote the pass through this loop
        //
        // depend_w, depend_x
        // include depenency for w^k (x)
        for(size_t p = 0; p < nnz; ++p) if( ! ident_zero_x[p] )
        {   size_t i = pattern.row()[p];
            size_t j = pattern.col()[p];
            if( transpose )
                std::swap(i, j);
            //
            // back propagate depenency on y
            if( depend_w[i] && ! depend_w[j] )
            {   change      = true;
                depend_w[j] = true;
            }
            //
            // depend_x
            // for propage dependency on A_{i,j}
            if( depend_w[i] & ! depend_x[p] )
            {   change      = true;
                depend_x[p] = true;
            }
        }
    }
    //
    // depend_x
    // terms corresponding to b(x)
    for(size_t i = 0; i < m; ++i)
        depend_x[nnz + i] = depend_w[i];
    //
    return true;
}
} // END_CPPAD_NAMESPACE

// END C++
# endif
