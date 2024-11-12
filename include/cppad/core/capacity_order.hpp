# ifndef CPPAD_CORE_CAPACITY_ORDER_HPP
# define CPPAD_CORE_CAPACITY_ORDER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin capacity_order}
{xrst_spell
   pre
   xq
   yq
}

Controlling Taylor Coefficients Memory Allocation
#################################################

Syntax
******
| *f* . ``capacity_order`` ( *c* )

See Also
========
:ref:`fun_property-name`

Purpose
*******
The Taylor coefficients calculated by :ref:`Forward-name` mode calculations
are retained in an :ref:`ADFun-name` object for subsequent use during
:ref:`Reverse-name` mode and higher order Forward mode calculations.
For example, a call to :ref:`Forward<forward_order-name>` with the syntax

   *yq* = *f* . ``Forward`` ( *q* , *xq* )

where *q*  > 0 and  *xq* . ``size`` () == *f* . ``Domain`` () ,
uses the lower order Taylor coefficients and
computes the *q*-th order Taylor coefficients for all
the variables in the operation sequence corresponding to *f* .
The ``capacity_order`` operation allows you to control that
amount of memory that is retained by an AD function object
(to hold ``Forward`` results for subsequent calculations).

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

c
*
The argument *c* has prototype

   ``size_t`` *c*

It specifies the number of Taylor coefficient orders that are allocated
in the AD operation sequence corresponding to *f* .

Pre-Allocating Memory
=====================
If you plan to make calls to ``Forward`` with the maximum value of
*q* equal to *Q* ,
it should be faster to pre-allocate memory for these calls using

   *f* . ``capacity_order`` ( *c* )

with *c* equal to :math:`Q + 1`.
If you do no do this, ``Forward`` will automatically allocate memory
and will copy the results to a larger buffer, when necessary.

Note that each call to :ref:`Dependent-name` frees the old memory
connected to the function object and sets the corresponding
taylor capacity to zero.

Freeing Memory
==============
If you no longer need the Taylor coefficients of order *q*
and higher (that are stored in *f* ),
you can reduce the memory allocated to *f* using

   *f* . ``capacity_order`` ( *c* )

with *c* equal to *q* .
Note that, if :ref:`ta_hold_memory-name` is true, this memory is not actually
returned to the system, but rather held for future use by the same thread.

Original State
**************
If *f* is :ref:`constructed<fun_construct-name>` with the syntax

   ``ADFun`` < *Base* > *f* ( *x* , *y* )

,
there is an implicit call to :ref:`forward_zero-name` with *xq* equal to
the value of the
:ref:`independent variables<glossary@Tape@Independent Variable>`
when the AD operation sequence was recorded.
This corresponds to *c*  == 1 .
{xrst_toc_hidden
   example/general/capacity_order.cpp
}
Example
*******
The file
:ref:`capacity_order.cpp-name`
contains an example and test of these operations.

{xrst_end capacity_order}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file capacity_order.hpp
Control of number of orders allocated.
\}
*/

/*!
Control of number of orders and directions allocated.

\tparam Base
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type AD<Base>.

\param c
is the number of orders to allocate memory for.
If <code>c == 0</code> then r must also be zero.
In this case num_order_taylor_, cap_order_taylor_, and num_direction_taylor_
are all set to zero.
In addition, taylor_.clear() is called.

\param r
is the number of directions to allocate memory for.
If <code>c == 1</code> then r must also be one.
In all cases, it must hold that
<code>
   r == num_direction_taylor_ || num_order_taylor <= 1
</code>
Upon return, num_direction_taylor_ is equal to r.

\par num_order_taylor_
The output value of num_order_taylor_ is the mininumum of its input
value and c. This minimum is the number of orders that are copied to the
new taylor coefficient buffer.

\par num_direction_taylor_
The output value of num_direction_taylor_ is equal to r.
*/

template <class Base, class RecBase>
void ADFun<Base,RecBase>::capacity_order(size_t c, size_t r)
{  // temporary indices
   size_t i, k, ell;

   if( (c == cap_order_taylor_) && (r == num_direction_taylor_) )
      return;

   if( c == 0 )
   {  CPPAD_ASSERT_UNKNOWN( r == 0 );
      taylor_.clear();
      num_order_taylor_     = 0;
      cap_order_taylor_     = 0;
      num_direction_taylor_ = r;
      return;
   }
   CPPAD_ASSERT_UNKNOWN(r==num_direction_taylor_ || num_order_taylor_<=1);

   // Allocate new taylor with requested number of orders and directions
   size_t new_len   = ( (c-1)*r + 1 ) * num_var_tape_;
   local::pod_vector_maybe<Base> new_taylor(new_len);

   // number of orders to copy
   size_t p = std::min(num_order_taylor_, c);
   if( p > 0 )
   {
      // old order capacity
      size_t C = cap_order_taylor_;

      // old number of directions
      size_t R = num_direction_taylor_;

      // copy the old data into the new matrix
      CPPAD_ASSERT_UNKNOWN( p == 1 || r == R );
      for(i = 0; i < num_var_tape_; i++)
      {  // copy zero order
         size_t old_index = ((C-1) * R + 1) * i + 0;
         size_t new_index = ((c-1) * r + 1) * i + 0;
         new_taylor[ new_index ] = taylor_[ old_index ];
         // copy higher orders
         for(k = 1; k < p; k++)
         {  for(ell = 0; ell < R; ell++)
            {  old_index = ((C-1) * R + 1) * i + (k-1) * R + ell + 1;
               new_index = ((c-1) * r + 1) * i + (k-1) * r + ell + 1;
               new_taylor[ new_index ] = taylor_[ old_index ];
            }
         }
      }
   }

   // replace taylor_ by new_taylor
   taylor_.swap(new_taylor);
   cap_order_taylor_     = c;
   num_order_taylor_     = p;
   num_direction_taylor_ = r;

   // note that the destructor for new_taylor will free the old taylor memory
   return;
}

/*!
User API control of number of orders allocated.

\tparam Base
The type used during the forward mode computations; i.e., the corresponding
recording of operations used the type AD<Base>.

\param c
is the number of orders to allocate memory for.
If <code>c == 0</code>,
num_order_taylor_, cap_order_taylor_, and num_direction_taylor_
are all set to zero.
In addition, taylor_.clear() is called.

\par num_order_taylor_
The output value of num_order_taylor_ is the mininumum of its input
value and c. This minimum is the number of orders that are copied to the
new taylor coefficient buffer.

\par num_direction_taylor_
If is zero (one), num_direction_taylor_ is set to zero (one).
Otherwise, if num_direction_taylor_ is zero, it is set to one.
Othwerwise, num_direction_taylor_ is not modified.
*/

template <class Base, class RecBase>
void ADFun<Base,RecBase>::capacity_order(size_t c)
{  size_t r;
   if( (c == 0) || (c == 1) )
   {  r = c;
      capacity_order(c, r);
      return;
   }
   r = num_direction_taylor_;
   if( r == 0 )
      r = 1;
   capacity_order(c, r);
   return;
}

} // END CppAD namespace


# endif
