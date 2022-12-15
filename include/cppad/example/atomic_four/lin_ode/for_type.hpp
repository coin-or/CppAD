# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FOR_TYPE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FOR_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_for_type.hpp}

Atomic Linear ODE Forward Type Calculation: Example Implementation
##################################################################

Purpose
*******
The ``for_type`` routine overrides the virtual functions
used by the atomic_four base; see
:ref:`for_type<atomic_four_for_type-name>` .

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

T(s)
====
We use :math:`\R{T} ( s )` to denote the ad_type of a scalar value :math:`s`.
There are four possible
:ref:`ad_types<atomic_four_for_type@ad_type>` :
identical_zero, constant, dynamic, and variable in that order.

Theory
******
This routine must calculate the following value for
:math:`i = 0, \ldots, m-1`; see :ref:`atomic_four_lin_ode@y(x)@m` :

.. math::

   \R{T} [ y_i (x) ] = \max_k \R{T} [  v_i^k (x) ]

The type :math:`\R{T} [ v_i^0 (x) ] = \R{T}[ b_i (x) ]`.
This is easy to calculate given the type of the components of *x* ;
see :ref:`atomic_four_lin_ode@x@b(x)` .
Furthermore, for :math:`k > 0`

.. math::

   v_i^k (x)
   =
   \frac{r}{k} \sum_{j=0}^{m-1} A_{i,j} (x) v_j^{k-1} (x)

.. math::

   \R{T} [ v_i^k (x) ]
   =
   \max_j \R{T} [ A_{i,j} (x) v_j^{k-1} (x) ]

.. math::

   \R{T} [ A_{i,j} (x) v_j^k (x) ]
   =
   \left\{ \begin{array}{ll}
      \R{identical\_zero} &
      \R{if} A_{i,j} (x) \W{\R{or}} v_j^{k-1} (x) \W{\R{is}} \R{identical\_zero}
      \\
      \max\{ \R{T} [ A_{i,j} (x) ] \W{,} \R{T} [ v_j^{k-1} (x) ] \} &
      \R{otherwise}
   \end{array} \right.

If :math:`A_{i,j} (x)` is not in the sparsity
:ref:`atomic_four_lin_ode@pattern` , it is identically zero.
Furthermore we are allowing for the case where
:math:`A_{i,j} (x)` is in the pattern and it is identically zero; i.e.,
the sparsity pattern is not efficient as it could be.
The type :math:`\R{T} [ A_{i,j} (x) ]` for components in the sparsity pattern
is easy to calculate given the type of the components of *x* ;
see :ref:`atomic_four_lin_ode@x@A(x)` .
Suppose :math:`\ell` is such that for all :math:`i`

.. math::

   \R{T} [ v_i^\ell (x) ] \leq \max_{k < \ell} \R{T} [ v_i^k (x) ]

It follows that

.. math::

   \R{T} [ v_j^{\ell+1} (x) ]  = \max_j \R{T} [ A_{i,j} (x) v_j^\ell (x) ]

.. math::

   \R{T} [ v_j^{\ell+1} (x) ]
   \leq
   \max_{k < \ell} \max_j \R{T} [ A_{i,j} (x) v_j^k (x) ]

.. math::

   \R{T} [ v_j^{\ell+1} (x) ]
   \leq
   \max_{k < \ell} \R{T} [ v_i^k (x) ]

From this it is clear that

.. math::

   \R{T} [ y_i (x) ] = \max_{k < \ell} \R{T} [  v_i^k (x) ]

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_for_type.hpp}
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
   // pattern, transpose, nnz
   Base      r;
   Base      step;
   sparse_rc pattern;
   bool      transpose;
   get(call_id, step, r, pattern, transpose);
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
   // type_y[i] = max T[ v_i^k (x) ] for k = 0
   for(size_t i = 0; i < m; ++i)
      type_y[i] = type_x[nnz + i];
   //
   // change
   // Did type_y change during the previous iteration of the while loop
   bool change = true;
   while(change)
   {  change = false;
      // we use k = 1, 2, ... to denote the pass through this loop
      // type_y[i] = max q < k T[ v_i^q (x) ]
      //
      for(size_t p = 0; p < nnz; ++p)
      {  size_t i = pattern.row()[p];
         size_t j = pattern.col()[p];
         if( transpose )
            std::swap(i, j);
         //
         // type_y[i], change
         if( type_x[p] != identical_zero_enum )
         {  // A_ij (x) is not identically zero
            if( type_y[j] > type_y[i] )
            {  change = true;
               type_y[i] = type_y[j];
                 }
         }
         //
         // type_y[i], change
         if( type_y[j] != identical_zero_enum )
         {  // There is a q < k such that v_j^q (x) not identically zero
            if( type_x[p] > type_y[i] )
            {  change = true;
               type_y[i] = type_x[p];
            }
         }
      }
   }
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
