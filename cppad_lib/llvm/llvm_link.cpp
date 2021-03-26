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
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*
-------------------------------------------------------------------------------
$begin llvm_link_ctor$$
$spell
    llvm_obj
    cppad
    CppAD
$$

$section Create an LLVM Object File Linker$$

$head Syntax$$
$codei%llvm_link %link_obj%(%msg%);%$$

$head Purpose$$
This creates the empty $code llvm_link$$ object $icode link_obj$$.

$head msg$$
If input value of $icode msg$$ does not matter.
If it is the empty string upon return, no error was detected.
Otherwise it is an error message and $icode link_obj$$ cannot be used.

$head Standard Math Library$$
The standard math library is automatically included in
$icode link_obj$$ during the constructor.

$head cppad_link_$$
Function names that begin with $code cppad_link_$$ are reserved
for use by CppAD.

$children%
    example/llvm/link_lib.cpp%
    example/llvm/link_adfun.cpp
%$$

$head Example$$
The files $cref llvm_link_lib.cpp$$, $cref llvm_link_adfun.cpp$$
contain examples / tests using this member function.

$end
*/
// BEGIN_CTOR
llvm_link::llvm_link(std::string& msg)
// END_CTOR
{   //
    // jit_
    llvm::Expected< std::unique_ptr<llvm::orc::LLJIT> > error_or_link =
        llvm::orc::LLJITBuilder().create();
    llvm::Error error = error_or_link.takeError();
    //
    if( error )
    {   msg += "llvm_link_ctor: " + local::llvm_error_msg(error);
        jit_ = nullptr;
        return;
    }
    jit_ = std::move( error_or_link.get() );
    CPPAD_ASSERT_UNKNOWN( jit_ != nullptr );
    //
    // link the standard math library
    msg = dynamic_lib(CPPAD_STD_MATH_LIBRARY_PATH);
    if( msg != "" )
    {   jit_ = nullptr;
        return;
    }
    //
    // add cppad_link library
    msg = dynamic_lib(CPPAD_LINK_LOCAL_PATH);
    if( msg != "" )
    {   msg = dynamic_lib(CPPAD_LINK_INSTALLED_PATH);
        if( msg != "" )
        {   msg = "llvm_link_ctor: " + msg;
            jit_ = nullptr;
            return;
        }
    }
    //
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

$section Add an Object File into LLVM Linker$$

$head Syntax$$
$icode%msg% = %link_obj%.load( %file_name% )%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_OBJECT_FILE%// END_OBJECT_FILE%1%$$

$head link_obj$$
Is a object constructed using $cref llvm_link_ctor$$.

$head file_name$$
This is the name of the object file that was created by
created by $cref llvm_ir_to_object_file$$.

$head msg$$
If the return value $icode msg$$ is the empty string, no error was detected.
Otherwise this is an error message and the object file was not loaded.

$head Example$$
The file $cref llvm_link_adfun.cpp$$ contains an example / test using
this member function.

$end
*/
// BEGIN_OBJECT_FILE
std::string llvm_link::object_file(const std::string& file_name)
// END_OBJECT_FILE
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

} // END_CPPAD_NAMESPACE
