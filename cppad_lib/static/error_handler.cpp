// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/error_handler.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// current error handler
ErrorHandler::Handler &ErrorHandler::Current(void)
{   static bool first_call = true;
    static Handler current = Default;
    //
    // CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
    if( first_call )
    {   if( local::set_get_in_parallel() )
        {   bool known       = false;
            int  line        = __LINE__;
            const char* file = __FILE__;
            const char* exp  = "";
            const char* msg  =
                "ErrorHandler::Current: first call in parallel mode";
            Call(known, line, file, exp, msg);
        }
        first_call = false;
    }
    return current;
}

} // END_CPPAD_NAMESPACE
