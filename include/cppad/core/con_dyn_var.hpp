# ifndef CPPAD_CORE_CON_DYN_VAR_HPP
# define CPPAD_CORE_CON_DYN_VAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
---------------------------------------------------------------------------

{xrst_begin con_dyn_var}

Constant, Dynamic, Parameter, and Variable
##########################################

Syntax
******
| *b* = ``Constant`` ( *x* )
| *b* = ``Dynamic`` ( *x* )
| *b* = ``Parameter`` ( *x* )
| *b* = ``Variable`` ( *x* )

x
*
The argument *x* has prototype

| |tab| ``const AD`` < *Base* >    & *x*
| |tab| ``const VecAD`` < *Base* > & *x*

b
*
The return value *b* has prototype

   ``bool`` *b*

Constant
********
The return value for ``Constant`` is true
is true if and only if *x* is
a :ref:`glossary@Parameter@Constant` parameter.
A :ref:`VecAD\<Base><VecAD-name>` object is a constant parameter
if no element of the vector depends on the independent variables.

Dynamic
*******
The return value for ``Dynamic`` is true
is true if and only if *x* is
a :ref:`glossary@Parameter@Dynamic` parameter.
No element of a :ref:`VecAD\<Base><VecAD-name>` object
can depend on the dynamic parameters and this function returns false
for these objects.

Parameter
*********
The return value for ``Parameter`` is true
is true if and only if *x* is
a :ref:`glossary@Parameter` .
A :ref:`VecAD\<Base><VecAD-name>` object is a parameter
if no element of the vector depends on the independent variables.

Variable
********
The return value for ``Variable`` is true
is true if and only if *x* is
a :ref:`glossary@Variable` .
A :ref:`VecAD\<Base><VecAD-name>` object is a variable
if any element of the vector depends on the independent variables.

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
   example/general/con_dyn_var.cpp
}
The file
:ref:`con_dyn_var.cpp-name`
contains an example and test of these functions.

{xrst_end con_dyn_var}
-----------------------------------------------------------------------------
*/

namespace CppAD {
   // -----------------------------------------------------------------------
   // Constant
   template <class Base>
   bool Constant(const AD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( x.tape_id_ == 0 )
         return true;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ != *AD<Base>::tape_id_ptr(thread);
   }
   //
   template <class Base>
   bool Constant(const VecAD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( x.tape_id_ == 0 )
         return true;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ != *AD<Base>::tape_id_ptr(thread);
   }
   // -----------------------------------------------------------------------
   // Dynamic
   template <class Base>
   bool Dynamic(const AD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( (x.tape_id_ == 0) || (x.ad_type_ != dynamic_enum) )
         return false;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ == *AD<Base>::tape_id_ptr(thread);
   }
   //
   template <class Base>
   bool Dynamic(const VecAD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( (x.tape_id_ == 0) || (x.ad_type_ != dynamic_enum) )
         return false;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ == *AD<Base>::tape_id_ptr(thread);
   }
   // -----------------------------------------------------------------------
   // Parameter
   template <class Base>
   bool Parameter(const AD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( (x.tape_id_ == 0) || (x.ad_type_ == dynamic_enum) )
         return true;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ != *AD<Base>::tape_id_ptr(thread);
   }
   //
   template <class Base>
   bool Parameter(const VecAD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( (x.tape_id_ == 0) || (x.ad_type_ == dynamic_enum) )
         return true;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ != *AD<Base>::tape_id_ptr(thread);
   }
   // -----------------------------------------------------------------------
   // Variable
   template <class Base>
   bool Variable(const AD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( (x.tape_id_ == 0) || (x.ad_type_ != variable_enum) )
         return false;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ == *AD<Base>::tape_id_ptr(thread);
   }
   //
   template <class Base>
   bool Variable(const VecAD<Base> &x)
   {  CPPAD_ASSERT_AD_TYPE( x );
      if( (x.tape_id_ == 0) || (x.ad_type_ != variable_enum) )
         return false;
      //
      size_t thread = size_t(x.tape_id_ % CPPAD_MAX_NUM_THREADS);
      return x.tape_id_ == *AD<Base>::tape_id_ptr(thread);
   }
}
// END CppAD namespace


# endif
