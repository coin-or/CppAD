# ifndef CALL_FUN_HPP
# define CALL_FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
# include "type.hpp"

template <class Base>
class call_fun_t {
private:
   inline static Vector<call_fun_t*> call_fun_vec_;
   inline static Vector<bool>        deleted_;
   const size_t                      function_id_;
public:
   // call_fun_ptr
   static call_fun_t* call_fun_ptr(size_t function_id)
   {  assert( ! deleted_[function_id] );
      return call_fun_vec_[function_id];
   }
   // ctor
   call_fun_t(void) :
   function_id_ ( call_fun_vec_.size() )
   {  // 2DO: cannot be in parallel mode
      call_fun_vec_.push_back( this );
      deleted_.push_back( false );
   }
   ~call_fun_t(void)
   {  // 2DO: cannot be in parallel mode
      deleted_[function_id_] = true;
   }
   // function_id
   size_t function_id(void)
   {  return function_id_;
   }
   // function_name
   virtual std::string function_name(void) const = 0;
   //
   // forward
   virtual bool forward(
      const Vector<Base>& x              ,
      Vector<Base>&       y              ) const = 0;
   //
   // rev_depend
   virtual bool rev_depend(
      Vector<bool>&       depend_x       ,
      const Vector<bool>& depend_y       ) const = 0;
};

# endif
