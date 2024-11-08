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
is the value that was stored in *v* [ *x* ] prior to this load.

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
   // BEGIN_FORWARD_LOAD_0
   // END_FORWARD_LOAD_0
}

op_code, i_z, num_vecad_ind, arg
********************************
see :ref:`var_load_op@op_code` ,
see :ref:`var_load_op@i_z` ,
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
If the value being loaded is a parameter (variable),
*vec_ad2isvar* [ *arg* [0] + *i_vec*  ] is false (true).

vec_ad2index
************
This vector has size *num_vecad_ind* .
If the value being loaded is a parameter (variable),
*vec_ad2index* [ *arg* [0] + *i_vec*  ]
is set to the parameter (variable) index
corresponding to the value being loaded.
If we are loading a parameter into the variable *z*,
only its zero order Taylor coefficient is non-zero.


{xrst_end var_load_forward_0}
*/
// BEGIN_FORWARD_LOAD_0
template <class Base>
inline void forward_load_0(
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
// END_FORWARD_LOAD_0
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
/*!
Forward mode, except for zero order, for op = LdpOp or op = LdvOp


<!-- replace preamble -->
The C++ source code corresponding to this operation is
\verbatim
   v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects.
We define the index corresponding to v[x] by
\verbatim
   i_pv = vec_ad2index[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading arg[1] below:
<!-- end preamble -->

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param play
is the tape that this operation appears in.
This is for error detection and not used when NDEBUG is defined.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param p
is the lowest order of the Taylor coefficient that we are computing.

\param q
is the highest order of the Taylor coefficient that we are computing.

\param r
is the number of directions for the Taylor coefficients that we
are computing.

\param cap_order
number of columns in the matrix containing the Taylor coefficients.

\par tpv
We use the notation
<code>tpv = (cap_order-1) * r + 1</code>
which is the number of Taylor coefficients per variable

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
arg[2]
Is the index of this vecad load instruction in the load_op2var array.

\param load_op2var
is a vector with size play->num_var_load_rec().
It contains the variable index corresponding to each load instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\par i_var
We use the notation
\verbatim
   i_var = size_t( load_op2var[ arg[2] ] )
\endverbatim

\param taylor
\n
Input
\n
If <code>i_var > 0</code>, v[x] is a variable and
for k = 1 , ... , q
<code>taylor[ i_var * tpv + (k-1)*r+1+ell ]</code>
is the k-th order coefficient for v[x] in the ell-th direction,
\n
\n
Output
\n
for k = p , ... , q,
<code>taylor[ i_z * tpv + (k-1)*r+1+ell ]</code>
is set to the k-order Taylor coefficient for z in the ell-th direction.
*/
template <class Base>
inline void forward_load_op(
   op_code_var          op_code              ,
   const local::player<Base>* play,
   size_t               p                    ,
   size_t               q                    ,
   size_t               r                    ,
   size_t               cap_order            ,
   size_t               i_z                  ,
   const addr_t*        arg                  ,
   const addr_t*        load_op2var       ,
          Base*          taylor               )
{
   CPPAD_ASSERT_NARG_NRES(op_code, 3, 1);
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < r);
   CPPAD_ASSERT_UNKNOWN( 0 < p);
   CPPAD_ASSERT_UNKNOWN( p <= q );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < play->num_var_load_rec() );
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

/*!
Reverse mode for op = LdpOp or LdvOp.

<!-- replace preamble -->
The C++ source code corresponding to this operation is
\verbatim
   v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects.
We define the index corresponding to v[x] by
\verbatim
   i_pv = vec_ad2index[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading arg[1] below:
<!-- end preamble -->

This routine is given the partial derivatives of a function
G(z , y[x] , w , u ... )
and it uses them to compute the partial derivatives of
\verbatim
   H( y[x] , w , u , ... ) = G[ z( y[x] ) , y[x] , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type
 Base.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param d
highest order the Taylor coefficient that we are computing the partial
derivative with respect to.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
 arg[2]
Is the index of this vecad load instruction in the
load_op2var array.

\param cap_order
number of columns in the matrix containing the Taylor coefficients
(not used).

\param taylor
matrix of Taylor coefficients (not used).

\param nc_partial
number of colums in the matrix containing all the partial derivatives
(not used if arg[2] is zero).

\param partial
If arg[2] is zero, y[x] is a parameter
and no values need to be modified; i.e., partial is not used.
Otherwise, y[x] is a variable and:
\n
\n
 partial [ i_z * nc_partial + k ]
for k = 0 , ... , d
is the partial derivative of G
with respect to the k-th order Taylor coefficient for z.
\n
\n
If arg[2] is not zero,
 partial [ arg[2] * nc_partial + k ]
for k = 0 , ... , d
is the partial derivative with respect to
the k-th order Taylor coefficient for x.
On input, it corresponds to the function G,
and on output it corresponds to the the function H.

\param load_op2var
is a vector with size play->num_var_load_rec().
It contains the variable index corresponding to each load instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\par Checked Assertions
\li NumArg(op) == 3
\li NumRes(op) == 1
\li d < cap_order
\li size_t(arg[2]) < i_z
*/
template <class Base>
inline void reverse_load_op(
   op_code_var    op_code     ,
   size_t         d           ,
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         cap_order   ,
   const Base*    taylor      ,
   size_t         nc_partial  ,
   Base*          partial     ,
   const addr_t*        load_op2var )
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


/*!
Forward mode sparsity operations for LdpOp and LdvOp

\param dependency
is this a dependency (or sparsity) calculation.

\copydetails CppAD::local::sparse_load_op
*/
template <class Vector_set>
inline void forward_sparse_load_op(
   bool               dependency     ,
   op_code_var        op             ,
   size_t             i_z            ,
   const addr_t*      arg            ,
   size_t             num_combined   ,
   const size_t*      combined       ,
   Vector_set&        var_sparsity   ,
   Vector_set&        vecad_sparsity )
{
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
   size_t i_v = combined[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );

   var_sparsity.assignment(i_z, i_v, vecad_sparsity);
   if( dependency & (op == LdvOp) )
      var_sparsity.binary_union(i_z, i_z, size_t(arg[1]), var_sparsity);

   return;
}


/*!
Reverse mode Jacobian sparsity operations for LdpOp and LdvOp

\param dependency
is this a dependency (or sparsity) calculation.

\copydetails CppAD::local::sparse_load_op
*/
template <class Vector_set>
inline void reverse_sparse_jacobian_load_op(
   bool               dependency     ,
   op_code_var        op             ,
   size_t             i_z            ,
   const addr_t*      arg            ,
   size_t             num_combined   ,
   const size_t*      combined       ,
   Vector_set&        var_sparsity   ,
   Vector_set&        vecad_sparsity )
{
   CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
   CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
   size_t i_v = combined[ arg[0] - 1 ];
   CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );

   vecad_sparsity.binary_union(i_v, i_v, i_z, var_sparsity);
   if( dependency & (op == LdvOp) )
      var_sparsity.binary_union( size_t(arg[1]), size_t(arg[1]), i_z, var_sparsity);

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
