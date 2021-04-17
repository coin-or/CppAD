# ifndef CPPAD_CORE_LLVM_FUNCTION_PTR_HPP
# define CPPAD_CORE_LLVM_FUNCTION_PTR_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/cppad_assert.hpp>
# include <cppad/core/llvm/link.hpp>
# include <cppad/local/llvm_error_msg.hpp>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*
-------------------------------------------------------------------------------
$begin llvm_link_function_ptr$$
$spell
    obj
    llvm
    jit
    ptr
$$

$section Get Pointer To A Compiled Function$$

$head Syntax$$
$icode%msg% = %link_obj%.function_ptr(%fun_name%, %fun_ptr%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head link_obj$$
Is a object constructed using $cref llvm_link_ctor$$.

$head fun_name$$
This the is the name of the function we are getting a pointer to.

$head function_ptr_t$$
This is the function pointer type.
If this function was compiled using $cref llvm_ir_to_object_file$$,
then $icode function_ptr_t$$ should be $cref compiled_ir_t$$.

$head fun_ptr$$
The input value of this argument does not matter.
Upon return, it is a pointer to a compiled function that has been loaded
using $cref llvm_link_object_file$$ or $cref llvm_link_dynamic_lib$$.
This function pointer is no longer valid when the $icode link_obj$$ is
deleted; e.g., drops out of scope.

$head msg$$
If the return value $icode msg$$ is the empty string, no error was detected.
Otherwise this is an error message and the return value of
$icode fun_ptr$$ is unspecified.

$head Example$$
The file $cref llvm_link_adfun.cpp$$ contains an example / test using
this member function.

$childtable%
    include/cppad/core/llvm/compiled_ir.hpp
%$$

$end
*/
// BEGIN_PROTOTYPE
template <class function_ptr_t>
std::string llvm_link::function_ptr(
    const std::string& fun_name  ,
    function_ptr_t&    fun_ptr   ) const
// END_PROTOTYPE
{   //
    // initialize msg
    std::string msg = "llvm_link::functon_ptr: ";
    //
    // symbol
    llvm::Expected<llvm::JITEvaluatedSymbol> error_or_symbol =
        jit_->lookup(fun_name);
    llvm::Error error = error_or_symbol.takeError();
    if( error )
    {   msg += "Error searching for " + fun_name + " in llvm_link object\n";
        msg += local::llvm_error_msg(error);
        //
        // Calling this handler clears the error and avoids an abort.  It
        // would be nice to make this part of llvm_error_msg, but that leads to
        // "undefined reference to `typeinfo for llvm::ErrorInfoBase'"
        llvm::handleAllErrors(
            std::move(error),
            [](const llvm::ErrorInfoBase& eib) {}
        );
        return msg;
    }
    llvm::JITEvaluatedSymbol symbol = error_or_symbol.get();
    //
    // fun_ptr
    fun_ptr = reinterpret_cast<function_ptr_t>( symbol.getAddress() );
    if( ! fun_ptr )
    {   msg += "Error getting address for function " + fun_name;
        return msg;
    }
    //
    // No error
    msg = "";
    return msg;
}

} // END_CPPAD_NAMESPACE

# endif
