# ifndef CPPAD_EXAMPLE_LLVM_GRAPH2LLVM_IR_HPP
# define CPPAD_EXAMPLE_LLVM_GRAPH2LLVM_IR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/graph/cpp_graph.hpp>
//
# include <llvm/IR/LLVMContext.h>
# include <llvm/IR/Module.h>
//
extern bool graph2llvm_ir(
    const CppAD::cpp_graph&             graph_obj  ,
    std::unique_ptr<llvm::LLVMContext>& context_ir ,
    std::unique_ptr<llvm::Module>&      module_ir
);

# endif
