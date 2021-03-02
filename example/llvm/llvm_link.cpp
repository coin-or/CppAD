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
# include <cppad/core/llvm_link.hpp>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
-------------------------------------------------------------------------------
$begin llvm_link_ctor$$
$spell
    llvm_obj
$$

$section Create an LLVM Object File Linker$$

$head Syntax$$
$codei%llvm_link %link_obj%;%$$

$head Purpose$$
This creates the empty $code llvm_link$$ object $icode link_obj$$.

$children%
    example/llvm/link_xam.cpp
%$$
$head Example$$
The file $cref llvm_link_xam.cpp$$ contains an example / test using
this member function.

$end
*/
llvm_link::llvm_link(void)
:
jit_( nullptr )
{   //
    // jit_
    llvm::Expected< std::unique_ptr<llvm::orc::LLJIT> > error_or_link =
        llvm::orc::LLJITBuilder().create();
    llvm::Error error = error_or_link.takeError();
    //
    if( error )
    {   // Error condition that must be checked for by other member functions
        CPPAD_ASSERT_UNKNOWN( jit_ == nullptr);
        return;
    }
    else
        jit_ = std::move( error_or_link.get() );
    CPPAD_ASSERT_UNKNOWN( jit_ != nullptr );
    return;
}
/*
-------------------------------------------------------------------------------
$begin llvm_link_load$$
$spell
    obj
    llvm
    jit
    ptr
$$

$section Load an Object File into LLVM Linker$$

$head Syntax$$
$icode%msg% = %link%_obj.load( %file_name% )%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_LOAD%// END_LOAD%1%$$

$head file_name$$
This is the name of the object file that was created by
created by $cref llvm_ir_to_object_file$$.

$head msg$$
If the return value $icode msg$$ is the empty string, no error was detected.
Otherwise this is an error message and the object file was not loaded.

$head Example$$
The file $cref llvm_link_xam.cpp$$ contains an example / test using
this member function.

$end
*/
// BEGIN_LOAD
std::string llvm_link::load(const std::string& file_name)
// END_LOAD
{   // 2DO: Figure out how to get the message from an llvm::Error
    // becasue Error::getError() function is deprecated.
    //
    // initialize msg
    std::string msg = "llvm_link::load: ";
    if( jit_ == nullptr )
    {   msg += "The constructor failed for object doing loading";
        return msg;
    }
    //
    // memory_buffer
    llvm::ErrorOr< std::unique_ptr<llvm::MemoryBuffer> > error_or_buffer =
        llvm::MemoryBuffer::getFile( file_name );
    std::error_code std_error_code = error_or_buffer.getError();
    if( std_error_code )
    {   msg += std_error_code.message();
        return msg;
    }
    std::unique_ptr<llvm::MemoryBuffer> memory_buffer(
        std::move( error_or_buffer.get() )
    );
    //
    // Add the objects to jit_
    llvm::Error error = jit_->addObjectFile( std::move(memory_buffer) );
    if( error )
    {   msg += "Error loading object file";
        return msg;
    }
    //
    // No error
    msg = "";
    return msg;
}
/*
-------------------------------------------------------------------------------
$begin llvm_link_compiled_ad_fun$$
$spell
    obj
    llvm
    jit
    ptr
$$

$section Get C++ Function Pointer To A Compiled AD Function$$

$head Syntax$$
$icode%msg% = %link_obj%.compiled_ad_fun(%function_name%, %function_ptr%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_COMPILED_AD_FUN%// END_COMPILED_AD_FUN%1%$$

$head compiled_ad_fun_t$$
This type is defined by
$srcfile%example/llvm/compiled_ad_fun.hpp%
    0%// BEGIN_COMPILED_AD_FUN_T%// END_COMPILED_AD_FUN_T%0
%$$

$head function_name$$
This the is the name of the function we are getting a pointer to.

$head function_ptr$$
The input value of this argument does not matter.
Upon return, it is a pointer to the corresponding C++ function.
This function pointer is no longer valid when the $icode link_obj$$ is
deleted; e.g., drops out of scope.

$head msg$$
If the return value $icode msg$$ is the empty string, no error was detected.
Otherwise this is an error message and the return value of
$icode function_ptr$$ is unspecified.

$head Example$$
The file $cref llvm_link_xam.cpp$$ contains an example / test using
this member function.

$end
*/
// BEGIN_COMPILED_AD_FUN
std::string llvm_link::compiled_ad_fun(
    const std::string& function_name ,
    compiled_ad_fun_t& function_ptr  ) const
// END_COMPILED_AD_FUN
{   // 2DO: Figure out how to get the message for an llvm::Error
    //
    // initialize msg
    std::string msg = "llvm::compiled_ad_fun: ";
    //
    // symbol
    llvm::Expected<llvm::JITEvaluatedSymbol> error_or_symbol =
        jit_->lookup(function_name);
    llvm::Error error = error_or_symbol.takeError();
    if( error )
    {   msg += "Error searching for " + function_name + " in object file";
        return msg;
    }
    llvm::JITEvaluatedSymbol symbol = error_or_symbol.get();
    //
    // function_cpp
    function_ptr = reinterpret_cast<compiled_ad_fun_t>( symbol.getAddress() );
    if( ! function_ptr )
    {   msg += "Error looking up address for function " + function_name;
        return msg;
    }
    //
    // No error
    msg = "";
    return msg;
}

} // END_CPPAD_NAMESPACE
