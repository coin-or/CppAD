// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>
# include <cppad/local/static_list.hpp>

namespace CppAD {
    //
    // tape_id_ptr
    # define CPPAD_STATIC_LIST_CASE(base) \
        template tape_id_t* AD<base>::tape_id_ptr(size_t thread);
    CPPAD_STATIC_LIST
    # undef CPPAD_STATIC_LIST_CASE
    //
    // tape_handle
    # define CPPAD_STATIC_LIST_CASE(base) \
        template local::ADTape<base>** AD<base>::tape_handle(size_t thread);
    CPPAD_STATIC_LIST
    # undef CPPAD_STATIC_LIST_CASE
}
