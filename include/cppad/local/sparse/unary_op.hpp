# ifndef CPPAD_LOCAL_SPARSE_UNARY_OP_HPP
# define CPPAD_LOCAL_SPARSE_UNARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {
/*!
\file sparse_unary_op.hpp
Forward and reverse mode sparsity patterns for unary operators.
*/


/*!
Forward mode Jacobian sparsity pattern for all unary operators.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = fun(x)
\endverbatim
where fun is a C++ unary function, or it has the form
\verbatim
   z = x op q
\endverbatim
where op is a C++ binary unary operator and q is a parameter.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.

\param i_z
variable index corresponding to the result for this operation;
i.e., z.

\param i_x
variable index corresponding to the argument for this operator;
i.e., x.


\param sparsity
\b Input: The set with index arg[0] in sparsity
is the sparsity bit pattern for x.
This identifies which of the independent variables the variable x
depends on.
\n
\n
\b Output: The set with index i_z in sparsity
is the sparsity bit pattern for z.
This identifies which of the independent variables the variable z
depends on.
\n

\par Checked Assertions:
\li i_x < i_z
*/

template <class Vector_set>
void for_jac_unary_op(
   size_t            i_z           ,
   size_t            i_x           ,
   Vector_set&       sparsity      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( i_x < i_z );

   sparsity.assignment(i_z, i_x, sparsity);
}
/*!
Reverse mode Jacobian sparsity pattern for all unary operators.

The C++ source code corresponding to a unary operation has the form
\verbatim
   z = fun(x)
\endverbatim
where fun is a C++ unary function, or it has the form
\verbatim
   z = x op q
\endverbatim
where op is a C++ bianry operator and q is a parameter.

This routine is given the sparsity patterns
for a function G(z, y, ... )
and it uses them to compute the sparsity patterns for
\verbatim
   H( x , w , u , ... ) = G[ z(x) , x , w , u , ... ]
\endverbatim

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse::pack_setvec or sparse::list_setvec.


\param i_z
variable index corresponding to the result for this operation;
i.e. the row index in sparsity corresponding to z.

\param i_x
variable index corresponding to the argument for this operator;
i.e. the row index in sparsity corresponding to x.

\param sparsity
\b Input:
The set with index i_z in sparsity
is the sparsity bit pattern for G with respect to the variable z.
\n
\b Input:
The set with index i_x in sparsity
is the sparsity bit pattern for G with respect to the variable x.
\n
\b Output:
The set with index i_x in sparsity
is the sparsity bit pattern for H with respect to the variable x.

\par Checked Assertions:
\li i_x < i_z
*/

template <class Vector_set>
void rev_jac_unary_op(
   size_t     i_z                     ,
   size_t     i_x                     ,
   Vector_set&            sparsity    )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( i_x < i_z );

   sparsity.binary_union(i_x, i_x, i_z, sparsity);

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
void rev_hes_lin_unary_op(
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
void rev_hes_nl_unary_op(
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
void for_hes_nl_unary_op(
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
