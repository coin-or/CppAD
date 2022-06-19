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
$begin jit_get_started.cpp$$
$spell
    CppAD
$$

$section JIT Computation of Derivatives: Example and Test$$

$head Purpose$$
This is a simple example using CppAD for Just In Time (JIT)
compilation, linking, and running of C source code that computes derivatives.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++

# include <cstddef>
# include <iostream>
# include <fstream>

# include <cppad/configure.hpp>
# if CPPAD_USE_CPLUSPLUS_2017
# include <filesystem>
# endif

// CALL_CONVENTION, CALL_IMPORT
# ifdef _MSC_VER
# define CALL_CONVENTION __cdecl
# define CALL_IMPORT     __declspec(dllimport)
# else
# define CALL_CONVENTION
# define CALL_IMPORT
# endif

namespace {
    // cppad_forward_zero_double
    extern "C"
    CALL_IMPORT typedef int (CALL_CONVENTION *cppad_forward_zero_double)(
        size_t        call_id,
        size_t        nx,
        const double* x,
        size_t        ny,
        double*       y,
        size_t*       compare_change
    );
}
# include <cppad/cppad.hpp>
bool get_started(void)
{   bool ok = true;
    //
    // p
# if CPPAD_USE_CPLUSPLUS_2017
    std::filesystem::path p;
# endif
    //
    using CppAD::AD;
    using CppAD::ADFun;
    using CppAD::Independent;
    using CppAD::NearEqual;
    //
    // f
    // f(r, a) =  (x_0 * cos(x_1) , x_0 * sin(x_1))^T
    size_t n = 2;
    size_t m = 2;
    CPPAD_TESTVECTOR( AD<double> ) ax(n), ay(m);
    for(size_t j = 0; j < n; ++j)
        ax[j] = AD<double>( j + 1);
    Independent(ax);
    ay[0] = ax[0] * cos( ax[1] );
    ay[1] = ax[0] * sin( ax[1] );
    ADFun<double> f(ax, ay);
    ADFun< AD<double> , double > af = f.base2ad();
    //
    // g(x) = f'(x)
    // g(x) = [ cos(x_1) , - x_0 * sin(x_1) ]
    //        [ sin(x_1) ,   x_0 * cos(x_1) ]
    Independent(ax);
    CPPAD_TESTVECTOR( AD<double> ) az(m * n);
    az = af.Jacobian(ax);
    ADFun<double> g(ax, az);
    g.function_name_set("g");
    //
    // csrc_file
    // created in std::filesystem::current_path()
    std::map<std::string, std::string> options;
    options["type"] = "double";
    std::string csrc_file = "jit_get_started.c";
    std::ofstream ofs;
    ofs.open(csrc_file , std::ofstream::out);
    g.to_csrc(ofs, options);
    ofs.close();
    //
    // p, ok
# if CPPAD_USE_CPLUSPLUS_2017
    p   = std::filesystem::current_path();
    p  /= csrc_file;
    ok &= std::filesystem::exists(p);
# endif
    //
    // dll_file
    // created in std::filesystem::current_path()
    std::string dll_file = "jit_getstarted.so";
    CPPAD_TESTVECTOR( std::string) csrc_files(1);
    csrc_files[0] = csrc_file;
    std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files);
    if( err_msg != "" )
    {   std::cerr << "jit_get_started: err_msg = " << err_msg << "\n";
        return false;
    }
    //
    // p, ok
# if CPPAD_USE_CPLUSPLUS_2017
    p   = std::filesystem::current_path();
    p  /= dll_file;
    ok &= std::filesystem::exists(p);
# endif
    //
    // dll_linker
    CppAD::link_dll_lib dll_linker(dll_file, err_msg);
    if( err_msg != "" )
    {   std::cerr << "jit_get_started: err_msg = " << err_msg << "\n";
        return false;
    }
    //
    // g_ptr
    std::string function_name = "cppad_forward_zero_g";
    void* void_ptr = dll_linker(function_name, err_msg);
    if( err_msg != "" )
    {   std::cerr << "jit_get_started: err_msg = " << err_msg << "\n";
        return false;
    }
    cppad_forward_zero_double g_ptr =
        reinterpret_cast<cppad_forward_zero_double>(void_ptr);
    //
    // x, z, compare_change
    // z = g(x)
    size_t nx = n, nz = m * n, call_id = 0, compare_change = 0;
    std::vector<double> x(nx), z(nz);
    x[0] = 0.3;
    x[1] = 0.5;
    g_ptr(call_id, nx, x.data(), nz, z.data(), &compare_change);
    //
    // ok
    ok &= compare_change == 0;
    //
    // eps99
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // ok
    // df_0 / dx_0 is stored in z[0 * n + 0]
    double value = z[ 0 * n + 0];
    double check = std::cos(x[1]);
    ok &= NearEqual(value, check, eps99, eps99);
    //
    // ok
    // df_0 / dx_1 is stored in z[0 * n + 1]
    value = z[ 0 * n + 1];
    check = - x[0] * std::sin(x[1]);
    ok &= NearEqual(value, check, eps99, eps99);
    //
    // ok
    // df_1 / dx_0 is stored in z[1 * n + 0]
    value = z[ 1 * n + 0];
    check = std::sin(x[1]);
    ok &= NearEqual(value, check, eps99, eps99);
    //
    // ok
    // df_1 / dx_1 is stored in z[1 * n + 1]
    value = z[ 1 * n + 1];
    check = x[0] * std::cos(x[1]);
    ok &= NearEqual(value, check, eps99, eps99);
    //
    return ok;
}
// END C++
