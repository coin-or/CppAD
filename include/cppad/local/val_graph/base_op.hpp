# ifndef  CPPAD_LOCAL_VAL_GRAPH_BASE_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_BASE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/val_type.hpp>
# include <cstdio>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_base_op dev}

The Value Operator Base Class
#############################

Prototype
*********
{xrst_literal
   // BEGIN_BASE_OP_T
   // END_BASE_OP_T
}

Purpose
*******
This section describes the value operator base class
(all the value operators are derived from this class).

Member Variables
****************
None of the value operators have any member variables,
so their functions are like static functions but cannot be declared as
such because they are virtual or override functions.

Value
*****
This template parameter is the type used for each element of the
:ref:`val_graph@Value Vector` .

base_op
*******
This is the base class used for operators.
All of its functions are public and pure virtual; i.e.,
they all must be defined in order for a derived class object to be used.

op_enum
*******
This member function returns the enum value corresponding to this operator;
see :ref:`val_graph_type@op_enum_t` .
{xrst_literal
   // BEGIN_OP_ENUM
   // END_OP_ENUM
}

n_before
********
This member function returns the number of auxiliary arguments
that come before the arguments that are value vector indices.
{xrst_literal
   // BEGIN_N_BEFORE
   // END_N_BEFORE
}

n_after
*******
This member function returns the number of auxiliary arguments
that come after the arguments that are value vector indices.
This can only be zero or one.
{xrst_literal
   // BEGIN_N_AFTER
   // END_N_AFTER
}


n_arg
*****
This member function returns the number of arguments for this operator.
{xrst_literal
   // BEGIN_N_ARG
   // END_N_ARG
}

Fixed
=====
If *n_after* returns zero, this operator has a fixed number of arguments.
In this case, the *arg_index* and *arg_vec* arguments to this function.
You can set *arg_index* to
*arg_vec*.size() (an invalid value) if you know this is not a call operator.

Variable
========
If *n_after* returns one, this operator has a variable number of arguments.
In this case the last argument is an auxiliary argument and is
equal to the number of arguments for this use of this operator.

n_res
*****
This member function returns the number of results for this operator.
{xrst_literal
   // BEGIN_N_RES
   // END_N_RES
}

eval
****
The values in *val_vec* with index less than *res_index*
are inputs to this member function.
The *n_res* values starting at *res_index* in *val_vec* are
computed by this function.
{xrst_literal
   // BEGIN_EVAL
   // END_EVAL
}

tape
====
This is a pointer to the tape that the operator is being evaluated for.
We use arg_vec to denote tape->arg_vec() below.

trace
=====
if this is true (false) the operator is (is not) printed
at the end of the eval operation.

ind_vec_vec
===========
The size of this vector is equal to the number of dynamic vectors.
The size of each element of this vector is equal to the size of the
corresponding dynamic vector.
If there are no dynamic vectors in the tape this argument must
be empty (or not present).
This argument is only used by the :ref:`val_vector_op-name` operations.

compare_false
=============
This is the number of :ref:`val_comp_op-name` that had a false
result for their comparisons.
This is both an input and output; i.e., each false comparison
will add one to this value.

name
****
is a short name, 5 or less characters, for this operator.

arg_index
*********
is the index in *arg_vec* where the *n_arg* arguments to this function start.

res_index
*********
is the index in *val_vec* where the results for this operator start.

val_vec
*******
is the entire :ref:`val_graph@Value Vector` .

arg_vec
*******
During  the eval function, arg_vec below denotes tape->arg_vec() .

Unary Operators
===============
If this operator is a unary operator

#. arg_vec[arg_index + 0] < res_index
#. val_vec[ arg_vec[ arg_index + 0 ] ] is the operand
#. val_vec[ res_index] is the result computed by eval

Binary Operators
================
If this operator is a binary operator

#. arg_vec[arg_index + 0] < res_index
#. arg_vec[arg_index + 1] < res_index
#. val_vec[ arg_vec[ arg_index + 1 ] ] is the left operand
#. val_vec[ arg_vec[ arg_index + 2 ] ] is the right operand
#. val_vec[ res_index] is the result computed by eval

is_unary
********
is true (false) if this is (is not) a unary operator;
see :ref:`val_unary_op-name`.
{xrst_literal
   // BEGIN_IS_UNARY
   // END_IS_UNARY
}

is_binary
*********
is true (false) if this is (is not) a binary operator;
see :ref:`val_binary_op-name`.
{xrst_literal
   // BEGIN_IS_BINARY
   // END_IS_BINARY
}


Operator Classes
****************
{xrst_comment BEGIN_SORT_THIS_LINE_PLUS_2}
{xrst_toc_table
   include/cppad/local/val_graph/binary_op.hpp
   include/cppad/local/val_graph/call_op.hpp
   include/cppad/local/val_graph/cexp_op.hpp
   include/cppad/local/val_graph/comp_op.hpp
   include/cppad/local/val_graph/con_op.hpp
   include/cppad/local/val_graph/csum_op.hpp
   include/cppad/local/val_graph/dis_op.hpp
   include/cppad/local/val_graph/pri_op.hpp
   include/cppad/local/val_graph/unary_op.hpp
   include/cppad/local/val_graph/vector_op.hpp
}
{xrst_comment END_SORT_THIS_LINE_MINUS_2}

{xrst_end val_base_op}
*/
//
// tape_t
template<class Value> class tape_t;

// BEGIN_BASE_OP_T
template <class Value> class base_op_t {
public:
// END_BASE_OP_T
   // BEGIN_OP_ENUM
   virtual op_enum_t op_enum(void)  const = 0;
   // END_OP_ENUM
   //
   // BEGIN_N_BEFORE
   virtual addr_t    n_before(void) const = 0;
   // END_N_BEFORE
   //
   // BEGIN_N_AFTER
   virtual addr_t    n_after(void)  const = 0;
   // END_N_AFTER
   //
   // BEGIN_N_ARG
   virtual addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const = 0;
   // END_N_ARG
   //
   // BEGIN_N_RES
   virtual addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const = 0;
   // END_N_RES
   //
   // BEGIN_EVAL
   virtual void eval(
      const tape_t<Value>*       tape          ,
      bool                       trace         ,
      addr_t                     arg_index     ,
      addr_t                     res_index     ,
      Vector<Value>&             val_vec       ,
      Vector< Vector<addr_t> >&  ind_vec_vec   ,
      size_t&                    compare_false ) const = 0;
   // END_EVAL
   //
   // BEGIN_IS_UNARY
   virtual bool is_unary(void) const
   // END_IS_UNARY
   {  return false; }
   //
   // BEGIN_IS_BINARY
   virtual bool is_binary(void) const
   // END_IS_BINARY
   {  return false; }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
