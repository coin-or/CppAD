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
template <class Value>
class tape_t {
public:
   // op_info_t
   typedef struct info {
      addr_t      arg_index;
      addr_t      res_index;
      op_base_t<Value>* op_ptr;
   } op_info_t;
private :
   size_t                n_ind_;     // number of independent values
   size_t                n_val_;     // index in val_vec of record result
   Vector<addr_t>        arg_vec_;   // index of operator arguments in val_vec
   Vector<Value>         con_vec_;   // constants used by the tape
   Vector<op_info_t>     op_vec_;    // operators that define this function
   Vector<addr_t>        dep_vec_;   // index in val_vec of dependent variables
   //
public :
   virtual std::string function_name( size_t function_id )
   {  return "";
   }
   virtual bool forward(
      size_t              function_id    ,
      const Vector<Value>& x             ,
      Vector<Value>&      y              )
   {  return false;
   }
   virtual bool rev_depend(
      size_t              function_id    ,
      Vector<bool>&       depend_x       ,
      const Vector<bool>& depend_y       )
   {  return false;
   }
   // ------------------------------------------------------------------------
   // n_val
   size_t n_val(void) const
   {  return n_val_; }
   //
   // arg_vec
   const Vector<addr_t>& arg_vec(void) const
   {  return arg_vec_; }
   //
   // con_vec
   const Vector<Value>& con_vec(void) const
   {  return con_vec_; }
   //
   // op_vec
   const Vector<op_info_t>& op_vec(void) const
   {  return op_vec_; }
   //
   // dep_vec
   const Vector<addr_t>& dep_vec(void) const
   {  return dep_vec_; }
   // ------------------------------------------------------------------------
   //
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
   void set_ind(size_t n_ind)
   {  n_ind_ = n_ind;
      n_val_ = n_ind;
      dep_vec_.resize(0);
      op_vec_.resize(0);
      con_vec_.resize(0);
# ifndef NDEBUG
      size_t zero = size_t( record_con_op(Value(0.0)) );
      assert ( zero == n_ind_ );
# else
      record_con_op( Value(0.0) );
# endif
      assert( n_val_ == n_ind + 1 );
   }
   //
   // set_dep
   void set_dep(const Vector<addr_t>& dep_vec)
   {  dep_vec_ = dep_vec; }
   //
   // record_op
   addr_t record_op(op_enum_t op_enum, const Vector<addr_t>& op_arg);
   // record_con_op
   addr_t record_con_op(const Value& constant);
   // record_fun_op
   addr_t record_fun_op(
      size_t function_id            ,
      size_t n_res                  ,
      const Vector<addr_t>& fun_arg
   );
   // eval_fun_op
   void eval_fun_op(
      bool          trace   ,
      size_t        i_op    ,
      Vector<Value>& val_vec
   ) const;
   //
   // eval
   void eval(
      bool          trace  ,
      Vector<Value>& val_vec) const
   {  assert( val_vec.size() == static_cast<size_t>(n_val_) );
      //
      // trace
      if( trace )
      {  // no operators for independent variables
         std::printf("independent values\n");
         for(size_t res_index = 0; res_index < n_ind_; ++res_index)
         {  Value res = val_vec[res_index];
            std::printf(
               "%5ld %10.3g\n", res_index, res
            );
         }
      }
      //
      // i_op
      size_t n_op = op_vec_.size();
      for(size_t i_op = 0; i_op < n_op; ++i_op)
      {  //
         // op_info
         const op_info_t& op_info = op_vec_[i_op];
         if( op_info.op_ptr->op_enum() == fun_op_enum )
         {  //
            // tape_t::eval_fun_op
            eval_fun_op(trace, i_op, val_vec);
         }
         else
         {  // op_ptr, arg_index, res_index
            op_base_t<Value>* op_ptr      = op_info.op_ptr;
            addr_t            arg_index   = op_info.arg_index;
            addr_t            res_index   = op_info.res_index;
            //
            // op_base_t<Value>::eval
            op_ptr->eval(
               trace, arg_index, arg_vec_, con_vec_, res_index, val_vec
            );
         }
      }
   }
   //
   // renumber
   void renumber(void);
   //
   // dead_code
   void dead_code(void);
};

# include "record_op.hpp"
# include "renumber.hpp"
# include "dead_code.hpp"
# include "fun_op.hpp"

# endif
