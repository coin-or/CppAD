# ifndef CPPAD_LOCAL_OP_CLASS_VAR_BASE_OP_HPP
# define CPPAD_LOCAL_OP_CLASS_VAR_BASE_OP_HPP
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
| ``CppAD::local::op_class::var_base_op_t`` *base_op*

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
is the number of arguments for this operator:
{xrst_literal
   // BEGIN N_ARG
   // END N_ARG
}

n_res
*****
is the number of results for this operator:
{xrst_literal
   // BEGIN N_RES
   // END N_RES
}

forward_0
*********
can be used to compute zero order forward mode results:
{xrst_literal
   // BEGIN FORWARD_0
   // END FORWARD_0
}

forward
*******
can be used to compute any order forward mode results:
{xrst_literal
   // BEGIN FORWARD_ANY
   // END FORWARD_ANY
}

forward_dir
***********
can be used to compute any order forward mode results
in multiple directions at the same time:
{xrst_literal
   // BEGIN FORWARD_DIR
   // END FORWARD_DIR
}

reverse
*******
can be used to compute any order reverse mode results:
{xrst_literal
   // BEGIN REVERSE
   // END REVERSE
}

Derived Documentation
*********************
{xrst_toc_table
   include/cppad/local/op_class/var_unary_op.hpp
   include/cppad/local/op_class/var_binary_op.hpp
   include/cppad/local/op_class/enum2op.hpp
   include/cppad/local/op_class/compare.hpp
}

{xrst_end var_base_op_t}
------------------------------------------------------------------------------
*/

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE

// BEGIN BASE_OP_T
template <class Base> class var_base_op_t
// END BASE_OP_T
{
public:
   //
   // BEGIN OP2ENUM
   virtual OpCode op2enum(void) const = 0;
   // END OP2ENUM
   //
   // BEGIN N_ARG
   virtual size_t n_arg(void) const = 0;
   // END N_ARG
   //
   // BEGIN N_RES
   virtual size_t n_res(void) const = 0;
   // END N_RES
   //
   // BEGIN FORWARD_0
   virtual void forward_0(
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const = 0;
   // END FORWARD_0
   //
   // BEGIN FORWARD_ANY
   virtual void forward(
      size_t        p           ,
      size_t        q           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const = 0;
   // END FORWARD_ANY
   //
   // BEGIN FORWARD_DIR
   virtual void forward_dir(
      size_t        q           ,
      size_t        r           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const = 0;
   // END FORWARD_DIR
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
