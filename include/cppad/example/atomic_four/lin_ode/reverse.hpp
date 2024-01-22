# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_REVERSE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_REVERSE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_reverse.hpp}
{xrst_spell
   lagrangian
   lll
   nnz
   simpson
}

Atomic Linear ODE Reverse Mode: Example Implementation
######################################################

Purpose
*******
The ``reverse`` routine overrides the virtual functions
used by the atomic_four base; see
:ref:`reverse<atomic_four_reverse-name>` .

First Order Theory
******************
We are given a vector :math:`w \in \B{R}^m` and need to compute

.. math::

   \partial_x w^\R{T} z(r, x)

see the definition of :ref:`atomic_four_lin_ode@z(t, x)` .
Consider the Lagrangian corresponding to
:math:`w^\R{T} z(r, x)` as the objective and the ODE as the constraint:

.. math::

   L(x, \lambda)
   =
   w^\R{T} z(r, x) +
      \int_0^r \lambda(t, x)^\R{T}
         [ A(x) z(t, x) - z_t (t, x) ] \R{d} t

where :math:`\lambda : \R{R} \times \B{R}^n \rightarrow \B{R}^m`
is a smooth function.
If :math:`z(t, x)` satisfies its ODE, then

.. math::

   \partial_x w^\R{T} z(r, x)
   =
   \partial_x L(x, \lambda)

We use the following integration by parts to replace the :math:`z_t (t, x)`
term in the integral defining :math:`L(x, \lambda)`:

.. math::

   - \int_0^r \lambda(t, x)^\R{T} z_t (t, x) \R{d} t
   =
   - \left. \lambda(t, x)^\R{T} z(t, x) \right|_0^r
   +
   \int_0^r \lambda_t (t, x)^\R{T} z(t, x) \R{d} t

Adding the condition :math:`\lambda(r, x) = w`,
and noting that :math:`z(0, x) = b(x)`, we have

.. math::

   L(x, \lambda)
   =
   \lambda(0, x)^\R{T} z(0, x)
   +
   \int_0^r \lambda_t (t, x)^\R{T} z(t, x) \R{d} t
   +
   \int_0^r \lambda(t, x)^\R{T} A(x) z(t, x) \R{d} t

.. math::

   L(x, \lambda)
   =
   \lambda(0, x)^\R{T} b (x)
   +
   \int_0^r [ \lambda_t (t, x)^\R{T} + \lambda(t, x)^\R{T} A(x) ]
      z(t, x) \R{d} t

.. math::

   L(x, \lambda)
   =
   \lambda(0, x)^\R{T} b (x)
   +
   \int_0^r z(t, x)^\R{T}
      [ \lambda_t (t, x) + A(x)^\R{T} \lambda(t, x) ] \R{d} t

The partial derivative
of :math:`L(x, \lambda)` with respect to :math:`b_j`,
(not including the dependence of :math:`\lambda(t, x)` on :math:`x`)
is :

.. math::

   \partial_{b(j)} L(x, \lambda)
   =
   \lambda_j (0, x)

The partial derivative
of :math:`L(x, \lambda)` with respect to :math:`A_{i,j}`
(not including The dependence of :math:`\lambda(t, x)` on :math:`x`)
is :

.. math::

   \partial_{A(i,j)} L(x, \lambda)
   =
   \int_0^r \partial_{A(i,j)} z(t, x)^\R{T}
      [ \lambda_t (t, x) + A(x)^\R{T} \lambda(t, x) ] \R{d} t
   +
   \int_0^r z_j (t, x) \lambda_i (t, x) \R{d} t

If :math:`\lambda(t, x)` satisfies the ODE

.. math::

   0 = \lambda_t (t, x) + A(x)^\R{T} \lambda(t, x)

The partial derivative with respect to :math:`A_{i,j}` is

.. math::

   \partial_{A(i,j)} L(x, \lambda)
   =
   \int_0^r z_j (t, x) \lambda_i (t, x) \R{d} t

In summary, we can compute
an approximate solution for the initial value ODE:

.. math::

   z_t (t, x) = A(x) z(t, x) \W{,} z(0, x) = b(x)

and approximate solution for the final value ODE:

.. math::

   \lambda_t (t, x) = - A(x)^\R{T} \lambda(t, x)
   \W{,}
   \lambda(r, x) = w

Using the notation
:ref:`atomic_four_lin_ode@pattern@nnz` ,
:ref:`atomic_four_lin_ode@pattern@row` , and
:ref:`atomic_four_lin_ode@pattern@col` ,
We can compute an approximation for

.. math::

   \partial_{x(k)} w^\R{T} z(r, x)
   =
   \left\{ \begin{array}{lll}
   \int_0^r \lambda_i (t, x) z_j (r, x) \R{d} t
   & \R{where} \; i = \R{row} [k] \W{,} j = \R{col}[k]
   & \R{if} \; k < nnz
   \\
   \lambda_i (0, x)
   & \R{where} \; i = k - nnz
   & \R{otherwise}
   %
   \end{array} \right.

{xrst_toc_hidden
   include/cppad/example/atomic_four/lin_ode/reverse_2.xrst
}
Second Order Theory
*******************
:ref:`atomic_four_lin_ode_reverse_2-name` .

Simpson's Rule
**************
This example uses Simpson's rule to approximate the integral

.. math::

   \int_0^r \lambda_i (t, x) z_j (t, x) \R{d} t

Any other approximation for this integral can be used.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_reverse.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// ----------------------------------------------------------------------------
// reverse override for Base
template <class Base>
bool atomic_lin_ode<Base>::reverse(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_x    ,
   size_t                                     order_up    ,
   const CppAD::vector<Base>&                 taylor_x    ,
   const CppAD::vector<Base>&                 taylor_y    ,
   CppAD::vector<Base>&                       partial_x   ,
   const CppAD::vector<Base>&                 partial_y   )
{
   // order_up
   if( order_up > 0 )
      return false;
   //
   // r, step, pattern, transpose
   Base      r;
   Base      step;
   sparse_rc pattern;
   bool      transpose;
   get(call_id, r, step, pattern, transpose);
   //
   reverse_one(
      r, step, pattern, transpose, select_x, taylor_x, partial_y, partial_x
   );
   //
   return true;
}
// ---------------------------------------------------------------------------
// reverse override for AD<Base>
template <class Base>
bool atomic_lin_ode<Base>::reverse(
   size_t                                           call_id    ,
   const CppAD::vector<bool>&                       select_x   ,
   size_t                                           order_up   ,
   const CppAD::vector< CppAD::AD<Base> >&          ataylor_x  ,
   const CppAD::vector< CppAD::AD<Base> >&          ataylor_y  ,
   CppAD::vector< CppAD::AD<Base> >&                apartial_x ,
   const CppAD::vector< CppAD::AD<Base> >&          apartial_y )
{
   // order_up
   if( order_up > 0 )
      return false;
   //
   reverse_one(call_id, select_x, ataylor_x, apartial_y, apartial_x);
   //
   return true;
}
// ---------------------------------------------------------------------------
// reverse_one
// Base version of first order reverse mode calculation as in Theory above
template <class Base>
void atomic_lin_ode<Base>::reverse_one(
   const Base&                                r           ,
   const Base&                                step        ,
   const sparse_rc&                           pattern     ,
   const bool&                                transpose   ,
   const CppAD::vector<bool>&                 select_x    ,
   const CppAD::vector<Base>&                 x           ,
   const CppAD::vector<Base>&                 w           ,
   CppAD::vector<Base>&                       partial_x   )
{
   //
   // nnz
   size_t nnz    = pattern.nnz();
   //
   // m
   size_t m = w.size();
   CPPAD_ASSERT_UNKNOWN( w.size() == m );
   CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
   CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
   //
   // n
# ifndef NDEBUG
   size_t n = nnz + m;
# endif
   //
   // x, partial_x
   CPPAD_ASSERT_UNKNOWN( x.size()  == n );
   CPPAD_ASSERT_UNKNOWN( partial_x.size() == n );
   //
   // n_step
   size_t n_step = 2;
   if( step < abs(r) / Base(n_step) )
      n_step = size_t( abs(r) / step );
   while( step < abs(r) / Base(n_step) )
      ++n_step;
   if( n_step % 2 == 1 )
      ++n_step;
   CPPAD_ASSERT_UNKNOWN( n_step % 2 == 0 );
   //
   // h
   Base h = r / Base(n_step);
   //
   // x_tmp = [A, b]
   CppAD::vector<Base> x_tmp = x;
   //
   // z_all
   CppAD::vector< CppAD::vector<Base> > z_all(n_step + 1);
   //
   // z_all[0] = z(0, x)
   z_all[0].resize(m);
   for(size_t i = 0; i < m; ++i)
      z_all[0][i] = x[nnz + i];
   //
   // p
   for(size_t p = 0; p < n_step; ++p)
   {  // x_tmp = [A, z(h*p, x) ]
      for(size_t i = 0; i < nnz; ++i)
         x_tmp[nnz + i] = z_all[p][i];
      //
      // z_all[p+1] = z( h*(p+1), x)
      z_all[p+1].resize(m);
      base_solver(h, step, pattern, transpose, x_tmp, z_all[p+1]);
   }
   // lambda_previous = lambda(r, x) = w
   CppAD::vector<Base> lambda_previous = w;
   //
   // lambda_middle, lambda_next
   CppAD::vector<Base> lambda_middle(m), lambda_next(m);
   //
   // partial_x
   // partial_A L(x, lambda)
   for(size_t k = 0; k < nnz; ++k)
      partial_x[k] = Base(0.0);
   //
   // p
   size_t p = n_step - 1;
   while(p)
   {  CPPAD_ASSERT_UNKNOWN( p % 2 == 1 );
      //
      // x_tmp = [ A, lambda( (p+1)*h, x ) ]
      for(size_t i = 0; i < m; ++i)
         x_tmp[nnz + i] = lambda_previous[i];
      //
      // lambda_middle = lambda(p*h, x)
      // We convert the final value ODE to an initial value ODE by changing
      // the sign of A^T and changing limits from [(p+1)*h, p*h] -> [0, h].
      base_solver(h, step, pattern, ! transpose, x_tmp, lambda_middle);
      //
      // x_tmp = [ A, lambda(p*h, x)]
      for(size_t i = 0; i < m; ++i)
         x_tmp[nnz + i] = lambda_middle[i];
      //
      // lambda_next = lambda((p-1), x)
      base_solver(h, step, pattern, ! transpose, x_tmp, lambda_next);
      //
      // partial_x
      // partail_A L(x, lambda)
      for(size_t k = 0; k < nnz; ++k) if( select_x[k] )
      {  size_t i = pattern.row()[k];
         size_t j = pattern.col()[k];
         if( transpose )
            std::swap(i, j);
         //
         // sum  = lambda_i ((p+1)*h, x) * z_j ((p+1)*h, x)
         Base sum = lambda_previous[i] * z_all[p+1][j];
         //
         // sum += 4 * lambad_i(p*h, x) * z_j(p*h, x)
         sum += Base(4.0) * lambda_middle[i] * z_all[p][j];
         //
         // sum += lambda_i ((p-1)*h, x) * z_j ((p-1)*h, x)
         sum += lambda_next[i] * z_all[p-1][j];
         //
         // Simpson's rule for int_0^2*h lambda_i (t, x) z_j (t, x) dt
         Base integral = h * sum / Base(3.0);
         //
         // partial_{A(i,j)}
         partial_x[k] += integral;
      }
      //
      // lambda_previous
      lambda_previous = lambda_next;
      //
      // p
      if( p == 1 )
         p = 0;
      else
         p -= 2;
   }
   //
   // partial_x
   // partial_b L(x, lambda) = lambda(0, x)
   for(size_t i = 0; i < m; ++i)
   {  // partial_{b(i)}
      partial_x[nnz + i] = lambda_next[i];
   }
   //
   return;
}
// ---------------------------------------------------------------------------
// reverse_one
// AD version of first order reverse mode calculation as in Theory above
template <class Base>
void atomic_lin_ode<Base>::reverse_one(
   const size_t&                              call_id     ,
   const CppAD::vector<bool>&                 select_x    ,
   const CppAD::vector< CppAD::AD<Base> >&    ax          ,
   const CppAD::vector< CppAD::AD<Base> >&    aw          ,
   CppAD::vector< CppAD::AD<Base> >&          apartial_x  )
{  typedef CppAD::AD<Base> ad_base;
   //
   //
   // r, step, pattern, transpose
   Base       r;
   Base       step;
   sparse_rc pattern;
   bool      transpose;
   get(call_id, step, r, pattern, transpose);
   //
   // nnz
   size_t nnz = pattern.nnz();
   //
   // m
   size_t m = aw.size();
   CPPAD_ASSERT_UNKNOWN( aw.size() == m );
   CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
   CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
   //
   // n
# ifndef NDEBUG
   size_t n = nnz + m;
# endif
   //
   // ax, apartial_x
   CPPAD_ASSERT_UNKNOWN( ax.size()  == n );
   CPPAD_ASSERT_UNKNOWN( apartial_x.size() == n );
   //
   // n_step
   size_t n_step = 2;
   if( step < abs(r) / Base(n_step) )
      n_step = size_t( abs(r) / step );
   while( step < abs(r) / Base(n_step) )
      ++n_step;
   if( n_step % 2 == 1 )
      ++n_step;
   CPPAD_ASSERT_UNKNOWN( n_step % 2 == 0 );
   //
   // h
   Base h = r / Base(n_step);
   //
   // ax_tmp = [A, b]
   CppAD::vector<ad_base> ax_tmp = ax;
   //
   // az_all
   CppAD::vector< CppAD::vector<ad_base> > az_all(n_step + 1);
   //
   // az_all[0] = z(0, x)
   az_all[0].resize(m);
   for(size_t i = 0; i < m; ++i)
      az_all[0][i] = ax[nnz + i];
   //
   // call_id_1
   size_t call_id_1 = (*this).set(h, step, pattern, transpose);
   //
   // p
   for(size_t p = 0; p < n_step; ++p)
   {  // ax_tmp = [A, z(h*p, x) ]
      for(size_t i = 0; i < nnz; ++i)
         ax_tmp[nnz + i] = az_all[p][i];
      //
      // az_all[p+1] = az( h*(p+1), x)
      // This interface requires a separate atomic function call for each
      // step in the Simpson's rule integration. Perhaps it would be more
      // efficient (but more complicated) to have an option whereby one call
      // that returns all the values in az_all expect for az_all[0].
      az_all[p+1].resize(m);
      (*this)(call_id_1, ax_tmp, az_all[p+1]);
   }
   // alambda_previous = lambda(r, x) = w
   CppAD::vector<ad_base> alambda_previous = aw;
   //
   // alambda_middle, alambda_next
   CppAD::vector<ad_base> alambda_middle(m), alambda_next(m);
   //
   // apartial_x
   // apartial_A L(x, lambda)
   for(size_t k = 0; k < nnz; ++k)
      apartial_x[k] = ad_base(0.0);
   //
   // call_id_2
   size_t call_id_2 = (*this).set(h, step, pattern, ! transpose);
   //
   //
   // p
   size_t p = n_step - 1;
   while(p)
   {  CPPAD_ASSERT_UNKNOWN( p % 2 == 1 );
      //
      // ax_tmp = [ A, lambda( (p+1)*h, x ) ]
      for(size_t i = 0; i < m; ++i)
         ax_tmp[nnz + i] = alambda_previous[i];
      //
      // alambda_middle = lambda(p*h, x)
      // We convert the final value ODE to an initial value ODE by changing
      // the sign of A^T and changing limits from [(p+1)*h, p*h] -> [0, h].
      (*this)(call_id_2, ax_tmp, alambda_middle);
      //
      // ax_tmp = [ A, lambda(p*h, x)]
      for(size_t i = 0; i < m; ++i)
         ax_tmp[nnz + i] = alambda_middle[i];
      //
      // alambda_next = lambda((p-1), x)
      (*this)(call_id_2, ax_tmp, alambda_next);
      //
      // apartial_x
      // partail_A L(x, lambda)
      for(size_t k = 0; k < nnz; ++k) if( select_x[k] )
      {  size_t i = pattern.row()[k];
         size_t j = pattern.col()[k];
         if( transpose )
            std::swap(i, j);
         //
         // asum  = lambda_i ((p+1)*h, x) * z_j ((p+1)*h, x)
         ad_base asum = alambda_previous[i] * az_all[p+1][j];
         //
         // asum += 4 * lambad_i(p*h, x) * z_j(p*h, x)
         asum += ad_base(4.0) * alambda_middle[i] * az_all[p][j];
         //
         // asum += lambda_i ((p-1)*h, x) * z_j ((p-1)*h, x)
         asum += alambda_next[i] * az_all[p-1][j];
         //
         // Simpson's rule for int_0^2*h lambda_i (t, x) z_j (t, x) dt
         ad_base aintegral = h * asum / ad_base(3.0);
         //
         // apartial_x
         // partial_{A(i,j)}
         apartial_x[k] += aintegral;
      }
      //
      // alambda_previous
      alambda_previous = alambda_next;
      //
      // p
      if( p == 1 )
         p = 0;
      else
         p -= 2;
   }
   //
   // apartial_x
   // partial_b L(x, lambda) = lambda(0, x)
   for(size_t i = 0; i < m; ++i)
   {  // partial_{b(i)}
      apartial_x[nnz + i] = alambda_next[i];
   }
   //
   return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
