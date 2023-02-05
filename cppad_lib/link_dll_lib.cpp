// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/to_string.hpp>
# include <cppad/utility/link_dll_lib.hpp>

# ifndef _WIN32
//  dlopen, dlsym, dlerror, dlclose, RTLD_LAZY
# include <dlfcn.h>
# else
//  LoadLibrary, GetProcAddress, FreeLibrary, GetLastError, RTLD_LAZY
# ifndef NOMINMAX
# define NOMINMAX
# endif
# include <windows.h>
# define RTLD_LAZY 0
# endif

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

# ifdef _WIN32
//
//  dlopen
void* link_dll_lib::dlopen(const char *filename, int flag)
{  HINSTANCE hinstance = LoadLibraryA(filename);
   return reinterpret_cast<void*>( hinstance );
}
// dlsym
void* link_dll_lib::dlsym(void* handle, const char* symbol)
{  HINSTANCE hinstance = reinterpret_cast<HINSTANCE>( handle );
   FARPROC   farproc   = GetProcAddress(hinstance, symbol);
   return reinterpret_cast<void*>( farproc );
}
// dlclose
int link_dll_lib::dlclose(void* handle)
{  HINSTANCE hinstance = reinterpret_cast<HINSTANCE>( handle );
   int result          = FreeLibrary(hinstance);
   return result;
}
// dlerror
const char* link_dll_lib::dlerror(void)
{  // should have a different result for each thread
   static char result[100];
   //
   DWORD dw        = GetLastError();
   std::string str = CppAD::to_string(dw);
   str             = "Microsoft GetLastError() = " + str;
   size_t  i = 0;
   while(i < 99 && i < str.size())
      result[i] = str[i];
   result[i] = '\0';
   //
   return result;
}
# endif
//
link_dll_lib::link_dll_lib(const std::string& dll_file, std::string& err_msg)
{  handle_ = dlopen(dll_file.c_str(), RTLD_LAZY);
   if( handle_ != nullptr )
      err_msg = "";
   else
   {  const char *err_str = dlerror();
      err_msg = "Error opening dll_file =" + dll_file;
      if( err_str != nullptr )
      {  err_msg += "\n";
         err_msg += err_str;
      }
   }
   ctor_err_msg_ = err_msg;
}
link_dll_lib::~link_dll_lib(void)
{  if( handle_ != nullptr )
      dlclose(handle_);
}
void* link_dll_lib::operator()
(const std::string& function_name, std::string& err_msg) const
// END_OPERATOR
{  if( handle_ == nullptr )
   {  err_msg = ctor_err_msg_;
      return nullptr;
   }
   void* fun_ptr = dlsym(handle_, function_name.c_str());
   if( fun_ptr == nullptr )
      err_msg = "Error finding function_name = " + function_name;
   else
      err_msg = "";
   return fun_ptr;
}

} // END_CPPAD_NAMESPACE
