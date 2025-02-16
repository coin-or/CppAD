# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_INFO_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin val_op_info_t dev}

Random Access Value Operator Information Used by Optimizer
##########################################################

Syntax
******
| |tab| ``op_info_t`` < *Tape* > *op_info* ( *tape* )
| |tab|  *op_info* . ``set`` ( *i_op* )
| |tab|  *var_name* = *op_info* . ``get_``\ *var_name* ( *i_op* )

op_info
*******
In order to randomly access this information,
this constructor creates two vector with elements of type ``addr_t``
and length equal to the number of operators.
{xrst_literal
   // BEGIN_OPT_INFO
   // END_OPT_INFO
}

tape
====
is the :ref:`val_tape-name` containing the operators we are
iterating over.

set
***
sets the operator index for subsequent get operation
{xrst_literal
   // BEGIN_SET
   // END_SET
}

i_op
====
is the index of this operator in the tape

get
***
The  *var_name* in the get operations must be one of the following:

var_name
========
is variable holding the value returned by the get function.
The get function name is ``get_``\ *var_name* .
The following list of *var_name* are the possible choices for the get function
(you can use a different name for the variable returned.)


.. csv-table::

   *var_name*  , type,       Meaning
   op_enum     , op_enum_t,  An enum value that determines the operator
   i_op        , addr_t,     value of *i_op* in the previous call to ``set``
   arg_index   , addr_t,     index of first argument for this operator
   res_index   , addr_t,     index of first result for this operator
   n_arg       , addr_t,     number of arguments for this operator
   n_before    , addr_t,     number of arguments before first value index
   n_after     , addr_t,     number of arguments after last value index
   is_con_op   , bool,       is this a constant operator; see is_con_op below


is_con_op
=========
A constant operator must have n_arg = 1, n_before = 0, and n_after = 0 .
Furthermore, the value of the constant is::

   con_vec[ arg_vec[ arg_index ] ]

The value of *is_con_op* must only depend on the value of *op_enum* ;
i.e., it two operators have the same *op_enum*, they have the same
*is_con_op* .

i_op
====
is an ``addr_t`` representation of the operator index.

{xrst_end val_op_info_t}
*/
# include <cppad/local/val_graph/tape.hpp>

// BEGIN_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
namespace CppAD { namespace local { namespace val_graph  {

template <class Tape>
class op_info_t {
public:
   //
   // value_t
   typedef typename Tape::value_t value_t;
   //
private:
   //
   // vector
   template <class Value_t> using vector = CppAD::vector<Value_t>;
   //
   // base_op_t
   typedef CppAD::local::val_graph::base_op_t<value_t> base_op_t;
   //
   // op_enum_t
   typedef CppAD::local::val_graph::op_enum_t op_enum_t;
   //
   // bool_vec_t
   typedef CppAD::vectorBool bool_vec_t;
   //
   // n_op_
   const addr_t n_op_;
   //
   // arg_vec_
   const vector<addr_t>& arg_vec_;
   //
   // con_vec_
   const vector<value_t>& con_vec_;
   //
   // op_enum_vec_
   const vector<uint8_t>& op_enum_vec_;
   //
   // op2arg_index_
   // effectively const (only set by constructor)
   Vector<addr_t> op2arg_index_;
   //
   // op2res_index_
   // effectively const (only set by constructor)
   Vector<addr_t> op2res_index_;
   //
   // ------------------------------------------------------------------------
   // set and get values
   //
   op_enum_t op_enum_;
   addr_t    i_op_;
   addr_t    arg_index_;
   addr_t    res_index_;
   addr_t    n_arg_;
   addr_t    n_before_;
   addr_t    n_after_;
   bool      is_con_op_;
   // ------------------------------------------------------------------------
   //
public:
   //
   // BEGIN_OPT_INFO
   // op_info_t op_info(tape)
   op_info_t(const tape_t<value_t>& tape)
   // END_OPT_INFO
   : n_op_ ( tape.n_op() )
   , arg_vec_ ( tape.arg_vec() )
   , con_vec_ ( tape.con_vec() )
   , op_enum_vec_ ( tape.op_enum_vec() )
   {  //
      // op2arg_index_, op2res_index_
      op2arg_index_.resize( n_op_ );
      op2res_index_.resize( n_op_ );
      bidir_iterator<value_t> bi_itr(tape, 0);
      for(addr_t i_op = 0; i_op < n_op_; ++i_op)
      {  op2arg_index_[i_op] = bi_itr.arg_index();
         op2res_index_[i_op] = bi_itr.res_index();
         ++bi_itr;
      }
   }
   // -----------------------------------------------------------------------
   // Results that do not depend on i_op
   // -----------------------------------------------------------------------
   //
   // n_op = op_info.n_op()
   addr_t n_op(void) const
   // END_N_OP
   {  return n_op_; }
   //
   // arg_vec = op_info.arg_vec()
   const vector<addr_t>& arg_vec(void) const
   {  return arg_vec_; }
   //
   // con_vec = op_info.con_vec()
   const vector<value_t>& con_vec(void) const
   {  return con_vec_; }
   //
   // ------------------------------------------------------------------------
   // Results that depend on i_op
   // ------------------------------------------------------------------------
   // set(i_op)
   // BEGIN_SET
   void set(addr_t i_op)
   // END_SET
   {  //
      // op_ptr
      op_enum_t op_enum = op_enum_t( op_enum_vec_[i_op] );
      base_op_t* op_ptr = op_enum2class<value_t>(op_enum);
      //
      // op_enum_, i_op_, arg_index_, res_index_, n_arg_, n_after_, n_before_
      op_enum_   = op_ptr->op_enum();
      i_op_      = i_op;
      arg_index_ = op2arg_index_[i_op];
      res_index_ = op2res_index_[i_op];
      n_arg_     = op_ptr->n_arg(arg_index_, arg_vec_);
      n_before_  = op_ptr->n_before();
      n_after_   = op_ptr->n_after();
      is_con_op_ = op_enum_ == con_op_enum;
   }
   //
   // get
   op_enum_t get_op_enum(void) const
   {  return op_enum_; }
   //
   addr_t get_i_op(void) const
   {  return i_op_; }
   //
   addr_t get_arg_index(void) const
   {  return arg_index_; }
   //
   addr_t res_index(void) const
   {  return res_index_; }
   //
   addr_t get_n_arg(void) const
   {  return n_arg_; }
   //
   addr_t get_n_before(void) const
   {  return n_before_; }
   //
   addr_t get_n_after(void) const
   {  return n_after_; }
   //
   bool get_is_con_op(void) const
   {  return is_con_op_; }
};


} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// --------------------------------------------------------------------------
# endif
