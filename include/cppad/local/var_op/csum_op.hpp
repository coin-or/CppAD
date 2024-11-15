# ifndef CPPAD_LOCAL_VAR_OP_CSUM_OP_HPP
# define CPPAD_LOCAL_VAR_OP_CSUM_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
/*
{xrst_begin_parent var_csum_op dev}
{xrst_spell
}

Variable Cumulative Summation Operator
######################################

CSumOp
******
is the op code for this operator.

User Syntax
***********
::

   z = s
       + x[0] + ... + x[ n1 - 1 ]
       - y[0] - ... - y[ n2 - 1 ]
       + u[0] + ... + u[ n3 - 1 ]
       - v[0] - ... - v[ n4 - 1 ]

s
*
is the constant parameter that initializes the summation.

x
*
is the vector of addition variables that appear in the sum.

y
*
is the vector of subtraction variables that appear in the sum.

u
*
is the vector of addition dynamic parameters that appear in the sum.

v
*
is the vector of subtraction dynamic parameters that appear in the sum.

z
*
is the result of the summation.

Base
****
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

i_z
***
is the variable index corresponding to the result for this operation;
i.e. the row index in taylor corresponding to *z* .

arg
***

arg[0]
======
is the index of the constant parameter *s* in the parameter vector.

arg[1]
======
is the index in arg of the end of the addition variables; i.e.,
*n1* = arg[1] - 5 and

arg[2]
======
is the index in arg of the end of the subtraction variables; i.e.,
*n2* = arg[2] - arg[1] and

arg[3]
======
is the index in arg of the end of the addition dynamic parameters; i.e.,
*n3* = arg[3] - arg[2] and

arg[4]
======
is the index in arg of the end of the subtraction dynamic parameters; i.e.,
*n4* = arg[4] - arg[3] and

arg[5+j]
========
for j = 0 , ... , n1 - 1, arg[5 + j]
is the index corresponding to the variable x[j] .

arg[ arg[1]+j ]
===============
for j = 0 , ... , n2 - 1, arg[ arg[1] + j ]
is the index corresponding to the variable y[j] .

arg[ arg[2]+j ]
===============
for j = 0 , ... , n3 - 1, arg[ arg[2] + j ]
is the index corresponding to the dynamic parameter u[j] .

arg[ arg[3]+j ]
===============
for j = 0 , ... , n4 - 1, arg[ arg[3] + j ]
is the index corresponding to the dynamic parameter v[j] .

arg[ arg[4] ]
=============
This is equal to arg[4] .
Note that there are arg[4] + 1 arguments to this operator
and having this value at the end enable reverse model to know how far
to back up to get to the start of this operation.

{xrst_end var_csum_op}
-----------------------------------------------------------------------------
{xrst_begin var_csum_forward_op dev}

Forward Mode Cumulative Summation Operation
###########################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_FORWARD_OP
   // END_CSUM_FORWARD_OP
}

x, y, u, v, z
*************
see
:ref:`var_csum_op@x` ,
:ref:`var_csum_op@y` ,
:ref:`var_csum_op@u` ,
:ref:`var_csum_op@v` ,
:ref:`var_csum_op@z`

i_z, arg
********
see
:ref:`var_csum_op@i_z` ,
:ref:`var_csum_op@arg`

p
*
lowest order of the Taylor coefficient that we are computing.

q
*
highest order of the Taylor coefficient that we are computing.

num_par
*******
is the number of parameters in parameter.

parameter
*********
is the parameter vector for this operation sequence.

cap_order
*********
number of columns in the matrix containing all the Taylor coefficients.

taylor
******

Input
=====
#. For j = 0, ..., i_z-1, and k = 0 , ... , q,
   taylor [ j * cap_order + k ]
   is the k-th order Taylor coefficient corresponding to the variable
   with index *j* .
#. For k = 0 , ... , q-1,
   taylor [ i_z * cap_order + k ]
   is the k-th order Taylor coefficient corresponding to z.

Output
======
For k = p , ... , q, taylor [ i_z * cap_order + k ]
is the k-th order Taylor coefficient corresponding to z.

{xrst_end var_csum_forward_op}
*/
// BEGIN_CSUM_FORWARD_OP
template <class Base>
inline void csum_forward_op(
   size_t        p           ,
   size_t        q           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
// END_CSUM_FORWARD_OP
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumRes(CSumOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN(
      arg[arg[4]] == arg[4]
   );
   //
   // zero
   Base zero(0);
   //
   // z
   Base* z = taylor + i_z    * cap_order;
   for(size_t k = p; k <= q; k++)
      z[k] = zero;
   //
   if( p == 0 )
   {  // constant parameter
      z[p] = parameter[ arg[0] ];
      // addition dynamic parameters
      for(addr_t i = arg[2]; i < arg[3]; ++i)
         z[p] += parameter[ arg[i] ];
      // subtraction dynamic parameters
      for(addr_t i = arg[3]; i < arg[4]; ++i)
         z[p] -= parameter[ arg[i] ];
   }
   // addition variables
   for(addr_t i = 5; i < arg[1]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * cap_order;
      for(size_t k = p; k <= q; k++)
         z[k] += x[k];
   }
   // subtraction variables
   for(addr_t i = arg[1]; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * cap_order;
      for(size_t k = p; k <= q; k++)
         z[k] -= x[k];
   }
}
/*
{xrst_begin var_csum_forward_dir dev}
{xrst_spell
   cpv
}

Multiple Direction Forward Mode Cumulative Summation Operation
##############################################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_FORWARD_DIR
   // END_CSUM_FORWARD_DIR
}

x, y, u, v, z
*************
see
:ref:`var_csum_op@x` ,
:ref:`var_csum_op@y` ,
:ref:`var_csum_op@u` ,
:ref:`var_csum_op@v` ,
:ref:`var_csum_op@z`

i_z, arg
********
see
:ref:`var_csum_op@i_z` ,
:ref:`var_csum_op@arg`

q
*
order of the Taylor coefficient that we are computing.

r
*
number of directions that we are computing the Taylor coefficient for.

num_par
*******
is the number of parameters in parameter.

parameter
*********
is the parameter vector for this operation sequence.

cap_order
*********
number of columns in the matrix containing all the Taylor coefficients.

taylor
******

cpv
===
For each variable there is one Taylor coefficient of order zero
and *r* coefficients for orders greater than zero.
The taylor coefficients capacity per variable is::

   cpv = (cap_order - 1) * r + 1

(j, k, ell)
===========
For j a variable index, k an order, and ell a direction index::

   if k == 0
      (j, k, ell) = j * cpv + (k-1) * r
   else
      (j, k, ell) = j * cpv + (k-1) * r + 1 + ell

The value taylor[ (j, k, ell) ] is the
Taylor coefficient corresponding to
the variable with index j, the order k, and the direction with index ell.


Input
=====
#. For j = 0, ..., i_z-1, k = 0 , ... , q, ell = 0 , ... , r-1,
   taylor [ (j, k, ell) ]
#. For k = 0 , ... , q-1, ell = 0 , ... , r-1,
   taylor [ (i_z, k, ell) ]

Output
======
For ell = 0, ... , r-1, taylor [ (i_z, q, ell) ]


{xrst_end var_csum_forward_dir}
*/
// BEGIN_CSUM_FORWARD_DIR
template <class Base>
inline void csum_forward_dir(
   size_t        q           ,
   size_t        r           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
// END_CSUM_FORWARD_DIR
{
   CPPAD_ASSERT_UNKNOWN( NumRes(CSumOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN(
      arg[arg[4]] == arg[4]
   );
   //
   // zero
   Base zero(0);
   //
   // cpv
   size_t cpv = (cap_order-1) * r + 1;
   //
   // m
   size_t m                  = (q-1)*r + 1;
   //
   // z
   Base* z = taylor + i_z * cpv + m;
   for(size_t ell = 0; ell < r; ell++)
      z[ell] = zero;
   //
   // addition variables
   for(addr_t i = 5; i < arg[1]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * cpv + m;
      for(size_t ell = 0; ell < r; ell++)
         z[ell] += x[ell];
   }
   //
   // subtraction variables
   for(addr_t i = arg[1]; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * cpv + m;
      for(size_t ell = 0; ell < r; ell++)
         z[ell] -= x[ell];
   }
}
/*!
Compute reverse mode Taylor coefficients for result of op = CsumOp.

This operation is
\verbatim
   z = q + x(0) + ... + x(m-1) - y(0) - ... - y(n-1).
   H(y, x, w, ...) = G[ z(x, y), y, x, w, ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type
Base.

\param d
order the highest order Taylor coefficient that we are computing
the partial derivatives with respect to.

\param i_z
variable index corresponding to the result for this operation;
i.e. the row index in taylor corresponding to z.

\param arg
-- arg[0]
parameter[arg[0]] is the parameter value s in this cummunative summation.

-- arg[1]
end in arg of addition variables in summation.
arg[5] , ... , arg[arg[1]-1] correspond to x(0), ... , x(m-1)

-- arg[2]
end in arg of subtraction variables in summation.
arg[arg[1]] , ... , arg[arg[2]-1] correspond to y(0), ... , y(n-1)

-- arg[3]
end in arg of addition dynamic parameters in summation.

-- arg[4]
end in arg of subtraction dynamic parameters in summation.

\param nc_partial
number of columns in the matrix containing all the partial derivatives.

\param partial
\b Input: partial [ arg[5+i] * nc_partial + k ]
for i = 0, ..., m-1
and k = 0 , ... , d
is the partial derivative of G(z, y, x, w, ...) with respect to the
k-th order Taylor coefficient corresponding to x(i)
\n
\b Input: partial [ arg[arg[1]+1] * nc_partial + k ]
for i = 0, ..., n-1
and k = 0 , ... , d
is the partial derivative of G(z, y, x, w, ...) with respect to the
k-th order Taylor coefficient corresponding to y(i)
\n
\b Input: partial [ i_z * nc_partial + k ]
for i = 0, ..., n-1
and k = 0 , ... , d
is the partial derivative of G(z, y, x, w, ...) with respect to the
k-th order Taylor coefficient corresponding to z.
\n
\b Output: partial [ arg[5+i] * nc_partial + k ]
for i = 0, ..., m-1
and k = 0 , ... , d
is the partial derivative of H(y, x, w, ...) with respect to the
k-th order Taylor coefficient corresponding to x(i)
\n
\b Output: partial [ arg[arg[1]+1] * nc_partial + k ]
for i = 0, ..., n-1
and k = 0 , ... , d
is the partial derivative of H(y, x, w, ...) with respect to the
k-th order Taylor coefficient corresponding to y(i)
*/

template <class Base>
inline void reverse_csum_op(
   size_t        d           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        nc_partial  ,
   Base*         partial     )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumRes(CSumOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( d < nc_partial );

   // Taylor coefficients and partial derivative corresponding to result
   Base* pz = partial + i_z * nc_partial;
   Base* px;
   size_t d1 = d + 1;
   for(size_t i = 5; i < size_t(arg[1]); ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      px    = partial + size_t(arg[i]) * nc_partial;
      size_t k = d1;
      while(k--)
         px[k] += pz[k];
   }
   for(size_t i = size_t(arg[1]); i < size_t(arg[2]); ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      px    = partial + size_t(arg[i]) * nc_partial;
      size_t k = d1;
      while(k--)
         px[k] -= pz[k];
   }
}


/*!
Forward mode Jacobian sparsity pattern for CSumOp operator.

This operation is
\verbatim
   z = q + x(0) + ... + x(m-1) - y(0) - ... - y(n-1).
\endverbatim

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
variable index corresponding to the result for this operation;
i.e. the index in sparsity corresponding to z.

\param arg
-- arg[0]
parameter[arg[0]] is the parameter value s in this cummunative summation.

-- arg[1]
end in arg of addition variables in summation.
arg[5] , ... , arg[arg[1]-1] correspond to x(0), ... , x(m-1)

-- arg[2]
end in arg of subtraction variables in summation.
arg[arg[1]] , ... , arg[arg[2]-1] correspond to y(0), ... , y(n-1)

-- arg[3]
end in arg of addition dynamic parameters in summation.

-- arg[4]
end in arg of subtraction dynamic parameters in summation.

\param sparsity
\b Input:
For i = 0, ..., m-1,
the set with index arg[5+i] in sparsity
is the sparsity bit pattern for x(i).
This identifies which of the independent variables the variable
x(i) depends on.
\n
\b Input:
For i = 0, ..., n-1,
the set with index arg[2+arg[0]+i] in sparsity
is the sparsity bit pattern for x(i).
This identifies which of the independent variables the variable
y(i) depends on.
\n
\b Output:
The set with index i_z in sparsity
is the sparsity bit pattern for z.
This identifies which of the independent variables the variable z
depends on.
*/

template <class Vector_set>
inline void forward_sparse_jacobian_csum_op(
   size_t           i_z         ,
   const addr_t*    arg         ,
   Vector_set&      sparsity    )
{  sparsity.clear(i_z);

   for(size_t i = 5; i < size_t(arg[2]); ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      sparsity.binary_union(
         i_z        , // index in sparsity for result
         i_z        , // index in sparsity for left operand
         size_t(arg[i]), // index for right operand
         sparsity     // sparsity vector for right operand
      );
   }
}

/*!
Reverse mode Jacobian sparsity pattern for CSumOp operator.

This operation is
\verbatim
   z = q + x(0) + ... + x(m-1) - y(0) - ... - y(n-1).
   H(y, x, w, ...) = G[ z(x, y), y, x, w, ... ]
\endverbatim

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
variable index corresponding to the result for this operation;
i.e. the index in sparsity corresponding to z.

\param arg
-- arg[0]
parameter[arg[0]] is the parameter value s in this cummunative summation.

-- arg[1]
end in arg of addition variables in summation.
arg[5] , ... , arg[arg[1]-1] correspond to x(0), ... , x(m-1)

-- arg[2]
end in arg of subtraction variables in summation.
arg[arg[1]] , ... , arg[arg[2]-1] correspond to y(0), ... , y(n-1)

-- arg[3]
end in arg of addition dynamic parameters in summation.

-- arg[4]
end in arg of subtraction dynamic parameters in summation.

\param sparsity
For i = 0, ..., m-1,
the set with index arg[5+i] in sparsity
is the sparsity bit pattern for x(i).
This identifies which of the dependent variables depend on x(i).
On input, the sparsity patter corresponds to G,
and on ouput it corresponds to H.
\n
For i = 0, ..., m-1,
the set with index arg[2+arg[0]+i] in sparsity
is the sparsity bit pattern for y(i).
This identifies which of the dependent variables depend on y(i).
On input, the sparsity patter corresponds to G,
and on ouput it corresponds to H.
\n
\b Input:
The set with index i_z in sparsity
is the sparsity bit pattern for z.
On input it corresponds to G and on output it is undefined.
*/

template <class Vector_set>
inline void reverse_sparse_jacobian_csum_op(
   size_t           i_z         ,
   const addr_t*    arg         ,
   Vector_set&      sparsity    )
{
   for(size_t i = 5; i < size_t(arg[2]); ++i)
   {
      CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      sparsity.binary_union(
         size_t(arg[i]), // index in sparsity for result
         size_t(arg[i]), // index in sparsity for left operand
         i_z        , // index for right operand
         sparsity     // sparsity vector for right operand
      );
   }
}
/*!
Reverse mode Hessian sparsity pattern for CSumOp operator.

This operation is
\verbatim
   z = q + x(0) + ... + x(m-1) - y(0) - ... - y(n-1).
   H(y, x, w, ...) = G[ z(x, y), y, x, w, ... ]
\endverbatim

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
variable index corresponding to the result for this operation;
i.e. the index in sparsity corresponding to z.

\param arg
-- arg[0]
parameter[arg[0]] is the parameter value s in this cummunative summation.

-- arg[1]
end in arg of addition variables in summation.
arg[5] , ... , arg[arg[1]-1] correspond to x(0), ... , x(m-1)

-- arg[2]
end in arg of subtraction variables in summation.
arg[arg[1]] , ... , arg[arg[2]-1] correspond to y(0), ... , y(n-1)

-- arg[3]
end in arg of addition dynamic parameters in summation.

-- arg[4]
end in arg of subtraction dynamic parameters in summation.

\param rev_jacobian
rev_jacobian[i_z]
is all false (true) if the Jabobian of G with respect to z must be zero
(may be non-zero).
\n
\n
For i = 0, ..., m-1
rev_jacobian[ arg[5+i] ]
is all false (true) if the Jacobian with respect to x(i)
is zero (may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.
\n
\n
For i = 0, ..., n-1
rev_jacobian[ arg[2+arg[0]+i] ]
is all false (true) if the Jacobian with respect to y(i)
is zero (may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.

\param rev_hes_sparsity
The set with index i_z in in rev_hes_sparsity
is the Hessian sparsity pattern for the function G
where one of the partials derivative is with respect to z.
\n
\n
For i = 0, ..., m-1
The set with index arg[5+i] in rev_hes_sparsity
is the Hessian sparsity pattern
where one of the partials derivative is with respect to x(i).
On input, it corresponds to the function G,
and on output it corresponds to the function H.
\n
\n
For i = 0, ..., n-1
The set with index arg[2+arg[0]+i] in rev_hes_sparsity
is the Hessian sparsity pattern
where one of the partials derivative is with respect to y(i).
On input, it corresponds to the function G,
and on output it corresponds to the function H.
*/

template <class Vector_set>
inline void reverse_sparse_hessian_csum_op(
   size_t           i_z                 ,
   const addr_t*    arg                 ,
   bool*            rev_jacobian        ,
   Vector_set&      rev_hes_sparsity    )
{
   for(size_t i = 5; i < size_t(arg[2]); ++i)
   {
      CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      rev_hes_sparsity.binary_union(
      size_t(arg[i]), // index in sparsity for result
      size_t(arg[i]), // index in sparsity for left operand
      i_z                , // index for right operand
      rev_hes_sparsity     // sparsity vector for right operand
      );
      rev_jacobian[arg[i]] |= rev_jacobian[i_z];
   }
}

} } } // END namespace
# endif
