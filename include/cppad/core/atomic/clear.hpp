# ifndef CPPAD_CORE_ATOMIC_CLEAR_HPP
# define CPPAD_CORE_ATOMIC_CLEAR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_base_clear$$
$spell
    sq
    alloc
$$

$section Free Static Variables$$

$head Syntax$$
$codei%atomic_base<%Base%>::clear()%$$

$head Purpose$$
Each $code atomic_base$$ objects holds onto work space in order to
avoid repeated memory allocation calls and thereby increase speed
(until it is deleted).
If an the $code atomic_base$$ object is global or static because,
the it does not get deleted.
This is a problem when using
$code thread_alloc$$ $cref/free_all/ta_free_all/$$
to check that all allocated memory has been freed.
Calling this $code clear$$ function will free all the
memory currently being held onto by the
$codei%atomic_base<%Base%>%$$ class.

$head Future Use$$
If there is future use of an $code atomic_base$$ object,
after a call to $code clear$$,
the work space will be reallocated and held onto.

$head Restriction$$
This routine cannot be called
while in $cref/parallel/ta_in_parallel/$$ execution mode.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/clear.hpp
Free static variables in atomic_base class.
*/
/*!
Free all thread_alloc static memory held by atomic_base (avoids reallocations).
(This does not include class_object() which is an std::vector.)
*/
template <class Base>
void atomic_base<Base>::clear(void)
{   CPPAD_ASSERT_KNOWN(
        ! thread_alloc::in_parallel() ,
        "cannot use atomic_base clear during parallel execution"
    );
    size_t i = class_object().size();
    while(i--)
    {   atomic_base* op = class_object()[i];
        if( op != CPPAD_NULL )
        {   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
                op->free_work(thread);
        }
    }
    return;
}

} // END_CPPAD_NAMESPACE
# endif
