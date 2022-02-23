/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>
# include <dlfcn.h>

namespace { // BEGIN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
bool binary_op(void)
{   // ok
    bool ok = true;
    //
    // AD
    using CppAD::AD;
    //
    // funciton_name
    std::string function_name = "binary_op";
    //
    // call_binary_op
    void (*call_binary_op)(
        size_t        call_id,
        size_t        n_x,
        const double* x,
        size_t        n_y,
        double*       y,
        size_t*       compare_change
    );
    //
    // n_x, ax
    size_t n_x = 2;
    CPPAD_TESTVECTOR( AD<double> ) ax(n_x);
    for(size_t j = 0; j < n_x; ++j)
        ax[j] = double(j + 2);
    CppAD::Independent(ax);
    //
    // n_y, ay
    size_t n_y = 4;
    CPPAD_TESTVECTOR( AD<double> ) ay(n_y);
    ay[0] = ax[0] + ax[1]; // add
    ay[1] = ax[0] / ax[1]; // div
    ay[2] = ax[0] * ax[1]; // mul
    ay[3] = ax[0] - ax[1]; // sub
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    f.function_name_set(function_name);
    //
    // csrc
    std::string csrc = f.to_csrc();
    //
    // c_file_name
    std::string c_file_name = function_name + ".c";
    //
    // o_file_name
    std::string o_file_name = function_name + ".o";
    //
    // so_file_name
    std::string so_file_name = "./" + function_name + ".so";
    //
    // binary_op.c
    {   std::ofstream file;
        file.open(c_file_name, std::ios::out);
        file << csrc;
        file.close();
    }
    //
    // binary_op.so
    int flag = std::system(nullptr);
    ok      &= flag != 0;
    if( flag != 0 )
    {   std::string command =
            "gcc -c -g -fPIC " + c_file_name + " -o " + o_file_name;
        flag = std::system( command.c_str() );
        ok  &= flag == 0;
        if( flag == 0 )
        {   command =
                "gcc -shared " + o_file_name + " -o " + so_file_name;
            flag = std::system( command.c_str() );
            ok  &= flag == 0;
        }
    }
    //
    // handle
    void* handle = dlopen(so_file_name.c_str(), RTLD_LAZY);
    if( handle == nullptr )
    {   char *errstr;
        errstr = dlerror();
        if( errstr != nullptr )
            std::cout << "Dynamic linking error = " << errstr << "\n";
        ok = false;
    }
    else
    {   // call binary_op
        *(void**)(&call_binary_op) = dlsym(handle, function_name.c_str());
        size_t call_id = 0;
        CppAD::vector<double> x(n_x), y(n_y);
        x[0] = 2.0;
        x[1] = 3.0;
        size_t compare_change;
        call_binary_op(
            call_id, n_x, x.data(), n_y, y.data(), &compare_change
        );
        //
        // check
        ok &= y[0] == x[0] + x[1];
        ok &= y[1] == x[0] / x[1];
        ok &= y[2] == x[0] * x[1];
        ok &= y[3] == x[0] - x[1];
    }
    return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE

bool to_csrc(void)
{   bool ok = true;
    ok     &= binary_op();
    return ok;
}
