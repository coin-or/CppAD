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
$begin optimize_llvm_ir$$
$spell
    llvm_ir
$$

$section Optimize an LLVM Intermediate Representation$$

$head Syntax$$
$icode%msg% = optimize_llvm_ir(%function_name, %module_ir%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head module_ir$$
This is the llvm module that contains the function.

$head function_name$$
This is the name of the function (in the module) that we are optimizing.

$head msg$$
If the return value $icode msg$$ is the empty string,
no error was detected.
Otherwise, $icode msg$$ describes the error and the function has not been
optimized.
$end
*/
# include "optimize_llvm_ir.hpp"
//
# include <llvm/IR/LegacyPassManager.h>
# include <llvm/Transforms/InstCombine/InstCombine.h>
# include <llvm/Transforms/Scalar.h>
# include <llvm/Transforms/Scalar/GVN.h>
//
// BEGIN_PROTOTYPE
std::string optimize_llvm_ir(
    llvm::Module*                  module_ir     ,
    const std::string&             function_name )
// END_PROTOTYPE
{   //
    // initialize return value with name of this routine
    std::string msg = "optimize_llvm_ir";
    //
    // function_ir
    llvm::Function* function_ir = module_ir->getFunction(function_name);
    if( ! function_ir )
    {   msg += "Cannot find function " + function_name + " in llvm_ir";
        return msg;
    }
    // optimization_pass_manager
    std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
    function_pass_manager =
        std::make_unique<llvm::legacy::FunctionPassManager>( module_ir );
    //
    // Do simple "peephole" optimizations and bit-twiddling optzns.
    function_pass_manager->add(llvm::createInstructionCombiningPass());
    // Reassociate expressions.
    function_pass_manager->add(llvm::createReassociatePass());
    // Eliminate Common SubExpressions.
    function_pass_manager->add(llvm::createGVNPass());
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    function_pass_manager->add(llvm::createCFGSimplificationPass());
    //
    function_pass_manager->doInitialization();
    //
    // run the optimizer on the function
    function_pass_manager->run(*function_ir);
    //
    // No error
    msg = "";
    return msg;
}
