# ifndef CPPAD_CORE_LLVM_LINK_HPP
# define CPPAD_CORE_LLVM_LINK_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/llvm/compiled_ir.hpp>
# include <llvm/ExecutionEngine/Orc/LLJIT.h>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

class llvm_link {
private:
    // llvm jit representations of functions
    std::unique_ptr<llvm::orc::LLJIT> jit_;
public:
    // ctor
    llvm_link(void);
    //
    // initailize
    std::string initialize(void);
    //
    // object_file
    std::string object_file(const std::string& file_name);
    //
    // dynamic_lib
    std::string dynamic_lib(const std::string& file_name);
    //
    // function_ptr
    template <typename function_ptr_t>
    std::string function_ptr(
        const std::string&  fun_name    ,
        function_ptr_t&     fun_ptr
    ) const;
};

} // END_CPPAD_NAMESPACE

// implementation of function_ptr (a template member function)
# include <cppad/core/llvm/function_ptr.hpp>

# endif
