# ifndef CPPAD_EXAMPLE_LLVM_LLVM_IR2OBJ_FILE_HPP
# define CPPAD_EXAMPLE_LLVM_LLVM_IR2OBJ_FILE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
//
# include <llvm/IR/Module.h>
//
std::string llvm_ir2obj_file(
    llvm::Module*                       module_ir     ,
    const std::string&                  file_name
);

# endif
