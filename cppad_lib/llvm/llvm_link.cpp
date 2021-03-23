/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <sstream>
# include <fstream>
# include <llvm/Support/raw_os_ostream.h>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/core/llvm/link.hpp>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// Under Construction
std::string llvm_link::dynamic_lib(const std::string& file_name)
{   typedef llvm::orc::DynamicLibrarySearchGenerator generator_t;
    typedef llvm::orc::JITDylib                      jit_dylib_t;
    //
    // initialize msg
    std::string msg = "llvm_link::dynamic_lib: ";
    if( jit_ == nullptr )
    {   msg += "The constructor failed for object linking library";
        return msg;
    }
    //
    // is
    std::ifstream is(file_name.c_str(), std::ifstream::binary);
    if( ! is )
    {   msg += "error reading library " + file_name;
        return msg;
    }
    //
    // length
    is.seekg(0, is.end);
    size_t length = is.tellg();
    length        = std::min(length, size_t(1000) );
    //
    // buffer
    is.seekg(0, is.beg);
    char* buffer = new char[length];
    is.read( buffer, int(length) );
    //
    // is_gnu_ld_script
    const char* pattern   = "/* GNU ld script\n";
    const char* match     = std::strstr(buffer, pattern);
    bool is_gnu_ld_script = match == buffer;
    //
    // mapped_file_name
    std::string mapped_file_name = file_name;
    if( is_gnu_ld_script )
    {   match = std::strstr(buffer, "GROUP");
        if( match == nullptr )
        {   msg += "cannot find GROUP in GNU ld script " + file_name;
            return msg;
        }
        size_t start = match + 5 - buffer;
        while( start < length && (buffer[start]==' ' || buffer[start]=='(') )
            ++start;
        size_t end = start;
        while( end < length && buffer[end] == ' ' )
            ++end;
        while( end < length && buffer[end] != ' ' )
            ++end;
        if( end == start || end == length )
        {   msg += "cannot find file name in GNU ld script " + file_name;
            return msg;
        }
        mapped_file_name.resize(end - start);
        for(size_t i = start; i < end; ++i)
            mapped_file_name[i - start] = buffer[i];
    }
    delete [] buffer;
    //
    // data_layout
    const llvm::DataLayout& data_layout  = jit_->getDataLayout();
    //
    // error_or_generator
    llvm::Expected< std::unique_ptr<generator_t> > error_or_generator =
        generator_t::Load(mapped_file_name.c_str(), data_layout.getGlobalPrefix());
    llvm::Error error = error_or_generator.takeError();
    if( error )
    {   std::stringstream ss;
        llvm::raw_os_ostream os( ss );
        os << error;
        os.flush();
        msg += "error linking the library " + file_name + "\n";
        if( file_name != mapped_file_name )
            msg += "mapped file name = " + mapped_file_name + "\n";
        msg += ss.str();
        return msg;
    }
    // gen
    std::unique_ptr<generator_t> gen_ptr = std::move(error_or_generator.get());
    //
    // jit_dylib
    jit_dylib_t& jit_dylib = jit_->getMainJITDylib();
    //
    jit_dylib.addGenerator( std::move(gen_ptr) );
    //
    msg = "";
    return msg;
}

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
    jit_ = std::move( error_or_link.get() );
    CPPAD_ASSERT_UNKNOWN( jit_ != nullptr );
    return;
}
/*
-------------------------------------------------------------------------------
$begin llvm_link_object_file$$
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
std::string llvm_link::object_file(const std::string& file_name)
// END_LOAD
{   // 2DO: Figure out how to get the message from an llvm::Error
    // becasue Error::getError() function is deprecated.
    //
    // initialize msg
    std::string msg = "llvm_link::object_file: ";
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
$begin llvm_link_compiled$$
$spell
    obj
    llvm
    jit
    ptr
$$

$section Get C++ Function Pointer To A Compiled AD Function$$

$head Syntax$$
$icode%msg% = %link_obj%.compiled(%function_name%, %function_ptr%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_COMPILED_AD_FUN%// END_COMPILED_AD_FUN%1%$$

$head function_name$$
This the is the name of the function we are getting a pointer to.

$head function_ptr$$
The input value of this argument does not matter.
Upon return, it is a
$cref llvm_compiled_t$$ pointer to the corresponding C++ function.
This function pointer is no longer valid when the $icode link_obj$$ is
deleted; e.g., drops out of scope.

$head msg$$
If the return value $icode msg$$ is the empty string, no error was detected.
Otherwise this is an error message and the return value of
$icode function_ptr$$ is unspecified.

$head Example$$
The file $cref llvm_link_xam.cpp$$ contains an example / test using
this member function.

$childtable%
    include/cppad/core/llvm_compiled.hpp
%$$

$end
*/
// BEGIN_COMPILED_AD_FUN
std::string llvm_link::compiled(
    const std::string& function_name ,
    llvm_compiled_t&   function_ptr  ) const
// END_COMPILED_AD_FUN
{   // 2DO: Figure out how to get the message for an llvm::Error
    //
    // initialize msg
    std::string msg = "llvm::compiled: ";
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
    function_ptr = reinterpret_cast<llvm_compiled_t>( symbol.getAddress() );
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
