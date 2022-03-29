# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_BASE_LIN_ODE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_BASE_LIN_ODE_HPP
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
$begin atomic_four_lin_ode_base_lin_ode.hpp$$
$spell
$$

$section
Atomic Multiply Base Matrices: Example Implementation
$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// base_lin_ode
template <class Base>
void atomic_lin_ode<Base>::base_lin_ode(
    const Base&                    r      ,
    size_t                         n_step ,
    const CppAD::vector<Base>&     x      ,
    CppAD::vector<Base>&           y      )
{
    class Fun {
    private:
        const CppAD::vector<Base>& x_;
    public:
        Fun(const CppAD::vector<Base>& x) : x_(x)
        { }
        void Ode(
            const Base&                s ,
            const CppAD::vector<Base>& z ,
            CppAD::vector<Base>&       f )
        {   size_t m = z.size();
            assert( f.size() == m );
            assert( x_.size() == m * m + m );
            //
            for(size_t i = 0; i < m; ++i)
            {   f[i] = Base(0);
                for(size_t j = 0; j < m; ++j)
                    f[i] += x_[ i * m + j] * z[j];
            }
        }
    };
    // m
    size_t m     = y.size();
    assert( x.size() == m * m + m );
    //
    // fun
    Fun fun(x);
    //
    // y
    Base si = Base(0.0);
    Base sf = r;
    CppAD::vector<Base> zi(m), e(m);
    for(size_t j = 0; j < m; ++j)
        zi[j] = x[ m * m + j];
    y = CppAD::Runge45(fun, n_step, si, sf, zi, e);
    return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
