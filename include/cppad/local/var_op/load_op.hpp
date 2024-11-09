# ifndef CPPAD_LOCAL_VAR_OP_LOAD_OP_HPP
# define CPPAD_LOCAL_VAR_OP_LOAD_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { namespace var_op {
/*
 ------------------------------------------------------------------------------
{xrst_begin_parent var_load_op dev}
{xrst_spell
   ldp
   ldv
}

Access an Element in a Variable VecAD Vector
############################################

User Syntax
***********
| *z* = *v* [ *x* ]


v
*
is the :ref:`VecAD-name` vector for this load operation.
If this vector is a constant before the load,
the index *x* is a variable and it is a variable after the load.

x
*
is the index for this load.

y
*
is the value that was stored in *v* [ *x* ] prior to this load;
see :ref:`var_store_op@y` .

z
*
is the new variable created by this load.
(This new variable is like a copy of *y* .)

Base
****
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by these operators done using type Base.

op_code
*******

.. csv-table::
   :widths: auto
   :header-rows: 1

   op_code, x, z
   LdpOp, parameter, variable
   LdvOp, variable, variable

i_z
***
is the variable index corresponding to *z* .

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
Is the number of this VecAD load instruction that came before this one.

{xrst_end var_load_op}
-------------------------------------------------------------------------------
{xrst_begin var_load_forward_0 dev}
{xrst_spell
   isvar
}

Zero Order Forward Load an Element of a VecAD Vector
####################################################

Prototype
*********
{xrst_literal
   // BEGIN_LOAD_FORWARD_0
   // END_LOAD_FORWARD_0
}

v, x, y, z
**********
see
:ref:`var_load_op@v` ,
:ref:`var_load_op@x` ,
:ref:`var_load_op@y` ,
:ref:`var_load_op@z`

Base, op_code, i_z, num_vecad_ind, arg
**************************************
see
:ref:`var_load_op@Base` ,
:ref:`var_load_op@op_code` ,
ref:`var_load_op@i_z` ,
:ref:`var_load_op@num_vecad_ind` ,
:ref:`var_load_op@arg` .

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
number of columns in the matrix containing the Taylor coefficients.

taylor
******
Is the matrix of Taylor coefficients for all the variables.

i_vec
*****
We use *i_vec* to denote the ``size_t`` value corresponding to
:ref:`var_load_op@x` .
If *x* is a parameter (variable) this is a parameter (variable) index.


vec_ad2isvar
************
This vector has size :ref:`var_load_op@num_vecad_ind` .
If *y* is a parameter (variable),
*vec_ad2isvar* [ *arg* [0] + *i_vec*  ] is false (true).

vec_ad2index
************
This vector has size *num_vecad_ind* .
If *y* a parameter (variable),
*vec_ad2index* [ *arg* [0] + *i_vec*  ]
is the parameter (variable) index
corresponding to the value being loaded.

{xrst_end var_load_forward_0}
*/
// BEGIN_LOAD_FORWARD_0
template <class Base>
inline void load_forward_0(
   op_code_var                op_code          ,
   size_t                     i_z              ,
   size_t                     num_vec_ind      ,
   const addr_t*              arg              ,
   size_t                     num_var          ,
   size_t                     num_par          ,
   const Base*                parameter        ,
   size_t                     cap_order        ,
   Base*                      taylor           ,
   const pod_vector<bool>&    vec_ad2isvar     ,
   const pod_vector<size_t>&  vec_ad2index     ,
   pod_vector<addr_t>&        load_op2var      )
// END_LOAD_FORWARD_0
{  CPPAD_ASSERT_NARG_NRES(op_code, 3, 1);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( vec_ad2isvar.size() == num_vec_ind );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < load_op2var.size() );
   //
   // i_vec
   // assign here to avoid compiler warning for default case
   addr_t i_vec = std::numeric_limits<addr_t>::max();
   switch(op_code)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      //
      case LdpOp:
      CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
      i_vec = addr_t( Integer( parameter[ arg[1] ] ) );
      break;
      //
      case LdvOp:
      CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_var );
      i_vec = addr_t(Integer( taylor[ size_t(arg[1]) * cap_order + 0 ] ));
      break;
   }
   //
   CPPAD_ASSERT_KNOWN(
      size_t(i_vec) < vec_ad2index[ arg[0] - 1 ] ,
      "VecAD: dynamic parmaeter index out or range during zero order forward"
   );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0] + i_vec) < num_vec_ind );
   //
   // i_y, isvar
   size_t i_y    = vec_ad2index[ arg[0] + i_vec ];
   bool   isvar  = vec_ad2isvar[ arg[0] + i_vec ];
   //
   // z
   Base* z       = taylor + i_z * cap_order;
   //
   // z, load_op2var
   if( isvar )
   {  CPPAD_ASSERT_UNKNOWN( i_y < i_z );
      load_op2var[ arg[2] ] = addr_t( i_y );
      Base* y = taylor + i_y * cap_order;
      z[0]      = y[0];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( i_y < num_par  );
      load_op2var[ arg[2] ] = 0;
      Base y    = parameter[i_y];
      z[0]      = y;
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin var_load_forward_nonzero dev}

Nonzero Order Forward Load an Element of a VecAD Vector
#######################################################

Prototype
*********
{xrst_literal
   // BEGIN_LOAD_FORWARD_NONZERO
   // END_LOAD_FORWARD_NONZERO
}

v, x, y, z
**********
see
:ref:`var_load_op@v` ,
:ref:`var_load_op@x` ,
:ref:`var_load_op@y` ,
:ref:`var_load_op@z`

Base, op_code, i_z, arg
***********************
see
:ref:`var_load_op@Base` ,
:ref:`var_load_op@op_code` ,
:ref:`var_load_op@i_z` ,
:ref:`var_load_op@arg` .

p
*
is the lowest order of the Taylor coefficient that we are computing.

q
*
is the highest order of the Taylor coefficient that we are computing.

r
*
is the number of directions for the Taylor coefficients that we
are computing.

cap_order
*********
number of columns in the matrix containing the Taylor coefficients.

y
*
see :ref:`var_load_op@y`

load_op2var
***********
maps the load operator index *arg* [2] to the
index corresponding to *y* for this load operation.
If the case where the index is zero,
*y* is a parameter; i.e., *y* is not a variable.

taylor
******

num_taylor_per_var
******************
We use the notation num_taylor_per_var = (cap_order-1) * r + 1 .

Input
=====
#. For k = 0, ..., q,
   taylor[ i_z * num_taylor_per_var + (k-1)*r+1+ell
   is the k-th order coefficient for *z* in the ell-th direction,
#. For k = 0, ..., q,
   taylor[ i_y * num_taylor_per_var + (k-1)*r+1+ell
   is the k-th order coefficient for *y* in the ell-th direction,

Output
======
for k = p, ..., q,
taylor[ i_z * num_taylor_per_var + (k-1)*r+1+ell ]
is set to the k-th order coefficient for *z* in the ell-th direction.

{xrst_end var_load_forward_nonzero}
*/
// BEGIN_LOAD_FORWARD_NONZERO
template <class Base>
inline void load_forward_nonzero(
   op_code_var                   op_code     ,
   size_t                        i_z         ,
   const addr_t*                 arg         ,
   size_t                        p           ,
   size_t                        q           ,
   size_t                        r           ,
   size_t                        cap_order   ,
   const pod_vector<addr_t>&     load_op2var ,
   Base*                         taylor      )
// END_LOAD_FORWARD_NONZERO
{
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 1);
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < r);
   CPPAD_ASSERT_UNKNOWN( 0 < p);
   CPPAD_ASSERT_UNKNOWN( p <= q );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < load_op2var.size() );
   //
   // i_y
   size_t i_y = size_t( load_op2var[ arg[2] ] );
   CPPAD_ASSERT_UNKNOWN( i_y < i_z );
   //
   // num_taylor_per_var
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   //
   // z
   Base* z  = taylor + i_z * num_taylor_per_var;
   //
   // z
   if( i_y > 0 )
   {  Base* y = taylor + i_y * num_taylor_per_var;
      for(size_t ell = 0; ell < r; ell++)
      {  for(size_t k = p; k <= q; k++)
         {  size_t m = (k-1) * r + 1 + ell;
            z[m]     = y[m];
         }
      }
   }
   else
   {  for(size_t ell = 0; ell < r; ell++)
      {  for(size_t k = p; k <= q; k++)
         {  size_t m = (k-1) * r + 1 + ell;
            z[m]     = Base(0.0);
         }
      }
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin var_load_reverse_op dev}

Reverse Mode Load an Element of a VecAD Vector
##############################################

Prototype
*********
{xrst_literal
   // BEGIN_LOAD_REVERSE_OP
   // END_LOAD_REVERSE_OP
}

v, x, y, z
**********
see
:ref:`var_load_op@v` ,
:ref:`var_load_op@x` ,
:ref:`var_load_op@y` ,
:ref:`var_load_op@z`

Base, op_code, i_z, arg
***********************
see
:ref:`var_load_op@Base` ,
:ref:`var_load_op@op_code` ,
:ref:`var_load_op@i_z` ,
:ref:`var_load_op@arg` .

y
*
see :ref:`var_load_op@y`

d
*
highest order the Taylor coefficient that we are computing the partial
derivative with respect to.

cap_order
*********
number of columns in the matrix containing the Taylor coefficients.

nc_partial
**********
number of columns in the matrix containing all the partial derivatives.

load_op2var
***********
This vector maps the load instruction index *arg* [2] to the corresponding
*y* variable index.
If this index is zero, *y* is a parameter (not a variable).

partial
*******
We use :math:`G(z, y, \ldots )` to denote a scalar valued function of the
variables up to variable index *i_z*  and define

.. math::

   H(y, \ldots ) = G [ z(y), y, \ldots )

On input, *partial* contains the partial derivatives of *G*
with respect to the Taylor coefficient of the corresponding variables.
On output, it contains the partial derivatives of *H*
with respect to the Taylor coefficient of the corresponding variables.

If *y* is a parameter,
nothing is modified by this call to ``reverse_load_op`` .
Otherwise, let *i_y* be the variable index corresponding to *y*; i.e.

| |tab| *i_y* = *load_op2var* [ *arg* [2] ]

For k = 0 , ... , d the k-th order Taylor coefficient for *z*
is added to the k-th order Taylor coefficient for *y*; i.e.,

|tab|
*partial* [ *i_y* * *nc_partial* + *k* ] +=
*partial* [ *i_z* * *nc_partial* + *k* ]

{xrst_end var_load_reverse_op}
*/
// BEGIN_LOAD_REVERSE_OP
template <class Base>
inline void load_reverse_op(
   op_code_var               op_code     ,
   size_t                    i_z         ,
   const addr_t*             arg         ,
   size_t                    d           ,
   const pod_vector<addr_t>& load_op2var ,
   size_t                    cap_order   ,
   size_t                    nc_partial  ,
   Base*                     partial     )
// END_LOAD_REVERSE_OP
{  //
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 1);
   CPPAD_ASSERT_UNKNOWN( d < cap_order );
   // i_y
   size_t i_y = size_t( load_op2var[ arg[2] ] );
   CPPAD_ASSERT_UNKNOWN( i_y < i_z );
   //
   // py
   if( i_y > 0 )
   {
      Base* pz   = partial + i_z * nc_partial;
      Base* py   = partial + i_y * nc_partial;
      size_t j = d + 1;
      while(j--)
         py[j]   += pz[j];
   }
}
/*
------------------------------------------------------------------------------
{xrst_begin var_load_forward_jac dev}

Forward Jacobian Sparsity for Store a VecAD Element
###################################################

Prototype
*********
{xrst_literal
   // BEGIN_LOAD_FORWARD_JAC
   // END_LOAD_FORWARD_JAC
}

v, x, y, z
**********
see
:ref:`var_load_op@v` ,
:ref:`var_load_op@x` ,
:ref:`var_load_op@y` ,
:ref:`var_load_op@z`

op_code, num_vecad_ind, arg
***************************
see :ref:`var_load_op@op_code` ,
:ref:`var_load_op@num_vecad_ind` ,
:ref:`var_load_op@arg` .

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
The sparsity pattern for *z*
is set equal to the sparsity pattern for *v.
If *dependency* is true, and *x* is a variable,
the sparsity pattern for *x* is added to the sparsity pattern for *z*.

vecad_sparsity
**************
The set with index *i_v* in *vecad_sparsity
is the sparsity pattern for the vector *v*.

{xrst_end var_load_forward_jac}
*/
// BEGIN_LOAD_FORWARD_JAC
template <class Vector_set>
inline void load_forward_jac(
   op_code_var               op_code        ,
   size_t                    num_vecad_ind  ,
   size_t                    i_z            ,
   const addr_t*             arg            ,
   bool                      dependency     ,
   const pod_vector<size_t>& vecad_ind      ,
   Vector_set&               var_sparsity   ,
   const Vector_set&         vecad_sparsity )
// END_LOAD_FORWARD_JAC
{
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 1);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( num_vecad_ind == vecad_ind.size() );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   //
   // var_sparsity[i_z]
   var_sparsity.assignment(i_z, i_v, vecad_sparsity);
   //
   // var_sparsity[i_z]
   if( dependency & (op_code == LdvOp) )
   {  size_t i_x = size_t( arg[1] );
      var_sparsity.binary_union(i_z, i_z, i_x, var_sparsity);
   }
   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_load_reverse_jac dev}

Reverse Jacobian Sparsity for Load a VecAD Element
##################################################

Prototype
*********
{xrst_literal
   // BEGIN_LOAD_REVERSE_JAC
   // END_LOAD_REVERSE_JAC
}

v, x, y, z
**********
see
:ref:`var_load_op@v` ,
:ref:`var_load_op@x` ,
:ref:`var_load_op@y` ,
:ref:`var_load_op@z`

op_code, num_vecad_ind, arg
***************************
see :ref:`var_load_op@op_code` ,
:ref:`var_load_op@num_vecad_ind` ,
:ref:`var_load_op@arg` .

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
The set with index *i_z* in *vecad_sparsity
is the sparsity pattern for the variable *z*.
If *dependency* is true and *x* is a variable,
the sparsity pattern for *z* is added to the sparsity pattern *x* .

vecad_sparsity
**************
The sparsity pattern for *z* is added to the sparsity pattern
with index *i_v* in *vecad_sparsity ( the sparsity pattern for *v* ).


{xrst_end var_load_reverse_jac}
*/
// BEGIN_LOAD_REVERSE_JAC
template <class Vector_set>
inline void load_reverse_jac(
   op_code_var               op_code        ,
   size_t                    num_vecad_ind  ,
   size_t                    i_z            ,
   const addr_t*             arg            ,
   bool                      dependency     ,
   const pod_vector<size_t>& vecad_ind      ,
   Vector_set&               var_sparsity   ,
   Vector_set&               vecad_sparsity )
// END_LOAD_REVERSE_JAC
{
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 1);
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_vecad_ind );
   //
   // i_v
   size_t i_v = vecad_ind[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );
   //
   // vecad_sparsity[i_v]
   vecad_sparsity.binary_union(i_v, i_v, i_z, var_sparsity);
   if( dependency & (op_code == LdvOp) )
   {  size_t i_x = size_t( arg[1] );
      var_sparsity.binary_union(i_x, i_x, i_z, var_sparsity);
   }
   return;
}


/*!
Reverse mode Hessian sparsity operations for LdpOp and LdvOp

\copydetails CppAD::local::sparse_load_op

\param var_jacobian
 var_jacobian[i_z]
is false (true) if the Jacobian of G with respect to z is always zero
(many be non-zero).

\param vecad_jacobian
 vecad_jacobian[i_v]
is false (true) if the Jacobian with respect to x is always zero
(may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.

*/
template <class Vector_set>
inline void reverse_sparse_hessian_load_op(
   op_code_var        op             ,
   size_t             i_z            ,
   const addr_t*      arg            ,
   size_t             num_combined   ,
   const size_t*      combined       ,
   Vector_set&        var_sparsity   ,
   Vector_set&        vecad_sparsity ,
   bool*              var_jacobian   ,
   bool*              vecad_jacobian )
{
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
   size_t i_v = combined[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );

   vecad_sparsity.binary_union(i_v, i_v, i_z, var_sparsity);

   vecad_jacobian[i_v] |= var_jacobian[i_z];

   return;
}


} } } // END namespace
# endif
