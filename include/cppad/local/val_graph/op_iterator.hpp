# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_ITERATOR_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_ITERATOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin val_op_iterator dev}

Class For Iterating Over Operators in a Value Tape
##################################################

Syntax
******
| |tab| ``op_iterator`` < *Value* > *op_itr* ( *tape* , *op_index* )
| |tab| *op_itr* . ``op_ptr`` ()
| |tab| *op_itr* . ``arg_index`` ()
| |tab| *op_itr* . ``res_index`` ()

Prototype
*********
{xrst_literal
   // BEGIN_CTOR
   // END_CTOR
}
{xrst_literal
   // BEGIN_MEMBER_FUNCTIONS
   // END_MEMBER_FUNCTIONS
}

Value
*****
is the :ref:`val_tape@Value` type for the tape.


tape
****
is the :ref:`val_tape-name` containing the operators we are
iterating over.

op_index
********
is the operator index where the iterator starts.
This must be zero (for the beginning of the tape)
or the number of operators in the tape (for the end of the tape).

op_itr
******
is the operator iterator.

{xrst_end val_op_iterator}
*/
# include <cppad/local/val_graph/op_enum2class.hpp>
# include <cppad/core/cppad_assert.hpp>

// BEGIN_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
namespace CppAD { namespace local { namespace val_graph  {

//
// tape_t;
template <class Value> class tape_t;

template <class Value>
class op_iterator {
//
private:
   //
   // tape_
   const tape_t<Value>& tape_;
   //
   // op_ptr_
   base_op_t<Value>* op_ptr_;
   //
   // op_index_
   addr_t op_index_;
   //
   // arg_index_
   addr_t arg_index_;
   //
   // res_index_
   addr_t res_index_;
   //
public:
   //
   // BEGIN_MEMBER_FUNCTIONS
   const base_op_t<Value>* op_ptr(void) const
   {  return op_ptr_; }
   addr_t arg_index(void) const
   {  return arg_index_; }
   addr_t res_index(void) const
   {  return res_index_; }
   // END_MEMBER_FUNCTIONS
   //
   // BEGIN_CTOR
   op_iterator(const tape_t<Value>& tape, addr_t op_index)
   // END_CTOR
   :
   tape_ ( tape )         ,
   op_ptr_(nullptr)       ,
   op_index_ ( op_index )
   {  addr_t n_op = tape.n_op();
      //
      if( op_index == 0 )
      {  op_enum_t op_enum = op_enum_t( tape.op_enum_vec()[op_index] );
         //
         arg_index_ = 0;
         res_index_ = tape.n_ind();
         op_ptr_    = op_enum2class<Value>(op_enum);
      }
      else if( op_index == n_op )
      {  arg_index_ = addr_t( tape.arg_vec().size() );
         res_index_ = tape.n_val();
      }
      else
      {  CPPAD_ASSERT_KNOWN( false,
            "op_iterator: op_index is not zero or number of operators in tape"
         );
      }
   }
   //
   // BEGIN_INCREMENT
   void operator++(void)
   // END_INCREMENT
   {  CPPAD_ASSERT_KNOWN( op_index_ < addr_t( tape_.n_op() ),
         "op_iterator: attempt to increment past the end of the tape"
      );
      //
      // n_arg, n_res
      addr_t n_arg = op_ptr_->n_arg(arg_index_, tape_.arg_vec());
      addr_t n_res = op_ptr_->n_res(arg_index_, tape_.arg_vec());
      //
      // op_index_, arg_index_, res_index_
      ++op_index_;
      res_index_ += n_res;
      //
      // op_ptr_
      if( op_index_ == tape_.n_op() )
      {  op_ptr_    = nullptr;
         arg_index_ = addr_t( tape_.arg_vec().size() ); // invalid value
      }
      else
      {  if( tape_.op2arg_index().size() == 0 )
            arg_index_ += n_arg;
         else
            arg_index_ = tape_.op2arg_index()[op_index_];
         //
         op_enum_t op_enum = op_enum_t( tape_.op_enum_vec()[op_index_] );
         op_ptr_           = op_enum2class<Value>(op_enum);
      }
   }
   //
   // BEGIN_DECREMENT
   void operator--(void)
   // END_DECREMENT
   {  CPPAD_ASSERT_KNOWN( 0 < op_index_,
         "op_iterator: attempt to decrement below the beginning of the tape"
      );
      //
      // op_index_
      --op_index_;
      //
      // op_ptr_
      op_enum_t op_enum = op_enum_t( tape_.op_enum_vec()[op_index_] );
      op_ptr_           = op_enum2class<Value>(op_enum);
      //
      // n_after
      addr_t n_after = op_ptr_->n_after();
      CPPAD_ASSERT_UNKNOWN( n_after == 0 || n_after == 1 );
      //
      // arg_index_
      if( 0 < tape_.op2arg_index().size() )
         arg_index_ = tape_.op2arg_index()[op_index_];
      else
      {  addr_t n_arg;
         if( n_after )
            n_arg = tape_.arg_vec()[arg_index_ - 1];
         else
         {  // arg_index
            // set to an invalid value for an index in the arg_vec vector.
            addr_t arg_index = addr_t( tape_.arg_vec().size() );
            n_arg = op_ptr_->n_arg(arg_index, tape_.arg_vec());
         }
         arg_index_ -= n_arg;
      }
      //
      // res_index_
      addr_t n_res = op_ptr_->n_res(arg_index_, tape_.arg_vec());
      res_index_ -= n_res;
   }
};


} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// --------------------------------------------------------------------------
# endif
