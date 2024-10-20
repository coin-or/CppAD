# ifndef CPPAD_LOCAL_OP_CLASS_BASE_OP_HPP
# define CPPAD_LOCAL_OP_CLASS_BASE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin var_base_op_t dev}

The Variable Operator Base Class
################################

Syntax
******
| ``CppAD::local::op_class::base_op_t`` *base_op*

Prototype
*********
{xrst_literal ,
   BEGIN NAMESPACE , END NAMESPACE
   BEGIN BASE_OP_T , END BASE_OP_T
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .

Derived Documentation
*********************
{xrst_toc_table
   include/cppad/local/op_class/binary_op.hpp
}

{xrst_end var_base_op_t}
------------------------------------------------------------------------------
*/

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
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

}}} // END namespace

# endif
