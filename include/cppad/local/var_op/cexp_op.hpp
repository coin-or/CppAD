# ifndef CPPAD_LOCAL_VAR_OP_CEXP_OP_HPP
# define CPPAD_LOCAL_VAR_OP_CEXP_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
/*
{xrst_begin_parent var_cexp_op dev}
{xrst_spell
   Ge
}

The Variable Conditional Expression Operator
############################################

CExpOp
******
is the op code for this operator.

User Syntax
***********
| *z* = ``CondExp`` *Rel* ( *left* , *right* , *if_true* , *if_false* )

Rel
***
is Lt, Le, Eq, Ge, Gt or Ne .

left
****
is the left operand for the comparison.

right
*****
is the right operand for the comparison.

if_true
*******
is the value assigned to *z* if the comparison result is true.

if_false
********
is the value assigned to *z* if the comparison result is false.

z
*
is the variable that results from the comparison.

arg
***

arg[0]
======
is static cast to addr_t from the enum type
{xrst_literal
   include/cppad/local/declare_ad.hpp
   // BEGIN_COMPARE_OP
   // END_COMPARE_OP
}
The operator corresponding to Ne does not appear because that case
is converted to the Eq case by switching *if_true* and *if_false* .
Thus it must hold that::

   arg[0] < addr_t(CompareNe)

arg[1]
======
The first four bits of this value are used as flags; see below.

arg[2]
======
If arg[1] & 1 is zero (is one) ,
arg[2] is the parameter (variable) index corresponding to *left*

arg[3]
======
If arg[1] & 2 is zero (is one) ,
arg[3] is the parameter (variable) index corresponding to *right*

arg[4]
======
If arg[1] & 4 is zero (is one) ,
arg[4] is the parameter (variable) index corresponding to *if_true*

arg[5]
======
If arg[1] & 8 is zero (is one) ,
arg[5] is the parameter (variable) index corresponding to *if_false*

{xrst_end var_cexp_op}
------------------------------------------------------------------------------
{xrst_begin var_cexp_forward_any dev}

Any Order Forward Conditional Expression Variable Operator
##########################################################

z
*
see
:ref:`var_cexp_op@z`

Prototype
*********
{xrst_literal
   // BEGIN_CEXP_FORWARD_ANY
   // END_CEXP_FORWARD_ANY
}

arg
***
see
:ref:`var_cexp_op@arg`

Base
****
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type Base.

i_z
***
is the variable index corresponding to the variable *z* .

num_par
*******
is the total number of values in the *parameter* vector.

parameter
*********
maps parameter indices to parameter values .

{xrst_template ;
   include/cppad/local/var_op/template/forward_op.xrst
   headers: cap_order, order_low, order_up, taylor
}

{xrst_end var_cexp_forward_any}
*/
// BEGIN_CEXP_FORWARD_ANY
template <class Base>
inline void cexp_forward_any(
   size_t         order_low   ,
   size_t         order_up    ,
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   const Base*    parameter   ,
   size_t         cap_order   ,
   Base*          taylor      )
// END_CEXP_FORWARD_ANY
{  Base y_0, y_1, y_2, y_3;
   size_t p = order_low;
   size_t q = order_up;
   //
   Base zero(0);
   Base* z = taylor + i_z * cap_order;

   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

   if( arg[1] & 1 )
   {
      y_0 = taylor[ size_t(arg[2]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
      y_0 = parameter[ arg[2] ];
   }
   if( arg[1] & 2 )
   {
      y_1 = taylor[ size_t(arg[3]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
      y_1 = parameter[ arg[3] ];
   }
   if( p == 0 )
   {  if( arg[1] & 4 )
      {
         y_2 = taylor[ size_t(arg[4]) * cap_order + 0 ];
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( size_t(arg[4]) < num_par );
         y_2 = parameter[ arg[4] ];
      }
      if( arg[1] & 8 )
      {
         y_3 = taylor[ size_t(arg[5]) * cap_order + 0 ];
      }
      else
      {  CPPAD_ASSERT_UNKNOWN( size_t(arg[5]) < num_par );
         y_3 = parameter[ arg[5] ];
      }
      z[0] = CondExpOp(
         CompareOp( arg[0] ),
         y_0,
         y_1,
         y_2,
         y_3
      );
      p++;
   }
   for(size_t d = p; d <= q; d++)
   {  if( arg[1] & 4 )
      {
         y_2 = taylor[ size_t(arg[4]) * cap_order + d];
      }
      else
         y_2 = zero;
      if( arg[1] & 8 )
      {
         y_3 = taylor[ size_t(arg[5]) * cap_order + d];
      }
      else
         y_3 = zero;
      z[d] = CondExpOp(
         CompareOp( arg[0] ),
         y_0,
         y_1,
         y_2,
         y_3
      );
   }
   return;
}

/*!
Multiple directions forward mode Taylor coefficients for op = CExpOp.

<!-- replace conditional_exp_op -->
The C++ source code coresponding to this operation is
\verbatim
   z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim
where Rel is one of the following: Lt, Le, Eq, Ge, Gt.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type
 Base.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
 arg[0]
is static cast to size_t from the enum type
\verbatim
   enum CompareOp {
      CompareLt,
      CompareLe,
      CompareEq,
      CompareGe,
      CompareGt,
      CompareNe
   }
\endverbatim
for this operation.
Note that arg[0] cannot be equal to CompareNe.
\n
\n
 arg[1] & 1
\n
If this is zero, y_0 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 2
\n
If this is zero, y_1 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 4
\n
If this is zero, y_2 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 8
\n
If this is zero, y_3 is a parameter. Otherwise it is a variable.
\n
\n
 arg[2 + j ] for j = 0, 1, 2, 3
\n
is the index corresponding to y_j.

\param num_par
is the total number of values in the vector parameter.

\param parameter
For j = 0, 1, 2, 3,
if y_j is a parameter, parameter [ arg[2 + j] ] is its value.

\param cap_order
number of columns in the matrix containing the Taylor coefficients.

\par Checked Assertions
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li arg[0] < static_cast<size_t> ( CompareNe )
\li arg[1] != 0; i.e., not all of y_0, y_1, y_2, y_3 are parameters.
\li For j = 0, 1, 2, 3 if y_j is a parameter, arg[2+j] < num_par.
<!-- end conditional_exp_op -->

\param q
is order of the Taylor coefficient of z that we are computing.

\param r
is the number of Taylor coefficient directions that we are computing.

\par tpv
We use the notation
<code>tpv = (cap_order-1) * r + 1</code>
which is the number of Taylor coefficients per variable

\param taylor
\b Input:
For j = 0, 1, 2, 3, k = 1, ..., q,
if y_j is a variable then
<code>taylor [ arg[2+j] * tpv + 0 ]</code>
is the zero order Taylor coefficient corresponding to y_j and
<code>taylor [ arg[2+j] * tpv + (k-1)*r+1+ell</code> is its
k-th order Taylor coefficient in the ell-th direction.
\n
\b Input:
For j = 0, 1, 2, 3, k = 1, ..., q-1,
<code>taylor [ i_z * tpv + 0 ]</code>
is the zero order Taylor coefficient corresponding to z and
<code>taylor [ i_z * tpv + (k-1)*r+1+ell</code> is its
k-th order Taylor coefficient in the ell-th direction.
\n
\b Output: <code>taylor [ i_z * tpv + (q-1)*r+1+ell ]</code>
is the q-th order Taylor coefficient corresponding to z
in the ell-th direction.
*/
template <class Base>
inline void cexp_forward_dir(
   size_t         order_up    ,
   size_t         n_dir       ,
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   const Base*    parameter   ,
   size_t         cap_order   ,
   Base*          taylor      )
{  Base y_0, y_1, y_2, y_3;
   size_t q = order_up;
   size_t r = n_dir;
   //
   Base zero(0);
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* z = taylor + i_z * num_taylor_per_var;

   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   if( arg[1] & 1 )
   {
      y_0 = taylor[ size_t(arg[2]) * num_taylor_per_var + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
      y_0 = parameter[ arg[2] ];
   }
   if( arg[1] & 2 )
   {
      y_1 = taylor[ size_t(arg[3]) * num_taylor_per_var + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
      y_1 = parameter[ arg[3] ];
   }
   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
   {  if( arg[1] & 4 )
      {
         y_2 = taylor[ size_t(arg[4]) * num_taylor_per_var + m + ell];
      }
      else
         y_2 = zero;
      if( arg[1] & 8 )
      {
         y_3 = taylor[ size_t(arg[5]) * num_taylor_per_var + m + ell];
      }
      else
         y_3 = zero;
      z[m+ell] = CondExpOp(
         CompareOp( arg[0] ),
         y_0,
         y_1,
         y_2,
         y_3
      );
   }
   return;
}

/*!
Compute zero order forward mode Taylor coefficients for op = CExpOp.

<!-- replace conditional_exp_op -->
The C++ source code coresponding to this operation is
\verbatim
   z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim
where Rel is one of the following: Lt, Le, Eq, Ge, Gt.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type
 Base.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
 arg[0]
is static cast to size_t from the enum type
\verbatim
   enum CompareOp {
      CompareLt,
      CompareLe,
      CompareEq,
      CompareGe,
      CompareGt,
      CompareNe
   }
\endverbatim
for this operation.
Note that arg[0] cannot be equal to CompareNe.
\n
\n
 arg[1] & 1
\n
If this is zero, y_0 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 2
\n
If this is zero, y_1 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 4
\n
If this is zero, y_2 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 8
\n
If this is zero, y_3 is a parameter. Otherwise it is a variable.
\n
\n
 arg[2 + j ] for j = 0, 1, 2, 3
\n
is the index corresponding to y_j.

\param num_par
is the total number of values in the vector parameter.

\param parameter
For j = 0, 1, 2, 3,
if y_j is a parameter, parameter [ arg[2 + j] ] is its value.

\param cap_order
number of columns in the matrix containing the Taylor coefficients.

\par Checked Assertions
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li arg[0] < static_cast<size_t> ( CompareNe )
\li arg[1] != 0; i.e., not all of y_0, y_1, y_2, y_3 are parameters.
\li For j = 0, 1, 2, 3 if y_j is a parameter, arg[2+j] < num_par.
<!-- end conditional_exp_op -->

\param taylor
\b Input:
For j = 0, 1, 2, 3,
if y_j is a variable then
 taylor [ arg[2+j] * cap_order + 0 ]
is the zero order Taylor coefficient corresponding to y_j.
\n
\b Output: taylor [ i_z * cap_order + 0 ]
is the zero order Taylor coefficient corresponding to z.
*/
template <class Base>
inline void cexp_forward_0(
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   const Base*    parameter   ,
   size_t         cap_order   ,
   Base*          taylor      )
{  Base y_0, y_1, y_2, y_3;
   //
   Base* z;

   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

   if( arg[1] & 1 )
   {
      y_0 = taylor[ size_t(arg[2]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
      y_0 = parameter[ arg[2] ];
   }
   if( arg[1] & 2 )
   {
      y_1 = taylor[ size_t(arg[3]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
      y_1 = parameter[ arg[3] ];
   }
   if( arg[1] & 4 )
   {
      y_2 = taylor[ size_t(arg[4]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[4]) < num_par );
      y_2 = parameter[ arg[4] ];
   }
   if( arg[1] & 8 )
   {
      y_3 = taylor[ size_t(arg[5]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[5]) < num_par );
      y_3 = parameter[ arg[5] ];
   }
   z = taylor + i_z * cap_order;
   z[0] = CondExpOp(
      CompareOp( arg[0] ),
      y_0,
      y_1,
      y_2,
      y_3
   );
   return;
}

/*!
Compute reverse mode Taylor coefficients for op = CExpOp.

This routine is given the partial derivatives of a function
G( z , y , x , w , ... )
and it uses them to compute the partial derivatives of
\verbatim
   H( y , x , w , u , ... ) = G[ z(y) , y , x , w , u , ... ]
\endverbatim
where y above represents y_0, y_1, y_2, y_3.

<!-- replace conditional_exp_op -->
The C++ source code coresponding to this operation is
\verbatim
   z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim
where Rel is one of the following: Lt, Le, Eq, Ge, Gt.

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type
 Base.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
 arg[0]
is static cast to size_t from the enum type
\verbatim
   enum CompareOp {
      CompareLt,
      CompareLe,
      CompareEq,
      CompareGe,
      CompareGt,
      CompareNe
   }
\endverbatim
for this operation.
Note that arg[0] cannot be equal to CompareNe.
\n
\n
 arg[1] & 1
\n
If this is zero, y_0 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 2
\n
If this is zero, y_1 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 4
\n
If this is zero, y_2 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 8
\n
If this is zero, y_3 is a parameter. Otherwise it is a variable.
\n
\n
 arg[2 + j ] for j = 0, 1, 2, 3
\n
is the index corresponding to y_j.

\param num_par
is the total number of values in the vector parameter.

\param parameter
For j = 0, 1, 2, 3,
if y_j is a parameter, parameter [ arg[2 + j] ] is its value.

\param cap_order
number of columns in the matrix containing the Taylor coefficients.

\par Checked Assertions
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li arg[0] < static_cast<size_t> ( CompareNe )
\li arg[1] != 0; i.e., not all of y_0, y_1, y_2, y_3 are parameters.
\li For j = 0, 1, 2, 3 if y_j is a parameter, arg[2+j] < num_par.
<!-- end conditional_exp_op -->

\param d
is the order of the Taylor coefficient of z that we are  computing.

\param taylor
\b Input:
For j = 0, 1, 2, 3 and k = 0 , ... , d,
if y_j is a variable then
 taylor [ arg[2+j] * cap_order + k ]
is the k-th order Taylor coefficient corresponding to y_j.
\n
 taylor [ i_z * cap_order + k ]
for k = 0 , ... , d
is the k-th order Taylor coefficient corresponding to z.

\param n_order
number of columns in the matrix containing the Taylor coefficients.

\param partial
\b Input:
For j = 0, 1, 2, 3 and k = 0 , ... , d,
if y_j is a variable then
 partial [ arg[2+j] * n_order + k ]
is the partial derivative of G( z , y , x , w , u , ... )
with respect to the k-th order Taylor coefficient corresponding to y_j.
\n
\b Input: partial [ i_z * cap_order + k ]
for k = 0 , ... , d
is the partial derivative of G( z , y , x , w , u , ... )
with respect to the k-th order Taylor coefficient corresponding to z.
\n
\b Output:
For j = 0, 1, 2, 3 and k = 0 , ... , d,
if y_j is a variable then
 partial [ arg[2+j] * n_order + k ]
is the partial derivative of H( y , x , w , u , ... )
with respect to the k-th order Taylor coefficient corresponding to y_j.

*/
template <class Base>
inline void cexp_reverse(
   size_t         i_z         ,
   const addr_t*  arg         ,
   size_t         num_par     ,
   const Base*    parameter   ,
   size_t         cap_order   ,
   const Base*    taylor      ,
   size_t         n_order     ,
   Base*          partial     )
{  // d
   //
   size_t d = n_order - 1;
   //
   Base y_0, y_1;
   Base zero(0);
   Base* pz;
   Base* py_2;
   Base* py_3;

   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );

   pz = partial + i_z * n_order + 0;
   if( arg[1] & 1 )
   {
      y_0 = taylor[ size_t(arg[2]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
      y_0 = parameter[ arg[2] ];
   }
   if( arg[1] & 2 )
   {
      y_1 = taylor[ size_t(arg[3]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
      y_1 = parameter[ arg[3] ];
   }
   if( arg[1] & 4 )
   {
      py_2 = partial + size_t(arg[4]) * n_order;
      size_t j = d + 1;
      while(j--)
      {  py_2[j] += CondExpOp(
            CompareOp( arg[0] ),
            y_0,
            y_1,
            pz[j],
            zero
         );
      }
   }
   if( arg[1] & 8 )
   {
      py_3 = partial + size_t(arg[5]) * n_order;
      size_t j = d + 1;
      while(j--)
      {  py_3[j] += CondExpOp(
            CompareOp( arg[0] ),
            y_0,
            y_1,
            zero,
            pz[j]
         );
      }
   }
   return;
}

/*!
Compute forward Jacobian sparsity patterns for op = CExpOp.

<!-- replace sparse_conditional_exp_op -->
The C++ source code coresponding to this operation is
\verbatim
   z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim
where Rel is one of the following: Lt, Le, Eq, Ge, Gt.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
 arg[0]
is static cast to size_t from the enum type
\verbatim
   enum CompareOp {
      CompareLt,
      CompareLe,
      CompareEq,
      CompareGe,
      CompareGt,
      CompareNe
   }
\endverbatim
for this operation.
Note that arg[0] cannot be equal to CompareNe.
\n
\n
 arg[1] & 1
\n
If this is zero, y_0 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 2
\n
If this is zero, y_1 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 4
\n
If this is zero, y_2 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 8
\n
If this is zero, y_3 is a parameter. Otherwise it is a variable.
\n
\n
 arg[2 + j ] for j = 0, 1, 2, 3
\n
is the index corresponding to y_j.

\param num_par
is the total number of values in the vector parameter.

\par Checked Assertions
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li arg[0] < static_cast<size_t> ( CompareNe )
\li arg[1] != 0; i.e., not all of y_0, y_1, y_2, y_3 are parameters.
\li For j = 0, 1, 2, 3 if y_j is a parameter, arg[2+j] < num_par.
<!-- end sparse_conditional_exp_op -->

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
   CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param sparsity
\b Input:
if y_2 is a variable, the set with index t is
the sparsity pattern corresponding to y_2.
This identifies which of the independent variables the variable y_2
depends on.
\n
\b Input:
if y_3 is a variable, the set with index t is
the sparsity pattern corresponding to y_3.
This identifies which of the independent variables the variable y_3
depends on.
\n
\b Output:
The set with index T is
the sparsity pattern corresponding to z.
This identifies which of the independent variables the variable z
depends on.
*/
template <class Vector_set>
inline void cexp_for_jac(
   bool               dependency    ,
   size_t             i_z           ,
   const addr_t*      arg           ,
   size_t             num_par       ,
   Vector_set&        sparsity      )
{  //
   //
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
# ifndef NDEBUG
   addr_t k = 1;
   for( size_t j = 0; j < 4; j++)
   {  if( ! ( arg[1] & k ) )
         CPPAD_ASSERT_UNKNOWN( size_t(arg[2+j]) < num_par );
      k *= 2;
   }
# endif
   sparsity.clear(i_z);
   if( dependency )
   {  if( arg[1] & 1 )
         sparsity.binary_union(i_z, i_z, size_t(arg[2]), sparsity);
      if( arg[1] & 2 )
         sparsity.binary_union(i_z, i_z, size_t(arg[3]), sparsity);
   }
   if( arg[1] & 4 )
      sparsity.binary_union(i_z, i_z, size_t(arg[4]), sparsity);
   if( arg[1] & 8 )
      sparsity.binary_union(i_z, i_z, size_t(arg[5]), sparsity);
   return;
}

/*!
Compute reverse Jacobian sparsity patterns for op = CExpOp.

This routine is given the sparsity patterns
for a function G(z, y, x, ... )
and it uses them to compute the sparsity patterns for
\verbatim
   H( y, x, w , u , ... ) = G[ z(x,y) , y , x , w , u , ... ]
\endverbatim
where y represents the combination of y_0, y_1, y_2, and y_3.

<!-- replace sparse_conditional_exp_op -->
The C++ source code coresponding to this operation is
\verbatim
   z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim
where Rel is one of the following: Lt, Le, Eq, Ge, Gt.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
 arg[0]
is static cast to size_t from the enum type
\verbatim
   enum CompareOp {
      CompareLt,
      CompareLe,
      CompareEq,
      CompareGe,
      CompareGt,
      CompareNe
   }
\endverbatim
for this operation.
Note that arg[0] cannot be equal to CompareNe.
\n
\n
 arg[1] & 1
\n
If this is zero, y_0 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 2
\n
If this is zero, y_1 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 4
\n
If this is zero, y_2 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 8
\n
If this is zero, y_3 is a parameter. Otherwise it is a variable.
\n
\n
 arg[2 + j ] for j = 0, 1, 2, 3
\n
is the index corresponding to y_j.

\param num_par
is the total number of values in the vector parameter.

\par Checked Assertions
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li arg[0] < static_cast<size_t> ( CompareNe )
\li arg[1] != 0; i.e., not all of y_0, y_1, y_2, y_3 are parameters.
\li For j = 0, 1, 2, 3 if y_j is a parameter, arg[2+j] < num_par.
<!-- end sparse_conditional_exp_op -->

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
   CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.


\param sparsity
if y_2 is a variable, the set with index t is
the sparsity pattern corresponding to y_2.
This identifies which of the dependent variables depend on the variable y_2.
On input, this pattern corresponds to the function G.
On ouput, it corresponds to the function H.
\n
\n
if y_3 is a variable, the set with index t is
the sparsity pattern corresponding to y_3.
This identifies which of the dependent variables depeond on the variable y_3.
On input, this pattern corresponds to the function G.
On ouput, it corresponds to the function H.
\n
\b Output:
The set with index T is
the sparsity pattern corresponding to z.
This identifies which of the dependent variables depend on the variable z.
On input and output, this pattern corresponds to the function G.
*/
template <class Vector_set>
inline void cexp_rev_jac(
   bool                dependency    ,
   size_t              i_z           ,
   const addr_t*       arg           ,
   size_t              num_par       ,
   Vector_set&         sparsity      )
{  //
   //
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
# ifndef NDEBUG
   addr_t k = 1;
   for( size_t j = 0; j < 4; j++)
   {  if( ! ( arg[1] & k ) )
         CPPAD_ASSERT_UNKNOWN( size_t(arg[2+j]) < num_par );
      k *= 2;
   }
# endif
   if( dependency )
   {  if( arg[1] & 1 )
         sparsity.binary_union( size_t(arg[2]), size_t(arg[2]), i_z, sparsity);
      if( arg[1] & 2 )
         sparsity.binary_union( size_t(arg[3]), size_t(arg[3]), i_z, sparsity);
   }
   // --------------------------------------------------------------------
   if( arg[1] & 4 )
      sparsity.binary_union( size_t(arg[4]), size_t(arg[4]), i_z, sparsity);
   if( arg[1] & 8 )
      sparsity.binary_union( size_t(arg[5]), size_t(arg[5]), i_z, sparsity);
   return;
}

/*!
Compute reverse Hessian sparsity patterns for op = CExpOp.

This routine is given the sparsity patterns
for a function G(z, y, x, ... )
and it uses them to compute the sparsity patterns for
\verbatim
   H( y, x, w , u , ... ) = G[ z(x,y) , y , x , w , u , ... ]
\endverbatim
where y represents the combination of y_0, y_1, y_2, and y_3.

<!-- replace sparse_conditional_exp_op -->
The C++ source code coresponding to this operation is
\verbatim
   z = CondExpRel(y_0, y_1, y_2, y_3)
\endverbatim
where Rel is one of the following: Lt, Le, Eq, Ge, Gt.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
 arg[0]
is static cast to size_t from the enum type
\verbatim
   enum CompareOp {
      CompareLt,
      CompareLe,
      CompareEq,
      CompareGe,
      CompareGt,
      CompareNe
   }
\endverbatim
for this operation.
Note that arg[0] cannot be equal to CompareNe.
\n
\n
 arg[1] & 1
\n
If this is zero, y_0 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 2
\n
If this is zero, y_1 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 4
\n
If this is zero, y_2 is a parameter. Otherwise it is a variable.
\n
\n
 arg[1] & 8
\n
If this is zero, y_3 is a parameter. Otherwise it is a variable.
\n
\n
 arg[2 + j ] for j = 0, 1, 2, 3
\n
is the index corresponding to y_j.

\param num_par
is the total number of values in the vector parameter.

\par Checked Assertions
\li NumArg(CExpOp) == 6
\li NumRes(CExpOp) == 1
\li arg[0] < static_cast<size_t> ( CompareNe )
\li arg[1] != 0; i.e., not all of y_0, y_1, y_2, y_3 are parameters.
\li For j = 0, 1, 2, 3 if y_j is a parameter, arg[2+j] < num_par.
<!-- end sparse_conditional_exp_op -->


\param jac_reverse
 jac_reverse[i_z]
is false (true) if the Jacobian of G with respect to z is always zero
(may be non-zero).
\n
\n
 jac_reverse[ arg[4] ]
If y_2 is a variable,
 jac_reverse[ arg[4] ]
is false (true) if the Jacobian with respect to y_2 is always zero
(may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.
\n
\n
 jac_reverse[ arg[5] ]
If y_3 is a variable,
 jac_reverse[ arg[5] ]
is false (true) if the Jacobian with respect to y_3 is always zero
(may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.

\param hes_sparsity
The set with index i_z in hes_sparsity
is the Hessian sparsity pattern for the function G
where one of the partials is with respect to z.
\n
\n
If y_2 is a variable,
the set with index arg[4] in hes_sparsity
is the Hessian sparsity pattern
where one of the partials is with respect to y_2.
On input, this pattern corresponds to the function G.
On output, this pattern corresponds to the function H.
\n
\n
If y_3 is a variable,
the set with index arg[5] in hes_sparsity
is the Hessian sparsity pattern
where one of the partials is with respect to y_3.
On input, this pattern corresponds to the function G.
On output, this pattern corresponds to the function H.
*/
template <class Vector_set>
inline void cexp_rev_hes(
   size_t               i_z           ,
   const addr_t*        arg           ,
   size_t               num_par       ,
   bool*                jac_reverse   ,
   Vector_set&          hes_sparsity  )
{  //
   //

   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < static_cast<size_t> (CompareNe) );
   CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
   CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
# ifndef NDEBUG
   addr_t k = 1;
   for( size_t j = 0; j < 4; j++)
   {  if( ! ( arg[1] & k ) )
         CPPAD_ASSERT_UNKNOWN( size_t(arg[2+j]) < num_par );
      k *= 2;
   }
# endif
   if( arg[1] & 4 )
   {
      hes_sparsity.binary_union( size_t(arg[4]), size_t(arg[4]), i_z, hes_sparsity);
      jac_reverse[ arg[4] ] |= jac_reverse[i_z];
   }
   if( arg[1] & 8 )
   {
      hes_sparsity.binary_union( size_t(arg[5]), size_t(arg[5]), i_z, hes_sparsity);
      jac_reverse[ arg[5] ] |= jac_reverse[i_z];
   }
   return;
}

} } } // END namespace
# endif
