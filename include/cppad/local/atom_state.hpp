# ifndef CPPAD_LOCAL_ATOM_STATE_HPP
# define CPPAD_LOCAL_ATOM_STATE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE

enum enum_atom_state {
   /// next AFunOp marks beginning of a atomic function call
   start_atom,

   /// next FunapOp (FunavOp) is a parameter (variable) argument
   arg_atom,

   /// next FunrpOp (FunrvOp) is a parameter (variable) result
   ret_atom,

   /// next AFunOp marks end of a atomic function call
   end_atom
};

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
