# ifndef CPPAD_LOCAL_STD_SET_HPP
# define CPPAD_LOCAL_STD_SET_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/define.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file std_set.hpp
Two constant standard sets (currently used for concept checking).
*/

/*!
A standard set with one element.
*/
template <class Scalar>
const std::set<Scalar>& one_element_std_set(void)
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
   static std::set<Scalar> one;
   if( one.empty() )
      one.insert(1);
   return one;
}
/*!
A standard set with a two elements.
*/
template <class Scalar>
const std::set<Scalar>& two_element_std_set(void)
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
   static std::set<Scalar> two;
   if( two.empty() )
   {  two.insert(1);
      two.insert(2);
   }
   return two;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
