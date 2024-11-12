# ifndef CPPAD_LOCAL_GRAPH_CPP_GRAPH_ITR_HPP
# define CPPAD_LOCAL_GRAPH_CPP_GRAPH_ITR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/vector.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>

// BEGIN_CPPAD_LOCAL_GRAPH_NAMESPACE
namespace CppAD { namespace local { namespace graph {

class cpp_graph_itr {
/*
{xrst_begin cpp_graph_itr_data dev}

C++ AD Graph Iterator Private Member Data
#########################################
{xrst_spell_off}
{xrst_code hpp} */
private:
   // valuse set by constructor
   const vector<graph_op_enum>*   operator_vec_;
   const vector<size_t>*          operator_arg_;
   //
   // set by constructor and ++
   size_t                         op_index_;
   size_t                         first_arg_;
   //
   // set by set_value
   graph_op_enum                  op_enum_;
   size_t                         first_node_;
   size_t                         n_result_;
   size_t                         call_id_;
   vector<size_t>                 str_index_;
   vector<size_t>                 arg_node_;
/* {xrst_code}
{xrst_spell_on}

{xrst_end cpp_graph_itr_data}
------------------------------------------------------------------------------
{xrst_begin cpp_graph_itr_set_value dev}
{xrst_spell
   str
}

C++ AD Graph Iterator set_value()
#################################

Syntax
******
| *itr* . ``set_value`` ()

op_index\_
**********
This input is the operator index for the value we are retrieving.

first_arg\_
***********
This input is the first argument index for the value we are retrieving.

first_node\_
************
The input value of this argument does not matter.
It is set to the index in ``operator_arg_``
of the first node argument for this operator.

op_enum\_
*********
The input value of this argument does not matter.
It is set to the :ref:`graph_op_enum-name` for the operator

str_index\_
***********
The input value of this argument does not matter.
Upon return its size is zero except for the special cases
listed below:

atom_graph_op, atom4_graph_op
=============================
If *op_enum_* is ``atom_graph_op`` or ``atom4_graph_op`` ,
``str_index_.size() == 1`` and
``str_index_[0]`` is the index in
:ref:`cpp_ad_graph@atomic_name_vec`
for the function called by this operator.

discrete_graph_op
=================
If *op_enum_* is ``discrete_graph_op`` ,
``str_index_.size() == 1`` and
``str_index_[0]`` is the index in
:ref:`cpp_ad_graph@discrete_name_vec`
for the function called by this operator.

print_graph_op
==============
If *op_enum_* is ``print_graph_op`` ,
``str_index_.size() == 2`` and
``str_index_[0]`` ( ``str_index_[1]`` )
is the index in
:ref:`cpp_ad_graph@print_text_vec` for the
:ref:`PrintFor@before`  (:ref:`PrintFor@after` ) text.

n_result\_
**********
The input value of this argument does not matter.
This is set to the number of result nodes for this operator.

call_id\_
*********
If *op_enum_* is ``atom4_graph_op`` ,
``call_id_`` is set to the :ref:`atomic_four_call@call_id`
for this function call.
If *op_enum_* is ``atom_graph_op`` ,
``call_id`` is set to zero.

arg_node\_
**********
The input value of this argument does not matter.
Upon return, its size is the number of arguments,
that are node indices, for this operator usage.
The value of the elements are the node indices.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
   void set_value(void)
/* {xrst_code}
{xrst_spell_on}

{xrst_end cpp_graph_itr_set_value}
*/
{  // initialize output values
   size_t invalid_index   = std::numeric_limits<size_t>::max();
   size_t n_arg      = invalid_index;
   first_node_       = invalid_index;
   n_result_         = invalid_index;
   call_id_          = invalid_index;
   str_index_.resize(0);
   arg_node_.resize(0);
   //
   // op_enum
   op_enum_          = (*operator_vec_)[op_index_];
   //
   // n_result_, n_arg, call_id_, str_index_
   switch( op_enum_ )
   {
      // unary operators
      case abs_graph_op:
      case acos_graph_op:
      case acosh_graph_op:
      case asin_graph_op:
      case asinh_graph_op:
      case atan_graph_op:
      case atanh_graph_op:
      case cos_graph_op:
      case cosh_graph_op:
      case erf_graph_op:
      case erfc_graph_op:
      case exp_graph_op:
      case expm1_graph_op:
      case log1p_graph_op:
      case log_graph_op:
      case neg_graph_op:
      case sign_graph_op:
      case sin_graph_op:
      case sinh_graph_op:
      case sqrt_graph_op:
      case tan_graph_op:
      case tanh_graph_op:
      first_node_ = first_arg_;
      n_result_   = 1;
      n_arg       = 1;
      break;

      // binary operators
      case add_graph_op:
      case azmul_graph_op:
      case div_graph_op:
      case mul_graph_op:
      case pow_graph_op:
      case sub_graph_op:
      first_node_ = first_arg_;
      n_result_   = 1;
      n_arg       = 2;
      break;

      // discrete_graph_op
      case discrete_graph_op:
      first_node_ = first_arg_ + 1;
      str_index_.push_back( (*operator_arg_)[first_node_ - 1] );
      n_result_   = 1;
      n_arg       = 1;
      break;

      // atom_graph_op
      case atom_graph_op:
      first_node_ = first_arg_ + 3;
      str_index_.push_back( (*operator_arg_)[first_node_ - 3] );
      call_id_    = 0;
      n_result_   = (*operator_arg_)[first_node_ - 2];
      n_arg       = (*operator_arg_)[first_node_ - 1];
      break;

      // atom4_graph_op
      case atom4_graph_op:
      first_node_ = first_arg_ + 4;
      str_index_.push_back( (*operator_arg_)[first_node_ - 4] );
      call_id_    = (*operator_arg_)[first_node_ - 3];
      n_result_   = (*operator_arg_)[first_node_ - 2];
      n_arg       = (*operator_arg_)[first_node_ - 1];
      break;

      // print_graph_op
      case print_graph_op:
      first_node_ = first_arg_ + 2;
      str_index_.push_back( (*operator_arg_)[first_node_ - 2] );
      str_index_.push_back( (*operator_arg_)[first_node_ - 1] );
      n_result_   = 0;
      n_arg       = 2;
      break;


      // conditional expressions
      case cexp_eq_graph_op:
      case cexp_le_graph_op:
      case cexp_lt_graph_op:
      first_node_ = first_arg_;
      n_result_   = 1;
      n_arg       = 4;
      break;

      // comparison operators
      case comp_eq_graph_op:
      case comp_le_graph_op:
      case comp_lt_graph_op:
      case comp_ne_graph_op:
      first_node_ = first_arg_;
      n_result_   = 0;
      n_arg       = 2;
      break;

      // sum_graph_op
      case sum_graph_op:
      first_node_ = first_arg_ + 1;
      n_result_   = 1;
      n_arg       = (*operator_arg_)[first_node_ - 1];
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
   }
   // set arg_node
   arg_node_.resize(n_arg);
   for(size_t i = 0; i < n_arg; i++)
      arg_node_[i] = (*operator_arg_)[first_node_ + i];

   return;
}
/* %$$
-------------------------------------------------------------------------------
{xrst_begin cpp_graph_itr_types dev}

C++ AD Graph Iterator Types
###########################

{xrst_spell_off}
{xrst_code hpp} */
public:
   typedef struct {
      graph_op_enum          op_enum;
      size_t                 n_result;
      size_t                 call_id;
      const vector<size_t>*  str_index_ptr;
      const vector<size_t>*  arg_node_ptr;
   } value_type;
   typedef std::input_iterator_tag    iterator_category;
/* {xrst_code}
{xrst_spell_on}

{xrst_end cpp_graph_itr_types}
------------------------------------------------------------------------------
{xrst_begin cpp_graph_itr_ctor dev}

C++ AD Graph Iterator Constructors
##################################

Syntax
******

| ``cpp_graph_itr`` *default*
| ``cpp_graph_itr`` *itr* ( *operator_vec* , *operator_arg* , *op_index*

Prototype
*********
{xrst_literal
   // BEGIN_CTOR
   // END_CTOR
}

default
*******
The result of the default constructor can only be used as a target
for the assignment operator.

operator_vec
************
Is the :ref:`cpp_ad_graph@operator_vec`
for the ``cpp_graph`` container that this iterator refers to.

operator_arg
************
Is the :ref:`operator_arg<cpp_ad_graph@operator_vec>`
for the ``cpp_graph`` container that this iterator refers to.

op_index
********
This must be either zero (the ``begin()`` for the container)
or equal to the size of *operator_vec*
(the ``end()`` for the container).

{xrst_end cpp_graph_itr_ctor}
*/
   cpp_graph_itr(void)
   : operator_vec_(nullptr), operator_arg_(nullptr)
   { }
   // BEGIN_CTOR
   cpp_graph_itr(
      const vector<graph_op_enum>& operator_vec   ,
      const vector<size_t>&        operator_arg   ,
      size_t                       op_index       )
   // END_CTOR
   :
   operator_vec_(&operator_vec) ,
   operator_arg_(&operator_arg) ,
   op_index_(op_index)
   {  // end constructor
      if( op_index == operator_vec.size() )
         return;
      //
      // begin constructor
      CPPAD_ASSERT_KNOWN( op_index == 0,
         "cpp_graph_itr: constructor op_index not 0 or operator_vec.size()"
      );
      // start at the beginning of operator_vec
      first_arg_ = 0;
      //
      // get the value, and first_node_, for this operator
      set_value();
   }
/* %$$
------------------------------------------------------------------------------
{xrst_begin cpp_graph_itr_input dev}

C++ AD Graph Iterator Input Operations
######################################

{xrst_spell_off}
{xrst_code hpp} */
   // itr == other
   bool operator==(const cpp_graph_itr& other) const
   {  return op_index_ == other.op_index_;
   }
   // itr != other
   bool operator!=(const cpp_graph_itr& other) const
   {  return op_index_ != other.op_index_;
   }
   // *itr
   value_type operator*(void)
   {  CPPAD_ASSERT_KNOWN( operator_vec_ != nullptr,
         "cpp_graph_itr: attempt to dereference default iterator"
      );
      CPPAD_ASSERT_KNOWN( op_index_ < operator_vec_->size(),
         "cpp_graph_itr: attempt to dereference past last element in graph"
      );
      value_type ret;
      ret.op_enum       = op_enum_;
      ret.n_result      = n_result_;
      ret.call_id       = call_id_;
      ret.str_index_ptr = &str_index_;
      ret.arg_node_ptr  = &arg_node_;
      return ret;
   }
   // ++itr
   cpp_graph_itr& operator++(void)
   {  ++op_index_;
      first_arg_ = first_node_ + arg_node_.size();
      set_value();
      return *this;
   }
   // itr++
   cpp_graph_itr operator++(int)
   {  cpp_graph_itr ret(*this);
      ++op_index_;
      first_arg_ = first_node_ + arg_node_.size();
      set_value();
      return ret;
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cpp_graph_itr_input}
*/

};

} } } // END_CPPAD_LOCAL_GRAPH_NAMESPACE

# endif
