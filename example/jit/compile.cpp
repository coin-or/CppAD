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
$begin jit_compile.cpp$$
$spell
    csrc
$$

$section JIT Compiler Options: Example and Test$$

$head compile$$
This example demonstrates setting the JIT
$cref/compile/create_dll_lib/options/compile/$$ option.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++

# include <cstddef>
# include <iostream>
# include <fstream>
# include <map>

// DLL_EXT
# ifdef _WIN32
# define DLL_EXT ".dll"
# else
# define DLL_EXT ".so"
# endif

# include <cppad/cppad.hpp>
bool compile(void)
{   bool ok = true;
    //
    using CppAD::AD;
    using CppAD::ADFun;
    using CppAD::Independent;
    using CppAD::NearEqual;
    //
    // compile
    std::string compile = "";
    int flag;
# ifdef _MSC_VER
    flag = std::system("cl 1> nul 2> nul");
    if( flag == 0 )
        compile = "cl /EHs /EHc /c /LD /TC /O2";
# else
    flag = std::system("gcc --version >& /dev/null");
    if( flag == 0 )
        compile = "gcc -c -fPIC -O2";
    flag = std::system("clang --version >& /dev/null");
    if( flag == 0 )
        compile = "clang -c -fPIC -O2";
# endif
    if( compile == "" )
        return ok;
    // std::cout << "compile = " << compile << "\n";
    //
    // nx, ny
    size_t nx = 2, ny = 1;
    //
    // f(x) = x_0 + x_1
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    ax[0] = 0.0;
    ax[1] = 1.0;
    Independent(ax);
    ay[0] = ax[0] + ax[1];
    ADFun<double> f(ax, ay);
    f.function_name_set("f");
    //
    // csrc_file
    // created in std::filesystem::current_path
    std::string c_type    = "double";
    std::string csrc_file = "compile.c";
    std::ofstream ofs;
    ofs.open(csrc_file , std::ofstream::out);
    f.to_csrc(ofs, c_type);
    ofs.close();
    //
    // dll_file
    // created in std::filesystem::current_path
    std::string dll_file = "jit_compile" DLL_EXT;
    CPPAD_TESTVECTOR( std::string) csrc_files(1);
    csrc_files[0] = csrc_file;
    std::map< std::string, std::string > options;
    if( compile != "" )
        options["compile"] = compile;
    std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
    if( err_msg != "" )
    {   std::cerr << "jit_compile: err_msg = " << err_msg << "\n";
        return false;
    }
    // dll_linker
    CppAD::link_dll_lib dll_linker(dll_file, err_msg);
    if( err_msg != "" )
    {   std::cerr << "jit_compile: err_msg = " << err_msg << "\n";
        return false;
    }
    //
    // f_ptr
    std::string function_name = "cppad_jit_f";
    void* void_ptr = dll_linker(function_name, err_msg);
    if( err_msg != "" )
    {   std::cerr << "jit_compile: err_msg = " << err_msg << "\n";
        return false;
    }
    //
    // jit_double
    using CppAD::jit_double;
    //
    // f_ptr
    jit_double f_ptr =
        reinterpret_cast<jit_double>(void_ptr);
    //
    // x, y, compare_change
    // y = f(x)
    size_t compare_change = 0;
    std::vector<double> x(nx), y(ny);
    x[0] = 0.3;
    x[1] = 0.5;
    f_ptr(nx, x.data(), ny, y.data(), &compare_change);
    //
    // ok
    ok &= compare_change == 0;
    //
    // ok
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    double check = x[0] + x[1];
    ok &= NearEqual(y[0], check, eps99, eps99);
    //
    return ok;
}
// END C++
