# ifndef CPPAD_LOCAL_PLAY_SEQUENTIAL_ITERATOR_HPP
# define CPPAD_LOCAL_PLAY_SEQUENTIAL_ITERATOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file sequential_iterator.hpp
*/

/*!
Constant sequential iterator for a player object.

\tparam Addr
An integer type capable of representing the largest value in the vectors
arg_vec, op2arg_vec, op2var_vec, var2op_vec.

\par
Except for constructor, the public API for this class is the same as
for the subgraph_iterator class.
*/
class const_sequential_iterator {
private:
   /// pointer to the first operator in the player, BeginOp = *op_begin_
   const opcode_t*           op_begin_;

   /// pointer one past last operator in the player, EndOp = *(op_end_ - 1)
   const opcode_t*           op_end_;

   /// pointer to the first argument for the first operator
   const addr_t*             arg_begin_;

   /// pointer on past last argumemnt for last operator
   const addr_t*             arg_end_;

   /// pointer to current operator
   const opcode_t*           op_cur_;

   /// pointer to first argument for current operator
   const addr_t*             arg_;

   /// number of variables in tape (not const for assignment operator)
   size_t                    num_var_;

   /// index of last result for current operator
   size_t                    var_index_;

   /// value of current operator; i.e. op_ = *op_cur_
   op_code_var               op_;
public:
   /// default constructor
   const_sequential_iterator(void) :
   op_begin_(nullptr)  ,
   op_end_(nullptr)    ,
   arg_begin_(nullptr) ,
   arg_end_(nullptr)   ,
   op_cur_(nullptr)    ,
   arg_(nullptr)       ,
   num_var_(0)            ,
   var_index_(0)          ,
   op_(NumberOp)
   { }
   /// assignment operator
   void operator=(const const_sequential_iterator& rhs)
   {
      op_begin_  = rhs.op_begin_;
      op_end_    = rhs.op_end_;
      arg_begin_ = rhs.arg_begin_;
      arg_end_   = rhs.arg_end_;
      op_cur_    = rhs.op_cur_;
      arg_       = rhs.arg_;
      num_var_   = rhs.num_var_;
      var_index_ = rhs.var_index_;
      op_        = rhs.op_;
      return;
   }
   /*!
   Create a sequential iterator starting either at beginning or end of tape

   \param num_var
   is the number of variables in the tape.

   \param op_vec
   is the vector of operators on the tape.

   \param arg_vec
   is the vector of arguments for all the operators

   \param op_index
   is the operator index that iterator will start at.
   It must be zero or op_vec_->size() - 1.

   \par Assumptions
   - op_code_var(op_vec_[0]) == BeginOp
   - op_code_var(op_vec_[op_vec_->size() - 1]) == EndOp
   */
   const_sequential_iterator(
      size_t                                num_var    ,
      const pod_vector<opcode_t>*           op_vec     ,
      const pod_vector<addr_t>*             arg_vec    ,
      size_t                                op_index   )
   :
   op_begin_   ( op_vec->data() )                   ,
   op_end_     ( op_vec->data() + op_vec->size() )  ,
   arg_begin_  ( arg_vec->data() )                  ,
   arg_end_    ( arg_vec->data() + arg_vec->size() ),
   num_var_    ( num_var )
   {  if( op_index == 0 )
      {
         // index of last result for BeginOp
         var_index_ = 0;
         //
         // first argument to BeginOp
         arg_       = arg_vec->data();
         //
         // BeginOp
         op_cur_    = op_begin_;
         op_        = op_code_var( *op_cur_ );
         CPPAD_ASSERT_UNKNOWN( op_ == BeginOp );
         CPPAD_ASSERT_NARG_NRES(op_, 1, 1);
      }
      else
      {  CPPAD_ASSERT_UNKNOWN(op_index == op_vec->size()-1);
         //
         // index of last result for EndOp
         var_index_ = num_var - 1;
         //
         // first argument to EndOp (has no arguments)
         arg_ = arg_vec->data() + arg_vec->size();
         //
         // EndOp
         op_cur_    = op_end_ - 1;
         op_        = op_code_var( *op_cur_ );
         CPPAD_ASSERT_UNKNOWN( op_ == EndOp );
         CPPAD_ASSERT_NARG_NRES(op_, 0, 0);
      }
   }
   /*!
   Advance iterator to next operator
   */
   const_sequential_iterator& operator++(void)
   {
      // first argument for next operator
      arg_ += NumArg(op_);
      //
      // next operator
      ++op_cur_;
      op_ = op_code_var( *op_cur_ );
      //
      // last result for next operator
      var_index_ += NumRes(op_);
      //
      return *this;
   }
   /*!
   Correction applied before ++ operation when current operator
   is CSumOp, CSkipOp, or AFunOP.
   */
   void correct_before_increment(void)
   {  //
      // arg_
      // actual number of arguments for this operator depends on argument data
      CPPAD_ASSERT_UNKNOWN( NumArg(op_) == 0 );
      const addr_t* arg = arg_;
      //
      switch( op_ )
      {  //
         default:
         CPPAD_ASSERT_UNKNOWN( false );
         break;
         //
         // CSumOp
         case CSumOp:
         CPPAD_ASSERT_UNKNOWN( arg + 4 < arg_end_ );
         arg_ += arg[4] + 1;
         CPPAD_ASSERT_UNKNOWN( *(arg_ - 1) == arg[4] + 1 );
         break;
         //
         // CSkipOp
         case CSkipOp:
         CPPAD_ASSERT_UNKNOWN( arg + 5 < arg_end_ );
         arg_ += 7 + arg[4] + arg[5];
         CPPAD_ASSERT_UNKNOWN( *(arg_ - 1) == 7 + arg[4] + arg[5] );
         break;
      }
      CPPAD_ASSERT_UNKNOWN( arg_ <= arg_end_ );
      return;
   }
   /*!
   Backup iterator to previous operator
   */
   const_sequential_iterator& operator--(void)
   {  //
      // last result for next operator
      var_index_ -= NumRes(op_);
      //
      // next operator
      --op_cur_;
      op_ = op_code_var( *op_cur_ );
      //
      // first argument for next operator
      arg_ -= NumArg(op_);
      //
      return *this;
   }
   /*!
   Correction applied after -- operation when current operator
   is CSumOp or CSkipOp.

   \param arg [out]
   corrected point to arguments for this operation.
   */
   void correct_after_decrement(const addr_t*& arg)
   {  //
      // arg
      // actual number of arguments for this operator depends on argument data
      CPPAD_ASSERT_UNKNOWN( NumArg(op_) == 0 );
      CPPAD_ASSERT_UNKNOWN( arg_begin_ < arg_ );
      //
      switch( op_ )
      {  //
         default:
         CPPAD_ASSERT_UNKNOWN( false );
         break;
         //
         // CSumOp
         case CSumOp:
         {  // index of arg[4]
            addr_t n_arg = *(arg_ - 1);
            arg          = arg_ - n_arg;
            CPPAD_ASSERT_UNKNOWN( arg[4] + 1 == n_arg );
         }
         break;
         //
         // CSkipOp
         case CSkipOp:
         {  addr_t n_arg = *(arg_ - 1);
            arg          = arg_ - n_arg;
            CPPAD_ASSERT_UNKNOWN( 7 + arg[4] + arg[5] == n_arg );
         }
         break;
      }
      //
      // arg_
      CPPAD_ASSERT_UNKNOWN( arg_begin_ <= arg );
      CPPAD_ASSERT_UNKNOWN( arg + NumArg(op_) <= arg_end_ );
      arg_ = arg;
   }
   /*!
   \brief
   Get information corresponding to current operator.

   \param op [out]
   op code for this operator.

   \param arg [out]
   pointer to the first arguement to this operator.

   \param var_index [out]
   index of the last variable (primary variable) for this operator.
   If there is no primary variable for this operator, var_index
   is not sepcified and could have any value.
   */
   void op_info(
      op_code_var&   op         ,
      const addr_t*& arg        ,
      size_t&        var_index  ) const
   {  // op
      CPPAD_ASSERT_UNKNOWN( op_begin_ <= op_cur_ && op_cur_ < op_end_ )
      op        = op_;
      //
      // arg
      arg = arg_;
      CPPAD_ASSERT_UNKNOWN( arg_begin_ <= arg );
      CPPAD_ASSERT_UNKNOWN( arg + NumArg(op) <= arg_end_ );
      //
      // var_index
      CPPAD_ASSERT_UNKNOWN( var_index_ < num_var_ || NumRes(op) == 0 );
      var_index = var_index_;
   }
   /// current operator index
   size_t op_index(void)
   {  return size_t(op_cur_ - op_begin_); }
};

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
