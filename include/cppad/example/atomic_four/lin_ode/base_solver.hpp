# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_BASE_SOLVER_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_BASE_SOLVER_HPP
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
$begin atomic_four_lin_ode_base_solver.hpp$$
$spell
    Rosen
    lin
    nnz
    vk
$$

$nospell
$bold This is old CppAD documentation:$$ Here is a link to its
$href%http://coin-or.github.io/CppAD%current documentation%$$.
$$
$section
Atomic Multiply Base Matrices: Example Implementation
$$

$head Syntax$$
$icode%lin_ode%.base_solver(
    %r%, %step%, %pattern%, %transpose%, %x%, %y%
)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

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
$cref/vk(x)/atomic_four_lin_ode/vk(x)/$$

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
// base_solver
// BEGIN_PROTOTYPE
template <class Base>
void atomic_lin_ode<Base>::base_solver(
    const Base&                    r         ,
    const Base&                    step      ,
    const sparse_rc&               pattern   ,
    const bool&                    transpose ,
    const CppAD::vector<Base>&     x         ,
    CppAD::vector<Base>&           y         )
// END_PROTOTYPE
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
    if( step < abs(r) )
        n_step = size_t( Integer( abs(r) / step ) ) + 1;
    CppAD::vector<Base> zi(m), e(m);
    for(size_t j = 0; j < m; ++j)
        zi[j] = x[nnz + j];
    y = CppAD::Rosen34(fun, n_step, ti, tf, zi, e);
    return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
