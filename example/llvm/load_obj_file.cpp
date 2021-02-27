/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <iostream>
# include "load_obj_file.hpp"
//
bool load_obj_file(
    const std::string file_name     ,
    const std::string function_name ,
    llvm::orc::LLJIT* llvm_jit      ,
    function_ptr_t&   function_ptr  )
{   bool ok = true;
    //
    //
    // memory_buffer
    llvm::ErrorOr< std::unique_ptr<llvm::MemoryBuffer> > error_or_buffer =
        llvm::MemoryBuffer::getFile( file_name );
    if( ! error_or_buffer )
    {   std::cerr << "Cannot load " << file_name << "\n";
        return false;
    }
    std::unique_ptr<llvm::MemoryBuffer> memory_buffer(
        std::move( error_or_buffer.get() )
    );
    //
    // exit_on_error
    llvm::ExitOnError exit_on_error;
    //
    // add function corresponding to file_name to llvm_jit
    exit_on_error( llvm_jit->addObjectFile( std::move(memory_buffer) ) );
    //
    // function_jit
    // Look up the JIT'd function
    auto function_jit = exit_on_error( llvm_jit->lookup(function_name) );
    //
    // function_cpp
    function_ptr = reinterpret_cast<function_ptr_t>(
        function_jit.getAddress()
    );
    //
    return ok;
}
