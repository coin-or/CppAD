// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>
# include <cppad/local/base_list.hpp>

namespace CppAD {
    //
    // tape_id_ptr
    # define CPPAD_BASE_LIST_CASE(base) \
        template tape_id_t* AD<base>::tape_id_ptr(size_t thread);
    CPPAD_BASE_LIST
    # undef CPPAD_BASE_LIST_CASE
}
