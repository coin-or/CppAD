# ifndef CPPAD_LOCAL_OPTIMIZE_USAGE_HPP
# define CPPAD_LOCAL_OPTIMIZE_USAGE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/define.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

typedef CPPAD_VEC_ENUM_TYPE usage_t;

enum enum_usage {
   /// This operator is not used.
   no_usage,

   /// This operator is used one or more times.
   yes_usage,

   /*!
   This operator is only used once, it is a summation operator,
   and its parrent is a summation operator. Furthermore, its result is not
   a dependent variable. Hence case it can be removed as part of a
   cumulative summation starting at its parent or above.
   */
   csum_usage
};


} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
# endif
