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
    for(size_t i = 0; i < m; ++i)
        type_y[i] = type_x[nnz + i];
    //
    // change
    bool change = true;
    while(change)
    {   change = false;
        //
        for(size_t k = 0; k < nnz; ++k)
        {   size_t i = pattern.row()[k];
            size_t j = pattern.col()[k];
            if( transpose )
                std::swap(i, j);
            //
            // type_y[i], change
            if( type_x[k] != identical_zero_enum )
            {   if( type_y[j] > type_y[i] )
                {   change = true;
                    type_y[i] = type_y[j];
                 }
            }
            if( type_y[j] != identical_zero_enum )
            {   if( type_x[k] > type_y[i] )
                {   change = true;
                    type_y[i] = type_x[k];
                }
            }
        }
    }
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
