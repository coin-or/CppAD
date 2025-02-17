# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_INFO_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin val_op_info_t dev}
{xrst_spell
   typedef
}

Optimization Information for Value Graphs
#########################################


Syntax
******
| |tab| ``val_op_info_t`` *op_info* ( *tape* )
| |tab| *con_name* = *op_info* . *con_name* ( )
| |tab| *op_info* . ``set`` ( *i_op* )
| |tab| *var_name* = *op_info* . ``get_`` *var_name* ()


op_info
*******
{xrst_literal ,
   // BEGIN_CLASS , // END_CLASS
   // BEGIN_OP_INFO , // END_OP_INFO
}

typedef
=======
{xrst_literal
   // BEGIN_TYPEDEF
   // END_TYPEDEF
}

tape
====
is the tape that we are optimizing.

op2arg_index
============
is a vector that maps operator index to
the index of the first argument, for this operator,
in the vector that contains all the operator arguments.

Requirements
************
The ``val_op_info_t`` class satisfied the
:ref:`opt_op_info@Op_info` requirements.

{xrst_end val_op_info_t}
*/
# include <cppad/local/val_graph/tape.hpp>

// BEGIN_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local { namespace val_graph  {
template <class Tape> class op_info_t {
// END_CLASS
public:
   //
   // BEGIN_TYPEDEF
   typedef typename Tape::value_t                value_t;
   typedef CppAD::local::val_graph::op_enum_t    op_enum_t;
   typedef CppAD::vector<addr_t>                 vec_addr_t;
   typedef CppAD::vector<value_t>                vec_value_t;
   // END_TYPEDEF
   //
private:
   //
   // base_op_t
   typedef CppAD::local::val_graph::base_op_t<value_t> base_op_t;
   //
   // n_op_
   const addr_t n_op_;
   //
   // arg_all_
   const vec_addr_t& arg_all_;
   //
   // con_all_
   const vec_value_t& con_all_;
   //
   // op_enum_vec_
   const CppAD::vector<uint8_t>& op_enum_vec_;
   //
   // op2arg_index_
   const vec_addr_t& op2arg_index_;
   //
public:
   //
   // BEGIN_OP_INFO
   // op_info_t op_info(tape)
   op_info_t(
      const tape_t<value_t>& tape         ,
      const vec_addr_t&      op2arg_index )
   // END_OP_INFO
   : n_op_ ( tape.n_op() )
   , arg_all_ ( tape.arg_vec() )
   , con_all_ ( tape.con_vec() )
   , op_enum_vec_ ( tape.op_enum_vec() )
   , op2arg_index_ ( op2arg_index )
   {  }
   // -----------------------------------------------------------------------
   // Results that do not depend on i_op
   // -----------------------------------------------------------------------
   //
   // n_op = op_info.n_op()
   addr_t n_op(void) const
   // END_N_OP
   {  return n_op_; }
   //
   // arg_all = op_info.arg_all()
   const vec_addr_t& arg_all(void) const
   {  return arg_all_; }
   //
   // con_all = op_info.con_all()
   const vec_value_t& con_all(void) const
   {  return con_all_; }
   //
   // BEGIN_GET
   // op_info.get(i_op, op_enum, arg_index, n_arg, n_before, n_after, is_con_op)
   void get(
      addr_t       i_op       ,
      op_enum_t&   op_enum    ,
      addr_t&      arg_index  ,
      addr_t&      n_arg      ,
      addr_t&      n_before   ,
      addr_t&      n_after    ,
      bool&        is_con_op  )
   // END_GET
   {  //
      // op_enum, op_ptr
      op_enum           = op_enum_t( op_enum_vec_[i_op] );
      base_op_t* op_ptr = op_enum2class<value_t>(op_enum);
      //
      // arg_index, n_arg, n_after, n_before
      arg_index  = op2arg_index_[i_op];
      n_arg      = op_ptr->n_arg(arg_index, arg_all_);
      n_before   = op_ptr->n_before();
      n_after    = op_ptr->n_after();
      is_con_op  = op_enum == con_op_enum;
   }
};


} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// --------------------------------------------------------------------------
# endif
