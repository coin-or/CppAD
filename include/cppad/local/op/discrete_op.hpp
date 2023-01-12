# ifndef CPPAD_LOCAL_OP_DISCRETE_OP_HPP
# define CPPAD_LOCAL_OP_DISCRETE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin discrete_op dev}
{xrst_spell
   ataylor
   tpv
}

Forward Mode Result for Discrete Functions
##########################################

Syntax
******

| ``forward_dis_op`` (
| |tab| *p* , *q* , *r* , *i_z* , *arg* , *cap_order* , *taylor*
| ) ``forward_dis_op`` (
| |tab| *p* , *q* , *r* , *i_z* , *arg* , *cap_order* , *ataylor*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Usage
*****
The C++ source code corresponding to this operation is

   *az* = *f* ( *ax* )

where *f* is a piecewise constant function and it's derivative is
always calculated as zero.

RecBase
*******
Is the Base type when this function was recorded; i.e.,
*ax* and *az* have type ``AD`` < *RecBase* > .

p
*
is the lowest order Taylor coefficient that will be calculated.

q
*
is the highest order Taylor coefficient that will be calculated.

r
*
is the number of directions, for each order,
that will be calculated (except for order zero which only has one direction).

i_z
***
variable index corresponding to the result for this operation;
i.e. the row index in *taylor* or *ataylor* corresponding to
*z* .

arg
***

arg[0]
======
is the index, in the order of the discrete functions defined by the user,
for this discrete function.

arg[1]
======
variable index corresponding to the argument for this operator;
i.e. the row index in *taylor* or *ataylor* corresponding to x.

cap_order
*********
maximum number of orders that will fit in the taylor array.

tpv
***
We use the notation

   *tpv* = ( *cap_order* ``-1`` ) * *r*  + 1

which is the number of Taylor coefficients per variable

taylor
******
The type of this parameter is *RecBase* .

Input
=====
The value taylor[ arg[1] * tpv + 0 ]
is the zero order Taylor coefficient corresponding to x.

Output
======
If *p* is zero,
taylor[ i_z * tpv + 0 ]
is the zero order Taylor coefficient corresponding to z.
For k = max(p, 1), ... , q,
taylor[ i_z * tpv + (k-1)*r + 1 + ell ]
is the k-th order Taylor coefficient corresponding to z
(which is zero).

ataylor
*******
The type of this parameter is ``AD`` < *RecBase* > .
Otherwise, it has the same description as *taylor* .

Asserts
*******
NumArg(op) == 2, NumRes(op) == 1,  q < cap_order, 0 < r

{xrst_end discrete_op}
*/
namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE

// ---------------------------------------------------------------------------
// BEGIN_PROTOTYPE
template <class RecBase>
void forward_dis_op(
   size_t        p           ,
   size_t        q           ,
   size_t        r           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   RecBase*      taylor      )
// END_PROTOTYPE
{
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
template <class RecBase>
void forward_dis_op(
   size_t        p           ,
   size_t        q           ,
   size_t        r           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   AD<RecBase>*  ataylor     )
{
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

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
