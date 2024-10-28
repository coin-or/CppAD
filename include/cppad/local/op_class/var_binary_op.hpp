# ifndef CPPAD_LOCAL_OP_CLASS_VAR_BINARY_OP_HPP
# define CPPAD_LOCAL_OP_CLASS_VAR_BINARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin_parent var_binary_op_t dev}
{xrst_spell
   div
   pv
   vp
   vv
   zmul
}

The Variable Binary Operator Class
##################################

Syntax
******
| ``CppAD::local::op_class::var_binary_op_t`` < *Base* >  *var_binary_op*

Prototype
*********
{xrst_literal ,
   BEGIN NAMESPACE , END NAMESPACE
   BEGIN BINARY_OP_T , END BINARY_OP_T
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .

Derived Operator Name
*********************
A derived class operator name corresponding to *var_binary_op*
has two operands and one primary result.
(The pow_pv and pow_vv operators have two auxiliary results.)
The last two characters in the name for the operator are
``p`` (for parameter) or ``v`` (for variable).
For example the name ``add_vp`` corresponding to addition
with a variable on the left (first) and a parameter on the right (second).

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
The number of arguments for a binary operator is always two:
{xrst_literal
   // BEGIN N_ARG
   // END N_ARG
}

n_res
*****
The number of results for a binary operand is usually one,
but the pow_pv and pow_vv operators are exceptions:
{xrst_literal
   // BEGIN N_RES
   // END N_RES
}

Auxiliary Result
****************
Only the power operators, where the exponent is a variable,
have three results.
The second and third results and are called auxiliary results.
They are used to make computation of the derivatives simpler.

Derivative Calculation
**********************
{xrst_toc_table}

Operators
*********

.. csv-table::
   :header-rows: 1

   Class Name, x, y, z
   {xrst_comment BEGIN_SORT_THIS_LINE_PLUS_1}
   add_pv,   parameter,  variable, z = x + y
   add_vv,    variable,  variable, z = x + y
   div_pv,   parameter,  variable, z = x / y
   div_vp,    variable, parameter, z = x / y
   div_vv,    variable,  variable, z = x / y
   mul_pv,   parameter,  variable, z = x * y
   mul_vv,    variable,  variable, z = x * y
   pow_vp,    variable, parameter, z = x ^ y
   sub_pv,   parameter,  variable, z = x - y
   sub_vp,    variable, parameter, z = x - y
   sub_vv,    variable,  variable, z = x - y
   sub_vv,    variable,  variable, z = x - y
   zmul_pv,  parameter,  variable, :ref:`azmul-name`
   zmul_vv,  parameter,  variable, :ref:`azmul-name`
   {xrst_comment END_SORT_THIS_LINE_MINUS_1}

{xrst_end var_binary_op_t}
# ----------------------------------------------------------------------------
{xrst_begin var_pow dev}
{xrst_spell
   pv
   vv
}

The Power Function with Variable Exponent
#########################################

Syntax
******
| ``CppAD::local::op_class::pow_pv_t`` < *Base* > var_pow_op
| ``CppAD::local::op_class::pow_vv_t`` < *Base* > var_pow_op

Prototype
*********

pow_pv
======
{xrst_literal ,
   include/cppad/local/op_class/pow_pv.hpp
   BEGIN _NAMESPACE, END _NAMESPACE
   BEGIN POW_PV_T , END POW_PV_T
}

pow_vv
======
{xrst_literal ,
   include/cppad/local/op_class/pow_vv.hpp
   BEGIN _NAMESPACE, END _NAMESPACE
   BEGIN POW_VV_T , END POW_VV_T
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .


op2enum
*******
is the enum value corresponding to this operator; i.e., the inverse of
:ref:`var_enum2op-name` .

n_res
*****
The number of results for the these operators and is always three:
{xrst_literal ,
   include/cppad/local/op_class/pow_vv.hpp
   // BEGIN _N_RES , // END _N_RES
}

.. math::

   z_0 &= \log( x )
   \\
   z_1 &= \log( x ) \cdot y
   \\
   z_2 & = \exp \left[ \log( x ) \cdot y \right]

{xrst_end var_pow}
# ----------------------------------------------------------------------------
{xrst_begin var_binary_forward dev}
{xrst_spell
   tpv
}

Forward Mode Variable Binary Operators
######################################

Syntax
******
| *var_binary_op* . ``forward_0`` (
| |tab| *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )
| *var_binary_op* . ``forward`` (
| |tab| *p* , *q* , *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )
| *var_binary_op* . ``forward_dir`` (
| |tab| *q* , *r* , *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )

Prototype
*********
{xrst_literal
   // BEGIN FORWARD
   // END FORWARD
}

{xrst_template ,
   include/cppad/local/op_class/var_binary_op.xrst
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
#. If *y* is a variable,
   the Taylor coefficients for variable *y* up to order *q* .
#. The Taylor coefficients for variable *z* up to order *p* ``-1`` .
#. If this operator has auxiliary results,
   the Taylor coefficients for the variables with indices *i_z* ``-1``
   and *i_z* ``-2`` up to order *p* ``-1`` .


Output
======
The Taylor coefficients for variable *z*
from order *p* up to order *q*.
If this operator has an auxiliary results,
the Taylor coefficients for the variables with index *i_z* ``-1``
and *i_z* ``-2`` up to order *q* .
In the case of ``forward_dir`` for all the *r* directions are computed.


{xrst_end var_binary_forward}
------------------------------------------------------------------------------
{xrst_begin var_binary_reverse dev}

Reverse Mode Variable Binary Operators
######################################

Syntax
******
| *var_binary_op* . ``reverse`` (
| *d* , *i_z* , *arg* , *parameter* , *cap_order* , *taylor* , *nc_partial* , *partial*
| )

Prototype
*********
{xrst_literal
   // BEGIN REVERSE
   // END REVERSE
}

{xrst_template ,
   include/cppad/local/op_class/var_binary_op.xrst
}

u, v
****
If an operator has an auxiliary results, *u* ( *v* ) is the variable index
for the auxiliary result is *i_z* ``- 1``  ( *i_z* ``-2`` ).
Otherwise, *u* and *v* are just a place holders and do not affect the
value of H or G.


G
*
We use :math:`G(z, y, x, w, \ldots )` to denote a scalar valued function
of the variables up to variable index *i_z* .

H
*
We use :math:`H(y, x, w, \ldots )` to denote the scalar valued function
of the variables up to variable index *i_z* ``-1``
(index *i_z* ``-2`` if this operator has an auxiliary variables)
defined by

.. math::

   H(y, x, w, \ldots ) = G [ z(x, y), v(x, y), u(x, y), y, x, w, \ldots ) ]

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

{xrst_end var_binary_reverse}
------------------------------------------------------------------------------
*/
# include <cppad/local/op_class/var_base_op.hpp>

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE

// BEGIN BINARY_OP_T
template <class Base> class var_binary_op_t : public var_base_op_t<Base>
// END BINARY_OP_T
{
public:
   // BEGIN OP2ENUM
   virtual OpCode op2enum(void) const override = 0;
   // END OP2ENUM
   //
   // BEGIN N_ARG
   size_t n_arg(void) const override {  return 2; }
   // END N_ARG
   //
   // BEGIN N_RES
   size_t n_res(void) const override = 0;
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
   static void forward(
      size_t        p           ,
      size_t        q           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   { assert(false); }
   //
   // forward_dir
   static void forward_dir(
      size_t        q           ,
      size_t        r           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   { assert(false); }
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
