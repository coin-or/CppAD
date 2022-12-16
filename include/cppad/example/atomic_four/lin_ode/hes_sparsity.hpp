# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_HES_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_HES_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_hes_sparsity.hpp}
{xrst_spell
   wk
}

Atomic Linear ODE Hessian Sparsity Pattern: Example Implementation
##################################################################

Purpose
*******
The ``hes_sparsity`` routine overrides the virtual functions
used by the atomic_four base class for Hessian sparsity calculations; see
:ref:`hes_sparsity<atomic_four_hes_sparsity-name>` .

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
:ref:`atomic_four_lin_ode@vk(x)` ,
and the following additional notation:

wk(x)
=====
Because we are using the :ref:`Rosen34-name` solver, our actual sequence
of operations is only fourth order accurate.
So it suffices to compute the sparsity pattern for

.. math::

   \tilde{y} (x) = \sum_{k=0}^4 v^k (x)

Note that the factor :math:`r / k`,
in the definition of :math:`v^k (x)`,
is constant (with respect to the variables).
Hence it suffices to compute the sparsity pattern for

.. math::

   h (x) = \sum_{k=0}^4 w^k (x)

where :math:`w^0 (x) = b(x)` and for :math:`k = 1, 2, \ldots`,
:math:`w^k (x) = A(x) w^{k-1} (x)`.

Example
*******
The file :ref:`atomic_four_lin_ode_sparsity.cpp-name`
contains an example and test using this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_hes_sparsity.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// hes_sparsity override
template <class Base>
bool atomic_lin_ode<Base>::hes_sparsity(
   size_t                                         call_id      ,
   const CppAD::vector<bool>&                     ident_zero_x ,
   const CppAD::vector<bool>&                     select_x     ,
   const CppAD::vector<bool>&                     select_y     ,
   CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{  //
   // adouble
   typedef AD<double> adouble;
   //
   // pattern_A, transpose, nnz
   Base      r;
   Base      step;
   sparse_rc pattern_A;
   bool      transpose;
   get(call_id, r, step, pattern_A, transpose);
   size_t nnz = pattern_A.nnz();
   //
   // m, n
   size_t m = select_y.size();
   size_t n = select_x.size();
   //
   // au
   vector<adouble> au(n);
   for(size_t j = 0; j < n; ++j)
      au[j] = 1.0;
   Independent( au );
   //
   // ax
   vector<adouble> ax(n);
   for(size_t j = 0; j < n; ++j)
      if( ident_zero_x[j] )
         ax[j] = 0.0;
      else
         ax[j] = au[j];
   //
   // aw
   // aw = w^0 (x)
   vector<adouble> aw(m);
   for(size_t i = 0; i < m; ++i)
      aw[i] = ax[nnz + i];
   //
   // ah = w^0 (x)
   vector<adouble> ah = aw;
   //
   // ah = sum_k=0^4 w^k (x)
   vector<adouble> awk(m);
   for(size_t k = 1; k < 5; ++k)
   {  // aw = w^{k-1} (x)
      //
      // awk = w^k (x)
      for(size_t i = 0; i < m; ++i)
         awk[i] = 0.0;
      for(size_t p = 0; p < nnz; ++p)
      {  //
         // i, j
         size_t i = pattern_A.row()[p];
         size_t j = pattern_A.col()[p];
         if( transpose )
            std::swap(i, j);
         //
         // awk
         awk[i] += ax[p] * aw[j];
      }
      //
      // ah = ah + w^k(x)
      for(size_t i = 0; i < m; ++i)
         ah[i] += awk[i];
      //
      // aw = w^k (x)
      aw = awk;
   }
   //
   // h
   ADFun<double> h;
   h.Dependent(au, ah);
   //
   // pattern_out
   // can use h.for_hes_sparsity or h.rev_hes_sparsity
   bool internal_bool = false;
   h.for_hes_sparsity(select_x, select_y, internal_bool, pattern_out);
   //
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
