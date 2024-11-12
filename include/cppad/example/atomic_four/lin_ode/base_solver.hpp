# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_BASE_SOLVER_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_BASE_SOLVER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_base_solver.hpp}

Atomic Linear ODE: Example Implementation
#########################################

Syntax
******
| *lin_ode* . ``base_solver`` (
| |tab| *r* , *step* , *pattern* , *transpose* , *x* , *y*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Notation
********
We use the notation:
:ref:`atomic_four_lin_ode@call_id`
:ref:`atomic_four_lin_ode@r`
:ref:`atomic_four_lin_ode@pattern`
:ref:`atomic_four_lin_ode@transpose`
:ref:`atomic_four_lin_ode@pattern@nnz` ,
:ref:`atomic_four_lin_ode@pattern@row` ,
:ref:`atomic_four_lin_ode@pattern@col` ,
:ref:`atomic_four_lin_ode@x` ,
:ref:`atomic_four_lin_ode@x@n` ,
:ref:`atomic_four_lin_ode@x@A(x)` ,
:ref:`atomic_four_lin_ode@x@b(x)` ,
:ref:`atomic_four_lin_ode@y(x)` ,
:ref:`atomic_four_lin_ode@y(x)@m` ,
:ref:`atomic_four_lin_ode@vk(x)`

Rosen34
*******
This example uses one step of
:ref:`rosen34-name` ODE solver.
Any initial value ODE solver, with any number of steps, could be used.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_base_solver.hpp}
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
      {  size_t m   = z.size();
         size_t nnz = pattern_.nnz();
         CPPAD_ASSERT_UNKNOWN( f.size() == m );
         CPPAD_ASSERT_UNKNOWN( x_.size() == nnz + m );
         CPPAD_ASSERT_UNKNOWN( pattern_.nr() == m );
         CPPAD_ASSERT_UNKNOWN( pattern_.nc() == m );
         //
         for(size_t i = 0; i < m; ++i)
            f[i] = Base(0);
         for(size_t k = 0; k < nnz; ++k)
         {  size_t i = pattern_.row()[k];
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
      {  size_t m   = z.size();
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
      {  size_t m   = z.size();
         size_t nnz = pattern_.nnz();
         CPPAD_ASSERT_UNKNOWN( f_x.size() == m * m );
         CPPAD_ASSERT_UNKNOWN( x_.size() == nnz + m );
         CPPAD_ASSERT_UNKNOWN( pattern_.nr() == m );
         CPPAD_ASSERT_UNKNOWN( pattern_.nc() == m );
         //
         for(size_t i = 0; i < m * m; ++i)
            f_x[i] = Base(0);
         for(size_t k = 0; k < nnz; ++k)
         {  size_t i = pattern_.row()[k];
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
