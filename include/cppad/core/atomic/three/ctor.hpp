# ifndef CPPAD_CORE_ATOMIC_THREE_CTOR_HPP
# define CPPAD_CORE_ATOMIC_THREE_CTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_ctor}

Atomic Function Constructor
###########################

Syntax
******
| ``class`` *atomic_user* : ``public CppAD::atomic_three<`` *Base* > {
| ``public:``
| |tab| *atomic_user* ( *ctor_arg_list* ) : ``CppAD::atomic_three<`` *Base* >( *name* )
| |tab| ...
| };
| *atomic_user afun* ( *ctor_arg_list* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

atomic_user
***********

ctor_arg_list
=============
Is a list of arguments for the *atomic_user* constructor.

afun
====
The object *afun* must stay in scope for as long
as the corresponding atomic function is used.
This includes use by any :ref:`ADFun\<Base><ADFun-name>` that
has this *atomic_user* operation in its
:ref:`operation sequence<glossary@Operation@Sequence>` .

Implementation
==============
The user defined *atomic_user* class is a publicly derived class of
``atomic_three`` < *Base* > .
It should be declared as follows:

| |tab| ``class`` *atomic_user* : ``public CppAD::atomic_three<`` *Base* > {
| |tab| ``public:``
| |tab| |tab| *atomic_user* ( *ctor_arg_list* ) : ``atomic_three`` < *Base* >( *name* )
| |tab| ...
| |tab| };

where ...
denotes the rest of the implementation of the derived class.
This includes completing the constructor and
all the virtual functions that have their
``atomic_three`` implementations replaced by
*atomic_user* implementations.

atomic_three
************

Restrictions
============
The ``atomic_three`` constructor and destructor cannot be called in
:ref:`parallel<ta_in_parallel-name>` mode.

Base
====
The template parameter determines the
:ref:`atomic_three_afun@Base`
type for this ``AD`` < *Base* > atomic operation.

name
====
This ``atomic_three`` constructor argument has the following prototype

   ``const std::string&`` *name*

It is the name for this atomic function and is used for error reporting.
The suggested value for *name* is *afun* or *atomic_user* ,
i.e., the name of the corresponding atomic object or class.

Example
*******

Define Constructor
==================
The following is an example of a atomic function constructor definition:
:ref:`get_started.cpp<atomic_three_get_started.cpp@Constructor>` .

Use Constructor
===============
The following is an example using a atomic function constructor:
:ref:`get_started.cpp<atomic_three_get_started.cpp@Use Atomic Function@Constructor>` .

{xrst_end atomic_three_ctor}
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_ctor.hpp
Constructors for atomic_three class.
*/

/*!
Base class for atomic_atomic functions.

\tparam Base
This class is used for defining an AD<Base> atomic operation y = g(x).

\par
make sure user does not invoke the default constructor
*/
template <class Base>
atomic_three<Base>::atomic_three(void)
{  CPPAD_ASSERT_KNOWN(false,
      "Attempt to use the atomic_three default constructor"
   );
}
/*!
Constructor

\param name
name used for error reporting
*/
// BEGIN_PROTOTYPE
template <class Base>
atomic_three<Base>::atomic_three(const std::string& name )
// END_PROTOTYPE
{  CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "atomic_three: constructor cannot be called in parallel mode."
   );
   //
   // atomic_index
   bool        set_null  = false;
   size_t      index     = 0;
   size_t      type      = 3;
   std::string copy_name = name;
   void*       copy_this = reinterpret_cast<void*>( this );
   index_  = local::atomic_index<Base>(
      set_null, index, type, &copy_name, copy_this
   );
   // initialize work pointers as null;
   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
      work_[thread] = nullptr;
}

} // END_CPPAD_NAMESPACE
# endif
