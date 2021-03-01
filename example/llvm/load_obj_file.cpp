/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin load_obj_file$$
$spell
    obj
    llvm
    jit
    ptr
$$

$section Load a C++ Function from an Object file$$

$head Syntax$$
$icode%msg% = load_obj_file(
    %file_name%, %function_name%, %llvm_jit%, %function_ptr%
)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head file_name$$
This is the name of the object file,
possibly created by $cref llvm_ir_to_object_file$$,
that contains the function.

$head function_name$$
This the is the name of the function we are loading into memory.

$head llvm_jit$$
This is the llvm structure that will hold the function's code.
These loads are cumulative; i.e. functions that were previously loaded
are not lost.

$head function_ptr$$
The input value of this argument does not matter.
Upon return, it is a pointer to the corresponding C++ function.

$head msg$$
If the return value $icode msg$$ is the empty string,
no error was detected.
Otherwise this is an error message and the function
was not loaded.

$end
*/
# include <iostream>
# include "load_obj_file.hpp"
//
// BEGIN_PROTOTYPE
std::string load_obj_file(
    const std::string file_name     ,
    const std::string function_name ,
    llvm::orc::LLJIT* llvm_jit      ,
    function_ptr_t&   function_ptr  )
// END_PROTOTYPE
{   // 2DO: Figure out how to get the message for an llvm::Error
    //
    // initialize msg
    std::string msg = "load_obj_file: ";
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
    // Add function corresponding to file_name to llvm_jit
    llvm::Error error = llvm_jit->addObjectFile( std::move(memory_buffer) );
    if( error )
    {   msg += "Error loading object file into llvm_jit";
        return msg;
    }
    //
    // symbol
    llvm::Expected<llvm::JITEvaluatedSymbol> error_or_symbol =
        llvm_jit->lookup(function_name);
    error = error_or_symbol.takeError();
    if( error )
    {   msg += "Error searching for " + function_name + " in object file";
        return msg;
    }
    llvm::JITEvaluatedSymbol symbol = error_or_symbol.get();
    //
    // function_cpp
    function_ptr = reinterpret_cast<function_ptr_t>( symbol.getAddress() );
    if( ! function_ptr )
    {   msg += "Error looking up address for function " + function_name;
        return msg;
    }
    //
    // No error
    msg = "";
    return msg;
}
