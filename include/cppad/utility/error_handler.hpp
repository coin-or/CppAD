# ifndef CPPAD_UTILITY_ERROR_HANDLER_HPP
# define CPPAD_UTILITY_ERROR_HANDLER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ErrorHandler}
{xrst_spell
   msg
   test test
}

Replacing the CppAD Error Handler
#################################

Syntax
******
| # ``include <cppad/utility/error_handler.hpp>``
| ``ErrorHandler`` *info* ( *handler* )
| ``ErrorHandler::Call`` ( *known* , *line* , *file* , *exp* , *msg* )

Constructor
***********
When you construct a ``ErrorHandler`` object,
the current CppAD error handler is replaced by *handler* .
When the object is destructed, the previous CppAD error handler is restored.

Parallel Mode
=============
The ``ErrorHandler`` constructor and destructor cannot be called in
:ref:`parallel<ta_in_parallel-name>` execution mode.
If this rule is not abided by, a raw C++ ``assert`` ,
instead of one that uses this error handler, will be generated.

Call
****
When ``ErrorHandler::Call`` is called,
the current CppAD error handler is used to report an error.
This starts out as a default error handler and can be replaced
using the ``ErrorHandler`` constructor.

info
****
The object *info* is used to store information
that is necessary to restore the previous CppAD error handler.
This restoration is done when the destructor for *info* is called.

handler
*******
The argument *handler* has prototype

| |tab| ``void`` (* *handler* )
| |tab| |tab| ( ``bool`` , ``int`` , ``const char`` * , ``const char`` * , ``const char`` * );

When an error is detected,
it is called with the syntax

   *handler* ( *known* , *line* , *file* , *exp* , *msg* )

This routine should not return; i.e., upon detection of the error,
the routine calling *handler* does not know how to proceed.

known
*****
The *handler* argument *known* has prototype

   ``bool`` *known*

If it is true, the error being reported is from a know problem.

line
****
The *handler* argument *line* has prototype

   ``int`` *line*

It reports the source code line number where the error is detected.

file
****
The *handler* argument *file* has prototype

   ``const char`` * *file*

and is a ``'\0'`` terminated character vector.
It reports the source code file where the error is detected.

exp
***
The *handler* argument *exp* has prototype

   ``const char`` * *exp*

and is a ``'\0'`` terminated character vector.
It is a source code boolean expression that should have been true,
but is false,
and thereby causes this call to *handler* .

msg
***
The *handler* argument *msg* has prototype

   ``const char`` * *msg*

and is a ``'\0'`` terminated character vector.
It reports the meaning of the error from the C++ programmers point of view.
{xrst_toc_hidden
   example/utility/error_handler.cpp
   include/cppad/core/cppad_assert.hpp
}
Example
*******
The file
:ref:`error_handler.cpp-name`
contains an example and test a test of using this routine.

{xrst_end ErrorHandler}
---------------------------------------------------------------------------
*/

# include <iostream>

# include <cppad/configure.hpp>
# include <cppad/local/set_get_in_parallel.hpp>
# include <cassert>
# include <cstdlib>

namespace CppAD { // BEGIN CppAD namespace

class ErrorHandler {
   template <class Base>
   friend void parallel_ad(void);
public:
   typedef void (*Handler)
      (bool, int, const char *, const char *, const char *);

   // construct a new handler
   ErrorHandler(Handler handler) : previous( Current() )
   {  if( local::set_get_in_parallel() )
      {  bool known       = true;
         int  line        = __LINE__;
         const char* file = __FILE__;
         const char* exp  = "! local::set_get_in_parallel()";
         const char* msg  =
            "Using ErrorHandler constructor in parallel mode.";
         Call(known, line, file, exp, msg);
      }
      Current() = handler;
   }

   // destructor for an error handler
   ~ErrorHandler(void)
   {  if( local::set_get_in_parallel() )
      {  bool known       = true;
         int  line        = __LINE__;
         const char* file = __FILE__;
         const char* exp  = "! local::set_get_in_parallel()";
         const char* msg  =
            "Using ErrorHandler destructor in parallel mode.";
         Call(known, line, file, exp, msg);
      }
      Current() = previous;
   }

   // report an error
   static void Call(
      bool        known,
      int         line ,
      const char *file ,
      const char *exp  ,
      const char *msg  )
   {  Handler handler = Current();
      handler(known, line, file, exp, msg);
   }

private:
   const Handler previous;

   // The default error handler
   static void Default(
      bool        known,
      int         line ,
      const char *file ,
      const char *exp  ,
      const char *msg  )
   {  using std::cerr;
      using std::endl;

      cerr << CPPAD_PACKAGE_STRING;
      if( known )
         cerr << " error from a known source:" << endl;
      else
         cerr << " error from unknown source"  << endl;
      if( msg[0] != '\0' )
         cerr << msg << endl;
      cerr << "Error detected by false result for"  << endl;
      cerr << "    "     << exp                     << endl;
      cerr << "at line " << line << " in the file " << endl;
      cerr << "    "     << file                    << endl;

      // terminate program execution
      assert(false);

      // termination when NDEBUG is defined
      std::exit(1);
   }

   // current error handler
   static Handler &Current(void)
   {  static bool first_call = true;
      static Handler current = Default;
      // CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
      // code below is like macro above but works when NDEBUG defined
      if( first_call )
      {  if( local::set_get_in_parallel() )
         {  bool known       = false;
            int  line        = __LINE__;
            const char* file = __FILE__;
            const char* exp  = "";
            const char* msg  = "";
            Call(known, line, file, exp, msg);
         }
         first_call = false;
      }
      return current;
   }
};

} // END CppAD namespace



# endif
