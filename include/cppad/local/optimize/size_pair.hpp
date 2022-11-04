# ifndef CPPAD_LOCAL_OPTIMIZE_SIZE_PAIR_HPP
# define CPPAD_LOCAL_OPTIMIZE_SIZE_PAIR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*!
\file size_pair.hpp
Information for one variable and one operation sequence.
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {

/*!
\file size_pair.hpp
Information for one variable in one operation sequence.
*/
struct struct_size_pair {
   size_t i_op;  /// operator index for this variable
   size_t i_var; /// variable index for this variable
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
