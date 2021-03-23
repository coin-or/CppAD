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
$begin llvm_link_xam.cpp$$

$section Example Linking a Compiled AD Function$$

$head CPPAD_STD_MATH_LIBRARY_PATH$$
This is the path to the standard math library which is used
by this example (to access the $code sin$$ function).

$head Source Code$$
$srcthisfile%8%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/core/llvm/ir.hpp>
# include <cppad/core/llvm/link.hpp>
namespace {
    template <class VectorFloat>
    VectorFloat algo(const VectorFloat& p, const VectorFloat& x)
    // END_PROTOTYPE
    {   size_t nx = x.size();
        size_t np = p.size();
        size_t ny = nx + np;
        //
        // define the actual function
        VectorFloat y(ny);
        for(size_t i = 0; i < np; ++i)
            y[i] = sin( p[i] ) + double(i);
        for(size_t i = 0; i < nx; ++i)
            y[i + np] = sin( x[i] ) + double(i + np);
        //
        // some operations that optimizer should remove
        typename VectorFloat::value_type sum = 0.0;
        for(size_t i = 0; i < ny; i++)
            sum += y[i];
        //
        return y;
    }
    void algo2adfun(size_t np, size_t nx, CppAD::ADFun<double>& adfun)
    {   using CppAD::AD;
        //
        // ap, ax
        CPPAD_TESTVECTOR( AD<double> ) ap(np), ax(nx);
        for(size_t i = 0; i < np; ++i)
            ap[i] = AD<double>( i + 1 );
        for(size_t i = 0; i < nx; ++i)
            ax[i] = AD<double>( i + np );
        //
        // ax independent variables,  ap dynamic parameters
        CppAD::Independent(ax, ap);
        //
        // ay
        CPPAD_TESTVECTOR( AD<double> ) ay = algo(ap, ax);
        //
        // f : x -> y
        adfun.Dependent(ax, ay);
        //
        // function_name
        adfun.function_name_set("link_xam");
        return;
    }
}
bool link_xam(void)
{   bool ok = true;
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
    // bin/test_one.sh.in assumes filename begins with llvm_
    std::string file_name = "llvm_" + function_name + ".o";
    ir_obj.to_object_file(file_name);
    //
    // link_obj
    CppAD::llvm_link link_obj;
    //
    // load object file
    msg = link_obj.object_file(file_name);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load the standard math library (for sin function)
    msg = link_obj.dynamic_lib(CPPAD_STD_MATH_LIBRARY_PATH);
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
    std::vector<double> input(np + nx);
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
    int32_t error_no    = fun_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no != 0;
    for(size_t i = 0; i < ny; ++i)
        ok &= output[i] == 0.0;
    //
    // correct call function
    len_input   = static_cast<int32_t>(np + nx);
    len_output  = static_cast<int32_t>(ny);
    error_no    = fun_ptr(
        len_input, input.data(), len_output, output.data()
    );
    ok &= error_no == 0;
    //
    // check output
    CPPAD_TESTVECTOR(double) p(np), x(nx), check(ny);
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
