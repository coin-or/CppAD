# ifndef CPPAD_CORE_ATOMIC_FOUR_CTOR_HPP
# define CPPAD_CORE_ATOMIC_FOUR_CTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_ctor}

Atomic Function Constructor
###########################

Syntax
******
| ``class`` *atomic_user* : ``public CppAD::atomic_four<`` *Base* > {
| ``public:``
| |tab| *atomic_user* ( *ctor_arg_list* ) : ``CppAD::atomic_four<`` *Base* >( *name* )
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
This includes use by any :ref:`ADFun\<Base><ADFun-name>` object that
has this *atomic_user* operation in its
:ref:`operation sequence<glossary@Operation@Sequence>` .

Implementation
==============
The user defined *atomic_user* class is a publicly derived class of
``atomic_four`` < *Base* > .
It should be declared as follows:

| |tab| ``class`` *atomic_user* : ``public CppAD::atomic_four<`` *Base* > {
| |tab| ``public:``
| |tab| |tab| *atomic_user* ( *ctor_arg_list* ) : ``atomic_four`` < *Base* >( *name* )
| |tab| ...
| |tab| };

where ...
denotes the rest of the implementation of the derived class.
This includes completing the constructor and
all the virtual functions that have their
``atomic_four`` implementations replaced by
*atomic_user* implementations.

atomic_four
***********

Restrictions
============
The ``atomic_four`` constructor and destructor cannot be called in
:ref:`parallel<ta_in_parallel-name>` mode.

Base
====
The template parameter determines the
:ref:`atomic_four_call@Base`
type for this ``AD`` < *Base* > atomic operation.

name
====
This ``atomic_four`` constructor argument has the following prototype

   ``const std::string&`` *name*

It is the name for this atomic function and is used for error reporting.
The suggested value for *name* is *afun* or *atomic_user* ,
i.e., the name of the corresponding atomic object or class.

Example
*******
The following is an example constructor definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN CONSTRUCTOR
   // END CONSTRUCTOR
}

{xrst_end atomic_four_ctor}
-------------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// atomic_four()
template <class Base>
atomic_four<Base>::atomic_four(void)
{  CPPAD_ASSERT_KNOWN(false,
      "Attempt to use the atomic_four default constructor"
   );
}

// atomic_four(name)
// BEGIN_PROTOTYPE
template <class Base>
atomic_four<Base>::atomic_four(const std::string& name )
// END_PROTOTYPE
{  CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "atomic_four: constructor cannot be called in parallel mode."
   );
   //
   // index_
   bool        set_null  = false;
   size_t      index     = 0;
   size_t      type      = 4;
   std::string copy_name = name;
   void*       copy_this = reinterpret_cast<void*>( this );
   index_  = local::atomic_index<Base>(
      set_null, index, type, &copy_name, copy_this
   );
   //
   // work_
   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
      work_[thread] = nullptr;
}

} // END_CPPAD_NAMESPACE
# endif
