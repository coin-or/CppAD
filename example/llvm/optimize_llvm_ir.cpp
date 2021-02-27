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

$section Optimize an LLVM Intermediate Representation$$

$end
*/
# include "optimize_llvm_ir.hpp"
//
# include <llvm/IR/LegacyPassManager.h>
# include <llvm/Transforms/InstCombine/InstCombine.h>
# include <llvm/Transforms/Scalar.h>
# include <llvm/Transforms/Scalar/GVN.h>
//
void optimize_llvm_ir(
    llvm::Function*                function_ir  ,
    std::unique_ptr<llvm::Module>& module_ir    )
{
    // optimization_pass_manager
    std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
    function_pass_manager =
        std::make_unique<llvm::legacy::FunctionPassManager>( module_ir.get() );
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
    return;
}
