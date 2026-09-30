// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>

namespace CppAD {
    //
    // defined in tape_link.hpp
    template tape_id_t* AD<float>::tape_id_ptr(size_t thread);
    template tape_id_t* AD<double>::tape_id_ptr(size_t thread);
}
