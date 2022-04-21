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
    Rosen
$$

$section
Atomic Multiply Base Matrices: Example Implementation
$$

$head Rosen34$$
This example uses one step of
$cref rosen34$$ ODE solver.
Any initial value ODE solver, with any number of steps, could be used.

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
    const Base&                    r         ,
    const sparse_rc&               pattern   ,
    const bool&                    transpose ,
    const CppAD::vector<Base>&     x         ,
    CppAD::vector<Base>&           y         )
{
    class Fun {
    private:
        const sparse_rc&           pattern_;
        const bool&                transpose_;
        const CppAD::vector<Base>& x_;
    public:
        Fun(
            const sparse_rc&           pattern   ,
            const bool&                transpose ,
            const CppAD::vector<Base>& x         )
        : pattern_(pattern), transpose_(transpose), x_(x)
        { }
        void Ode(
            const Base&                t ,
            const CppAD::vector<Base>& z ,
            CppAD::vector<Base>&       f )
        {   size_t m   = z.size();
            size_t nnz = pattern_.nnz();
            CPPAD_ASSERT_UNKNOWN( f.size() == m );
            CPPAD_ASSERT_UNKNOWN( x_.size() == nnz + m );
            CPPAD_ASSERT_UNKNOWN( pattern_.nr() == m );
            CPPAD_ASSERT_UNKNOWN( pattern_.nc() == m );
            //
            for(size_t i = 0; i < m; ++i)
                f[i] = Base(0);
            for(size_t k = 0; k < nnz; ++k)
            {   size_t i = pattern_.row()[k];
                size_t j = pattern_.col()[k];
                if( transpose_ )
                    std::swap(i, j);
                f[i] += x_[k] * z[j];
            }
        }
        void Ode_ind(
            const Base&                t   ,
            const CppAD::vector<Base>& z   ,
            CppAD::vector<Base>&       f_t )
        {   size_t m   = z.size();
# ifndef NDEBUG
            size_t nnz = pattern_.nnz();
            CPPAD_ASSERT_UNKNOWN( f_t.size() == m );
            CPPAD_ASSERT_UNKNOWN( x_.size() == nnz + m );
            CPPAD_ASSERT_UNKNOWN( pattern_.nr() == m );
            CPPAD_ASSERT_UNKNOWN( pattern_.nc() == m );
# endif
            //
            for(size_t i = 0; i < m; ++i)
                f_t[i] = Base(0);
        }
        void Ode_dep(
            const Base&                t   ,
            const CppAD::vector<Base>& z   ,
            CppAD::vector<Base>&       f_x )
        {   size_t m   = z.size();
            size_t nnz = pattern_.nnz();
            CPPAD_ASSERT_UNKNOWN( f_x.size() == m * m );
            CPPAD_ASSERT_UNKNOWN( x_.size() == nnz + m );
            CPPAD_ASSERT_UNKNOWN( pattern_.nr() == m );
            CPPAD_ASSERT_UNKNOWN( pattern_.nc() == m );
            //
            for(size_t i = 0; i < m * m; ++i)
                f_x[i] = Base(0);
            for(size_t k = 0; k < nnz; ++k)
            {   size_t i = pattern_.row()[k];
                size_t j = pattern_.col()[k];
                if( transpose_ )
                    std::swap(i, j);
                f_x[i * m + j] = x_[k];
            }
        }
    };
    //
    // nnz
    size_t nnz = pattern.nnz();
    // m
    size_t m     = y.size();
    CPPAD_ASSERT_UNKNOWN( x.size() == nnz + m );
    //
    // fun
    Fun fun(pattern, transpose, x);
    //
    // y
    Base ti       = Base(0.0);
    Base tf       = r;
    size_t n_step = 1;
    CppAD::vector<Base> zi(m), e(m);
    for(size_t j = 0; j < m; ++j)
        zi[j] = x[nnz + j];
    y = CppAD::Rosen34(fun, n_step, ti, tf, zi, e);
    return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
