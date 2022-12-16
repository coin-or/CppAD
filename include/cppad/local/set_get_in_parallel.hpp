# ifndef CPPAD_LOCAL_SET_GET_IN_PARALLEL_HPP
# define CPPAD_LOCAL_SET_GET_IN_PARALLEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cassert>
# include <cppad/configure.hpp>
namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE

/*!
\file set_get_in_parallel.hpp
File used to set and get user in_parallel routine.
*/
/*!
Set and call the routine that determine if we are in parallel execution mode.

\return
value retuned by most recent setting for in_parallel_new.
If set is true,
or the most recent setting is nullptr (its initial value),
the return value is false.
Otherwise the function corresponding to the most recent setting
is called and its value returned by set_get_in_parallel.

\param in_parallel_new [in]
If set is false, in_parallel_new it is not used.
Otherwise, the current value of in_parallel_new becomes the
most recent setting for in_parallel_user.

\param set
If set is true, then parallel_new is becomes the most
recent setting for this set_get_in_parallel.
In this case, it is assumed that we are currently in sequential execution mode.
*/
static bool set_get_in_parallel(
   bool (*in_parallel_new)(void) ,
   bool set = false           )
{  static bool (*in_parallel_user)(void) = nullptr;

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
