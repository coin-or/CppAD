# ifndef CPPAD_UTILITY_LINK_DLL_LIB_HPP
# define CPPAD_UTILITY_LINK_DLL_LIB_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin link_dll_lib}
{xrst_spell
   msg
}

Link a Dynamic Link Library
###########################

Syntax
******
| # ``include <cppad/utility/link_dll_lib.hpp>``
| ``link_dll_lib`` *dll_linker* ( *dll_file* , *err_msg* )
| *fun_ptr* = *dll_linker* ( *function_name* , *err_msg* )
|

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

dll_linker
**********
Is the dynamic link object that holds an in memory version of the library,

err_msg
*******
If *err_msg* is non-empty, it contains an error message
for the corresponding operation.

dll_file
********
Is the file containing the dynamic link library.

function_name
*************
Is the name of an external entry point in the dll.

fun_ptr
*******
Is a ``void*`` version of a pointer the function corresponding to
*function_name* .

Warning !!
==========
*fun_ptr* becomes invalid when the *dll_linker* destructor is called.

{xrst_toc_hidden
   example/utility/dll_lib.cpp
}
Example
*******
The file :ref:`dll_lib.cpp-name` contains an example and test of
``link_dll_lib`` .

{xrst_end link_dll_lib}
*/

# include <string>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

class link_dll_lib {
private:
   // pointer to the dll object
   void* handle_;
   //
   // error message during constructor
   std::string ctor_err_msg_;
   //
# ifdef _WIN32
   static void*       dlopen(const char *filename, int flag);
   static void*       dlsym(void* handle, const char* symbol);
   static int         dlclose(void* handle);
   static const char* dlerror(void);
# endif
   //
public:
   // BEGIN_PROTOTYPE
   link_dll_lib(const std::string& dll_file, std::string& err_msg);
   ~link_dll_lib(void);
   void* operator()
   (const std::string& function_name, std::string& err_msg) const;
   // END_PROTOTYPE
};

} // END_CPPAD_NAMESPACE

# endif
