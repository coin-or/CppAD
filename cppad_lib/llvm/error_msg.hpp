# ifndef CPPAD_CPPAD_LIB_LLVM_ERROR_MSG_HPP
# define CPPAD_CPPAD_LIB_LLVM_ERROR_MSG_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <string>
# include <llvm/Support/Error.h>

namespace CppAD {
    std::string llvm_error_msg(const llvm::Error& error_obj);
}

# endif
