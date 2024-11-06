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
   numvar
}

Zero Order Forward Store an Element of a VecAD Vector
#####################################################

Prototype
*********
{xrst_literal
   // BEGIN_FORWARD_STORE_0
   // END_FORWARD_STORE_0
}

op_code, num_vecad_ind, arg
***************************
see :ref:`var_store_op@op_code` ,
:ref:`var_store_op@num_vecad_ind` ,
:ref:`var_store_op@arg` .

numvar
******
is the number of variables in this recording.

num_par
*******
is the number of parameters in this recording.

parameter
*********
This is the vector of parameters for this recording which has size *num_par* .

cap_order
*********
number of columns in the matrix containing the Taylor coefficients.

taylor
******
Is the matrix of Taylor coefficients for all the variables.

i_vec
*****
We use *i_vec* to denote the ``size_t`` value corresponding to
:ref:`var_store_op@x` .

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
// BEGIN_FORWARD_STORE_0
template <class Base>
inline void forward_store_0(
   op_code_var          op_code        ,
   const addr_t*        arg            ,
   size_t               numvar         ,
   size_t               num_par        ,
   const Base*          parameter      ,
   size_t               cap_order      ,
   const Base*          taylor         ,
   pod_vector<bool>&    vec_ad2isvar   ,
   pod_vector<size_t>&  vec_ad2index   )
// END_FORWARD_STORE_0
{  //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( vec_ad2isvar.size() == vec_ad2index.size() )
   //
   // index
   size_t index = size_t( arg[2] );
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
      break;
      //
      case StpvOp:
      i_vec = addr_t( Integer( parameter[ arg[1] ] ) );
      isvar = true;
      break;
      //
      case StvpOp:
      i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
      isvar = false;
      break;
      //
      case StvvOp:
      i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
      isvar = true;
      break;
   }
   //
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: zero order forward index out of range"
   );
   CPPAD_ASSERT_UNKNOWN( isvar || index < num_par );
   CPPAD_ASSERT_UNKNOWN( ! isvar || index < numvar );
   //
   // vec_ad2isvar, vec_ad2index
   vec_ad2isvar[ arg[0] + i_vec ]  = isvar;
   vec_ad2index[ arg[0] + i_vec ]  = index;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_store_forward_sparse dev}

Forward Sparsity for Store a VecAD Element
##########################################

Prototype
*********
{xrst_literal
   // BEGIN_FORWARD_STORE_SPARSE
   // END_FORWARD_STORE_SPARSE
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
We use the notation *i_v* defined by

|tab| *i_v* = vecad_ind[ arg[0] - 1 ]

This is the index of the VecAD vector and is less than the number of
VecAD vectors in the recording.

var_sparsity
************
If :ref:`var_store_op@y` is a variable,
the sets with index arg[2] in *var_sparsity* is the sparsity pattern for *y* .
Otherwise, *y* is a parameter and its sparsity pattern is empty


vecad_sparsity
**************
The set with index *i_v* in *vecad_sparsity
is the sparsity pattern for the vector *v*.
The sparsity pattern for *y* is added
to the sparsity pattern for *v* .
If *dependency* is true and *x* is a variable,
the sparsity pattern for *x* is also added to the sparsity pattern for *v*.

{xrst_end var_store_forward_sparse}
*/
// BEGIN_FORWARD_STORE_SPARSE
template <class Vector_set>
inline void forward_store_sparse(
   op_code_var               op_code        ,
   size_t                    num_vecad_ind  ,
   const addr_t*             arg            ,
   bool                      dependency     ,
   const pod_vector<size_t>& vecad_ind      ,
   Vector_set&               var_sparsity   ,
   Vector_set&               vecad_sparsity )
// END_FORWARD_STORE_SPARSE
{  //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 0);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( num_vecad_ind == vecad_ind.size() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
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
      vecad_sparsity.binary_union(i_v, i_v, size_t(arg[2]), var_sparsity);
      break;
      //
      case StvpOp:
      if( dependency )
         vecad_sparsity.binary_union(i_v, i_v, size_t(arg[1]), var_sparsity);
      break;
      //
      case StvvOp:
      if( dependency )
         vecad_sparsity.binary_union(i_v, i_v, size_t(arg[1]), var_sparsity);
      vecad_sparsity.binary_union(i_v, i_v, size_t(arg[2]), var_sparsity);
      break;
   }
   return;
}

/*!
Reverse mode sparsity operations for StpvOp, StvpOp, and StvvOp

<!-- replace preamble -->
The C++ source code corresponding to this operation is
\verbatim
   v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects.
We define the index corresponding to v[x] by
\verbatim
   i_v_x = vec_ad2index[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading arg[1] below:
<!-- end preamble -->

This routine is given the sparsity patterns for
G(v[x], y , w , u ... ) and it uses them to compute the
sparsity patterns for
\verbatim
   H(y , w , u , ... ) = G[ v[x], y , w , u , ... ]
\endverbatim

\param dependency
is this a dependency (or sparsity) calculation.

\copydetails CppAD::local::sparse_store_op
*/
template <class Vector_set>
inline void reverse_sparse_jacobian_store_op(
   bool               dependency      ,
   op_code_var        op              ,
   const addr_t*      arg             ,
   size_t             num_combined    ,
   const size_t*      combined        ,
   Vector_set&        var_sparsity    ,
   Vector_set&        vecad_sparsity  )
{
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
   size_t i_v = combined[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < var_sparsity.n_set() );

   if( dependency & ( (op == StvpOp) || (op == StvvOp) ) )
      var_sparsity.binary_union( size_t(arg[1]), size_t(arg[1]), i_v, vecad_sparsity);
   if( (op == StpvOp) || (op == StvvOp) )
      var_sparsity.binary_union( size_t(arg[2]), size_t(arg[2]), i_v, vecad_sparsity);

   return;
}

/*!
Reverse mode sparsity operations for StpvOp and StvvOp

<!-- replace preamble -->
The C++ source code corresponding to this operation is
\verbatim
   v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects.
We define the index corresponding to v[x] by
\verbatim
   i_v_x = vec_ad2index[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading arg[1] below:
<!-- end preamble -->

This routine is given the sparsity patterns for
G(v[x], y , w , u ... )
and it uses them to compute the sparsity patterns for
\verbatim
   H(y , w , u , ... ) = G[ v[x], y , w , u , ... ]
\endverbatim

\copydetails CppAD::local::sparse_store_op

\param var_jacobian
 var_jacobian[ arg[2] ]
is false (true) if the Jacobian of G with respect to y is always zero
(may be non-zero).

\param vecad_jacobian
 vecad_jacobian[i_v]
is false (true) if the Jacobian with respect to x is always zero
(may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.
*/
template <class Vector_set>
inline void reverse_sparse_hessian_store_op(
   op_code_var        op           ,
   const addr_t*      arg          ,
   size_t             num_combined ,
   const size_t*      combined     ,
   Vector_set&        var_sparsity ,
   Vector_set&        vecad_sparsity ,
   bool*              var_jacobian   ,
   bool*              vecad_jacobian )
{
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
   size_t i_v = combined[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < var_sparsity.n_set() );

   var_sparsity.binary_union( size_t(arg[2]), size_t(arg[2]), i_v, vecad_sparsity);

   var_jacobian[ arg[2] ] |= vecad_jacobian[i_v];

   return;
}

} } } // END namespace
# endif
