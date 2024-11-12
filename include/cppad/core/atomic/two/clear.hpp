# ifndef CPPAD_CORE_ATOMIC_TWO_CLEAR_HPP
# define CPPAD_CORE_ATOMIC_TWO_CLEAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_clear app}

Free Static Variables
#####################

Syntax
******
| ``atomic_base`` < *Base* >:: ``clear`` ()

Purpose
*******
Each ``atomic_base`` objects holds onto work space in order to
avoid repeated memory allocation calls and thereby increase speed
(until it is deleted).
If an the ``atomic_base`` object is global or static because,
the it does not get deleted.
This is a problem when using
``thread_alloc`` :ref:`free_all<ta_free_all-name>`
to check that all allocated memory has been freed.
Calling this ``clear`` function will free all the
memory currently being held onto by the
``atomic_base`` < *Base* > class.

Future Use
**********
If there is future use of an ``atomic_base`` object,
after a call to ``clear`` ,
the work space will be reallocated and held onto.

Restriction
***********
This routine cannot be called
while in :ref:`parallel<ta_in_parallel-name>` execution mode.

{xrst_end atomic_two_clear}
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_clear.hpp
Free static variables in atomic_base class.
*/
/*!
Free all thread_alloc static memory held by atomic_base (avoids reallocations).
(This does not include class_object() which is an std::vector.)
*/
template <class Base>
void atomic_base<Base>::clear(void)
{  CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "cannot use atomic_base clear during parallel execution"
   );
   bool         set_null = true;
   size_t       index  = 0;
   size_t       type  = 0;          // set to avoid warning
   std::string* name  = nullptr;
   void*        v_ptr = nullptr; // set to avoid warning
   size_t       n_atomic = local::atomic_index<Base>(
      set_null, index, type, name, v_ptr
   );
   //
   set_null = false;
   for(index = 1; index <= n_atomic; ++index)
   {  local::atomic_index<Base>(set_null, index, type, name, v_ptr);
      if( type == 2 )
      {  atomic_base* op = reinterpret_cast<atomic_base*>(v_ptr);
         if( op != nullptr )
         {  for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
               op->free_work(thread);
         }
      }
   }
   return;
}

} // END_CPPAD_NAMESPACE
# endif
