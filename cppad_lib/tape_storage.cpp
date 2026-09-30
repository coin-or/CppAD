// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Storage for the tape identifier table.

This non-template function provides a single definition point for the
per-thread tape ID storage, avoiding duplicate function-local statics
when CppAD templates are instantiated across multiple shared libraries.
*/
tape_id_t* tape_storage_id_ptr(size_t thread)
{  static tape_id_t table[CPPAD_MAX_NUM_THREADS] = {};
   return table + thread;
}

/*!
Storage for the tape handle table.

Uses void* to avoid depending on the template parameter Base.
The caller (tape_link.hpp) casts back to local::ADTape<Base>**.
*/
void** tape_storage_handle(size_t thread)
{  static void* table[CPPAD_MAX_NUM_THREADS] = {};
   return table + thread;
}

} // END_CPPAD_NAMESPACE
