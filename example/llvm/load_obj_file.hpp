# ifndef CPPAD_EXAMPLE_LLVM_LOAD_OBJ_FILE_HPP
# define CPPAD_EXAMPLE_LLVM_LOAD_OBJ_FILE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <llvm/ExecutionEngine/Orc/LLJIT.h>
// function_ptr_t
typedef void (*function_ptr_t)(double*, double*);
//
// load_obj_file
extern std::string load_obj_file(
    const std::string file_name     ,
    const std::string function_name ,
    llvm::orc::LLJIT* llvm_jit      ,
    function_ptr_t&   function_ptr
);

# endif
