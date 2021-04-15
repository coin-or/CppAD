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
$begin llvm_print_ir.cpp$$

$section Print an LLVM Internal Representation $$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <limits>
# include <cppad/cppad.hpp>
# include <cppad/core/llvm/ir.hpp>

bool print_ir(void)
{   bool ok = true;
    using CppAD::AD;
    std::string msg;
    //
    // nx, ny
    size_t nx = 1, ny = 1;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    //
    // f(x) = x * x
    ax[0] = 0.0;
    CppAD::Independent(ax);
    ay[0] = ax[0] * ax[0];
    CppAD::ADFun<double> f(ax, ay);
    f.function_name_set("print_ir");
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // os
    std::stringstream os;
    ir_obj.print(os);
    //
    // text appears as one line at beginning of IR definition of this function
    std::string text;
    text  = "define i32 @print_ir("
                 "i32 %len_input, double* %input_ptr, "
                 "i32 %len_output, double* %output_ptr, "
                 "i32 %len_msg, i8* %msg_ptr"
            ")";
    //
    // check for text in output
    size_t index = os.str().find(text);
    ok          &= index < os.str().size();
    //
    return ok;
}
// END C++
