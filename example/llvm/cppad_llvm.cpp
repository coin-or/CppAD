/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <llvm/IR/Type.h>
# include <llvm/IR/LLVMContext.h>
# include <llvm/IR/Module.h>
# include <llvm/IR/Module.h>
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/Verifier.h>
# include <llvm/IR/LegacyPassManager.h>
//
# include <llvm/Support/raw_os_ostream.h>
# include <llvm/Support/InitLLVM.h>
# include <llvm/Support/TargetSelect.h>
# include <llvm/Support/InitLLVM.h>
# include <llvm/Support/TargetSelect.h>
# include "llvm/Support/TargetRegistry.h"
# include "llvm/Support/Host.h"
# include "llvm/Support/raw_ostream.h"
//
# include <llvm/Transforms/InstCombine/InstCombine.h>
# include <llvm/Transforms/Scalar/GVN.h>
# include <llvm/Transforms/Scalar.h>
//
# include <llvm/ExecutionEngine/Orc/LLJIT.h>
# include <llvm/ExecutionEngine/ExecutionEngine.h>
//
# include <llvm/ADT/DenseMap.h>
//
# include "algo.hpp"
# include "algo2adfun.hpp"
# include "graph2llvm_ir.hpp"
# include "llvm_ir2graph.hpp"
# include "optimize_llvm_ir.hpp"
# include "llvm_ir2obj_file.hpp"
# include "load_obj_file.hpp"

namespace { // BEGIN_EMPTY_SPACE
// ----------------------------------------------------------------------------
bool test_object_file(
    size_t                              np            ,
    size_t                              nx            ,
    size_t                              ny            ,
    std::unique_ptr<llvm::Module>&      module_ir     ,
    const std::string&                  function_name )
{   bool ok = true;
    //
    std::string file_name = function_name + ".o";
    //
    // llvm_ir2obj_file
    std::string msg;
    msg = llvm_ir2obj_file(module_ir.get(), file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    // exit_on_error
    llvm::ExitOnError exit_on_error;
    //
    // llvm_jit
    std::unique_ptr<llvm::orc::LLJIT> llvm_jit =
        exit_on_error( llvm::orc::LLJITBuilder().create() );
    //
    // function_ptr
    function_ptr_t function_ptr;
    msg = load_obj_file(file_name, function_name, llvm_jit.get(), function_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    CppAD::vector<double> input(np + nx);
    for(size_t i = 0; i < np + nx; ++i)
        input[i] = double(i) + 4.0;
    std::cout << "jit: input = " << input << "\n";
    //
    // call function
    CppAD::vector<double> output(ny);
    function_ptr(input.data(), output.data());
    std::cout << "jit: output = " << output << "\n";
    //
    // check output
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    CppAD::vector<double> p(np), x(nx), check(ny);
    for(size_t i = 0; i < np; ++i)
        p[i] = input[i];
    for(size_t i = 0; i < nx; ++i)
        x[i] = input[np + i];
    check = algo(p, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(output[i], check[i], eps99, eps99);
    //
    return ok;
}


// ----------------------------------------------------------------------------
bool test_jit(
    size_t                              np            ,
    size_t                              nx            ,
    size_t                              ny            ,
    std::unique_ptr<llvm::LLVMContext>& context_ir    ,
    std::unique_ptr<llvm::Module>&      module_ir     ,
    const std::string&                  function_name )

{   bool ok = true;
    //
    // safe_module
    llvm::orc::ThreadSafeModule safe_module = llvm::orc::ThreadSafeModule(
        std::move(module_ir), std::move(context_ir)
    );

    // exit_on_error
    llvm::ExitOnError exit_on_error;

    // Create an LLJIT instance that has ir_function.
    std::unique_ptr<llvm::orc::LLJIT> ll_jit =
        exit_on_error( llvm::orc::LLJITBuilder().create() );
    exit_on_error( ll_jit->addIRModule( std::move(safe_module) ) );

    // function_jit
    // Look up the JIT'd function
    auto function_jit = exit_on_error( ll_jit->lookup(function_name) );
    //
    // function_cpp
    typedef void (*function_ptr) (double*, double*);
    function_ptr function_cpp = reinterpret_cast<function_ptr>(
        function_jit.getAddress()
    );
    //
    // input
    CppAD::vector<double> input(np + nx);
    for(size_t i = 0; i < np + nx; ++i)
        input[i] = double(i) + 4.0;
    std::cout << "jit: input = " << input << "\n";
    //
    // call function
    CppAD::vector<double> output(ny);
    function_cpp(input.data(), output.data());
    std::cout << "jit: output = " << output << "\n";
    //
    // check output
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    CppAD::vector<double> p(np), x(nx), check(ny);
    for(size_t i = 0; i < np; ++i)
        p[i] = input[i];
    for(size_t i = 0; i < nx; ++i)
        x[i] = input[np + i];
    check = algo(p, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(output[i], check[i], eps99, eps99);
    //
    return ok;
}

} // END_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
bool cppad_llvm(void)
{   bool ok = true;
    using CppAD::vector;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // np, nx, f
    size_t np = 1;
    size_t nx = 2;
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
    //
    size_t ny = f.Range();
    //
    // x, p
    vector<double> x(nx), p(np);
    for(size_t i = 0; i < np; ++i)
        p[i] = double(i + 4);
    for(size_t i = 0; i < nx; ++i)
        x[i] = double(i + np + 4);
    vector<double> check = algo(p, x);
    //
    // create a cpp_graph from this function
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    graph_obj.print(std::cout);
    //
    // create llvm context and module corresponding to this function
    std::unique_ptr<llvm::LLVMContext> context_ir = nullptr;
    std::unique_ptr<llvm::Module>      module_ir  = nullptr;
    ok &= graph2llvm_ir(graph_obj, context_ir, module_ir);
    //
    // print the correpsonding llvm function
    std::string     function_name = graph_obj.function_name_get();
    llvm::Function* function_ir   = module_ir->getFunction(function_name);
    llvm::raw_os_ostream os( std::cout );
    os << "\nBefore Optimization:\n";
    os << *function_ir;
    //
    // optimize the function and print result
    std::string msg;
    msg = optimize_llvm_ir(module_ir.get(), function_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    os << "\nAfter Optimization:\n";
    os << *function_ir;
    //
    // convert back to graph
    msg = llvm_ir2graph(
        graph_obj, module_ir.get(), function_name, np, nx, ny
    );
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    // print the graph
    graph_obj.print(std::cout);
    //
    // create an optimzied version of f
    CppAD::ADFun<double> g;
    g.from_graph(graph_obj);
    //
    // check
    g.new_dynamic(p);
    vector<double> y = g.Forward(0, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(y[i], check[i], eps99, eps99);
    //
    // test_object_file
    ok &= test_object_file(np, nx, ny, module_ir, function_name);
    //
    // test_jit
    ok &= test_jit( np, nx, ny, context_ir, module_ir, function_name);
    //
    return ok;
}
