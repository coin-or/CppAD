# ifndef CPPAD_EXAMPLE_LLVM_LLVM_IR_HPP
# define CPPAD_EXAMPLE_LLVM_LLVM_IR_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <llvm/IR/Module.h>
# include <cppad/core/graph/cpp_graph.hpp>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

class llvm_ir {
private:
    std::string                        function_name_;
    size_t                             n_dynamic_ind_;
    size_t                             n_variable_ind_;
    size_t                             n_variable_dep_;
    std::unique_ptr<llvm::LLVMContext> context_ir_;
    std::unique_ptr<llvm::Module>      module_ir_;
public:
    llvm_ir(void);
    std::string from_graph(const cpp_graph& graph_obj);
    std::string to_graph(cpp_graph& graph_obj) const;
    void print(void) const;
};

} // END_CPPAD_NAMESPACE

# endif
