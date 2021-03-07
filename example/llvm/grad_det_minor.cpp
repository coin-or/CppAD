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
$begin llvm_grad_det_minor$$

$section Example: LLVM Complied Derivative of Determinant by Minors$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
//
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/core/llvm_ir.hpp>
# include <cppad/core/llvm_link.hpp>

bool grad_det_minor(void)
{   using CppAD::AD;
    bool ok         = true;
    std::string msg = "";
    //
    // row and column size of the matrix
    size_t size = 3;
    //
    // number independent variables and number of elements in matrix
    size_t nx = size * size;
    //
    // object for computing determinant
    CppAD::det_by_minor< AD<double> > a_det(size);
    //
    // choose a matrix
    CPPAD_TESTVECTOR(double) matrix(nx);
    CppAD::uniform_01(nx, matrix);
    //
    // copy to independent variables
    CPPAD_TESTVECTOR( AD<double> )   a_A(nx);
    for(size_t j = 0; j < nx; ++j)
        a_A[j] = matrix[j];
    //
    // declare independent variables for function computation
    size_t abort_op_index = 0;
    bool   record_compare = false;
    CppAD::Independent(a_A, abort_op_index, record_compare);
    //
    // AD computation of the determinant
    CPPAD_TESTVECTOR( AD<double> ) a_detA(1);
    a_detA[0] = a_det(a_A);
    //
    // create function objects for f : A -> detA
    CppAD::ADFun<double> f;
    f.Dependent(a_A, a_detA);
    //
    // AD<double> version of f
    CppAD::ADFun< AD<double>, double > a_f;
    a_f = f.base2ad();
    //
    // declare independent variables for gradient computation
    CppAD::Independent(a_A, abort_op_index, record_compare);
    //
    // vectors of reverse mode weights
    CPPAD_TESTVECTOR( AD<double> ) a_w(1);
    a_w[0] = 1.0;
    //
    // AD computation of the gradient
    CPPAD_TESTVECTOR( AD<double> )  a_gradient(nx);
    a_f.Forward(0, a_A);
    a_gradient = a_f.Reverse(1, a_w);
    //
    // create function objects for g : A -> det'( detA  )
    CppAD::ADFun<double> g;
    g.Dependent(a_A, a_gradient);
    //
    // Set name of this function to llvm_grad_det_minor
    std::string function_name = "grad_det_minor";
    g.function_name_set(function_name);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    g.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    /*
    // This is not yet working
    ir_obj.optimize();
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    */
    //
    // llvm_grad_det_minor.o
    // bin/test_one.sh.in assumes file name begins with llvm_
    std::string file_name = "llvm_" + function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // link_obj
    CppAD::llvm_link link_obj;
    msg = link_obj.load(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // get next matrix
    CppAD::uniform_01(nx, matrix);
    //
    // get pointer to compiled version of function
    CppAD::llvm_compiled_t function_ptr;
    msg = link_obj.compiled(function_name, function_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // evaluate gradient
    std::vector<double> input(nx), gradient(nx);;
    for(size_t i = 0; i < nx; ++i)
        input[i] = matrix[i];
    int32_t error_no = function_ptr(
        int32_t(nx), input.data(), int32_t(nx), gradient.data()
    );
    if( error_no != 0 )
    {   std::cerr << "\nerror_no != 0\n";
        return false;
    }
    //
    // check value of gradient
    ok &= CppAD::det_grad_33(input, gradient);
    //
    return ok;
}
// END C++
