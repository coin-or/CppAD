# ifndef CPPAD_LOCAL_OP_STORE_OP_HPP
# define CPPAD_LOCAL_OP_STORE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
{xrst_begin store_op_var dev}
{xrst_spell
   isvar
   pv
   vp
   vv
}
Changing an Element in a Variable VecAD Vector
##############################################

See Also
********
:ref:`op_code_var store<op_code_var@Store>` .

Syntax
******

| ``forward_store_`` *IV* _ ``op_0`` (
| |tab| *i_z* ,
| |tab| *arg* ,
| |tab| *num_par* ,
| |tab| *parameter* ,
| |tab| *cap_order* ,
| |tab| *taylor* ,
| |tab| *vec_ad2isvar* ,
| |tab| *vec_ad2index*
| )

where the index type *I* and the value being stored type *V*
are ``p`` (for parameter) or ``v`` (for variable).

Prototype
*********
{xrst_literal
   // BEGIN_FORWARD_STORE_PP_OP_0
   // END_FORWARD_STORE_PP_OP_0
}
The prototype for
``forward_store_pv_op_0`` ,
``forward_store_vp_op_0`` , and
``forward_store_vv_op_0`` ,
are the same except for the function name.

Notation
********

v
=
We use *v* to denote the :ref:`VecAD-name` vector for this operation.

x
=
We use *x* to denote the ``AD`` < ``Base`` >
index for this operation.

i_vec
=====
We use *i_vec* to denote the ``size_t`` value
corresponding to *x* .

n_load
======
This is the number of load instructions in this recording.

n_all
=====
This is the number of values in the single array that includes
all the vectors together with the size of each vector.

Base
****
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

i_z
***
is the AD variable index corresponding to the result of this load operation.

arg
***

arg[0]
======
is the offset of this VecAD vector relative to the beginning
of the *vec_ad2isvar* and *vec_ad2index* arrays.

arg[1]
======
If this is
``forward_load_p_op_0`` (``forward_load_v_op_0`` )
*arg* [1] is the parameter index (variable index)
corresponding to :ref:`load_op_var@Notation@i_vec` .

arg[2]
======
Is the index of this VecAD load instruction in the
*load_op2var* array.

num_par
*******
is the number of parameters in this recording.

parameter
*********
This is the vector of parameters for this recording which has size
*num_par* .

cap_order
*********
number of columns in the matrix containing the Taylor coefficients.

taylor
******
Is the matrix of Taylor coefficients for all the variables.

vec_ad2isvar
************
This vector has size *n_all* and
the input values of its elements does not matter.
If the value being stored is a parameter (variable),
*vec_ad2isvar* [ *arg* [0] + *i_vec*  ]
is set to false (true).

vec_ad2index
************
This array has size *n_all*
and the input value of its elements does not matter.
If the value being stored is a parameter (variable),
*vec_ad2index* [ *arg* [0] + *i_vec*  ]
is set to the parameter (variable) index
corresponding to the value being stored.

{xrst_end store_op_var}
*/
// BEGIN_FORWARD_STORE_PP_OP_0
template <class Base>
void forward_store_pp_op_0(
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   const Base*    parameter   ,
   size_t         cap_order   ,
   const Base*    taylor      ,
   bool*          vec_ad2isvar   ,
   size_t*        vec_ad2index   )
// END_FORWARD_STORE_PP_OP_0
{  addr_t i_vec = addr_t( Integer( parameter[ arg[1] ] ) );
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: zero order forward dynamic parameter index out of range"
   );
   CPPAD_ASSERT_UNKNOWN( NumArg(StppOp) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(StppOp) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );

   vec_ad2isvar[ arg[0] + i_vec ]  = false;
   vec_ad2index[ arg[0] + i_vec ]  = size_t(arg[2]);
}
template <class Base>
void forward_store_pv_op_0(
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   const Base*    parameter   ,
   size_t         cap_order   ,
   const Base*    taylor      ,
   bool*          vec_ad2isvar   ,
   size_t*        vec_ad2index   )
{  addr_t i_vec = addr_t( Integer( parameter[ arg[1] ] ) );
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: zero order forward dynamic parameter index out of range"
   );
   CPPAD_ASSERT_UNKNOWN( NumArg(StpvOp) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(StpvOp) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );

   vec_ad2isvar[ arg[0] + i_vec ]  = true;
   vec_ad2index[ arg[0] + i_vec ]  = size_t(arg[2]);
}
template <class Base>
void forward_store_vp_op_0(
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   size_t         cap_order   ,
   const Base*    taylor      ,
   bool*          vec_ad2isvar   ,
   size_t*        vec_ad2index   )
{
   addr_t i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: zero order forward variable index out of range"
   );

   CPPAD_ASSERT_UNKNOWN( NumArg(StvpOp) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(StvpOp) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );

   vec_ad2isvar[ arg[0] + i_vec ]  = false;
   vec_ad2index[ arg[0] + i_vec ]  = size_t(arg[2]);
}
template <class Base>
void forward_store_vv_op_0(
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   size_t         cap_order   ,
   const Base*    taylor      ,
   bool*          vec_ad2isvar   ,
   size_t*        vec_ad2index   )
{
   addr_t i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: index during zero order forward sweep is out of range"
   );

   CPPAD_ASSERT_UNKNOWN( NumArg(StvpOp) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(StvpOp) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );

   vec_ad2isvar[ arg[0] + i_vec ]  = true;
   vec_ad2index[ arg[0] + i_vec ]  = size_t(arg[2]);
}
// ---------------------------------------------------------------------------
/*
==============================================================================
<!-- define preamble -->
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
==============================================================================
*/
/*!
Shared documnetation for sparsity operations corresponding to
op = StpvOp or StvvOp (not called).

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param op
is the code corresponding to this operator;
i.e., StpvOp, StvpOp, or StvvOp.

\param arg
\n
 arg[0]
is the offset corresponding to this VecAD vector in the combined array.
\n
\n
 arg[2]
\n
The set with index arg[2] in var_sparsity
is the sparsity pattern corresponding to y.
(Note that arg[2] > 0 because y is a variable.)

\param num_combined
is the total number of elements in the VecAD address array.

\param combined
 combined [ arg[0] - 1 ]
is the index of the set in vecad_sparsity corresponding
to the sparsity pattern for the vector v.
We use the notation i_v below which is defined by
\verbatim
   i_v = combined[ arg[0] - 1 ]
\endverbatim

\param var_sparsity
The set  with index arg[2] in var_sparsity
is the sparsity pattern for y.
This is an input for forward mode operations.
For reverse mode operations:
The sparsity pattern for v is added to the spartisy pattern for y.

\param vecad_sparsity
The set with index i_v in vecad_sparsity
is the sparsity pattern for v.
This is an input for reverse mode operations.
For forward mode operations, the sparsity pattern for y is added
to the sparsity pattern for the vector v.

\par Checked Assertions
\li NumArg(op) == 3
\li NumRes(op) == 0
\li 0 <  arg[0]
\li arg[0] < num_combined
\li arg[2] < var_sparsity.n_set()
\li i_v       < vecad_sparsity.n_set()
*/
template <class Vector_set>
void sparse_store_op(
   OpCode         op             ,
   const addr_t*  arg            ,
   size_t         num_combined   ,
   const size_t*  combined       ,
   Vector_set&    var_sparsity   ,
   Vector_set&    vecad_sparsity )
{
   // This routine is only for documentation, it should not be used
   CPPAD_ASSERT_UNKNOWN( false );
}



/*!
Forward mode sparsity operations for StpvOp and StvvOp

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

\param dependency
is this a dependency (or sparsity) calculation.

\copydetails CppAD::local::sparse_store_op
*/
template <class Vector_set>
void forward_sparse_store_op(
   bool                dependency     ,
   OpCode              op             ,
   const addr_t*       arg            ,
   size_t              num_combined   ,
   const size_t*       combined       ,
   Vector_set&         var_sparsity   ,
   Vector_set&         vecad_sparsity )
{
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
   size_t i_v = combined[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < var_sparsity.n_set() );

   if( dependency & ( (op == StvvOp) || (op == StvpOp) ) )
      vecad_sparsity.binary_union(i_v, i_v, size_t(arg[1]), var_sparsity);

   if( (op == StpvOp) || (op == StvvOp ) )
      vecad_sparsity.binary_union(i_v, i_v, size_t(arg[2]), var_sparsity);

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
void reverse_sparse_jacobian_store_op(
   bool               dependency      ,
   OpCode             op              ,
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
void reverse_sparse_hessian_store_op(
   OpCode             op           ,
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

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
