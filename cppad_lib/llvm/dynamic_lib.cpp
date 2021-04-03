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
# include <cppad/core/llvm/link.hpp>
# include <cppad/local/llvm_error_msg.hpp>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
-------------------------------------------------------------------------------
$begin llvm_link_dynamic_lib$$
$spell
    obj
$$

$section Add a Dynamic Library File into LLVM Linker$$

$head Syntax$$
$icode%msg% = %link_obj%.dynamic_lib( %file_name% )%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head link_obj$$
Is a object constructed using $cref llvm_link_ctor$$.

$head file_name$$
This is the name of the dynamic link library.

$head msg$$
If the return value $icode msg$$ is the empty string, no error was detected.
Otherwise this is an error message and the object file was not loaded.

$children%
    example/llvm/link_discrete.cpp%
    example/llvm/link_atomic.cpp
%$$
$head Example$$
The files
$cref llvm_link_discrete.cpp$$ and $cref llvm_link_atomic.cpp$$
contain examples / tests using this member function,.

$end
*/
// BEGIN_PROTOTYPE
std::string llvm_link::dynamic_lib(const std::string& file_name)
// END_PROTOTYPE
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
    size_t length = size_t( is.tellg() );
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
        size_t start = size_t( match + 5 - buffer);
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
    char  global_prefix                  = data_layout.getGlobalPrefix();
    //
    // error_or_generator
    llvm::Expected< std::unique_ptr<generator_t> > error_or_generator =
        generator_t::Load(mapped_file_name.c_str(), global_prefix);
    llvm::Error error_obj = error_or_generator.takeError();
    if( error_obj )
    {   msg += "error linking the library " + file_name + "\n";
        if( file_name != mapped_file_name )
            msg += "mapped file name = " + mapped_file_name + "\n";
        msg += local::llvm_error_msg(error_obj);
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

} // END_CPPAD_NAMESPACE
