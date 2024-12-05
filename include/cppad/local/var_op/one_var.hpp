# ifndef CPPAD_LOCAL_VAR_OP_ONE_VAR_HPP
# define CPPAD_LOCAL_VAR_OP_ONE_VAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace var_op {

/*
{xrst_begin_parent var_one_var dev}

Sparsity Calculations for Operators With One Variable
#####################################################

Unary Operators
***************
| *z* = *Fun* ( *x* )

See :ref:`var_unary_op@Fun` for unary operators
for some of the possible values of *Fun* and the corresponding operators.

Binary Operators
****************
| *z* = *fun* ( *x* , *y* )
| *z* = *x*  *Op* *y*


See :ref:`var_binary_op@Fun` for binary operators,
restricted to the case where just *x* or just *y* is a variable,
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

i_v
***
is the variable index corresponding to the argument that is a variable.

{xrst_end var_one_var}
------------------------------------------------------------------------------
{xrst_begin var_one_var_for_jac dev}

Forward Jacobian Sparsity for One Variable Argument Operators
#############################################################

x, y, z
*******
see
:ref:`var_one_var@x` ,
:ref:`var_one_var@y` ,
:ref:`var_one_var@z`

Prototype
*********
{xrst_literal
   // BEGIN_ONE_VAR_FOR_JAC
   // END_ONE_VAR_FOR_JAC
}

Vector_set, i_z, i_v
********************
see
:ref:`var_one_var@Vector_set` ,
:ref:`var_one_var@i_z` ,
:ref:`var_one_var@i_v`

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

{xrst_end var_one_var_for_jac}
*/
// BEGIN_ONE_VAR_FOR_JAC
template <class Vector_set>
void one_var_for_jac(
   size_t            i_z           ,
   size_t            i_v           ,
   Vector_set&       sparsity      )
// END_ONE_VAR_FOR_JAC
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( i_v < i_z );

   sparsity.assignment(i_z, i_v, sparsity);
}
/*
------------------------------------------------------------------------------
{xrst_begin var_one_var_rev_jac dev}

Reverse Jacobian Sparsity for One Variable Argument Operators
#############################################################

x, y, z
*******
see
:ref:`var_one_var@x` ,
:ref:`var_one_var@y` ,
:ref:`var_one_var@z`

Prototype
*********
{xrst_literal
   // BEGIN_ONE_VAR_REV_JAC
   // END_ONE_VAR_REV_JAC
}

Vector_set, i_z, i_v
********************
see
:ref:`var_one_var@Vector_set` ,
:ref:`var_one_var@i_z` ,
:ref:`var_one_var@i_v`

sparsity
********
Use z(v) to denote the variable *z* as a function of the variable *v*
and define H in terms of G by::

   H( v , u , ... ) = G[ z(v) , u , ... ]

On input, *sparsity* is a sparsity pattern for the Jacobian of *G* .
Upon return, *sparsity* is a sparsity pattern for the Jacobian of *H* .

{xrst_end var_one_var_rev_jac}
*/
// BEGIN_ONE_VAR_REV_JAC
template <class Vector_set>
void one_var_rev_jac(
   size_t            i_z         ,
   size_t            i_v         ,
   Vector_set&       sparsity    )
// END_ONE_VAR_REV_JAC
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( i_v < i_z );

   sparsity.binary_union(i_v, i_v, i_z, sparsity);

   return;
}
// ---------------------------------------------------------------------------
/*!
Reverse mode Hessian sparsity pattern for linear unary operators.

The C++ source code corresponding to this operation is
\verbatim
      z = fun(x)
\endverbatim
where fun is a linear functions; e.g. abs, or
\verbatim
   z = x op q
\endverbatim
where op is a C++ binary operator and q is a parameter.

\copydetails CppAD::local::reverse_sparse_hessian_unary_op
*/
template <class Vector_set>
void one_var_rev_hes_lin(
   size_t              i_z               ,
   size_t              i_x               ,
   bool*               rev_jacobian      ,
   const Vector_set&   for_jac_sparsity  ,
   Vector_set&         rev_hes_sparsity  )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( i_x < i_z );

   // check for no effect
   if( ! rev_jacobian[i_z] )
      return;

   rev_hes_sparsity.binary_union(i_x, i_x, i_z, rev_hes_sparsity);

   rev_jacobian[i_x] = true;
   return;
}

/*!
Reverse mode Hessian sparsity pattern for non-linear unary operators.

The C++ source code corresponding to this operation is
\verbatim
      z = fun(x)
\endverbatim
where fun is a non-linear functions; e.g. sin. or
\verbatim
   z = q / x
\endverbatim
where q is a parameter.


\copydetails CppAD::local::reverse_sparse_hessian_unary_op
*/
template <class Vector_set>
void one_var_rev_hes_nl(
   size_t              i_z               ,
   size_t              i_x               ,
   bool*               rev_jacobian      ,
   const Vector_set&   for_jac_sparsity  ,
   Vector_set&         rev_hes_sparsity  )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( i_x < i_z );

   // check for no effect
   if( ! rev_jacobian[i_z] )
      return;

   rev_hes_sparsity.binary_union(i_x, i_x, i_z, rev_hes_sparsity);
   rev_hes_sparsity.binary_union(i_x, i_x, i_x, for_jac_sparsity);

   rev_jacobian[i_x] = true;
   return;
}
// ---------------------------------------------------------------------------
/*
{xrst_begin for_hes_nl_unary_op dev}
{xrst_spell
   np
   numvar
}

Forward Hessian Sparsity for Non-linear Unary Operators
#######################################################

Syntax
******
| ``local::for_hes_nl_unary_op`` (
| |tab| *np1* , *numvar* , *i_v* , *for_sparsity*
| )

Prototype
*********
{xrst_literal
   // BEGIN_for_hes_nl_unary_op
   // END_for_hes_nl_unary_op
}

C++ Source
**********
The C++ source code corresponding to this operation is

   *w* = *fun* ( *v*  )

where *fun* is a non-linear function.

np1
***
This is the number of independent variables plus one;
i.e. size of *x* plus one.

numvar
******
This is the total number of variables in the tape.

i_w
***
is the index of the variable corresponding to the result *w* .

i_v
***
is the index of the variable corresponding to the argument *v* .

for_sparsity
************
We have the conditions *np1* = *for_sparsity* . ``end`` ()
and *for_sparsity* . ``n_set`` () = *np1* + *numvar* .

Input Jacobian Sparsity
=======================
For *i* = 0, ..., *i_w* ``-1`` ,
the *np1* + *i* row of *for_sparsity* is the Jacobian sparsity
for the *i*-th variable. These values do not change.
Note that *i* =0 corresponds to a parameter and
the corresponding Jacobian sparsity is empty.

Input Hessian Sparsity
======================
For *j* =1, ..., *n* ,
the *j*-th row of *for_sparsity* is the Hessian sparsity
before including the function :math:`w(x)`.

Output Jacobian Sparsity
========================
the *i_w* row of *for_sparsity* is the Jacobian sparsity
for the variable *w* .

Output Hessian Sparsity
=======================
For *j* =1, ..., *n* ,
the *j*-th row of *for_sparsity* is the Hessian sparsity
after including the function :math:`w(x)`.

{xrst_end for_hes_nl_unary_op}
*/
// BEGIN_for_hes_nl_unary_op
template <class Vector_set>
void one_var_for_hes_nl(
   size_t              np1            ,
   size_t              numvar         ,
   size_t              i_w            ,
   size_t              i_v            ,
   Vector_set&         for_sparsity   )
// END_for_hes_nl_unary_op
{  CPPAD_ASSERT_UNKNOWN( i_v < i_w );
   CPPAD_ASSERT_UNKNOWN( i_w < numvar );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + numvar );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.number_elements(np1) == 0 );

   // set Jacobian sparsity J(i_w)
   for_sparsity.assignment(np1 + i_w, np1 + i_v, for_sparsity);

   // set of independent variables that v depends on
   typename Vector_set::const_iterator itr(for_sparsity, i_v + np1);

   // loop over independent variables with non-zero partial for v
   size_t i_x = *itr;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(i_v)
      for_sparsity.binary_union(i_x, i_x, i_v + np1, for_sparsity);
      i_x = *(++itr);
   }
   return;
}

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE
# endif
