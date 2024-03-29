# ifndef CPPAD_LOCAL_SPARSE_BINARY_OP_HPP
# define CPPAD_LOCAL_SPARSE_BINARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {
// END_DECLARE_NAMESPACE

/*!
\file sparse_binary_op.hpp
Forward and reverse mode sparsity patterns for binary operators.
*/


/*!
Forward mode Jacobian sparsity pattern for all binary operators.

The C++ source code corresponding to a binary operation has the form
\verbatim
   z = fun(x, y)
\endverbatim
where fun is a C++ binary function and both x and y are variables,
or it has the form
\verbatim
   z = x op y
\endverbatim
where op is a C++ binary unary operator and both x and y are variables.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
variable index corresponding to the result for this operation;
i.e., z.

\param arg
 arg[0]
variable index corresponding to the left operand for this operator;
i.e., x.
\n
\n arg[1]
variable index corresponding to the right operand for this operator;
i.e., y.

\param sparsity
\b Input:
The set with index arg[0] in sparsity
is the sparsity bit pattern for x.
This identifies which of the independent variables the variable x
depends on.
\n
\n
\b Input:
The set with index arg[1] in sparsity
is the sparsity bit pattern for y.
This identifies which of the independent variables the variable y
depends on.
\n
\n
\b Output:
The set with index i_z in sparsity
is the sparsity bit pattern for z.
This identifies which of the independent variables the variable z
depends on.

\par Checked Assertions:
\li arg[0] < i_z
\li arg[1] < i_z
*/

template <class Vector_set>
void for_jac_binary_op(
   size_t            i_z           ,
   const addr_t*     arg           ,
   Vector_set&       sparsity      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_z );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

   sparsity.binary_union(i_z, size_t(arg[0]), size_t(arg[1]), sparsity);

   return;
}

/*!
Reverse mode Jacobian sparsity pattern for all binary operators.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = fun(x, y)
\endverbatim
where fun is a C++ unary function and x and y are variables,
or it has the form
\verbatim
   z = x op y
\endverbatim
where op is a C++ bianry operator and x and y are variables.

This routine is given the sparsity patterns
for a function G(z, y, x, ... )
and it uses them to compute the sparsity patterns for
\verbatim
   H( y, x, w , u , ... ) = G[ z(x,y) , y , x , w , u , ... ]
\endverbatim

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
variable index corresponding to the result for this operation;
i.e., z.

\param arg
 arg[0]
variable index corresponding to the left operand for this operator;
i.e., x.

\n
\n arg[1]
variable index corresponding to the right operand for this operator;
i.e., y.

\param sparsity
The set with index i_z in sparsity
is the sparsity pattern for z corresponding ot the function G.
\n
\n
The set with index arg[0] in sparsity
is the sparsity pattern for x.
On input, it corresponds to the function G,
and on output it corresponds to H.
\n
\n
The set with index arg[1] in sparsity
is the sparsity pattern for y.
On input, it corresponds to the function G,
and on output it corresponds to H.
\n
\n

\par Checked Assertions:
\li arg[0] < i_z
\li arg[1] < i_z
*/
template <class Vector_set>
void rev_jac_binary_op(
   size_t              i_z           ,
   const addr_t*       arg           ,
   Vector_set&         sparsity      )
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
{xrst_begin sparse_for_hes_nl_binary_op dev}
{xrst_spell
   div
   np
   numvar
}

Forward Hessian Sparsity for Nonlinear Binary Operators
#######################################################

Namespace
*********
{xrst_literal
   // BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
   // END_DECLARE_NAMESPACE
}

for_hes_mul_op
**************

Syntax
======
``for_hes_mul_op`` ( *i_v* , *np1* , *numvar* , *for_sparsity* )

Prototype
=========
{xrst_literal
   // BEGIN_for_hes_mul_op
   // END_for_hes_mul_op
}

for_hes_div_op
**************

Syntax
======
``for_hes_div_op`` ( *i_v* , *np1* , *numvar* , *for_sparsity* )

Prototype
=========
{xrst_literal
   // BEGIN_for_hes_div_op
   // END_for_hes_div_op
}

for_hes_pow_op
**************

Syntax
======
``for_hes_pow_op`` ( *i_v* , *np1* , *numvar* , *for_sparsity* )

Prototype
=========
{xrst_literal
   // BEGIN_for_hes_pow_op
   // END_for_hes_pow_op
}

C++ Source
**********
The C++ source code corresponding to this operation is

| |tab| |tab| *w* = *v0* * *v1*
| |tab| |tab| *w* = *v0* / *v1*
| |tab| |tab| *w* = ``pow`` ( *v0* , *v1* )

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

arg
***
is the index of the argument vector for the nonlinear binary operation; i.e.,
*arg* [0] , *arg* [1] are the left and right operands; i.e.,
corresponding to *v0* , *v1* .

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

{xrst_end sparse_for_hes_nl_binary_op}
*/
// BEGIN_for_hes_mul_op
template <class Vector_set>
void for_hes_mul_op(
   size_t              np1           ,
   size_t              numvar        ,
   size_t              i_w           ,
   const addr_t*       arg           ,
   Vector_set&         for_sparsity  )
// END_for_hes_mul_op
{  //
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + numvar );
   //
   size_t i_v0 = size_t(arg[0]);
   size_t i_v1 = size_t(arg[1]);
   CPPAD_ASSERT_UNKNOWN( i_v0 < i_w );
   CPPAD_ASSERT_UNKNOWN( i_v1 < i_w );
   CPPAD_ASSERT_UNKNOWN( i_w  < numvar );

   // set Jacobian sparsity J(i_w)
   for_sparsity.binary_union(np1 + i_w, np1 + i_v0, np1 + i_v1, for_sparsity);

   // --------------------------------------------------
   // set of independent variables that v0 depends on
   typename Vector_set::const_iterator itr_0(for_sparsity, i_v0 + np1);

   // loop over independent variables non-zero partial for v0
   size_t i_x = *itr_0;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(v1)
      for_sparsity.binary_union(i_x, i_x, i_v1 + np1, for_sparsity);
      i_x = *(++itr_0);
   }
   // --------------------------------------------------
   // set of independent variables that v1 depends on
   typename Vector_set::const_iterator itr_1(for_sparsity, i_v1 + np1);

   // loop over independent variables with non-zero partial for v1
   i_x = *itr_1;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(v0)
      for_sparsity.binary_union(i_x, i_x, i_v0 + np1, for_sparsity);
      i_x = *(++itr_1);
   }
   return;
}
// BEGIN_for_hes_div_op
template <class Vector_set>
void for_hes_div_op(
   size_t              np1           ,
   size_t              numvar        ,
   size_t              i_w           ,
   const addr_t*       arg           ,
   Vector_set&         for_sparsity  )
// END_for_hes_div_op
{  //
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + numvar );
   //
   size_t i_v0 = size_t(arg[0]);
   size_t i_v1 = size_t(arg[1]);
   CPPAD_ASSERT_UNKNOWN( i_v0 < i_w );
   CPPAD_ASSERT_UNKNOWN( i_v1 < i_w );
   CPPAD_ASSERT_UNKNOWN( i_w  < numvar );

   // set Jacobian sparsity J(i_w)
   for_sparsity.binary_union(np1 + i_w, np1 + i_v0, np1 + i_v1, for_sparsity);

   // --------------------------------------------------
   // set of independent variables that v0 depends on
   typename Vector_set::const_iterator itr_0(for_sparsity, i_v0 + np1);

   // loop over independent variables non-zero partial for v0
   size_t i_x = *itr_0;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(v1)
      for_sparsity.binary_union(i_x, i_x, i_v1 + np1, for_sparsity);
      i_x = *(++itr_0);
   }
   // --------------------------------------------------
   // set of independent variables that v1 depends on
   typename Vector_set::const_iterator itr_1(for_sparsity, i_v1 + np1);

   // loop over independent variables with non-zero partial for v1
   i_x = *itr_1;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(v0)
      for_sparsity.binary_union(i_x, i_x, i_v0 + np1, for_sparsity);
      // N(i_x) = N(i_x) union J(v1)
      for_sparsity.binary_union(i_x, i_x, i_v1 + np1, for_sparsity);
      i_x = *(++itr_1);
   }
   return;
}
// BEGIN_for_hes_pow_op
template <class Vector_set>
void for_hes_pow_op(
   size_t              np1           ,
   size_t              numvar        ,
   size_t              i_w           ,
   const addr_t*       arg           ,
   Vector_set&         for_sparsity  )
// END_for_hes_pow_op
{  //
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + numvar );
   //
   size_t i_v0 = size_t(arg[0]);
   size_t i_v1 = size_t(arg[1]);
   CPPAD_ASSERT_UNKNOWN( i_v0 < i_w );
   CPPAD_ASSERT_UNKNOWN( i_v1 < i_w );
   CPPAD_ASSERT_UNKNOWN( i_w  < numvar );

   // set Jacobian sparsity J(i_w)
   for_sparsity.binary_union(np1 + i_w, np1 + i_v0, np1 + i_v1, for_sparsity);

   // --------------------------------------------------
   // set of independent variables that v0 depends on
   typename Vector_set::const_iterator itr_0(for_sparsity, i_v0 + np1);

   // loop over independent variables non-zero partial for v0
   size_t i_x = *itr_0;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(v0)
      for_sparsity.binary_union(i_x, i_x, i_v0 + np1, for_sparsity);
      // N(i_x) = N(i_x) union J(v1)
      for_sparsity.binary_union(i_x, i_x, i_v1 + np1, for_sparsity);
      i_x = *(++itr_0);
   }
   // --------------------------------------------------
   // set of independent variables that v1 depends on
   typename Vector_set::const_iterator itr_1(for_sparsity, i_v1 + np1);

   // loop over independent variables with non-zero partial for v1
   i_x = *itr_1;
   while( i_x < np1 )
   {  // N(i_x) = N(i_x) union J(v0)
      for_sparsity.binary_union(i_x, i_x, i_v0 + np1, for_sparsity);
      // N(i_x) = N(i_x) union J(v1)
      for_sparsity.binary_union(i_x, i_x, i_v1 + np1, for_sparsity);
      i_x = *(++itr_1);
   }
   return;
}
// ---------------------------------------------------------------------------
} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE
# endif
