# ifndef CPPAD_LOCAL_VAR_OP_STORE_OP_HPP
# define CPPAD_LOCAL_VAR_OP_STORE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
/*
{xrst_begin_parent var_store_op dev}
{xrst_spell
   stpp
   stpv
   stvp
   stvv
}

Store an Element of a Variable VecAD Vector
###########################################

StppOp, StpvOp, StvpOp, StvvOp
******************************
are the op codes for these operators.

User Syntax
***********
| *v* [ *x* ] = *y*

v
*
is the :ref:`VecAD-name` vector for this store operation.
This vector is a variable after the store.
if this is a StppOp operation, *v* is a variable before the store.

x
*
is the index for this store.

y
*
is the value being stored.

Base
****
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by these operators done using type Base.

op_code
*******

.. csv-table::
   :widths: auto
   :header-rows: 1

   op_code, x, y
   StppOp, parameter, parameter
   StpvOp, parameter, variable
   StvpOp, variable,  parameter
   StvvOp, variable,  variable

num_vecad_ind
*************
is the size of the single array that includes
all the VecAD vectors together with the size of each vector.

arg
***

arg[0]
======
this argument is the offset of the vector *v*
relative to the beginning of the single array
that contains all VecAD elements and sizes.
This offset corresponds to the first element of *v* and not its size
which comes just before the first element.

arg[1]
======
If *x* is a parameter (variable), arg[1] is the parameter index
(variable index) to *x* .

arg[2]
======
If *y* is a parameter (variable), arg[2] is the parameter index
(variable index) to *y* .

{xrst_end var_store_op}
------------------------------------------------------------------------------
{xrst_begin var_store_forward_0 dev}
{xrst_spell
   isvar
}

Zero Order Forward Store an Element of a VecAD Vector
#####################################################

v, x, y
*******
see
:ref:`var_store_op@v` ,
:ref:`var_store_op@x` ,
:ref:`var_store_op@y`

Prototype
*********
{xrst_literal
   // BEGIN_STORE_FORWARD_0
   // END_STORE_FORWARD_0
}

Base, op_code, num_vecad_ind, arg
*********************************
see
:ref:`var_store_op@Base` ,
:ref:`var_store_op@op_code` ,
:ref:`var_store_op@num_vecad_ind` ,
:ref:`var_store_op@arg` .

num_var
*******
is the number of variables in this recording.

num_par
*******
is the number of parameters in this recording.

parameter
*********
This is the vector of parameters for this recording which has size *num_par* .

cap_order
*********
is the maximum number of orders that can fit in *taylor* .

taylor
******
Is the matrix of Taylor coefficients for all the variables.

i_vec
*****
We use *i_vec* to denote the ``size_t`` value corresponding to
:ref:`var_store_op@x` .
If *x* is a parameter (variable) this is a parameter (variable) index.

vec_ad2isvar
************
This vector has size :ref:`var_store_op@num_vecad_ind` .
The input values of its elements does not matter.
If the value being stored is a parameter (variable),
*vec_ad2isvar* [ *arg* [0] + *i_vec*  ] is set to false (true).

vec_ad2index
************
This vector has size *num_vecad_ind* .
The input value of its elements does not matter.
If the value being stored is a parameter (variable),
*vec_ad2index* [ *arg* [0] + *i_vec*  ]
is set to the parameter (variable) index
corresponding to the value being stored.

{xrst_end var_store_forward_0}
*/
// BEGIN_STORE_FORWARD_0
template <class Base>
inline void store_forward_0(
   op_code_var          op_code        ,
   const addr_t*        arg            ,
   size_t               num_var        ,
   size_t               num_par        ,
   const Base*          parameter      ,
   size_t               cap_order      ,
   const Base*          taylor         ,
   pod_vector<bool>&    vec_ad2isvar   ,
   pod_vector<size_t>&  vec_ad2index   )
// END_STORE_FORWARD_0
{  //
   //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( vec_ad2isvar.size() == vec_ad2index.size() )
   //
   // i_y
   size_t i_y = size_t( arg[2] );
   //
   // i_vec, isvar
   // assign here to avoid compiler warning for default case
   addr_t i_vec = std::numeric_limits<addr_t>::max();
   bool   isvar = false;
   switch(op_code)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      //
      case StppOp:
      i_vec = addr_t( Integer( parameter[ arg[1] ] ) );
      isvar = false;
      CPPAD_ASSERT_UNKNOWN( i_y < num_par );
      break;
      //
      case StpvOp:
      i_vec = addr_t( Integer( parameter[ arg[1] ] ) );
      isvar = true;
      CPPAD_ASSERT_UNKNOWN( i_y < num_var );
      break;
      //
      case StvpOp:
      i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
      isvar = false;
      CPPAD_ASSERT_UNKNOWN( i_y < num_par );
      break;
      //
      case StvvOp:
      i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
      isvar = true;
      CPPAD_ASSERT_UNKNOWN( i_y < num_var );
      break;
   }
   //
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: zero order forward index out of range"
   );
   //
   // vec_ad2isvar, vec_ad2index
   vec_ad2isvar[ arg[0] + i_vec ]  = isvar;
   vec_ad2index[ arg[0] + i_vec ]  = i_y;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_store_for_jac dev}

Forward Jacobian Sparsity for Store a VecAD Element
###################################################

v, x, y
*******
see
:ref:`var_store_op@v` ,
:ref:`var_store_op@x` ,
:ref:`var_store_op@y`

Prototype
*********
{xrst_literal
   // BEGIN_STORE_FOR_JAC
   // END_STORE_FOR_JAC
}

op_code, num_vecad_ind, arg
***************************
see :ref:`var_store_op@op_code` ,
:ref:`var_store_op@num_vecad_ind` ,
:ref:`var_store_op@arg` .

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

dependency
**********
If true (false) we are including (are not including)
dependencies that have derivative zero in the sparsity pattern.
For example, the :ref:`Discrete-name` functions have derivative zero,
but the value depends on its argument.

vecad_ind
*********
is a vector with size *num_vec_ind* .
We use the notation *i_v* defined by

|tab| *i_v* = vecad_ind[ arg[0] - 1 ]

This is the index of the VecAD vector and is less than the number of
VecAD vectors in the recording.

var_sparsity
************
If :ref:`var_store_op@y` is a variable,
the sets with index arg[2] in *var_sparsity* is the sparsity pattern for *y* .
Otherwise, *y* is a parameter and its sparsity pattern for *y* is empty


vecad_sparsity
**************
The set with index *i_v* in *vecad_sparsity
is the sparsity pattern for the vector *v*.
The sparsity pattern for *y* is added
to the sparsity pattern for *v* .
If *dependency* is true and *x* is a variable,
the sparsity pattern for *x* is also added to the sparsity pattern for *v*.

{xrst_end var_store_for_jac}
*/
// BEGIN_STORE_FOR_JAC
template <class Vector_set>
inline void store_for_jac(
   op_code_var               op_code        ,
   size_t                    num_vecad_ind  ,
   const addr_t*             arg            ,
   bool                      dependency     ,
   const pod_vector<size_t>& vecad_ind      ,
   const Vector_set&         var_sparsity   ,
   Vector_set&               vecad_sparsity )
// END_STORE_FOR_JAC
{  //
   //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( num_vecad_ind == vecad_ind.size() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   //
   // i_x, i_y
   size_t i_x = size_t( arg[1] );
   size_t i_y = size_t( arg[2] );
   //
   switch(op_code)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      //
      case StppOp:
      break;
      //
      case StpvOp:
      vecad_sparsity.binary_union(i_v, i_v, i_y, var_sparsity);
      break;
      //
      case StvpOp:
      if( dependency )
         vecad_sparsity.binary_union(i_v, i_v, i_x, var_sparsity);
      break;
      //
      case StvvOp:
      if( dependency )
         vecad_sparsity.binary_union(i_v, i_v, i_x, var_sparsity);
      vecad_sparsity.binary_union(i_v, i_v, i_y, var_sparsity);
      break;
   }
   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_store_rev_jac dev}

Reverse Jacobian Sparsity for Store a VecAD Element
###################################################

v, x, y
*******
see
:ref:`var_store_op@v` ,
:ref:`var_store_op@x` ,
:ref:`var_store_op@y`

Prototype
*********
{xrst_literal
   // BEGIN_STORE_REV_JAC
   // END_STORE_REV_JAC
}

op_code, num_vecad_ind, arg
***************************
see :ref:`var_store_op@op_code` ,
:ref:`var_store_op@num_vecad_ind` ,
:ref:`var_store_op@arg` .

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

dependency
**********
If true (false) we are including (are not including)
dependencies that have derivative zero in the sparsity pattern.
For example, the :ref:`Discrete-name` functions have derivative zero,
but the value depends on its argument.

vecad_ind
*********
is a vector with size *num_vec_ind* .
We use the notation *i_v* defined by

|tab| *i_v* = vecad_ind[ arg[0] - 1 ]

This is the index of the VecAD vector and is less than the number of
VecAD vectors in the recording.

var_sparsity
************
If :ref:`var_store_op@y` is a variable,
the sparsity pattern for *v* is added to the sparsity pattern for *y*.
If *dependency* is true and *x* is a variable,
the sparsity pattern for *v* is also added to the sparsity pattern for *x*.

vecad_sparsity
**************
The set with index *i_v* in *vecad_sparsity
is the sparsity pattern for the vector *v*.

{xrst_end var_store_rev_jac}
*/
// BEGIN_STORE_REV_JAC
template <class Vector_set>
inline void store_rev_jac(
   op_code_var               op_code        ,
   size_t                    num_vecad_ind  ,
   const addr_t*             arg            ,
   bool                      dependency     ,
   const pod_vector<size_t>& vecad_ind      ,
   Vector_set&               var_sparsity   ,
   const Vector_set&         vecad_sparsity )
// END_STORE_REV_JAC
{  //
   //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( num_vecad_ind == vecad_ind.size() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   //
   // i_x, i_y
   size_t i_x = size_t( arg[1] );
   size_t i_y = size_t( arg[2] );
   //
   switch(op_code)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      //
      case StppOp:
      break;
      //
      case StpvOp:
      var_sparsity.binary_union(i_y, i_y, i_v, vecad_sparsity);
      break;
      //
      case StvpOp:
      if( dependency )
         var_sparsity.binary_union(i_x, i_x, i_v, vecad_sparsity);
      break;
      //
      case StvvOp:
      if( dependency )
         var_sparsity.binary_union(i_x, i_x, i_v, vecad_sparsity);
      var_sparsity.binary_union(i_y, i_y, i_v, vecad_sparsity);
      break;
   }
   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_store_rev_hes dev}

Reverse Hessian Sparsity for Store a VecAD Element
##################################################

v, x, y
*******
see
:ref:`var_store_op@v` ,
:ref:`var_store_op@x` ,
:ref:`var_store_op@y`

Prototype
*********
{xrst_literal
   // BEGIN_STORE_REV_HES
   // END_STORE_REV_HES
}

op_code, num_vecad_ind, arg
***************************
see :ref:`var_store_op@op_code` ,
:ref:`var_store_op@num_vecad_ind` ,
:ref:`var_store_op@arg` .

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

vecad_ind
*********
is a vector with size *num_vec_ind* .
We use the notation *i_v* defined by

|tab| *i_v* = vecad_ind[ arg[0] - 1 ]

This is the index of the VecAD vector and is less than the number of
VecAD vectors in the recording.
It is also the index of the hessian
sparsity pattern for *v* in *vecad_sparsity*.

var_sparsity
************
If :ref:`var_store_op@y` is a variable,
the hessian sparsity pattern for *v* is added to the
hessian sparsity pattern for *y*.

vecad_sparsity
**************
The set with index *i_v* in *vecad_sparsity
is the hessian sparsity pattern for the vector *v*.

var_rev_jac
***********
If the scalar function we are computing the Hessian sparsity of
has a non-zero partial w.r.t. *v*,
and *y* is a variable, *var_rev_jac* [ *i_y* ] is set to true.
This is because the scalar function has non-zero partial w.r.t. *y* .

vecad_rev_jac
*************
the *i_v* component of this vector is true ,
if the scalar function has non-zero partial w.r.t *v*.

{xrst_end var_store_rev_hes}
*/
// BEGIN_STORE_REV_HES
template <class Vector_set>
inline void store_rev_hes(
   op_code_var               op_code        ,
   const addr_t*             arg            ,
   size_t                    num_vecad_ind  ,
   const pod_vector<size_t>& vecad_ind      ,
   Vector_set&               var_sparsity   ,
   const Vector_set&         vecad_sparsity ,
   bool*                     var_rev_jac    ,
   const pod_vector<bool>&   vecad_rev_jac  )
// END_STORE_REV_HES
{  //
   //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   CPPAD_ASSERT_UNKNOWN( vecad_ind.size() == num_vecad_ind );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   //
   // i_y
   size_t i_y = size_t( arg[2] );
   //
   switch(op_code)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      //
      case StpvOp:
      case StvvOp:
      var_sparsity.binary_union(i_y, i_y, i_v, vecad_sparsity);
      var_rev_jac[i_y] |= vecad_rev_jac[i_v];
      break;
      //
      case StppOp:
      case StvpOp:
      break;
   }
   //
   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_store_for_hes dev}

Forward Hessian Sparsity for Store a VecAD Element
##################################################

v, x, y
*******
see
:ref:`var_store_op@v` ,
:ref:`var_store_op@x` ,
:ref:`var_store_op@y`

Prototype
*********
{xrst_literal
   // BEGIN_STORE_FOR_HES
   // END_STORE_FOR_HES
}

op_code, num_vecad_ind, arg
***************************
see :ref:`var_store_op@op_code` ,
:ref:`var_store_op@num_vecad_ind` ,
:ref:`var_store_op@arg` .

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

vecad_ind
*********
is a vector with size *num_vec_ind* .
We use the notation *i_v* defined by

|tab| *i_v* = vecad_ind[ arg[0] - 1 ]

This is the index of the VecAD vector and is less than the number of
VecAD vectors in the recording.
It is also the index of the hessian
sparsity pattern for *v* in *vecad_sparsity*.

var_sparsity
************
If :ref:`var_store_op@y` is a variable,
the hessian sparsity pattern for *v* is added to the
hessian sparsity pattern for *y*.

vecad_sparsity
**************
The set with index *i_v* in *vecad_sparsity
is the hessian sparsity pattern for the vector *v*.

var_rev_jac
***********
If the scalar function we are computing the Hessian sparsity of
has a non-zero partial w.r.t. *v*,
and *y* is a variable, *var_rev_jac* [ *i_y* ] is set to true.
This is because the scalar function has non-zero partial w.r.t. *y* .

vecad_rev_jac
*************
the *i_v* component of this vector is true ,
if the scalar function has non-zero partial w.r.t *v*.

{xrst_end var_store_for_hes}
*/
// BEGIN_STORE_FOR_HES
template <class Vector_set>
inline void store_for_hes(
   op_code_var               op_code        ,
   const addr_t*             arg            ,
   size_t                    num_vecad_ind  ,
   size_t                    n              ,
   const pod_vector<size_t>& vecad_ind      ,
   Vector_set&               vecad_sparsity ,
   const Vector_set&         for_hes_sparse )
// END_STORE_FOR_HES
{  //
   //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   CPPAD_ASSERT_UNKNOWN( vecad_ind.size() == num_vecad_ind );
   //
   // np1
   size_t np1 = n + 1;
   CPPAD_ASSERT_UNKNOWN( for_hes_sparse.end() == np1 );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   //
   // i_y
   size_t i_y = size_t( arg[2] );
   //
   switch(op_code)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      //
      // vecad_sparsity
      // set Jacobian sparsity for vector with index i_v
      case StpvOp:
      case StvvOp:
      vecad_sparsity.binary_union(i_v, i_v, np1 + i_y, for_hes_sparse);
      break;
      //
      case StppOp:
      case StvpOp:
      break;
   }
   //
   return;
}

} } } // END namespace
# endif
