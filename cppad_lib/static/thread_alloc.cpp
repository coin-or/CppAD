// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/thread_alloc.hpp>
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
{xrst_begin ta_thread_info dev}
{xrst_spell
    nullptr
    inuse
}

Get pointer to the information for this thread
##############################################

Syntax
******
{xrst_code cpp}
    info = thread_alloc::thread_info(thread, clear)
{xrst_code}

Prototype
*********
{xrst_literal ,
    include/cppad/utility/thread_alloc.hpp
    BEGIN_THREAD_INFO, END_THREAD_INFO
}

thread
******
is the thread number for this information pointer.

clear
*****
If clear is true, then the information pointer for this thread
is deleted and the nullptr pointer is returned.
There must be no memory currently in either the inuse or available
lists when this routine is called.

info
****
is the current information pointer for this thread.
If clear is false, and the current pointer is nullptr,
a new information record is allocated and its pointer returned.
In this case, if info is the returned pointer,
{xrst_code cpp}
    info->count_inuse == 0
    info->count_available == 0
{xrst_code}
In addition,
for c = 0 , ... , CPPAD_MAX_NUM_CAPACITY-1
{xrst_code cpp}
    info->root_inuse_[c].next_ == nullptr
    info->root_available_[c].next_ == nullptr
{xrst_code}

{xrst_end ta_thread_info}
-----------------------------------------------------------------------------
*/

thread_alloc::thread_alloc_info* thread_alloc::thread_info(
    size_t             thread  ,
    bool               clear   )
{   static thread_alloc_info* all_info[CPPAD_MAX_NUM_THREADS];
    static thread_alloc_info  zero_info;

    CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

    CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS );

    thread_alloc_info* info = all_info[thread];
    if( clear )
    {   if( info != nullptr )
        {
# ifndef NDEBUG
            CPPAD_ASSERT_UNKNOWN(
                info->count_inuse_     == 0 &&
                info->count_available_ == 0
            );
            for(size_t c = 0; c < CPPAD_MAX_NUM_CAPACITY; c++)
            {   CPPAD_ASSERT_UNKNOWN(
                    info->root_inuse_[c].next_     == nullptr &&
                    info->root_available_[c].next_ == nullptr
                );
            }
# endif
            if( thread != 0 )
                ::operator delete( reinterpret_cast<void*>(info) );
            info             = nullptr;
            all_info[thread] = info;
        }
    }
    else if( info == nullptr )
    {   if( thread == 0 )
            info = &zero_info;
        else
        {   size_t size = sizeof(thread_alloc_info);
            void* v_ptr = ::operator new(size);
            info        = reinterpret_cast<thread_alloc_info*>(v_ptr);
        }
        all_info[thread] = info;

        // initialize the information record
        for(size_t c = 0; c < CPPAD_MAX_NUM_CAPACITY; c++)
        {   info->root_inuse_[c].next_       = nullptr;
            info->root_available_[c].next_   = nullptr;
        }
        info->count_inuse_     = 0;
        info->count_available_ = 0;
    }
    return info;
}
/*
------------------------------------------------------------------------------
{xrst_begin ta_capacity_info dev}

Vector of capacity information for this allocator
#################################################

Syntax
******
{xrst_code cpp}
    info  = thread_alloc::capacity_info()
{xrst_code}

Prototype
*********
{xrst_literal ,
    include/cppad/utility/thread_alloc.hpp
    BEGIN_CAPACITY_INFO , END_CAPACITY_INFO
}

{xrst_end ta_capacity_info}
*/
const thread_alloc::capacity_t* thread_alloc::capacity_info(void)
{   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
    static const capacity_t capacity;
    return &capacity;
}

} // END_CPPAD_NAMESPACE
