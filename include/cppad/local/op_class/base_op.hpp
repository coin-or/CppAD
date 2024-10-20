# ifndef CPPAD_LOCAL_OP_CLASS_BASE_OP_HPP
# define CPPAD_LOCAL_OP_CLASS_BASE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin_parent base_op_t dev}
{xrst_spell
   div
   pv
   vp
   vv
   zmul
}

The Operator Base Class
#######################

Syntax
******
| ``CppAD::local::base_op_t`` *base_op*

Prototype
*********
{xrst_literal ,
   BEGIN NAMESPACE , END NAMESPACE
   BEGIN BASE_OP_T , END BASE_OP_T
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by this routine are done using
type *Base* .

Derived Documentation
*********************
{xrst_toc_table}

Binary Operators
****************

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
   sub_pv,   parameter,  variable, z = x - y
   sub_vp,    variable, parameter, z = x - y
   sub_vv,    variable,  variable, z = x - y
   sub_vv,    variable,  variable, z = x - y
   zmul_pv,  parameter,  variable, :ref:`azmul-name`
   zmul_vv,  parameter,  variable, :ref:`azmul-name`
   {xrst_comment END_SORT_THIS_LINE_MINUS_1}

{xrst_end base_op_t}
# ----------------------------------------------------------------------------
{xrst_begin derived_binary_forward dev}
{xrst_spell
   vp
   tpv
}

Forward Variable Binary Operators
#################################

Syntax
******
| *base_op* . ``forward_0`` (
| |tab| *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )
| *base_op* . ``forward`` (
| |tab| *p* , *q* , *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )
| *base_op* . ``forward_dir`` (
| |tab| *q* , *r* , *i_z* , *arg* , *parameter* *cap_order* , *taylor*
| )

Prototype
*********
{xrst_literal
   // BEGIN FORWARD
   // END FORWARD
}

{xrst_template ,
   include/cppad/local/op_class/binary_op.xrst
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

Output
======
The Taylor coefficients for variable *z*
from order *p* up to order *q*
(and in the case of ``forward_dir`` for all the *r* directions).

{xrst_end derived_binary_forward}
------------------------------------------------------------------------------
{xrst_begin derived_binary_reverse dev}
{xrst_spell
   vp
}

Reverse Variable Binary Operators
#################################

Syntax
******
| *base_op* . ``reverse`` (
| *d* , *i_z* , *arg* , *parameter* , *cap_order* , *taylor* , *nc_partial* , *partial*
| )

Prototype
*********
{xrst_literal
   // BEGIN REVERSE
   // END REVERSE
}

{xrst_template ,
   include/cppad/local/op_class/binary_op.xrst
}

G
*
We use :math:`G(z, y, x, w, \ldots )` to denote a scalar valued function
of the variables up to variable index *i_z* .

H
*
We use :math:`H(y, x, w, \ldots )` to denote the scalar valued function
of the variables up to variable index *i_z* ``-1`` defined by

.. math::

   H(y, x, w, \ldots ) = G [ z(x, y), y, x, w, \ldots ) ]

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

{xrst_end derived_binary_reverse}
------------------------------------------------------------------------------
*/

// BEGIN NAMESPACE
namespace CppAD { namespace local {
// END NAMESPACE

// BEGIN BASE_OP_T
template <class Base> class base_op_t
// END BASE_OP_T
{
public:
   // BEGIN FORWARD
   // forward_0
   virtual void forward_0(
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const = 0;
   //
   // forward
   virtual void forward(
      size_t        p           ,
      size_t        q           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const = 0;
   //
   // forward_dir
   virtual void forward_dir(
      size_t        q           ,
      size_t        r           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const = 0;
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
      Base*         partial     ) const = 0;
   // END REVERSE
};

}} // end namespace

# endif
