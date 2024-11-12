# ifndef CPPAD_CORE_CHKPOINT_TWO_DYNAMIC_HPP
# define CPPAD_CORE_CHKPOINT_TWO_DYNAMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin chkpoint_two_dynamic}
{xrst_spell
   chk
}

Dynamic Parameters in Checkpoint Functions
##########################################

Syntax
******
| *chk_fun* . ``new_dynamic`` ( *dynamic* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

chk_fun
*******
This object must have been created using the
:ref:`chkpoint_two<chkpoint_two_ctor@chk_fun>` constructor.

Base
====
This is the :ref:`chkpoint_two_ctor@Base` type
in the *chk_fun* constructor.

fun
===
This is the function :ref:`chkpoint_two_ctor@fun`
in the *chk_fun* constructor.

BaseVector
**********
This must be a :ref:`SimpleVector-name` with elements of type *Base* .

dynamic
*******
This is a vector with new values for the dynamic parameters
in the function *fun* .
Is size must be equal to
:ref:`fun.size_dyn_ind()<fun_property@size_dyn_par>` .
This only affects the copy of *fun* used by *chk_fun* .

Multi-Threading
***************
If one is using :ref:`in_parallel<ta_in_parallel-name>` ,
there is a separate copy of *fun* for each thread.
In this case, only the dynamic parameters in the copy for the current
:ref:`thread number<ta_thread_num-name>` are changed.

{xrst_end chkpoint_two_dynamic}
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_two/dynamic.hpp
Change the dynnamic parameter in a checkpoint function.
*/

/*!
Constructor

\tparam Base
base class for recording AD<Base> operations using this checkpoint object.

\param dynamic
is the new values for the dynamic parameters in the function
defining this checkpoint object.
*/

// BEGIN_PROTOTYPE
template <class Base>
template <class BaseVector>
void chkpoint_two<Base>::new_dynamic(const BaseVector& dynamic)
// END_PROTOTYPE
{  ADFun<Base>* g_ptr = &g_;
   if( use_in_parallel_ )
   {  size_t thread = thread_alloc::thread_num();
      allocate_member(thread);
      g_ptr = &(member_[thread]->g_);
   }
# ifndef NDEBUG
   else if( thread_alloc::in_parallel() )
   {  std::string msg = atomic_three<Base>::atomic_name();
      msg += ": use_in_parallel is false and in_parallel() is true";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# endif
   g_ptr->new_dynamic(dynamic);
}

} // END_CPPAD_NAMESPACE
# endif
