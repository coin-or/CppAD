# ifndef CPPAD_CORE_CHECK_FOR_NAN_HPP
# define CPPAD_CORE_CHECK_FOR_NAN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin check_for_nan}
{xrst_spell
   newline
}
Check an ADFun Object For Nan Results
#####################################

Syntax
******
| *f* . ``check_for_nan`` ( *b* )
| *b* = *f* . ``check_for_nan`` ()
| ``get_check_for_nan`` ( *vec* , *file* )

Debugging
*********
If ``NDEBUG`` is not defined, and
the result of a :ref:`forward<forward_order-name>` or :ref:`reverse<reverse_any-name>`
calculation contains a  :ref:`nan-name` ,
CppAD can halt with an error message.

f
*
For the syntax where *b* is an argument,
*f* has prototype

   ``ADFun`` < *Base* > *f*

(see ``ADFun`` < *Base* > :ref:`constructor<fun_construct-name>` ).
For the syntax where *b* is the result,
*f* has prototype

   ``const ADFun`` < *Base* > *f*

b
*
This argument or result has prototype

   ``bool`` *b*

If *b* is true (false),
future calls to *f* . ``Forward`` will (will not) check for ``nan`` .

Default
*******
The value for this setting after construction of *f* is true.
The value of this setting is not affected by calling
:ref:`Dependent-name` for this function object.

Error Message
*************
If this error is detected during zero order forward mode,
the values of the independent variables that resulted in the ``nan``
are written to a temporary binary file.
This is so that you can run the original source code with those values
to see what is causing the ``nan`` .

vector_size
===========
The error message with contain the text
``vector_size`` = *vector_size* followed the newline character
``'\n'`` .
The value of *vector_size* is the number of elements
in the independent vector.

file_name
=========
The error message with contain the text
``file_name`` = *file_name* followed the newline character
``'\n'`` .
The value of *file_name* is the name of the temporary file
that contains the dependent variable values.

index
=====
The error message will contain the text
``index`` = *index* followed by the newline character ``'\n'`` .
The value of *index* is the lowest dependent variable index
that has the value ``nan`` .

get_check_for_nan
*****************
This routine can be used to get the independent variable
values that result in a ``nan`` .

vec
===
This argument has prototype

   ``CppAD::vector<`` *Base* >& *vec*

It size must be equal to the corresponding value of
:ref:`check_for_nan@Error Message@vector_size`
in the corresponding error message.
The input value of its elements does not matter.
Upon return, it will contain the values for the independent variables,
in the corresponding call to :ref:`Independent-name` ,
that resulted in the ``nan`` .
(Note that the call to ``Independent`` uses an vector with elements
of type ``AD`` < *Base* > and *vec* has elements of type
*Base* .)

file
====
This argument has prototype

   ``const std::string&`` *file*

It must be the value of
:ref:`check_for_nan@Error Message@file_name`
in the corresponding error message.

Example
*******
{xrst_toc_hidden
   example/general/check_for_nan.cpp
}
The file
:ref:`check_for_nan.cpp-name`
contains an example and test of these operations.

{xrst_end check_for_nan}
*/

# include <fstream>
# include <cppad/utility/vector.hpp>
# include <cppad/local/temp_file.hpp>


namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Set check_for_nan

\param value
new value for this flag.
*/
template <class Base, class RecBase>
void ADFun<Base,RecBase>::check_for_nan(bool value)
{  check_for_nan_ = value; }

/*!
Get check_for_nan

\return
current value of check_for_nan_.
*/
template <class Base, class RecBase>
bool ADFun<Base,RecBase>::check_for_nan(void) const
{  return check_for_nan_; }

/*!
Stores a vector in a file when nans occur.

\param vec [in]
is the vector that is stored.

\param [out] file_name
is the file where the vector is stored
*/
template <class Base>
void put_check_for_nan(const CppAD::vector<Base>& vec, std::string& file_name)
{
   // char_size
   std::streamsize char_size = std::streamsize( sizeof(Base) * vec.size() );
   //
   // char_ptr
   const char* char_ptr   = reinterpret_cast<const char*>( vec.data() );
   //
   // file_name
   file_name = local::temp_file();
   //
   // write data to file_name
   std::fstream file_out(file_name.c_str(), std::ios::out|std::ios::binary );
   file_out.write(char_ptr, char_size);
   file_out.close();
   //
   return;
}
/*!
Gets a vector that was stored by put_check_for_nan.

\param vec [out]
is the vector that is stored.

\param file_name [in]
is the file where the vector is stored
*/
template <class Base>
void get_check_for_nan(CppAD::vector<Base>& vec, const std::string& file_name)
{  //
   std::streamsize char_size = std::streamsize( sizeof(Base) * vec.size() );
   char* char_ptr   = reinterpret_cast<char*>( vec.data() );
   //
   std::fstream file_in(file_name.c_str(), std::ios::in|std::ios::binary );
   file_in.read(char_ptr, char_size);
   //
   return;
}

} // END_CPPAD_NAMESPACE

# endif
