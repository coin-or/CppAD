# ifndef CPPAD_CORE_LLVM_COMPILED_HPP
# define CPPAD_CORE_LLVM_COMPILED_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cstdint>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_LLVM_COMPILED_T
// error_no = function_ptr(len_input, input, len_output, output)
typedef int32_t (*llvm_compiled_t)(int32_t, double* , int32_t, double*);
// END_LLVM_COMPILED_T

} // END_CPPAD_NAMESPACE

# endif
