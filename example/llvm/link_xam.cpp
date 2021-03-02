/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include "algo.hpp"
# include "algo2adfun.hpp"
# include <cppad/core/llvm_ir.hpp>
# include <cppad/core/llvm_link.hpp>
/*
$begin llvm_link_xam.cpp$$

$section Example Linking a Compiled AD Function$$

$srcthisfile%8%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
bool link_xam(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::llvm_ir;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    size_t np = 1;
    size_t nx = 2;
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
    //
    // create a cpp_graph from this function
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    std::string function_name = graph_obj.function_name_get();
    //
    // llvm_ir object
    llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // create object file
    std::string file_name = function_name + ".o";
    ir_obj.to_object_file(file_name);
    //
    // link_obj
    CppAD::llvm_link link_obj;
    //
    // load file
    link_obj.load(file_name);
    //
    // function_ptr
    CppAD::llvm_compiled_t function_ptr;
    msg = link_obj.compiled(function_name, function_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    CppAD::vector<double> input(np + nx);
    for(size_t i = 0; i < np + nx; ++i)
        input[i] = double(i) + 4.0;
    //
    // vector to hold return value
    size_t ny = f.Range();
    CppAD::vector<double> output(ny);
    for(size_t i = 0; i < ny; ++i)
        output[i] = 0.0;
    //
    // incorrect call to function
    int32_t len_input   = 0;
    int32_t len_output  = 0;
    int32_t error_no    = function_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no != 0;
    for(size_t i = 0; i < ny; ++i)
        ok &= output[i] == 0.0;
    //
    // correct call function
    len_input   = static_cast<int32_t>(np + nx);
    len_output  = static_cast<int32_t>(ny);
    error_no    = function_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    //
    // check output
    CppAD::vector<double> p(np), x(nx), check(ny);
    for(size_t i = 0; i < np; ++i)
        p[i] = input[i];
    for(size_t i = 0; i < nx; ++i)
        x[i] = input[np + i];
    check = algo(p, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(output[i], check[i], eps99, eps99);
    //
    //
    return ok;
}
// END C++
