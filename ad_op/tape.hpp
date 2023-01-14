# ifndef TAPE_HPP
# define TAPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "type.hpp"
# include "add_op.hpp"
# include "con_op.hpp"
# include "sub_op.hpp"

// tape_t
template <class Base>
class tape_t {
public:
   // op_info_t
   typedef struct info {
      addr_t      arg_index;
      addr_t      res_index;
      op_t<Base>* op_ptr;
   } op_info_t;
private :
   addr_t                n_ind_;     // number of independent values
   addr_t                n_val_;     // index in val_vec of next result
   Vector<addr_t>        arg_vec_;   // index of operator arguments in val_vec
   Vector<Base>          con_vec_;   // constants used by the tape
   Vector<op_info_t>     op_vec_;    // operators that define this function
   Vector<addr_t>        dep_vec_;   // index in val_vec of dependent variables
public :
   void swap(tape_t& other)
   {  std::swap( n_ind_, other.n_ind_ );
      std::swap( n_val_, other.n_val_);
      arg_vec_.swap( other.arg_vec_ );
      con_vec_.swap( other.con_vec_ );
      op_vec_.swap( other.op_vec_ );
      dep_vec_.swap( other.dep_vec_ );
   }
   //
   // set_ind
   void set_ind(addr_t n_ind)
   {  n_ind_ = n_ind;
      n_val_ = n_ind;
      op_vec_.resize(0);
      con_vec_.resize(0);
   }
   //
   // set_dep
   void set_dep(const Vector<addr_t>& dep_vec)
   {  dep_vec_ = dep_vec; }
   //
   // get_dep
   const Vector<addr_t>& get_dep(void)
   {  return dep_vec_; }
   //
   // next_op
   addr_t next_op(op_enum_t op_enum, const Vector<addr_t>& op_arg)
   {  //
      // n_val
      addr_t n_val = n_val_;
      //
      // n_arg
      addr_t n_arg = static_cast<addr_t>( arg_vec_.size() );
      //
      // op_ptr
      op_t<Base>* op_ptr = nullptr;
      switch(op_enum)
      {
         case add_op_enum:
         op_ptr = get_add_op_instance<Base>();
         break;

         case sub_op_enum:
         op_ptr = get_sub_op_instance<Base>();
         break;

         default:
         assert( false );
      }
      //
      // op_vec_
      op_info_t op_info = {n_arg, n_val, op_ptr};
      op_vec_.push_back(op_info);
      //
      // arg_vec_
      addr_t n_op_arg = op_ptr->n_arg();
      for(addr_t i = 0; i < n_op_arg; ++i)
         arg_vec_.push_back( op_arg[i] );
      //
      // n_val_
      n_val_ = n_val + op_ptr->n_res();
      //
      return n_val;
   }
   //
   // next_con_op
   addr_t next_con_op(const Base& constant)
   {  //
      // n_con
      addr_t n_con = static_cast<addr_t>( con_vec_.size() );
      con_vec_.push_back( constant );
      //
      // n_val
      addr_t n_val = n_val_;
      //
      // n_arg
      addr_t n_arg = static_cast<addr_t>( arg_vec_.size() );
      //
      // op_ptr
      op_t<Base>* op_ptr = get_con_op_instance<Base>();
      //
      // op_vec_
      op_info_t op_info = {n_arg, n_val, op_ptr};
      op_vec_.push_back(op_info);
      //
      // arg_vec_
      arg_vec_.push_back( n_con );
      //
      // n_val_
      n_val_ = n_val + op_ptr->n_res();
      //
      return n_val;
   }
   //
   // n_val
   addr_t n_val(void) const
   {  return n_val_; }
   //
   // eval
   void eval(
      bool          trace  ,
      Vector<Base>& val_vec) const
   {  assert( val_vec.size() == static_cast<size_t>(n_val_) );
      size_t n_op = op_vec_.size();
      for(size_t i = 0; i < n_op; ++i)
      {  const op_info_t& op_info = op_vec_[i];
         op_t<Base>* op_ptr       = op_info.op_ptr;
         addr_t      arg_index    = op_info.arg_index;
         addr_t      res_index    = op_info.res_index;
         op_ptr->eval(
            trace, arg_index, arg_vec_, con_vec_, res_index, val_vec
         );
      }
   }
   //
   // renumber
   void renumber(void);
   //
   // dead_code
   void dead_code(void);
};

# include "renumber.hpp"
# include "dead_code.hpp"

# endif
