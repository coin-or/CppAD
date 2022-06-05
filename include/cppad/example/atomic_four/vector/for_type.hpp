# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_FOR_TYPE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_FOR_TYPE_HPP
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
$begin atomic_four_vector_for_type.hpp$$
$spell
    Jacobian
    jac
$$

$section Atomic Vector Forward Type Calculation: Example Implementation$$

$head Purpose$$
The $code for_type$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/for_type/atomic_four_for_type/$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// for_type override
template <class Base>
bool atomic_vector<Base>::for_type(
    size_t                                     call_id     ,
    const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
    CppAD::vector<CppAD::ad_type_enum>&        type_y      )
{
    // n, m, op
    size_t n     = type_x.size();
    size_t m     = type_y.size();
    op_enum_t op = op_enum_t( call_id );
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
    switch(op)
    {
        // addition, subtraction
        // not sure result is identically 0 unless both are identically 0
        case add_enum:
        case sub_enum:
        for(size_t i = 0; i < m; ++i)
            type_y[i] = std::max( type_x[i] , type_x[m + i] );
        break;


        // multiplication
        // treat multiplication by zero like absolute zero
        case mul_enum:
        for(size_t i = 0; i < m; ++i)
        {   if( type_x[i] == identical_zero_enum )
                type_y[i] = identical_zero_enum;
            else if( type_x[m + i] == identical_zero_enum )
                type_y[i] = identical_zero_enum;
            else
                type_y[i] = std::max( type_x[i] , type_x[m + i] );
        }
        break;

        // division
        // treat divition of zero like absolute zero
        case div_enum:
        for(size_t i = 0; i < m; ++i)
        {   if( type_x[i] == identical_zero_enum )
                type_y[i] = identical_zero_enum;
            else
                type_y[i] = std::max( type_x[i] , type_x[m + i] );
        }
        break;

        // unary minus
        case neg_enum:
        for(size_t i = 0; i < m; ++i)
            type_y[i] = type_x[i];
        break;

        // error
        case number_op_enum:
        assert(false);
        break;
    }
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
