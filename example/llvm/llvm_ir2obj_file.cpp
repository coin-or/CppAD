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
$begin llvm_ir2obj_file$$
$spell
    llvm_ir
    obj
$$

$section Create Object File Corresponding to a Module$$

$head Syntax$$
$icode%msg% = llvm_ir2obj_file(%module_ir%, %file_name%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head module_ir$$
This is the llvm module that we are creating a object file for.

$head file_name$$
This is the name of the file (not the function in the file)
that is created.
This can be a local or global path.

$head Side Effects$$
The target information in $icode module_ir$$ is set to the current host; i.e,
the machine that is currently executing.

$end
*/
# include "llvm_ir2obj_file.hpp"
//
# include <llvm/IR/LegacyPassManager.h>
//
# include <llvm/Support/TargetSelect.h>
# include <llvm/Support/Host.h>
# include <llvm/Support/TargetRegistry.h>
# include <llvm/Support/FileSystem.h>
//
# include <llvm/Target/TargetOptions.h>
# include <llvm/Target/TargetMachine.h>
//
// BEGIN_PROTOTYPE
std::string llvm_ir2obj_file(
    llvm::Module*             module_ir  ,
    const std::string&        file_name  )
// END_PROTOTYPE
{   //
    // initialize error message as starting with name of this routine
    std::string msg = "llvm_ir2obj_file: ";
    //
    // Initialize llvm Target functions
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    //
    // target
    std::string          target_triple = llvm::sys::getDefaultTargetTriple();
    std::string          error_message;
    const  llvm::Target* target        = llvm::TargetRegistry::lookupTarget(
        target_triple, error_message
    );
    if( ! target )
    {   msg += error_message;
        return msg;
    }
    //
    // target_machine
    const char*                        cpu      = "generic";
    const char*                        features = "";
    llvm::TargetOptions                target_options;
    llvm::Optional<llvm::Reloc::Model> reloc_model;
    llvm::TargetMachine*  target_machine = target->createTargetMachine(
        target_triple, cpu, features, target_options, reloc_model
    );
    //
    // set target for this module
    module_ir->setTargetTriple(target_triple);
    //
    // set data layout for this module
    module_ir->setDataLayout( target_machine->createDataLayout() );
    //
    // output_stream
    std::error_code          std_error_code;
    llvm::sys::fs::OpenFlags open_flag = llvm::sys::fs::OF_None;
    llvm::raw_fd_ostream output_stream(
        file_name, std_error_code, open_flag
    );
    if( std_error_code )
    {   msg += std_error_code.message();
        return msg;
    }
    //
    // pass_manager
    llvm::legacy::PassManager pass_manager;
    llvm::CodeGenFileType     file_type = llvm::CGFT_ObjectFile;
    llvm::raw_pwrite_stream*  dwo_out   = nullptr;
    bool not_supported = target_machine->addPassesToEmitFile(
        pass_manager, output_stream, dwo_out, file_type
    );
    if( not_supported )
    {   msg += "Cannot emit object code for " + target_triple;
        return msg;
    }
    //
    // run the passes and create the output file
    pass_manager.run(*module_ir);
    output_stream.flush();
    //
    // No error
    msg = "";
    return msg;
}
