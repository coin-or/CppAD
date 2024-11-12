# ifndef CPPAD_CORE_ATOMIC_TWO_CTOR_HPP
# define CPPAD_CORE_ATOMIC_TWO_CTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_ctor app}

Atomic Function Constructor
###########################

Syntax
******
| *atomic_user afun* ( *ctor_arg_list* )
| ``atomic_base`` < *Base* >( *name* , *sparsity* )

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
``atomic_base`` < *Base* > .
It should be declared as follows:

| |tab| ``class`` *atomic_user* : ``public CppAD::atomic_base<`` *Base* > {
| |tab| ``public:``
| |tab| |tab| *atomic_user* ( *ctor_arg_list* ) : ``atomic_base`` < *Base* >( *name* , *sparsity* )
| |tab| ...
| |tab| };

where ...
denotes the rest of the implementation of the derived class.
This includes completing the constructor and
all the virtual functions that have their
``atomic_base`` implementations replaced by
*atomic_user* implementations.

atomic_base
***********

Restrictions
============
The ``atomic_base`` constructor and destructor cannot be called in
:ref:`parallel<ta_in_parallel-name>` mode.

Base
====
The template parameter determines the
*Base* type for this ``AD`` < *Base* > atomic operation.

name
====
This ``atomic_base`` constructor argument has the following prototype

   ``const std::string&`` *name*

It is the name for this atomic function and is used for error reporting.
The suggested value for *name* is *afun* or *atomic_user* ,
i.e., the name of the corresponding atomic object or class.

sparsity
========
This ``atomic_base`` constructor argument has prototype

   ``atomic_base`` < *Base* >:: ``option_enum`` *sparsity*

The current *sparsity* for an ``atomic_base`` object
determines which type of sparsity patterns it uses
and its value is one of the following:

.. list-table::
   :widths: auto

   * - *sparsity*
     - sparsity patterns
   * - ``atomic_base`` < *Base* >:: ``pack_sparsity_enum``
     - :ref:`CppAD_vector@vectorBool`
   * - ``atomic_base`` < *Base* >:: ``bool_sparsity_enum``
     - :ref:`vector<CppAD_vector-name>` ``<bool>``
   * - ``atomic_base`` < *Base* >:: ``set_sparsity_enum``
     - :ref:`vector<CppAD_vector-name>` ``<std::set<std::size_t> >``

There is a default value for *sparsity* if it is not
included in the constructor (which may be either the bool or set option).

{xrst_end atomic_two_ctor}
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_ctor.hpp
Constructors for atomic_base class.
*/

/*!
Base class for atomic_atomic functions.

\tparam Base
This class is used for defining an AD<Base> atomic operation y = f(x).

\par
make sure user does not invoke the default constructor
*/
template <class Base>
atomic_base<Base>::atomic_base(void)
{  CPPAD_ASSERT_KNOWN(false,
      "Attempt to use the atomic_base default constructor"
   );
}
/*!
Constructor

\param name
name used for error reporting

\param sparsity [in]
what type of sparsity patterns are computed by this function,
bool_sparsity_enum or set_sparsity_enum. Default value is
bool sparsity patterns.
*/
template <class Base>
atomic_base<Base>::atomic_base(
      const std::string&     name,
      option_enum            sparsity
) :
sparsity_( sparsity               )
{  CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "atomic_base: constructor cannot be called in parallel mode."
   );
   CPPAD_ASSERT_UNKNOWN( constant_enum < dynamic_enum );
   CPPAD_ASSERT_UNKNOWN( dynamic_enum < variable_enum );
   //
   // atomic_index
   bool        set_null  = false;
   size_t      index     = 0;
   size_t      type      = 2;
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
