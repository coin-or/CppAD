# ifndef CPPAD_EXAMPLE_LLVM_LLVM_LINK_HPP
# define CPPAD_EXAMPLE_LLVM_LLVM_LINK_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include "compiled_ad_fun.hpp"
# include <llvm/ExecutionEngine/Orc/LLJIT.h>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

class llvm_link {
private:
    // llvm jit representations of functions that is used for linking
    std::unique_ptr<llvm::orc::LLJIT> jit_;
public:
    // no-const member functions
    llvm_link(void);
    std::string load(const std::string& file_name);
    //
    // const member functions
    std::string compiled_ad_fun(
        const std::string&  file_name    ,
        compiled_ad_fun_t&  function_ptr
    ) const;
};

} // END_CPPAD_NAMESPACE

# endif
