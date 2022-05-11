# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FOR_TYPE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FOR_TYPE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_four_lin_ode_for_type.hpp$$
$spell
    Jacobian
    jac
$$

$section
Atomic Linear ODE Forward Type Calculation: Example Implementation$$

$head Purpose$$
The $code for_type$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/for_type/atomic_four_for_type/$$.

$head Representation$$
We use the following representation for
$cref/y(x)/atomic_four_lin_ode/y(x)/$$:
$latex \[
y(x) = \exp [ r A(x) ] b(x) = \sum_{k=0}^\infty \frac{r^k}{k!} A(x)^k b(x)
\] $$
Define $latex v^0 (x) = b(x)$$ and for $latex k = 1, 2, \ldots$$,
$latex v^k (x) = (r / k) A(x) v^{k-1} (x)$$.
Using this notation,
$latex \[
y(x) = \sum_{k=0}^\infty v^k (x)
\] $$

$head Notation$$
We use $latex \R{T} ( s )$$ to denote the ad_type of a scalar value $latex s$$.
There are four possible
$cref/ad_types/atomic_four_for_type/ad_type/$$:
identical_zero, constant, dynamic, and variable in that order.

$head Theory$$
This routine must calculate the following value for
$latex i = 0, \ldots, m-1$$; see $cref/m/atomic_four_lin_ode/y(x)/m/$$:
$latex \[
    \R{T} [ y_i (x) ] = \max_k \R{T} [  v_i^k (x) ]
\] $$
The type $latex \R{T} [ v_i^0 (x) ] = \R{T}[ b_i (x) ]$$.
This is easy to calculate given the type of the components of $icode x$$;
see $cref/b(x)/atomic_four_lin_ode/x/b(x)/$$.
Furthermore, for $latex k > 0$$
$latex \[
v_i^k (x)
=
\frac{r}{k} \sum_{j=0}^{m-1} A_{i,j} (x) v_j^{k-1} (x)
\] $$
$latex \[
\R{T} [ v_i^k (x) ]
=
\max_j \R{T} [ A_{i,j} (x) v_j^{k-1} (x) ]
\]
$$
$latex \[
\R{T} [ A_{i,j} (x) v_j^k (x) ]
=
\left\{ \begin{array}{ll}
    \R{identical\_zero} &
    \R{if} A_{i,j} (x) \W{\R{or}} v_j^{k-1} (x) \W{\R{is}} \R{identical\_zero}
    \\
    \max\{ \R{T} [ A_{i,j} (x) ] \W{,} \R{T} [ v_j^{k-1} (x) ] \} &
    \R{otherwise}
\end{array} \right.
\] $$
If $latex A_{i,j} (x)$$ is not in the sparsity
$cref/pattern/atomic_four_lin_ode/pattern/$$, it is identically zero.
Furthermore we are allowing for the case where
$latex A_{i,j} (x)$$ is in the pattern and it is identically zero; i.e.,
the sparsity pattern is not efficient as it could be.
The type $latex \R{T} [ A_{i,j} (x) ]$$ for components in the sparsity pattern
is easy to calculate given the type of the components of $icode x$$;
see $cref/A(x)/atomic_four_lin_ode/x/A(x)/$$.
Suppose $latex \ell$$ is such that for all $latex i$$
$latex \[
    \R{T} [ v_i^\ell (x) ] \leq \max_{k < \ell} \R{T} [ v_i^k (x) ]
\] $$
It follows that
$latex \[
    \R{T} [ v_j^{\ell+1} (x) ]  = \max_j \R{T} [ A_{i,j} (x) v_j^\ell (x) ]
\] $$
$latex \[
    \R{T} [ v_j^{\ell+1} (x) ]
    \leq
    \max_{k < \ell} \max_j \R{T} [ A_{i,j} (x) v_j^k (x) ]
\] $$
$latex \[
    \R{T} [ v_j^{\ell+1} (x) ]
    \leq
    \max_{k < \ell} \R{T} [ v_i^k (x) ]
\] $$
From this it is clear that
$latex \[
    \R{T} [ y_i (x) ] = \max_{k < \ell} \R{T} [  v_i^k (x) ]
\] $$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// for_type override
template <class Base>
bool atomic_lin_ode<Base>::for_type(
    size_t                                     call_id     ,
    const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
    CppAD::vector<CppAD::ad_type_enum>&        type_y      )
{
    // nnz
    Base      r;
    sparse_rc pattern;
    bool      transpose;
    get(call_id, r, pattern, transpose);
    size_t nnz = pattern.nnz();
    //
    // m
    size_t m     = type_y.size();
    CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
    //
    // type_x
    CPPAD_ASSERT_UNKNOWN( type_x.size() == nnz + m );
    //
    // type_y[i] = type_b[i]
    // type_y[i] = max T[ v_i^k (x) ] for k = 0
    for(size_t i = 0; i < m; ++i)
        type_y[i] = type_x[nnz + i];
    //
    // change
    // Did type_y change during the previous iteration of the while loop
    bool change = true;
    while(change)
    {   change = false;
        // we use k = 1, 2, ... to denote the pass through this loop
        // type_y[i] = max q < k T[ v_i^q (x) ]
        //
        for(size_t p = 0; p < nnz; ++p)
        {   size_t i = pattern.row()[p];
            size_t j = pattern.col()[p];
            if( transpose )
                std::swap(i, j);
            //
            // type_y[i], change
            if( type_x[p] != identical_zero_enum )
            {   // A_ij (x) is not identically zero
                if( type_y[j] > type_y[i] )
                {   change = true;
                    type_y[i] = type_y[j];
                 }
            }
            //
            // type_y[i], change
            if( type_y[j] != identical_zero_enum )
            {   // There is a q < k such that v_j^q (x) not identically zero
                if( type_x[p] > type_y[i] )
                {   change = true;
                    type_y[i] = type_x[p];
                }
            }
        }
    }
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
