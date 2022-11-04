# ifndef CPPAD_CORE_ABORT_RECORDING_HPP
# define CPPAD_CORE_ABORT_RECORDING_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin abort_recording$$
$spell
$$

$section Abort Recording of an Operation Sequence$$


$head Syntax$$
$codei%AD<%Base%>::abort_recording()%$$

$head Purpose$$
Sometimes it is necessary to abort the recording of an operation sequence
that started with a call of the form
$codei%
   Independent(%x%)
%$$
If such a recording is currently in progress,
$code abort_recording$$ will stop the recording and delete the
corresponding information.
Otherwise, $code abort_recording$$ has no effect.

$children%
   example/general/abort_recording.cpp
%$$
$head Example$$
The file
$cref abort_recording.cpp$$
contains an example and test of this operation.

$end
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
