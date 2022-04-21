# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_JAC_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_JAC_SPARSITY_HPP
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
$begin atomic_four_lin_ode_jac_sparsity.hpp$$
$spell
    Jacobian
    jac
$$

$section
Atomic Linear ODE Jacobian Sparsity Pattern: Example Implementation
$$

$head Purpose$$
The $code jac_sparsity$$ routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
$cref/jac_sparsity/atomic_four_jac_sparsity/$$.

$head Example$$
The file $cref atomic_four_lin_ode_sparsity.cpp$$
contains an example and test using this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// jac_sparsity override
template <class Base>
bool atomic_lin_ode<Base>::jac_sparsity(
    size_t                                         call_id      ,
    bool                                           dependency   ,
    const CppAD::vector<bool>&                     select_x     ,
    const CppAD::vector<bool>&                     select_y     ,
    CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
    //
    // r, pattern, transpose, nnz
    Base      r;
    sparse_rc pattern;
    bool      transpose;
    get(call_id, r, pattern, transpose);
    size_t nnz = pattern.nnz();
    //
    // m, n
    size_t m = select_y.size();
    size_t n = select_x.size();
    //
    CPPAD_ASSERT_UNKNOWN( n == nnz + m );
    CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
    //
    // list_setvec
    typedef CppAD::local::sparse::list_setvec list_setvec;
    //
    // setvec
    list_setvec setvec;
    size_t n_set = m;
    size_t end   = n;
    setvec.resize(n_set, end);
    //
    // initial condition
    for(size_t i = 0; i < m; ++i) if( select_y[i] )
    {   size_t element = nnz + i;
        if( select_x[element] )
            setvec.add_element(i, element);
    }
    //
    // change
    bool change = true;
    while(change)
    {   change = false;
        //
        for(size_t k = 0; k < nnz; ++k) if( select_x[k] )
        {   size_t i = pattern.row()[k];
            size_t j = pattern.col()[k];
            if( transpose )
                std::swap(i, j);
            if( select_y[i] & select_y[j] )
            {   if( ! setvec.is_element(i, k) )
                {   change = true;
                    setvec.add_element(i, k);
                }
                list_setvec::const_iterator itr(setvec, j);
                size_t element = *itr;
                while(element != end )
                {   if( select_x[element] )
                    {   if( ! setvec.is_element(i, element) )
                        {   change = true;
                            setvec.add_element(i, element);
                        }
                    }
                    ++itr;
                    element = *itr;
                }
            }
        }
    }
    //
    // pattern_out
    pattern_out.resize(m, n, 0);
    for(size_t i = 0; i < m; ++i)
    {   list_setvec::const_iterator itr(setvec, i);
        size_t element = *itr;
        while(element != end )
        {   size_t j = element;
            pattern_out.push_back(i, j);
            ++itr;
            element = *itr;
        }
    }
    //
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
