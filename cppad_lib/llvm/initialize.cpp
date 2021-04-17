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
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/local/llvm_initialize.hpp>

# include <llvm/Support/CommandLine.h>
# include <llvm/Support/TargetSelect.h>
//
namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
-------------------------------------------------------------------------------
$begin llvm_initialize$$
$spell
    obj
    llvm
$$

$section Initialize LLVM Routines$$

$head Syntax$$
$icode%msg% = local::llvm_initialize()%$$

$head Purpose$$
This function performs initialization of llvm that only needs to be done once.
A static variable is used to ensure the initialization is only done once,
hence $code llvm_initialize$$  cannot be called in parallel mode.

$srccode%cpp% */
void llvm_initialize(void)
{   static bool first_call = true;
    CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
    if( first_call )
    {   first_call = false;
        //
        // We are not using comamnd line options
        int argc = 1;
        const char *argv[1];
        argv[0] = "llvm_initialize";
        llvm::cl::ParseCommandLineOptions(argc, argv);
        //
        // Initialize llvm Target
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
    }
    return;
}
/* %$$
$end
*/

} } // END_CPPAD_LOCAL_NAMESPACE
