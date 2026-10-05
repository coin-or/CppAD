# ifndef CPPAD_UTILITY_MEMORY_LEAK_HPP
# define CPPAD_UTILITY_MEMORY_LEAK_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/define.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_MEMORY_LEAK
extern bool CPPAD_LIB_EXPORT memory_leak(size_t add_static = 0);
// END_MEMORY_LEAK

} // END_CPPAD_NAMESPACE
# endif
