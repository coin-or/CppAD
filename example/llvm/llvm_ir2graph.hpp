# ifndef CPPAD_EXAMPLE_LLVM_LLVM_IR2GRAPH_HPP
# define CPPAD_EXAMPLE_LLVM_LLVM_IR2GRAPH_HPP
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
# include <llvm/IR/Module.h>
# include <llvm/IR/Constants.h>
# include <llvm/IR/InstIterator.h>
//
extern std::string llvm_ir2graph(
    CppAD::cpp_graph&                         graph_obj      ,
    const llvm::Module*                       module_ir      ,
    const std::string&                        function_name  ,
    size_t                                    n_dynamic_ind  ,
    size_t                                    n_variable_ind ,
    size_t                                    n_dependent
);

# endif
