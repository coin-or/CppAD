# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_JAC_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_JAC_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_jac_sparsity.hpp}
{xrst_spell
   nnz
}

Atomic Linear ODE Jacobian Sparsity Pattern: Example Implementation
###################################################################

Purpose
*******
The ``jac_sparsity`` routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
:ref:`jac_sparsity<atomic_four_jac_sparsity-name>` .

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

S[ g(x) ]
=========
We use :math:`S [ g(x) ]` to denote the sparsity pattern
for a function :math:`g : \B{R}^n \rightarrow \B{R}^m` as a vector of sets.
To be specific, for :math:`i = 0, \ldots , m-1`,
:math:`S_i [ g(x) ]` is the set of indices between
zero and :math:`n - 1` such that
:math:`\partial g_i (x) / \partial x_j` is possibly non-zero.

N [ g(x) ]
==========
We use :math:`N[ g(x) ]` to denote the set of :math:`i`
such that :math:`g_i (x)` is not identically zero.

J_i [ A(x) ]
============
We use :math:`J_i [ A(x) ]` to denote the set of :math:`j`
between zero and :math:`m-1` such that
:math:`A_{i,j}` is not known to be identically zero.

P_i [ g(x) ]
============
We use :math:`P_i [ g(x) ]` to denote the set if sparsity pattern indices

.. math::

   P_i [ g(x) ] = \left\{ p  \W{:}
      0 \leq p < \R{nnz} \W{,}
      i = \R{row} [p] \W{,}
      \R{col}[p] \in N [ g(x) ]
   \right\}

Theory
******
This routine must calculate the following value for
:math:`i = 0, \ldots, m-1`; see :ref:`atomic_four_lin_ode@y(x)@m` :

.. math::

   S_i [ y (x) ] = \bigcup_k S_i [  v^k (x) ]

The set :math:`S_i [ v^0 (x) ]` has just one element
corresponding to :math:`b_i (x)`; i.e,

.. math::

   S_i [ v^0 (x) ] = \{ \R{nnz} + i \}

see :ref:`atomic_four_lin_ode@x@b(x)` .
Furthermore, for :math:`k > 0`,

.. math::

   v^k (x) = \frac{r}{k} A(x) v^{k-1} (x)

.. math::

   S_i [ v^k (x) ] = S_i [ A(x) v^{k-1} (x) ]

.. math::

   S_i [ v^k (x) ] =  P_i [ v^{k-1} (x) ]
   \cup \left\{ S_j [ v^{k-1} (x) ] \W{:}  j \in J_i [ A(x) ] \right\}

Suppose that :math:`\ell` is such that for all :math:`i`
the following two conditions hold

.. math::

   N [ v^\ell (x) ]  \subset \bigcup_{k < \ell} N [ v^k (x) ]

.. math::

   S_i [ v^\ell (x) ] \subset \bigcup_{k < \ell} S_i [ v^k (x) ]

From the first condition above it follows that

.. math::

   P_i [ v^\ell (x) ] \subset \bigcup_{k < \ell} P_i [ v^k (x) ]

Using the second condition we have

.. math::

   S_i [ v^{\ell+1} (x) ] =  P_i [ v^\ell (x) ]
   \cup \left\{ S_j [ v^\ell (x) ] \W{:} j \in J_i [ A(x) ] \right\}

.. math::

   S_i [ v^{\ell+1} (x) ] \subset
   \left\{ S_j [ v^\ell (x) ] \W{:} j \in J_i [ A(x) ] \right\}
      \bigcup_{k \leq \ell } S_i [ v^k (x) ]

.. math::

   S_i [ v^{\ell+1} (x) ]
   \subset
   \bigcup_{k \leq \ell} S_i [ v^k (x) ] \cup
         \left\{ S_j [ v^k (x) ] \W{:} j \in J_i [ A(x) ] \right\}

.. math::

   S_i [ v^{\ell+1} (x) ]
   \subset
   \bigcup_{k < \ell} S_i [ v^k (x) ] \cup
         \left\{ S_j [ v^k (x) ] \W{:} j \in J_i [ A(x) ] \right\}

.. math::

   S_i [ v^{\ell+1} (x) ]
   \subset
   \bigcup_{k \leq \ell} S_i [ v^k (x) ]

.. math::

   S_i [ v^{\ell+1} (x) ]
   \subset
   \bigcup_{k < \ell} S_i [ v^k (x) ]

It follows that

.. math::

   S_i [ y(x) ] = \bigcup_{k < \ell} S_i [ v^k (x) ]

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

{xrst_end atomic_four_lin_ode_jac_sparsity.hpp}
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
   const CppAD::vector<bool>&                     ident_zero_x ,
   const CppAD::vector<bool>&                     select_x     ,
   const CppAD::vector<bool>&                     select_y     ,
   CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
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
   CPPAD_ASSERT_UNKNOWN( n == nnz + m );
   CPPAD_ASSERT_UNKNOWN( pattern_A.nr() == m );
   CPPAD_ASSERT_UNKNOWN( pattern_A.nc() == m );
   //
   // pattern_out
   // Accumulates elements of the sparsity pattern for y(x) that satisfy
   // select_x and select_y
   pattern_out.resize(m, n, 0);
   //
   // list_setvec
   // This vector of sets interface is not in the CppAD user API
   typedef CppAD::local::sparse::list_setvec list_setvec;
   //
   // setvec
   // Accumulates the sparsity pattern for y(x) that satisfy select_x.
   // There are m sets and the set elements are between zero and n-1.
   list_setvec setvec;
   size_t n_set = m;
   size_t end   = n;
   setvec.resize(n_set, end);
   //
   // setvec, pattern_out
   // iniialize as equal to S[ v^0 (x) ]
   for(size_t i = 0; i < m; ++i)
   {  size_t element = nnz + i;
      if( select_x[element] )
      {  setvec.add_element(i, element);
         if( select_y[i] )
            pattern_out.push_back(i, element);
      }
   }
   //
   // non_zero
   // Accumulates union_k for k < ell, N[ v^k (x) ]
   // initialize as N[ v^0 (x) ]
   CppAD::vector<bool> non_zero(m);
   for(size_t i = 0; i < m; ++i)
      non_zero[i] = ! ident_zero_x[nnz + i];
   //
   // change
   // Did setvec or non_zero change in previous iteration of while loop
   bool change = true;
   while(change)
   {  change = false;
      // we use k = 1, 2, ... to denote the pass through this loop
      // setvec[i] contains union q < k S_i [ v^q (x) ]
      // non_zero  contains union q < k N [ v^q (x) ]
      //
      // For each element of the sparsity pattern for A subject to select_x
      for(size_t p = 0; p < nnz; ++p) if( select_x[p] )
      {  CPPAD_ASSERT_UNKNOWN( ! ident_zero_x[p] );
         size_t i = pattern_A.row()[p];
         size_t j = pattern_A.col()[p];
         if( transpose )
            std::swap(i, j);
         //
         if( non_zero[j] )
         {  // p, corresponding to A_{i,j}, is in P_i [ v^k (x) ]
            if( ! setvec.is_element(i, p) )
            {  change = true;
               setvec.add_element(i, p);
               if( select_y[i] )
                  pattern_out.push_back(i, p);
            }
         }
         // j is in J_i [ A(x) ]
         list_setvec::const_iterator itr(setvec, j);
         size_t element = *itr;
         while(element != end )
         {  if( ! setvec.is_element(i, element) )
            {  change = true;
               setvec.add_element(i, element);
               if( select_y[i] )
                  pattern_out.push_back(i, element);
                 }
                 ++itr;
                 element = *itr;
         }
         // A_{i,j} update to non_zero
         if( non_zero[i] == false )
         {  if( non_zero[j] == true )
            {  change = true;
               non_zero[i] = true;
            }
         }
      }
   }
   //
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
