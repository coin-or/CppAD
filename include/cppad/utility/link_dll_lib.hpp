# ifndef CPPAD_UTILITY_LINK_DLL_LIB_HPP
# define CPPAD_UTILITY_LINK_DLL_LIB_HPP
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
$spell
    dll
    cppad
    hpp
    ptr
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io%current documentation%$$.
$$
$section Link a Dynamic Link Library$$

$head Syntax$$
$codei%# include <cppad/utility/link_dll_lib.hpp>
%$$
$codei%link_dll_lib %dll_linker%(%dll_file%, %err_msg%)
%$$
$icode%fun_ptr% = %dll_linker%(%function_name%, %err_msg%)
%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head dll_linker$$
Is the dynamic link object that holds an in memory version of the library,
It must not be deleted for as long as any $icode fun_ptr$$ return
values is used.

$head err_msg$$
If $icode err_msg$$ is non-empty, it contains an error message
for the corresponding operation.

$head dll_file$$
Is the file containing the dynamic link library.

$head function_name$$
Is the name of an external entry point in the dll.

$head fun_ptr$$
Is a $code void*$$ version of a pointer the function corresponding to
$icode function_name$$.

$children%
    example/utility/dll_lib.cpp
%$$
$head Example$$
The file $cref dll_lib.cpp$$ contains an example and test of
$code link_dll_lib$$.

$end
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
