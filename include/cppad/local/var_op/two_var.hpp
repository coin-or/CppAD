# ifndef CPPAD_LOCAL_VAR_OP_TWO_VAR_HPP
# define CPPAD_LOCAL_VAR_OP_TWO_VAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace var_op {
// END_DECLARE_NAMESPACE

/*
{xrst_begin_parent var_two_var dev}

Sparsity Calculations for Operators With Two Variables
######################################################

Binary Operators
****************
| *z* = *fun* ( *x* , *y* )
| *z* = *x*  *Op* *y*

See :ref:`var_binary_op@Fun` for binary operators,
restricted to the case where both *x* and *y* are variables,
for some of the possible values of *Fun* and the corresponding operators.

x
*
is the first operand for this operator .

y
*
is the second operand for this operator

z
*
is the primary result for this operator.

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

i_z
***
is the variable index corresponding to *z* .

arg
***

arg[0]
======
is the variable index corresponding to x.

arg[1]
======
is the variable index corresponding to y.

{xrst_end var_two_var}
------------------------------------------------------------------------------
{xrst_begin var_two_var_for_jac dev}

Forward Jacobian Sparsity for Two Variable Argument Operators
#############################################################

x, y, z
*******
see
:ref:`var_two_var@x` ,
:ref:`var_two_var@y` ,
:ref:`var_two_var@z`

Prototype
*********
{xrst_literal
   // BEGIN_TWO_VAR_FOR_JAC
   // END_TWO_VAR_FOR_JAC
}

Vector_set, i_z, arg
********************
see
:ref:`var_two_var@Vector_set` ,
:ref:`var_two_var@i_z` ,
:ref:`var_two_var@arg`

sparsity
********

Input
=====
for *j* < *i_z*  and *j* not an auxiliary result,
the set with index *j* in *sparsity* is
the Jacobian sparsity for the variable with index *j* .

Output
======
The set with index *i_z* in *sparsity* is
the Jacobian sparsity for the variable *z* .

{xrst_end var_two_var_for_jac}
*/
// BEGIN_TWO_VAR_FOR_JAC
template <class Vector_set>
void two_var_for_jac(
   size_t            i_z           ,
   const addr_t*     arg           ,
   Vector_set&       sparsity      )
// END_TWO_VAR_FOR_JAC
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   sparsity.binary_union(i_z, size_t(arg[0]), size_t(arg[1]), sparsity);

   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_two_var_rev_jac dev}

Reverse Jacobian Sparsity for Two Variable Argument Operators
#############################################################

x, y, z
*******
see
:ref:`var_two_var@x` ,
:ref:`var_two_var@y` ,
:ref:`var_two_var@z`

Prototype
*********
{xrst_literal
   // BEGIN_TWO_VAR_REV_JAC
   // END_TWO_VAR_REV_JAC
}

Vector_set, i_z, arg
********************
see
:ref:`var_two_var@Vector_set` ,
:ref:`var_two_var@i_z` ,
:ref:`var_two_var@arg`

sparsity
********
Use z(x, y) to denote the variable *z* as a function of the variables
*x* , *y* , and define H in terms of G by::

   H( x, y, ... ) = G[ z(x, y) , x, y, ... ]

On input, *sparsity* is a sparsity pattern for the Jacobian of *G* .
Upon return, *sparsity* is a sparsity pattern for the Jacobian of *H* .

{xrst_end var_two_var_rev_jac}
*/
// BEGIN_TWO_VAR_REV_JAC
template <class Vector_set>
void two_var_rev_jac(
   size_t              i_z           ,
   const addr_t*       arg           ,
   Vector_set&         sparsity      )
// END_TWO_VAR_REV_JAC
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   sparsity.binary_union( size_t(arg[0]), size_t(arg[0]), i_z, sparsity);
   sparsity.binary_union( size_t(arg[1]), size_t(arg[1]), i_z, sparsity);

   return;
}
// ---------------------------------------------------------------------------
/*!
Reverse mode Hessian sparsity pattern for add and subtract operators.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = x op y
\endverbatim
where op is + or - and x, y are variables.

\copydetails CppAD::local::reverse_sparse_hessian_binary_op
*/
template <class Vector_set>
void rev_hes_addsub_op(
   size_t               i_z                ,
   const addr_t*        arg                ,
   bool*                jac_reverse        ,
   const Vector_set&    for_jac_sparsity   ,
   Vector_set&          rev_hes_sparsity   )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   // check for no effect
   if( ! jac_reverse[i_z] )
      return;

   // propagate hessian sparsity from i_z to arg[0] and arg[1]
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), i_z, rev_hes_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), i_z, rev_hes_sparsity
   );

   jac_reverse[arg[0]] = true;
   jac_reverse[arg[1]] = true;

   return;
}

/*!
Reverse mode Hessian sparsity pattern for multiplication operator.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = x * y
\endverbatim
where x and y are variables.

\copydetails CppAD::local::reverse_sparse_hessian_binary_op
*/
template <class Vector_set>
void rev_hes_mul_op(
   size_t               i_z                ,
   const addr_t*        arg                ,
   bool*                jac_reverse        ,
   const Vector_set&    for_jac_sparsity   ,
   Vector_set&          rev_hes_sparsity   )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   // check for no effect
   if( ! jac_reverse[i_z] )
      return;

   // progagate hessian sparsity from i_z to arg[0] and arg[1]
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), i_z, rev_hes_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), i_z, rev_hes_sparsity
   );

   // new hessian sparsity terms between i_z and arg[0], arg[1]
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), size_t(arg[1]), for_jac_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), size_t(arg[0]), for_jac_sparsity
   );

   jac_reverse[arg[0]] = true;
   jac_reverse[arg[1]] = true;
   return;
}

/*!
Reverse mode Hessian sparsity pattern for division operator.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = x / y
\endverbatim
where x and y are variables.

\copydetails CppAD::local::reverse_sparse_hessian_binary_op
*/
template <class Vector_set>
void rev_hes_div_op(
   size_t               i_z                ,
   const addr_t*        arg                ,
   bool*                jac_reverse        ,
   const Vector_set&    for_jac_sparsity   ,
   Vector_set&          rev_hes_sparsity   )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   // check for no effect
   if( ! jac_reverse[i_z] )
      return;

   // propagate hessian sparsity from i_z to arg[0] and arg[1]
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), i_z, rev_hes_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), i_z, rev_hes_sparsity
   );

   // new hessian sparsity terms between i_z and arg[0], arg[1]
   rev_hes_sparsity.binary_union(
         size_t(arg[0]), size_t(arg[0]), size_t(arg[1]), for_jac_sparsity
   );
   rev_hes_sparsity.binary_union(
         size_t(arg[1]), size_t(arg[1]), size_t(arg[0]), for_jac_sparsity
   );
   rev_hes_sparsity.binary_union(
         size_t(arg[1]), size_t(arg[1]), size_t(arg[1]), for_jac_sparsity
   );

   jac_reverse[arg[0]] = true;
   jac_reverse[arg[1]] = true;
   return;
}

/*!
Reverse mode Hessian sparsity pattern for power function.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = pow(x, y)
\endverbatim
where x and y are variables.

\copydetails CppAD::local::reverse_sparse_hessian_binary_op
*/
template <class Vector_set>
void rev_hes_pow_op(
   size_t               i_z                ,
   const addr_t*        arg                ,
   bool*                jac_reverse        ,
   const Vector_set&    for_jac_sparsity   ,
   Vector_set&          rev_hes_sparsity   )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   // check for no effect
   if( ! jac_reverse[i_z] )
      return;

   // propigate hessian sparsity from i_z to arg[0] and arg[1]
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), i_z, rev_hes_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), i_z, rev_hes_sparsity
   );

   // new hessian sparsity terms between i_z and arg[0], arg[1]
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), size_t(arg[0]), for_jac_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[0]), size_t(arg[0]), size_t(arg[1]), for_jac_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), size_t(arg[0]), for_jac_sparsity
   );
   rev_hes_sparsity.binary_union(
      size_t(arg[1]), size_t(arg[1]), size_t(arg[1]), for_jac_sparsity
   );

   // I cannot think of a case where this is necessary, but it including
   // it makes it like the other cases.
   jac_reverse[arg[0]] = true;
   jac_reverse[arg[1]] = true;
   return;
}
// ---------------------------------------------------------------------------
/*
{xrst_begin var_two_var_for_hes dev}
{xrst_spell
}

Forward Hessian Sparsity for Two Variable Argument Operators
############################################################

x, y, z
*******
see
:ref:`var_two_var@x` ,
:ref:`var_two_var@y` ,
:ref:`var_two_var@z`

Prototype
*********
{xrst_literal
   // BEGIN_TWO_VAR_FOR_HES
   // END_TWO_VAR_FOR_HES
}

Vector_set, i_z, arg
********************
see
:ref:`var_two_var@Vector_set` ,
:ref:`var_two_var@i_z` ,
:ref:`var_two_var@arg`


linear
******

linear[0]
=========
This value is true (false) if the :math:`z(x, y)`
must have zero second partial derivative with respect to *x*
(may have non-zero second partial).

linear[1]
=========
This value is true (false) if the :math:`z(x, y)`
must have zero second partial derivative with respect to *y*
(may have non-zero second partial).

linear[3]
=========
This value is true (false) if the :math:`z(x, y)`
must have zero cross partial derivative
(may have non-zero cross partial).

n_independent_p1
****************
is the number of independent variables (in the tape) plus one.

num_var
*******
This is the total number of variables in the tape
(counting the phantom variable at index zero).

for_sparsity
************
On input, all the linear and nonlinear interactions up to the
arguments to the atomic function have been take into account.
Upon return, the linear and nonlinear interactions in
the atomic function have been take into account.

Hessian Sparsity
================
For *j* equal 1 to *n_independent_p1* - 1,
if *i* is in set with index *j* ,
the Hessian may have a non-zero partial with respect to the
independent variables with indices ( *i* - 1, *j* - 1 ) .
Note that the index zero is not used because it corresponds to the
phantom variable on the tape.

Jacobian Sparsity
=================
If *i* is in the set with index *n_independent_p1* + *j* ,
the variable with index *j* may have a non-zero partial with resect to the
independent variable with index *i* - 1 .

{xrst_end var_two_var_for_hes}
*/
// BEGIN_TWO_VAR_FOR_HES
template <class Vector_set>
void two_var_for_hes(
   size_t        n_independent_p1    ,
   size_t        num_var             ,
   size_t        i_z                 ,
   const addr_t* arg                 ,
   bool*         linear              ,
   Vector_set&   for_sparsity        )
// END_TWO_VAR_FOR_HES
{  //
   // np1, i_x, i_y, linear_x, linear_y, linear_xy
   size_t np1 = n_independent_p1;
   size_t i_x = size_t( arg[0] );
   size_t i_y = size_t( arg[1] );
   bool linear_x  = linear[0];
   bool linear_y  = linear[1];
   bool linear_xy = linear[2];
   //
   CPPAD_ASSERT_UNKNOWN( i_x < i_z  && i_y < i_z );
   CPPAD_ASSERT_UNKNOWN( i_z < num_var );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + num_var );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.number_elements(np1) == 0 );
   //
   //
   // for_sparsity
   // Jacobian sparsity for z
   for_sparsity.binary_union(np1 + i_z, np1 + i_x, np1 + i_y, for_sparsity);
   //
   //
   if( ! linear_x )
   {  //
      // itr_x, i_u
      typename Vector_set::const_iterator itr_x(for_sparsity, i_x + np1);
      size_t i_u = *itr_x;
      while( i_u < np1 )
      {  // x depends on the independent variable u
         //
         // for_sparsity
         // update Hessian term with one parital w.r.t u other w.r.t
         // independent variables that x depends on
         for_sparsity.binary_union(i_u, i_u, i_x + np1, for_sparsity);
         //
         // i_u
         i_u = *(++itr_x);
      }
   }
   if( ! linear_y )
   {  //
      // itr_y, i_u
      typename Vector_set::const_iterator itr_y(for_sparsity, i_y + np1);
      size_t i_u = *itr_y;
      while( i_u < np1 )
      {  // y depends on the independent variable u
         //
         // for_sparsity
         // update Hessian term with one parital w.r.t u other w.r.t
         // independent variables that y depends on
         for_sparsity.binary_union(i_u, i_u, i_y + np1, for_sparsity);
         //
         // i_u
         i_u = *(++itr_y);
      }
   }
   if( ! linear_xy )
   {  //
      // itr_x, i_u
      typename Vector_set::const_iterator itr_x(for_sparsity, i_x + np1);
      size_t i_u = *itr_x;
      while( i_u < np1 )
      {  // x depends on the independent variable u
         //
         // for_sparsity
         // update Hessian term with one parital w.r.t u other w.r.t
         // independent variables that y depends on
         for_sparsity.binary_union(i_u, i_u, i_y + np1, for_sparsity);
         //
         // i_u
         i_u = *(++itr_x);
      }
      //
      // itr_y, i_u
      typename Vector_set::const_iterator itr_y(for_sparsity, i_y + np1);
      i_u = *itr_y;
      while( i_u < np1 )
      {  // y depends on the independent variable u
         //
         // for_sparsity
         // update Hessian term with one parital w.r.t u other w.r.t
         // independent variables that x depends on
         for_sparsity.binary_union(i_u, i_u, i_x + np1, for_sparsity);
         //
         // i_u
         i_u = *(++itr_y);
      }
   }
}
// ---------------------------------------------------------------------------
} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE
# endif
