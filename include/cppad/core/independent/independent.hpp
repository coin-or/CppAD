# ifndef CPPAD_CORE_INDEPENDENT_INDEPENDENT_HPP
# define CPPAD_CORE_INDEPENDENT_INDEPENDENT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/independent.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*
{xrst_begin independent_all dev}

Independent: All Arguments Present
##################################

Purpose
*******
This implements :ref:`Independent-name` with all the possible arguments present.

Syntax
******
| ``Independent`` ( *x* , *abort_op_index* , *record_compare* , *dynamic* )

Prototype
*********
{xrst_literal
   // BEGIN_ALL_ARGUMENT
   // END_ALL_ARGUMENT
}

Base
****
The base type the recording started by this operation.

ADVector
********
is simple vector type with elements of type ``AD`` < *Base* > .

x
*
is the vector of the independent variables.

abort_op_index
**************
operator index at which execution will be aborted (during  the recording
of operations). The value zero corresponds to not aborting (will not match).

record_compare
**************
should comparison operators be recorded.

dynamic
*******
is the independent dynamic parameter vector.

{xrst_end independent_all}
*/
// BEGIN_ALL_ARGUMENT
template <class ADVector>
void Independent(
   ADVector&  x              ,
   size_t     abort_op_index ,
   bool       record_compare ,
   ADVector&  dynamic        )
// END_ALL_ARGUMENT
{  CPPAD_ASSERT_KNOWN(
      abort_op_index == 0 || record_compare,
      "Independent: abort_op_index is non-zero and record_compare is false."
   );
   typedef typename ADVector::value_type ADBase;
   typedef typename ADBase::value_type   Base;
   CPPAD_ASSERT_KNOWN(
      ADBase::tape_ptr() == nullptr,
      "Independent: cannot create a new tape because\n"
      "a previous tape is still active (for this thread).\n"
      "AD<Base>::abort_recording() would abort this previous recording."
   );
   local::ADTape<Base>* tape = ADBase::tape_manage(new_tape_manage);
   tape->Independent(x, abort_op_index, record_compare, dynamic);
}
/*
----------------------------------------------------------------------------
{xrst_begin independent_x_abort_record dev}

Independent: Default For dynamic
################################

Purpose
*******
This implements :ref:`Independent-name` using
the default for the dynamic argument.

Syntax
******
``Independent`` ( *x* , *abort_op_index* , *record_compare* )

Prototype
*********
{xrst_literal
   // BEGIN_THREE_ARGUMENT
   // END_THREE_ARGUMENT
}

Base
****
The base type the recording started by this operation.

ADVector
********
is simple vector type with elements of type ``AD`` < *Base* > .

x
*
is the vector of the independent variables.

abort_op_index
**************
operator index at which execution will be aborted (during  the recording
of operations). The value zero corresponds to not aborting (will not match).

record_compare
**************
should comparison operators be recorded.

{xrst_end independent_x_abort_record}
*/
// BEGIN_THREE_ARGUMENT
template <class ADVector>
void Independent(ADVector &x, size_t abort_op_index, bool record_compare)
// END_THREE_ARGUMENT
{  ADVector dynamic(0); // empty vector
   Independent(x, abort_op_index, record_compare, dynamic);
}
/*
------------------------------------------------------------------------------
{xrst_begin independent_x_abort_op_index dev}

Independent: Default For record_compare, dynamic
################################################

Purpose
*******
This implements :ref:`Independent-name` using
the default for the record_compare and dynamic arguments.

Syntax
******
``Independent`` ( *x* , *abort_op_index* )

Prototype
*********
{xrst_literal
   // BEGIN_X_ABORT_OP_INDEX
   // END_X_ABORT_OP_INDEX
}

Base
****
The base type the recording started by this operation.

ADVector
********
is simple vector type with elements of type ``AD`` < *Base* > .

x
*
is the vector of the independent variables.

abort_op_index
**************
operator index at which execution will be aborted (during  the recording
of operations). The value zero corresponds to not aborting (will not match).

{xrst_end independent_x_abort_op_index}
*/
// BEGIN_X_ABORT_OP_INDEX
template <class ADVector>
void Independent(ADVector &x, size_t abort_op_index)
// END_X_ABORT_OP_INDEX
{  bool     record_compare = true;
   ADVector dynamic(0); // empty vector
   Independent(x, abort_op_index, record_compare, dynamic);
}
/*
------------------------------------------------------------------------------
{xrst_begin independent_x_dynamic dev}

Independent: Default For abort_op_index, record_compare
#######################################################

Purpose
*******
This implements :ref:`Independent-name` using
the default for the abort_op_index and record_compare.

Syntax
******
``Independent`` ( *x* , *dynamic* )

Prototype
*********
{xrst_literal
   // BEGIN_X_DYNAMIC
   // END_X_DYNAMIC
}

Base
****
The base type the recording started by this operation.

ADVector
********
is simple vector type with elements of type ``AD`` < *Base* > .

x
*
is the vector of the independent variables.

dynamic
*******
is the independent dynamic parameter vector.

{xrst_end independent_x_dynamic}
*/
// BEGIN_X_DYNAMIC
template <class ADVector>
void Independent(ADVector& x, ADVector& dynamic)
// END_X_DYNAMIC
{  size_t   abort_op_index = 0;
   bool     record_compare = true;
   Independent(x, abort_op_index, record_compare, dynamic);
}
/*
------------------------------------------------------------------------------
{xrst_begin independent_x dev}

Independent: Default For abort_op_index, record_compare, dynamic
################################################################

Purpose
*******
This implements :ref:`Independent-name` using
the default for the abort_op_index, record_compare and dynamic arguments.

Syntax
******
``Independent`` ( *x* )

Prototype
*********
{xrst_literal
   // BEGIN_ONE_ARGUMENT
   // END_ONE_ARGUMENT
}

Base
****
The base type the recording started by this operation.

ADVector
********
is simple vector type with elements of type ``AD`` < *Base* > .

x
*
is the vector of the independent variables.

{xrst_end independent_x}
*/
// BEGIN_ONE_ARGUMENT
template <class ADVector>
void Independent(ADVector &x)
// END_ONE_ARGUMENT
{  size_t   abort_op_index = 0;
   bool     record_compare = true;
   ADVector dynamic(0); // empty vector
   Independent(x, abort_op_index, record_compare, dynamic);
}

} // END_CPPAD_NAMESPACE
# endif
