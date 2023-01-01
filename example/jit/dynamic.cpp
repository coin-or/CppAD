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
$begin jit_dynamic.cpp$$
$spell
    csrc
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io%current documentation%$$.
$$
$section JIT With Dynamic Parameters: Example and Test$$

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
bool dynamic(void)
{   bool ok = true;
    //
    using CppAD::AD;
    using CppAD::ADFun;
    using CppAD::Independent;
    using CppAD::NearEqual;
    //
    // nx, ny
    size_t np = 2, nx = 2, ny = 1;
    //
    // f(x) = x_0 + x_1
    CPPAD_TESTVECTOR( AD<double> ) ap(np), ax(nx), ay(ny);
    ap[0] = 0.0;
    ap[1] = 1.0;
    ax[0] = 2.0;
    ax[1] = 4.0;
    Independent(ax, ap);
    ay[0] = ap[0] * cos(ax[0])  + ap[1] * sin(ax[1]);
    ADFun<double> f(ax, ay);
    f.function_name_set("f");
    //
    // csrc_file
    // created in std::filesystem::current_path
    std::string c_type    = "double";
    std::string csrc_file = "jit_dynamic.c";
    std::ofstream ofs;
    ofs.open(csrc_file , std::ofstream::out);
    f.to_csrc(ofs, c_type);
    ofs.close();
    //
    // dll_file
    // created in std::filesystem::current_path
    std::string dll_file = "jit_dynamic" DLL_EXT;
    CPPAD_TESTVECTOR( std::string) csrc_files(1);
    csrc_files[0] = csrc_file;
    std::map< std::string, std::string > options;
    std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files, options);
    if( err_msg != "" )
    {   std::cerr << "jit_dynamic: err_msg = " << err_msg << "\n";
        return false;
    }
    // dll_linker
    CppAD::link_dll_lib dll_linker(dll_file, err_msg);
    if( err_msg != "" )
    {   std::cerr << "jit_dynamic: err_msg = " << err_msg << "\n";
        return false;
    }
    //
    // f_ptr
    std::string function_name = "cppad_jit_f";
    void* void_ptr = dll_linker(function_name, err_msg);
    if( err_msg != "" )
    {   std::cerr << "dynamic: err_msg = " << err_msg << "\n";
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
    // u, y, compare_change
    // y = f(u)
    size_t nu             = np + nx;
    size_t compare_change = 0;
    std::vector<double> u(nu), y(ny);
    u[0] = 0.3;  // p[0]
    u[1] = 0.5;  // p[0]
    u[2] = 0.7;  // x[0]
    u[3] = 0.9;  // x[1]
    f_ptr(nu, u.data(), ny, y.data(), &compare_change);
    //
    // ok
    ok &= compare_change == 0;
    //
    // ok
    // f(u) = p[0] * cos(x[0]) + p[1] * sin(x[1])
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    double check = u[0] * std::cos(u[2]) + u[1] * std::sin(u[3]);
    ok &= NearEqual(y[0], check, eps99, eps99);
    //
    return ok;
}
// END C++
