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

linear[2]
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
         // update Hessian term with one parital w.r.t u
         if( ! linear_xy )
         {  // other independent variables that z depends on
            for_sparsity.binary_union(i_u, i_u, i_z + np1, for_sparsity);
         }
         else
         {  // other independent variables that x depends on
            for_sparsity.binary_union(i_u, i_u, i_x + np1, for_sparsity);
         }
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
         // update Hessian term with one parital w.r.t u
         if( ! linear_xy )
         {  // other independent variables that z depends on
            for_sparsity.binary_union(i_u, i_u, i_z + np1, for_sparsity);
         }
         else
         {  // other independent variables that y depends on
            for_sparsity.binary_union(i_u, i_u, i_y + np1, for_sparsity);
         }
         //
         // i_u
         i_u = *(++itr_y);
      }
   }
   if( (! linear_xy) && linear_x )
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
   }
   if( (! linear_xy) && linear_y )
   {  //
      // itr_y, i_u
      typename Vector_set::const_iterator itr_y(for_sparsity, i_y + np1);
      size_t i_u = *itr_y;
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
/*
{xrst_begin var_two_var_rev_hes dev}

Reverse Jacobian Sparsity for Two Variable Argument Operators
#############################################################

x, y, z
*******
see
:ref:`var_two_var@x` ,
:ref:`var_two_var@y` ,
:ref:`var_two_var@z`

G and H
*******
Use z(x, y) to denote the variable *z* as a function of the variables
*x* , *y* , and define H in terms of G by::

   H( x, y, ... ) = G[ z(x, y) , x, y, ... ]

Prototype
*********
{xrst_literal
   // BEGIN_TWO_VAR_REV_HES
   // END_TWO_VAR_REV_HES
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

linear[2]
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

for_jac_sparsity
****************
The set with index *j* is the forward Jacobian sparsity
pattern for the variable with index *j*.

rev_jac_include
***************
If the *j* element of this vector is true,
the variable with index *j* is included in the Hessian,
or affects the value of a variable that is included in the Hessian.

Input
=====
::

   for j = num_var, ... , i_z + 1
      rev_jac_include[j] is an input

Output
======
rev_jac_include[i_z] is an output

rev_hes_sparsity
****************
On input (output), this is the sparsity pattern for *G* ( *H* ).
For each variable index *j* ,
*rev_hes_sparsity* [ *j* ] is the set of indices
that may have non-zero cross partials with variable index *j* .

Example
=======
If the indices in the sets correspond to the independent variables,
then *rev_hes_sparsity* ``.end()`` is the number of independent variables.
For *i* a variable index between 1 and the number of independent variables,
*i* - 1 is the corresponding independent variable index.
(The index *i* = 0 corresponds to the phantom variable at the beginning
of the tape. )
{xrst_end var_two_var_rev_hes}
*/
// BEGIN_TWO_VAR_REV_HES
template <class Vector_set>
void two_var_rev_hes(
   size_t              i_z               ,
   const addr_t*       arg               ,
   bool*               linear            ,
   bool*               rev_jacobian      ,
   const Vector_set&   for_jac_sparsity  ,
   Vector_set&         rev_hes_sparsity  )
// END_TWO_VAR_REV_HES
{  //
   // check for nothing to do
   if( ! rev_jacobian[i_z] )
      return;
   //
   // i_x, i_y, linear_x, linear_y, linear_xy
   size_t i_x = size_t( arg[0] );
   size_t i_y = size_t( arg[1] );
   bool linear_x  = linear[0];
   bool linear_y  = linear[1];
   bool linear_xy = linear[2];
   //
   CPPAD_ASSERT_UNKNOWN( i_x < i_z );
   CPPAD_ASSERT_UNKNOWN( i_y < i_z );
   //
   // rev_hes_sparsity
   // propagate form z to x and y
   rev_hes_sparsity.binary_union(i_x, i_x, i_z, rev_hes_sparsity);
   rev_hes_sparsity.binary_union(i_y, i_y, i_z, rev_hes_sparsity);
   //
   if( ! linear_x )
   {  rev_hes_sparsity.binary_union(i_x, i_x, i_x, for_jac_sparsity);
      rev_hes_sparsity.binary_union(i_y, i_y, i_x, for_jac_sparsity);
   }
   if( ! linear_y )
   {  rev_hes_sparsity.binary_union(i_x, i_x, i_y, for_jac_sparsity);
      rev_hes_sparsity.binary_union(i_y, i_y, i_y, for_jac_sparsity);
   }
   if( ! linear_xy )
   {  rev_hes_sparsity.binary_union(i_x, i_x, i_y, for_jac_sparsity);
      rev_hes_sparsity.binary_union(i_y, i_y, i_x, for_jac_sparsity);
   }
   //
   // rev_jacobian
   rev_jacobian[i_x] = true;
   rev_jacobian[i_y] = true;
}
// ---------------------------------------------------------------------------
} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE
# endif
