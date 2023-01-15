# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_REV_DEPEND_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_REV_DEPEND_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_rev_depend.hpp}

Atomic Vector Forward Type Calculation: Example Implementation
##############################################################

Purpose
*******
The ``rev_depend`` routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
:ref:`rev_depend<atomic_four_rev_depend-name>` .

Example
*******
The file :ref:`atomic_four_vector_rev_depend.cpp-name`
contains an example and test that uses this member function.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_rev_depend.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// rev_depend override
template <class Base>
bool atomic_vector<Base>::rev_depend(
   size_t                         call_id     ,
   const CppAD::vector<bool>&     ident_zero_x,
   CppAD::vector<bool>&           depend_x    ,
   const CppAD::vector<bool>&     depend_y    )
{
   // n, m
   size_t n     = depend_x.size();
   size_t m     = depend_y.size();
   //
   // type_y
   if( n == m  )
   {  // unary operator
      for(size_t i = 0; i < m; ++i)
         depend_x[i] = depend_y[i];
   }
   else
   {  // binary operator
      for(size_t i = 0; i < m; ++i)
      {  depend_x[i]     = depend_y[i];
         depend_x[m + i] = depend_y[i];
      }
   }
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
