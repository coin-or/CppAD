/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin llvm_link_discrete.cpp$$
$spell
    cpp
$$

$section Example Linking a Discrete AD Function$$

$head discrete.cpp$$
$srcfile%example/llvm/discrete.cpp%0%// BEGIN C++%// END C++%0%$$


$head This File$$
$srcthisfile%0%// BEGIN C++%// END C++%2%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/core/llvm/ir.hpp>
# include <cppad/core/llvm/link.hpp>


extern "C" double discrete_heaviside(double x);

namespace {
    double heaviside(const double &x)
    {   return discrete_heaviside(x); }
    CPPAD_DISCRETE_FUNCTION(double, heaviside)
}


bool link_discrete(void)
{   bool ok = true;
    using CppAD::llvm_ir;
    using CppAD::AD;
    //
    size_t nx = 1, ny = 1;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    ax[0] = 0.0;
    CppAD::Independent(ax);
    ay[0] = heaviside( ax[0] );
    CppAD::ADFun<double> f(ax, ay);
    f.function_name_set("link_discrete");
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
    std::string adfun_obj_file = "llvm_" + function_name + ".o";
    msg = ir_obj.to_object_file(adfun_obj_file);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // link_obj
    CppAD::llvm_link link_obj(msg);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load object file
    msg = link_obj.object_file(adfun_obj_file);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // The function discrete_heaviside is in the the following library
    std::string discrete_lib = "libdiscrete.so";
    //
    // load library
    msg = link_obj.dynamic_lib(discrete_lib);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    std::vector<double> input(nx);
    //
    // vector to hold return value
    CppAD::vector<double> output(ny);
    output[0] = std::numeric_limits<double>::quiet_NaN();
    //
    // call
    int32_t error_no;
    int32_t len_input   = static_cast<int32_t>(nx);
    int32_t len_output  = static_cast<int32_t>(ny);
    //
    // heaviside(-1)
    input[0] = -1.0;
    error_no = fun_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    ok &= output[0] == 0.0;
    //
    // heaviside(0)
    input[0] = 0.0;
    error_no = fun_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    ok &= output[0] == 0.5;
    //
    // heaviside(1)
    input[0] = 1.0;
    error_no = fun_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    ok &= output[0] == 1.0;
    //
    return ok;
}
// END C++
