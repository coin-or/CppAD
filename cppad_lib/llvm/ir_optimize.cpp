/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/llvm_ir.hpp>
//
# include <llvm/IR/DerivedTypes.h>
# include <llvm/IR/InstIterator.h>
# include <llvm/IR/IRBuilder.h>
# include <llvm/IR/LegacyPassManager.h>
# include <llvm/IR/Type.h>
# include <llvm/IR/Verifier.h>
//
# include <llvm/Support/FileSystem.h>
# include <llvm/Support/Host.h>
# include <llvm/Support/raw_os_ostream.h>
# include <llvm/Support/TargetRegistry.h>
# include <llvm/Support/TargetSelect.h>
//
# include <llvm/Target/TargetOptions.h>
# include <llvm/Target/TargetMachine.h>
//
//
# include <llvm/Transforms/InstCombine/InstCombine.h>
# include <llvm/Transforms/Scalar.h>
# include <llvm/Transforms/Scalar/GVN.h>
/*
------------------------------------------------------------------------------
$begin llvm_ir_optimize$$
$spell
    llvm_ir
    obj
$$

$section Optimize an LLVM Intermediate Representation$$

$head Syntax$$
$icode%ir_obj%.optimize()%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_OPTIMIZE%// END_OPTIMIZE%1%$$

$head ir_obj$$
This is a $cref/llvm_ir/llvm_ir_ctor/$$ object.
It contains an LLVM Intermediate Representation (IR)
that is optimized.

$children%
    example/llvm/optimize.cpp
%$$
$head Example$$
The file $cref llvm_optimize.cpp$$ contains an example and test
of this operation.

$end
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_OPTIMIZE
void llvm_ir::optimize(void)
// END_OPTIMIZE
{
    // function_ir
    llvm::Function* function_ir = module_ir_->getFunction(function_name_);
    CPPAD_ASSERT_UNKNOWN( function_ir != nullptr )
    //
    // optimization_pass_manager
    std::unique_ptr<llvm::legacy::FunctionPassManager> function_pass_manager;
    function_pass_manager =
        std::make_unique<llvm::legacy::FunctionPassManager>( module_ir_.get() );
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

} // END_CPPAD_NAMESPACE
