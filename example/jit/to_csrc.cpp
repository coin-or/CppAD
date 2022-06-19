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
$begin jit_to_csrc.cpp$$
$spell
    csrc
$$

$section C Source Code Corresponding to a Function: Example and Test$$

$head to_csrc$$
The actual call to $code to_csrc$$ is in the following section of the
example below:
$srcthisfile%0%// BEGIN_TO_CSRC%// END_TO_CSRC%1%$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++

# include <cstddef>
# include <iostream>
# include <fstream>

// CALL_CONVENTION, CALL_IMPORT
# ifdef _MSC_VER
# define CALL_CONVENTION __cdecl
# define CALL_IMPORT     __declspec(dllimport)
# else
# define CALL_CONVENTION
# define CALL_IMPORT
# endif

namespace {
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
bool to_csrc(void)
{   bool ok = true;
    //
    using CppAD::AD;
    using CppAD::ADFun;
    using CppAD::Independent;
    using CppAD::NearEqual;
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
    std::map<std::string, std::string> options;
    options["type"] = "double";
    std::string csrc_file = "to_csrc.c";
    std::ofstream ofs;
    ofs.open(csrc_file , std::ofstream::out);
// BEGIN_TO_CSRC
    f.to_csrc(ofs, options);
// END_TO_CSRC
    ofs.close();
    //
    // dll_file
    // created in std::filesystem::current_path
    std::string dll_file = "jit_getstarted.so";
    CPPAD_TESTVECTOR( std::string) csrc_files(1);
    csrc_files[0] = csrc_file;
    std::string err_msg = CppAD::create_dll_lib(dll_file, csrc_files);
    if( err_msg != "" )
    {   std::cerr << "to_csrc: err_msg = " << err_msg << "\n";
        return false;
    }
    // dll_linker
    CppAD::link_dll_lib dll_linker(dll_file, err_msg);
    if( err_msg != "" )
    {   std::cerr << "to_csrc: err_msg = " << err_msg << "\n";
        return false;
    }
    //
    // f_ptr
    std::string function_name = "cppad_forward_zero_f";
    void* void_ptr = dll_linker(function_name, err_msg);
    if( err_msg != "" )
    {   std::cerr << "to_csrc: err_msg = " << err_msg << "\n";
        return false;
    }
    cppad_forward_zero_double f_ptr =
        reinterpret_cast<cppad_forward_zero_double>(void_ptr);
    //
    // x, y, compare_change
    // y = f(x)
    size_t call_id = 0, compare_change = 0;
    std::vector<double> x(nx), y(ny);
    x[0] = 0.3;
    x[1] = 0.5;
    f_ptr(call_id, nx, x.data(), ny, y.data(), &compare_change);
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
