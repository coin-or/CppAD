# ifndef CPPAD_LINK_DLL_LIB_HPP
# define CPPAD_LINK_DLL_LIB_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin link_dll_lib$$

$section Link a Dynamic Link Library$$

$head Syntax$$
$codei%# include <cppad/utility/link_dll_lib.hpp>
%$$
$icode%link_dll_lib %dll_linker%(%dll_file%, %err_msg%)
%$$
$icode%fun_ptr% = %dll_linker%(%function_name%, %err_msg%)
%$$

$end
*/

# ifndef _WIN32
//  dlopen, dlsym, dlerror, dlclose, RTLD_LAZY
# include <dlfcn.h>
# else
//  RTLD_LAZY
# define RTLD_LAZY 0
# endif

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

class link_dll_lib {
private:
    // pointer to the dll object
    void* handle_;
    //
    // error during constructor
    std::string ctor_err_msg_;
# ifdef _WIN32
    //  dlopen
    static void* dlopen(const char *filename, int flag)
    {   HINSTANCE hinstance = LoadLibrary(filename);
        return reinterpret_cast<void*>( hinstance );
    }
    // dlsym
    static void* dlsym(void* handle, const char* symbol)
    {   HINSTANCE hinstance = reinterpret_cast<HINSTANCE>( handle );
        FARPROC   farproc   = GetProcAddress(hinstance, symbol);
        return reinterpret_cast<void*>( farproc );
    }
    // dlclose
    static int dlclose(void* handle)
    {   HINSTANCE hinstance = reinterpret_cast<HINSTANCE>( handle );
        int result          = FreeLibrary(hinstance);
        return result;
    }
    // dlerror
    static const char* dlerror(void)
    {   // should have a different result for each thread
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
public:
    // ctor
    link_dll_lib(const std::string& dll_file, std::string& err_msg)
    {   handle_ = dlopen(dll_file.c_str(), RTLD_LAZY);
        if( handle_ != nullptr )
            err_msg = "";
        else
        {   const char *err_str = dlerror();
            err_msg = "Error opening dll_file =" + dll_file;
            if( err_str != nullptr )
            {   err_msg += "\n";
                err_msg += err_str;
            }
        }
        ctor_err_msg_ = err_msg;
    }
    // destructor
    ~link_dll_lib(void)
    {   if( handle_ != nullptr )
            dlclose(handle_);
    }
    // operator()
    void* operator()
    (const std::string& function_name, std::string& err_msg) const
    {   if( handle_ == nullptr )
        {   err_msg = ctor_err_msg_;
            return nullptr;
        }
        void* fun_ptr = dlsym(handle_, function_name.c_str());
        if( fun_ptr == nullptr )
            err_msg = "Error finding function_name = " + function_name;
        else
            err_msg = "";
        return fun_ptr;
    }
};

} // END_CPPAD_NAMESPACE

# endif
