# ifndef CPPAD_LOCAL_SET_GET_IN_PARALLEL_HPP
# define CPPAD_LOCAL_SET_GET_IN_PARALLEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cassert>
namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
{xrst_begin set_get_in_parallel dev}
{xrst_spell
   nullptr
}

Set in_parallel Routine or Get In Parallel Mode
###############################################
Set and call the routine that determine if we are in parallel execution mode.

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

set
===
If *set* is true, set are setting the current in_parallel routine.
In this case we must be in sequential execution mode; i.e., not parallel.
If *set* is false,
we are getting the result for the current in_parallel routine.

in_parallel_new
***************
#. If *set* is true, *in_parallel_new* becomes the
   most recent setting for the user's in_parallel routine.
   The nullptr in_parallel routine will always return false.
#. If *set* is false, the value of *in_parallel_new* does not matter.

flag
****
#. If *set* is false, the return value *flag* is the current value
   for the current in_parallel routine. (It is false if the current
   in_parallel routine is the nullptr.)
#. If *set* is true, the return value *flag* is unspecified.

{xrst_end set_get_in_parallel}
*/
// BEGIN PROTOTYPE
// flag = CppAD::local::set_get_inparallel( .. )
inline bool set_get_in_parallel(
   bool set                      = false   ,
   bool (*in_parallel_new)(void) = nullptr )
// END PROTOTYPE
{  typedef bool (*function_ptr)(void);
   static function_ptr in_parallel_user = nullptr;

   if( set )
   {  in_parallel_user = in_parallel_new;
      // Doing a raw assert in this case because set_get_in_parallel is used
      // by ErrorHandler and hence cannot use ErrorHandler.
      // CPPAD_ASSERT_UNKNOWN( in_parallel_user() == false )
      assert(in_parallel_user == nullptr || in_parallel_user() == false);
      return false;
   }
   //
   if( in_parallel_user == nullptr )
      return false;
   //
   return in_parallel_user();
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
