# ifndef CPPAD_EXAMPLE_ATOMIC_VECTOR_FOR_TYPE_HPP
# define CPPAD_EXAMPLE_ATOMIC_VECTOR_FOR_TYPE_HPP
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
$begin atomic_vector_for_type.hpp$$
$spell
    Jacobian
    jac
$$

$section Atomic Vector Forward Type Calculation: Example Implementation$$

$head Purpose$$
The $code for_type$$ routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
$cref/for_type/atomic_four_for_type/$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_vector.hpp>
//
// for_type override
template <class Base>
bool atomic_vector<Base>::for_type(
    size_t                                     call_id     ,
    const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
    CppAD::vector<CppAD::ad_type_enum>&        type_y      )
{
    // n, m
    size_t n     = type_x.size();
    size_t m     = type_y.size();
    //
    // type_y
    if( n == m )
    {   // unary operator
        for(size_t i = 0; i < m; ++i)
            type_y[i] = type_x[i];
    }
    else
    {   // binary operator
        for(size_t i = 0; i < m; ++i)
            type_y[i] = std::max( type_x[i] , type_x[m + i] );
    }
    return true;
}
// END C++
# endif
