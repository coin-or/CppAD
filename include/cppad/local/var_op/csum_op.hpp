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
*n1* = arg[1] - 5 .

arg[2]
======
is the index in arg of the end of the subtraction variables; i.e.,
*n2* = arg[2] - arg[1] .

arg[3]
======
is the index in arg of the end of the addition dynamic parameters; i.e.,
*n3* = arg[3] - arg[2] .

arg[4]
======
is the index in arg of the end of the subtraction dynamic parameters; i.e.,
*n4* = arg[4] - arg[3] .

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

order_low
*********
lowest order of the Taylor coefficient that we are computing.

order_up
********
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
::

   for j = 0, ..., i_z - 1
      for k = 0 , ... , order_up
         taylor [ j * cap_order + k ] is an input

   for  k = 0 , ... , order_up - 1
      taylor [ i_z * cap_order + k ] is an input

Output
======
::
   for k = order_low , ... , order_up
      taylor [ i_z * cap_order + k ] is an output

{xrst_end var_csum_forward_op}
*/
// BEGIN_CSUM_FORWARD_OP
template <class Base>
inline void csum_forward_op(
   size_t        order_low   ,
   size_t        order_up    ,
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
   CPPAD_ASSERT_UNKNOWN( order_up < cap_order );
   CPPAD_ASSERT_UNKNOWN( order_low <= order_up );
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
   for(size_t k = order_low; k <= order_up; k++)
      z[k] = zero;
   //
   if( order_low == 0 )
   {  // constant parameter
      z[order_low] = parameter[ arg[0] ];
      // addition dynamic parameters
      for(addr_t i = arg[2]; i < arg[3]; ++i)
         z[order_low] += parameter[ arg[i] ];
      // subtraction dynamic parameters
      for(addr_t i = arg[3]; i < arg[4]; ++i)
         z[order_low] -= parameter[ arg[i] ];
   }
   // addition variables
   for(addr_t i = 5; i < arg[1]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * cap_order;
      for(size_t k = order_low; k <= order_up; k++)
         z[k] += x[k];
   }
   // subtraction variables
   for(addr_t i = arg[1]; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * cap_order;
      for(size_t k = order_low; k <= order_up; k++)
         z[k] -= x[k];
   }
}
/*
{xrst_begin var_csum_forward_dir dev}
{xrst_spell
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

order_up
********
order of the Taylor coefficient that we are computing.

n_dir
*****
number of directions that we are computing the Taylor coefficient for.

num_par
*******
is the number of parameters in parameter.

parameter
*********
is the parameter vector for this operation sequence.

{xrst_comment cap_order, taylor}
{xrst_template ,
   include/cppad/local/var_op/forward_dir.xrst
}

{xrst_end var_csum_forward_dir}
*/
// BEGIN_CSUM_FORWARD_DIR
template <class Base>
inline void csum_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
// END_CSUM_FORWARD_DIR
{
   CPPAD_ASSERT_UNKNOWN( NumRes(CSumOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( order_up < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < order_up );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN(
      arg[arg[4]] == arg[4]
   );
   //
   // zero
   Base zero(0);
   //
   // per_variable
   size_t per_variable = (cap_order-1) * n_dir + 1;
   //
   // m
   size_t m = (order_up - 1) * n_dir + 1;
   //
   // z
   Base* z = taylor + i_z * per_variable + m;
   for(size_t ell = 0; ell < n_dir; ell++)
      z[ell] = zero;
   //
   // addition variables
   for(addr_t i = 5; i < arg[1]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * per_variable + m;
      for(size_t ell = 0; ell < n_dir; ell++)
         z[ell] += x[ell];
   }
   //
   // subtraction variables
   for(addr_t i = arg[1]; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* x = taylor + size_t(arg[i]) * per_variable + m;
      for(size_t ell = 0; ell < n_dir; ell++)
         z[ell] -= x[ell];
   }
}
/*
---------------------------------------------------------------------------
{xrst_begin var_csum_reverse_op dev}

Reverse Mode Cumulative Summation Operation
###########################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_REVERSE_OP
   // END_CSUM_REVERSE_OP
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

{xrst_comment document n_order, partial}
{xrst_template ;
   include/cppad/local/var_op/reverse.xrst
   @x, y@  ; x, y, u, v
}

{xrst_end var_csum_reverse_op}
*/
// BEGIN_CSUM_REVERSE_OP
template <class Base>
inline void csum_reverse_op(
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        n_order     ,
   Base*         partial     )
// END_CSUM_REVERSE_OP
{  // d
   size_t d = n_order - 1;
   //
   //
   CPPAD_ASSERT_UNKNOWN( NumRes(CSumOp) == 1 );
   //
   // pz, dp1
   Base* pz = partial + i_z * n_order;
   size_t dp1 = d + 1;
   //
   // addition variables
   for(addr_t i = 5; i < arg[1]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* px = partial + size_t(arg[i]) * n_order;
      size_t k = dp1;
      while(k--)
         px[k] += pz[k];
   }
   //
   // subtraction varables
   for(addr_t i = arg[1]; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      Base* px = partial + size_t(arg[i]) * n_order;
      size_t k = dp1;
      while(k--)
         px[k] -= pz[k];
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin var_csum_forward_jac dev}

Forward Jacobian Sparsity for Cumulative Summation
##################################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_FORWARD_JAC
   // END_CSUM_FORWARD_JAC
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

Vector_set
**********
is the type used for vectors of sets.
It must satisfy the :ref:`SetVector-name` concept.

i_z

sparsity
********

Input
=====
For k = 0 , ... , i_z - 1,
the set with index k in *sparsity*
identifies which independent variables the variable with index k depends on.

Output
======
The set with index i_z in *sparsity*
identifies which independent variables the variable *z* depends on.

{xrst_end var_csum_forward_jac}
*/
// BEGIN_CSUM_FORWARD_JAC
template <class Vector_set>
inline void csum_forward_jac(
   size_t           i_z         ,
   const addr_t*    arg         ,
   Vector_set&      sparsity    )
// END_CSUM_FORWARD_JAC
{  //
   // sparsity
   sparsity.clear(i_z);
   //
   // addition and subtraction variables
   for(addr_t i = 5; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      sparsity.binary_union(
         i_z            , // index in sparsity for result
         i_z            , // index in sparsity for left operand
         size_t(arg[i]) , // index for right operand
         sparsity         // sparsity vector for right operand
      );
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin var_csum_reverse_jac dev}

Reverse Jacobian Sparsity for Cumulative Summation
##################################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_REVERSE_JAC
   // END_CSUM_REVERSE_JAC
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

Vector_set
**********
is the type used for vectors of sets.
It must satisfy the :ref:`SetVector-name` concept.


sparsity
********
The set with index *i_z* is the sparsity pattern for *z* .
This sparsity pattern is added th the sparsity pattern for the
variables in the vectors *x* and *y* .

{xrst_end var_csum_reverse_jac}
*/
// BEGIN_CSUM_REVERSE_JAC
template <class Vector_set>
inline void csum_reverse_jac(
   size_t           i_z         ,
   const addr_t*    arg         ,
   Vector_set&      sparsity    )
// END_CSUM_REVERSE_JAC
{
   // addition and subtraction variables
   for(addr_t i = 5; i < arg[2]; ++i)
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
/*
{xrst_begin var_csum_reverse_hes dev}

Reverse Hessian Sparsity for Cumulative Summation
#################################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_REVERSE_HES
   // END_CSUM_REVERSE_HES
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

G, H
****
We use *G* to denote the scalar function we are computing the
sparsity pattern for as a function of the variables up to and including *z* .
We use *H* for the function with *z* replaced by its operator; i.e.::

   H(x, y, u, v, ...) = G[ z(x, y, u, v) , x, y, u, v, ... ]

Vector_set
**********
is the type used for vectors of sets.
It must satisfy the :ref:`SetVector-name` concept.

rev_jacobian
************
#. If rev_jacobian[i_z] is true (false),
   *G* may depend (does not depend) on *z* .

#. If *j* is the index of an addition variable in the vector *x* or *y* ,
   and *G* may depend on *z* ,
   rev_jacobian[j] is set to true.

rev_hes_sparsity
****************
On input, *rev_hes_sparsity*
contains the Hessian sparsity pattern for the function *G* .
On output, it contains the Hessian sparsity pattern for the function *H* .

{xrst_end var_csum_reverse_hes}
*/
// BEGIN_CSUM_REVERSE_HES
template <class Vector_set>
inline void csum_reverse_hes(
   size_t           i_z                 ,
   const addr_t*    arg                 ,
   bool*            rev_jacobian        ,
   Vector_set&      rev_hes_sparsity    )
// END_CSUM_REVERSE_HES
{  //
   // addition and subtraction variables
   for(addr_t i = 5; i < arg[2]; ++i)
   {
      CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      rev_hes_sparsity.binary_union(
         size_t(arg[i])     , // index in sparsity for result
         size_t(arg[i])     , // index in sparsity for left operand
         i_z                , // index for right operand
         rev_hes_sparsity     // sparsity vector for right operand
      );
      rev_jacobian[arg[i]] |= rev_jacobian[i_z];
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin var_csum_forward_hes dev}

Forward Hessian Sparsity for Cumulative Summation
#################################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_FORWARD_HES
   // END_CSUM_FORWARD_HES
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

Vector_set
**********
is the type used for vectors of sets.
It must satisfy the :ref:`SetVector-name` concept.

n
*
is the number of independent variables on the tape.

for_hes_sparse
**************
see :ref:`local_sweep_for_hes@for_hes_sparse` .

{xrst_end var_csum_forward_hes}
*/
// BEGIN_CSUM_FORWARD_HES
template <class Vector_set>
inline void csum_forward_hes(
   const addr_t*             arg            ,
   size_t                    i_z            ,
   size_t                    n              ,
   Vector_set&               for_hes_sparse )
// END_CSUM_FORWARD_HES
{  //
   // np1
   size_t np1 = n + 1;
   //
   // for_hes_sparse
   for_hes_sparse.clear(np1 + i_z);
   //
   // addition and subtraction variables
   for(addr_t i = 5; i < arg[2]; ++i)
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < i_z );
      //
      // linear functions only modify forward Jacobian sparsity
      for_hes_sparse.binary_union(
         np1 + i_z            , // index in sparsity for result
         np1 + i_z            , // index in sparsity for left operand
         np1 + size_t(arg[i]) , // index for right operand
         for_hes_sparse         // sparsity vector for right operand
      );
   }
}

} } } // END namespace
# endif
