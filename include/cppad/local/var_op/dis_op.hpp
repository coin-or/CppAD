# ifndef CPPAD_LOCAL_VAR_OP_DIS_OP_HPP
# define CPPAD_LOCAL_VAR_OP_DIS_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin_parent var_dis_op dev}

Variable Discrete Operator
##########################

DisOp
*****
is the op code for this operator.

User Syntax
***********
| *z* = *name* ( *x* )

name
****
is the :ref:`Discrete@name` of he discrete function.

x
*
is the argument for this discrete function.

z
*
is the new variable created by this function evaluation.
(Note that this is called :ref:`Discrete@y` is the user documentation
for discrete functions.)

arg
***

arg[0]
======
is the index that identifies this discrete function.

arg[1]
======
variable index corresponding to the argument for this function call.


{xrst_end var_dis_op}
------------------------------------------------------------------------------
{xrst_begin dis_forward_dir dev}
{xrst_spell
   ataylor
   tpv
}

Forward Mode Result for Discrete Functions
##########################################

name, x, z, arg
***************
see
:ref:`var_dis_op@name` ,
:ref:`var_dis_op@x` ,
:ref:`var_dis_op@z` ,
:ref:`var_dis_op@arg`

Prototype
*********

RecBase
=======
{xrst_literal
   // BEGIN_DIS_FORWARD_ANY
   // END_DIS_FORWARD_ANY
}

AD<RecBase>
===========
{xrst_literal
   // BEGIN_AD_DIS_FORWARD_ANY
   // END_AD_DIS_FORWARD_ANY
}

RecBase
*******
Is the Base type when this function was recorded; i.e.,
:ref:`Discrete@ax` and :ref:`Discrete@ay` have type ``AD`` < *RecBase* > .

order_low
*********
is the lowest order Taylor coefficient that will be calculated.

order_up
********
is the highest order Taylor coefficient that will be calculated.

n_dir
*****
is the number of directions, for each order,
that will be calculated (except for order zero which only has one direction).

i_z
***
variable index corresponding to the result for this operation;
i.e. the row index in *taylor* or *ataylor* corresponding to *z* .

cap_order
*********
is the maximum number of orders that can fit in *taylor* or *ataylor* .

tpv
***
We use the notation

   *tpv* = ( *cap_order* - 1 ) * *n_dir*  + 1

which is the number of Taylor coefficients per variable

taylor
******

Input
=====
The zero order Taylor coefficient corresponding to x::

   taylor[ arg[1] * tpv + 0 ]

Output
======
If *order_low* is zero::

   taylor[ i_z * tpv + 0 ]

is the zero order Taylor coefficient corresponding to z.
For k = max(order_low, 1), ... , order_up,

   taylor[ i_z * tpv + (k-1)*n_dir + 1 + ell ]

is the k-th order Taylor coefficient corresponding to z (which is zero).

ataylor
*******
This has the same description as *taylor* except that its type is
``AD`` < *RecBase* > instead of *RecBase* .

{xrst_end dis_forward_dir}
*/
namespace CppAD { namespace local { namespace var_op {

// ---------------------------------------------------------------------------
// BEGIN_DIS_FORWARD_ANY
template <class RecBase>
inline void dis_forward_dir(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   RecBase*      taylor      )
// END_DIS_FORWARD_ANY
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   size_t r = n_dir;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(DisOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(DisOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < r );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   RecBase* x = taylor + size_t(arg[1]) * num_taylor_per_var;
   RecBase* z = taylor +    i_z * num_taylor_per_var;

   if( p == 0 )
   {  z[0]  = discrete<RecBase>::eval(size_t(arg[0]), x[0]);
      p++;
   }
   for(size_t ell = 0; ell < r; ell++)
      for(size_t k = p; k <= q; k++)
         z[ (k-1) * r + 1 + ell ] = RecBase(0.0);
}
// ---------------------------------------------------------------------------
// BEGIN_AD_DIS_FORWARD_ANY
template <class RecBase>
inline void dis_forward_dir(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   AD<RecBase>*  ataylor     )
// END_AD_DIS_FORWARD_ANY
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   size_t r = n_dir;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(DisOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(DisOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < r );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   AD<RecBase>* ax = ataylor + size_t(arg[1]) * num_taylor_per_var;
   AD<RecBase>* az = ataylor +    i_z * num_taylor_per_var;

   if( p == 0 )
   {  az[0]  = discrete<RecBase>::ad_eval(size_t(arg[0]), ax[0]);
      p++;
   }
   for(size_t ell = 0; ell < r; ell++)
      for(size_t k = p; k <= q; k++)
         az[ (k-1) * r + 1 + ell ] = AD<RecBase>(0.0);
}

} } } // END namespace
# endif
