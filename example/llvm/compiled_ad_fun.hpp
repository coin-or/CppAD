# ifndef CPPAD_EXAMPLE_LLVM_COMPILED_AD_FUN_HPP
# define CPPAD_EXAMPLE_LLVM_COMPILED_AD_FUN_HPP
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

// BEGIN_COMPILED_AD_FUN_T
// error_no = function_ptr(input, output)
typedef int32_t (*compiled_ad_fun_t)(double* , double* );
// END_COMPILED_AD_FUN_T

} // END_CPPAD_NAMESPACE

# endif
