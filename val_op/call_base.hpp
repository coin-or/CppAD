# ifndef CALL_FUN_HPP
# define CALL_FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
# include "type.hpp"
/*
{xrst_begin val_op_call_base dev}
{xrst_spell
   xam
}

The Function Call Base Class
############################

Syntax
******
::

   template <class Value>
   class derived_t : public call_base_t<Value> { ...


Purpose
*******
This the base class for defining functions corresponding to the
:ref:`fun_op <val_op_fun-name>` .
The pure virtual members must be defined by the derived class.

function_id
***********
{xrst_literal
   // BEGIN_FUNCTION_ID
   // END_FUNCTION_ID
}
This returns the function identifier for a derived class object.

function_name
*************
{xrst_literal
   // BEGIN_FUNCTION_NAME
   // END_FUNCTION_NAME
}
This returns a name for the function
corresponding to the derived class object.

call_base_ptr
*************
{xrst_literal
   // BEGIN_CALL_BASE_PTR
   // END_CALL_BASE_PTR
}
This returns a pointer that can be used to access the
derived class object corresponding to the specified function_id .

forward
*******
{xrst_literal
   // BEGIN_FORWARD
   // END_FORWARD
}

call_id
=======
This can be used to identify
extra information used by the function call evaluation.

x
=
This is the argument vector for the function call.

y
=
This is the return vector for the function call.
The size of *y* is correct on input and should not change.
The input value of its elements are not specified.

rev_depend
**********
{xrst_literal
   // BEGIN_REV_DEPEND
   // END_REV_DEPEND
}
This routine performs reverse dependency analysis.

call_id
=======
This can be used to identify
extra information used by the function call evaluation.

depend_y
========
This has size equal to the number of results for the function call.
It the *i*-th component of this vector is true (false),
the results of interest depend (do not depend)
on the value of y[i].

depend_x
========
This has size equal to the number of arguments for the function call.
For each argument index *j*,
if there is an *i* such that,
depend_y[i] is true, and y[i] depends on x[j],
then depend_x[j] must  be set to true.
The size of *depend_x* is correct on input and should not change.
Its element values on input are not specified.

clear
*****
This is used to free memory that holds information about the
derived member objects.
The destructor for all the derived member objects must be called
before clear is called.

{xrst_comment fun_xam.cpp is included by fun_op.hpp}
Example
*******
The file :ref:`fun_xam.cpp <val_op_fun_xam.cpp-name>`
is an example and test that uses this class.

{xrst_end val_op_call_base}
*/
template <class Value>
class call_base_t {
private:
   inline static Vector<call_base_t*> call_base_vec_;
   inline static Vector<bool>        deleted_;
   const size_t                      function_id_;
public:
   // ctor
   call_base_t(void) :
   function_id_ ( call_base_vec_.size() )
   {  // 2DO: cannot be in parallel mode
      call_base_vec_.push_back( this );
      deleted_.push_back( false );
   }
   ~call_base_t(void)
   {  // 2DO: cannot be in parallel mode
      deleted_[function_id_] = true;
   }
   // BEGIN_FUNCTION_ID
   size_t function_id(void)
   // END_FUNCTION_ID
   {  return function_id_;
   }
   // BEGIN_FUNCTION_NAME
   virtual std::string function_name(void) const = 0;
   // END_FUNCTION_NAME
   //
   // BEGIN_CALL_BASE_PTR
   static call_base_t* call_base_ptr(size_t function_id)
   // END_CALL_BASE_PTR
   {  assert( ! deleted_[function_id] );
      return call_base_vec_[function_id];
   }
   // BEGIN_FORWARD
   virtual bool forward(
      size_t               call_id       ,
      const Vector<Value>& x             ,
      Vector<Value>&       y             ) const = 0;
   // END_FORWARD
   // BEGIN_REV_DEPEND
   virtual bool rev_depend(
      size_t              call_id        ,
      Vector<bool>&       depend_x       ,
      const Vector<bool>& depend_y       ) const = 0;
   // END_REV_DEPEND
   //
   // BEGIN_CLEAR
   static bool clear(void)
   // END_CLEAR
   {  for(size_t i = 0; i < deleted_.size(); ++i)
         if( ! deleted_[i] )
            return false;
      call_base_vec_.clear();
      deleted_.clear();
      return true;
   }
};

# endif
