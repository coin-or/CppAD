# ifndef CPPAD_CORE_AD_IO_HPP
# define CPPAD_CORE_AD_IO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ad_input}
{xrst_spell
   istream
}

AD Input Stream Operator
########################

Syntax
******
| *is* >> *x*

Purpose
*******
Sets *x* to a :ref:`glossary@Parameter`
with value *b* corresponding to

   *is* >> *b*

where *b* is a *Base* object.
It is assumed that this *Base* input operation returns
a reference to *is* .

is
**
The operand *is* has prototype

   ``std::istream&`` *is*

x
*
The operand *x* has one of the following prototypes

   ``AD`` < *Base* >& *x*

Result
******
The result of this operation can be used as a reference to *is* .
For example, if the operand *y* has prototype

   ``AD`` < *Base* > *y*

then the syntax

   *is* >> *x* >> *y*

will first read the *Base* value of *x* from *is* ,
and then read the *Base* value to *y* .

Operation Sequence
******************
The result of this operation is not an
:ref:`glossary@AD of Base` object.
Thus it will not be recorded as part of an
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Example
*******
{xrst_toc_hidden
   example/general/ad_input.cpp
}
The file
:ref:`ad_input.cpp-name`
contains an example and test of this operation.

{xrst_end ad_input}
------------------------------------------------------------------------------
{xrst_begin ad_output}
{xrst_spell
   ostream
}

AD Output Stream Operator
#########################

Syntax
******
*os* << *x*

See Also
********
:ref:`PrintFor-name`

Purpose
*******
Writes the *Base* value, corresponding to *x* ,
to the output stream *os* .

Assumption
**********
If *b* is a *Base* object,

   *os* << *b*

returns a reference to *os* .

os
**
The operand *os* has prototype

   ``std::ostream&`` *os*

x
*
The operand *x* has one of the following prototypes

| |tab| ``const AD`` < *Base* >& *x*
| |tab| ``const VecAD`` < *Base* >:: ``reference&`` *x*

Result
******
The result of this operation can be used as a reference to *os* .
For example, if the operand *y* has prototype

   ``AD`` < *Base* > *y*

then the syntax

   *os* << *x* << *y*

will output the value corresponding to *x*
followed by the value corresponding to *y* .

Operation Sequence
******************
The result of this operation is not an
:ref:`glossary@AD of Base` object.
Thus it will not be recorded as part of an
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Example
*******
{xrst_toc_hidden
   example/general/ad_output.cpp
}
The file
:ref:`ad_output.cpp-name`
contains an example and test of this operation.

{xrst_end ad_output}
------------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file ad_io.hpp
AD<Base> input and ouput stream operators.
*/
// ---------------------------------------------------------------------------
/*!
Read an AD<Base> object from an input stream.

\tparam Base
Base type for the AD object.

\param is [in,out]
Is the input stream from which that value is read.

\param x [out]
is the object that is being set to a value.
Upone return, x.value_ is read from the input stream
and x.tape_is_ is zero; i.e., x is a parameter.
*/
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
std::istream& operator >> (std::istream& is, AD<Base>& x)
{  // like assignment to a base type value
   x.tape_id_ = 0;
   CPPAD_ASSERT_UNKNOWN( Parameter(x) );
   return (is >> x.value_);
}
// ---------------------------------------------------------------------------
/*!
Write an AD<Base> object to an output stream.

\tparam Base
Base type for the AD object.

\param os [in,out]
Is the output stream to which that value is written.

\param x
is the object that is being written to the output stream.
This is equivalent to writing x.value_ to the output stream.
*/
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
std::ostream& operator << (std::ostream &os, const AD<Base> &x)
{  return (os << x.value_); }
// ---------------------------------------------------------------------------
/*!
Write a VecAD_reference<Base> object to an output stream.

\tparam Base
Base type for the VecAD_reference object.

\param os [in,out]
Is the output stream to which that value is written.

\param x
is the element of the VecAD object that is being written to the output stream.
This is equivalent to writing the corresponing Base value to the stream.
*/
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
std::ostream& operator << (std::ostream &os, const VecAD_reference<Base> &x)
{  return (os << x.ADBase()); }

} // END_CPPAD_NAMESPACE
# endif
