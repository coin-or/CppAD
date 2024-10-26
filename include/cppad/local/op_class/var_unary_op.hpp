# ifndef CPPAD_LOCAL_OP_CLASS_VAR_UNARY_OP_HPP
# define CPPAD_LOCAL_OP_CLASS_VAR_UNARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin_parent var_unary_op_t dev}
{xrst_spell
   Euler
   erfc
   expm
   neg
}

The Variable Unary Operator Class
#################################

var_unary_op
************
| ``CppAD::local::op_class::var_unary_op_t`` < *Base* > *var_unary_op*

Prototype
*********
{xrst_literal ,
   BEGIN NAMESPACE , END NAMESPACE
   BEGIN UNARY_OP_T , END UNARY_OP_T
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .

Derived Operator Name
*********************
A derived class operator name corresponding to *var_unary_op*
has one primary operand and one primary result.
The last character in the name for a unary operator is
``v`` (for variable).
For example the name ``sin_v`` corresponding to the sine function
of a variable.

op2enum
*******
is the enum value corresponding to this operator; i.e., the inverse of
:ref:`var_enum2op-name` .
{xrst_literal
   // BEGIN OP2ENUM
   // END OP2ENUM
}

n_arg
*****
The number of arguments for a unary operator is usually one
(but the erf and erfc functions are exceptions):
{xrst_literal
   // BEGIN N_ARG
   // END N_ARG
}

n_res
*****
The number of results for a unary operand is usually one or two
(but the erf and erfc functions are exceptions):
{xrst_literal
   // BEGIN N_RES
   // END N_RES
}

Auxiliary Results
*****************
If a unary operator has more than one result,
the first result is called the primary result and the others are called
auxiliary results.
The auxiliary results. are used to speed up computation of the derivatives.
For example, the auxiliary result for ``sin_v``
is the value of the cosine function at the same argument.

Derivative Calculation
**********************
{xrst_toc_table
   include/cppad/local/op_class/erf.hpp
}

Operators
*********

.. csv-table::
   :header-rows: 1

   Class Name, x, z
   {xrst_comment BEGIN_SORT_THIS_LINE_PLUS_1}
   abs_v,   variable, absolute value of x
   acos_v,  variable, inverse cosine of x
   acosh_v, variable, inverse hyperbolic cosine of x
   asin_v,  variable, inverse sine of x
   asinh_v, variable, inverse hyperbolic sine of x
   atan_v,  variable, inverse tangent of x
   atanh_v, variable, inverse hyperbolic tangent of x
   cos_v,   variable, cosine of x
   cosh_v,  variable, hyperbolic cosine of x
   erf_v,   variable, error function of x
   erfc_v,  variable, complementary error function of x
   exp_v,   variable, Euler's number to the power x
   expm1_v, variable, (Euler's number to the power x) minus one
   log1p_v, variable, logarithm base Euler's number of (one plus x)
   log_v,   variable, logarithm base Euler's number of x
   neg_v,   variable, negative of x; i.e. -x
   sign_v,  variable, :ref:`sign-name` of x
   sin_v,   variable, sine of x
   sinh_v,  variable, hyperbolic sine of x
   sqrt_v,  variable, square root of x
   tan_v,   variable, tangent of x
   tanh_v,  variable, hyperbolic tangent of x
   {xrst_comment END_SORT_THIS_LINE_MINUS_1}

{xrst_end var_unary_op_t}
# ----------------------------------------------------------------------------
{xrst_begin var_unary_forward dev}
{xrst_spell
   erfc
   tpv
}

Forward Mode Variable Unary Operators
#####################################

Syntax
******
| *var_unary_op* . ``forward_0`` (
| |tab| *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )
| *var_unary_op* . ``forward`` (
| |tab| *p* , *q* , *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )
| *var_unary_op* . ``forward_dir`` (
| |tab| *q* , *r* , *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )

Prototype
*********
{xrst_literal
   // BEGIN FORWARD
   // END FORWARD
}

{xrst_template ,
   include/cppad/local/op_class/var_unary_op.xrst
}

p
*
is lowest order of the Taylor coefficient that we are computing;
*p* <= *q* .
If *p* is not present ( ``forward_0`` and ``forward_dir`` ),
the lowest order of the Taylor coefficient is order *q* .

q
*
is highest order of the Taylor coefficient that we are computing.
*q* < *cap_order* .
The default value for *q* (when it is not present) is zero.
If *q* is not present ( ``forward_0`` ),
the highest order of the Taylor coefficient is order zero .

r
*
If this argument is present ( ``forward_dir`` ),
it is number of directions for Taylor coefficients that we are computing.
Otherwise, we are only computing the coefficients for one direction.

taylor
******

forward_0
=========
The Taylor coefficient corresponding to
variable index *i* and order zero is

   *taylor* [ *i* * *cap_order* ]

forward
=======
The Taylor coefficient corresponding to
variable index *i* and order *k* is

   *taylor* [ *i* * *cap_order* + *k*  ]

forward_dir
===========
The zero order Taylor coefficient for a variable
is the same for all directions.  We use the notation

   *tpv* = ( *cap_order* ``- 1`` ) * ``r`` + 1

which is the number of Taylor coefficients per variable.
The zero order Taylor coefficient for variable index *i* is

   *taylor* [ *i* * *tpv*  + 0 ]

For *k* > 0* ,
and *ell* = 0 , .. , *r-1* ,
The Taylor coefficient for variable *i* ,
order *k* , and direction *ell* is

   *taylor* [ *i* * *tpv* + ( *k* ``- 1`` ) * *r* + *ell*  + 1 ]


Input
=====

#. If *x* is a variable,
   the Taylor coefficients for variable *x* up to order *q* .
#. The Taylor coefficients for variable *z* up to order *p* ``-1`` .
#. If this operator has auxiliary results,
   the Taylor coefficients for the variable with index *i_z* - *j*
   for *j* = 1 up to the number of auxiliary results,
   and up to order *p* ``-1`` .

Output
======
The Taylor coefficients for variable *z*
from order *p* up to order *q*
(and in the case of ``forward_dir`` for all the *r* directions).
If this operator has an auxiliary results,
the Taylor coefficients for the variable with index *i_z* - *j*
for *j* = 1 up to the number of auxiliary results,
and up to order *q* .


{xrst_end var_unary_forward}
------------------------------------------------------------------------------
{xrst_begin var_unary_reverse dev}
{xrst_spell
   erfc
}

Reverse Mode Variable Unary Operators
#####################################

Syntax
******
| *var_unary_op* . ``reverse`` (
| *d* , *i_z* , *arg* , *parameter* , *cap_order* , *taylor* , *nc_partial* , *partial*
| )

Prototype
*********
{xrst_literal
   // BEGIN REVERSE
   // END REVERSE
}

{xrst_template ,
   include/cppad/local/op_class/var_unary_op.xrst
}

y
*
If an operator has an auxiliary results,
*y* is the vector of auxiliary results.
Otherwise, *y* is just a place holder and does not affect the
value of H or G.

G
*
We use :math:`G(z, y, x, w, \ldots )` to denote a scalar valued function
of the variables up to variable index *i_z* .

H
*
We use :math:`H(x, w, \ldots )` to denote the scalar valued function
of the variables up to variable index *i_z* - *n_res*
where *n_res* is the number of results.

.. math::

   H(x, w, \ldots ) = G [ z(x), y(x), x, w, \ldots ) ]

d
*
is this highest order Taylor coefficient that we are computing
partial derivatives with respect to;
*d* < *cap_order* and *d* < *nc_partial* .


taylor
******
The Taylor coefficient corresponding to
variable index *i* and order *k* is

   *taylor* [ *i* * *cap_order* + *k*  ]

nc_partial
**********
is the number of columns in the partial array.

partial
*******
The partial derivative w.r.t. the variable with index *i* and
Taylor coefficient of order *k* is

   *partial* [ *i* * *nc_partial* + *k* ]

for *k* = 0 , ..., *d* .

Input
=====
For variable *i* = 0 ,..., *i_z* ,
*partial* contains the
partial derivatives of :math:`G(z, y, x, w, \ldots)`.

Output
======
The array *partial* contains the
partial derivatives of :math:`H(x, w, \ldots)`.
The partial derivative for the variable with index *i_z* is unspecified
(may have been used for temporary storage).
If this operator has an auxiliary variables,
The partial derivative for the variable with index *i_z* - *j*``-1``
for *j* < *n_res* is also unspecified.



{xrst_end var_unary_reverse}
------------------------------------------------------------------------------
*/
# include <cppad/local/op_class/var_base_op.hpp>

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE

// BEGIN UNARY_OP_T
template <class Base> class var_unary_op_t : public var_base_op_t<Base>
// END UNARY_OP_T
{
public:
   // BEGIN OP2ENUM
   virtual OpCode op2enum(void) const override = 0;
   // END OP2ENUM
   //
   // BEGIN N_ARG
   virtual size_t n_arg(void) const override = 0;
   // END N_ARG
   //
   // BEGIN N_RES
   virtual size_t n_res(void) const override = 0;
   // END N_RES
   //
   // BEGIN FORWARD
   // forward_0
   static void forward_0(
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   { assert(false); }
   //
   // forward
   virtual void forward(
      size_t        p           ,
      size_t        q           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const override = 0;
   //
   // forward_dir
   virtual void forward_dir(
      size_t        q           ,
      size_t        r           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const override = 0;
   // END FORWARD
   //
   // BEGIN REVERSE
   virtual void reverse(
      size_t        d           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      const Base*   taylor      ,
      size_t        nc_partial  ,
      Base*         partial     ) const override = 0;
   // END REVERSE
};

}}} // END namespace

# endif
