# ifndef CPPAD_CORE_ABORT_RECORDING_HPP
# define CPPAD_CORE_ABORT_RECORDING_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin abort_recording}

Abort Recording of an Operation Sequence
########################################

Syntax
******
| ``AD`` < *Base* >:: ``abort_recording`` ()

Purpose
*******
Sometimes it is necessary to abort the recording of an operation sequence
that started with a call of the form

   ``Independent`` ( *x* )

If such a recording is currently in progress,
``abort_recording`` will stop the recording and delete the
corresponding information.
Otherwise, ``abort_recording`` has no effect.
{xrst_toc_hidden
   example/general/abort_recording.cpp
}
Example
*******
The file
:ref:`abort_recording.cpp-name`
contains an example and test of this operation.

{xrst_end abort_recording}
----------------------------------------------------------------------------
*/


namespace CppAD {
   template <class Base>
   void AD<Base>::abort_recording(void)
   {  local::ADTape<Base>* tape = AD<Base>::tape_ptr();
      if( tape != nullptr )
         AD<Base>::tape_manage(delete_tape_manage);
   }
}

# endif
