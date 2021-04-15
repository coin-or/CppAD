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
-----------------------------------------------------------------------------
$begin llvm_ir_print$$
$spell
    std::ostream
    llvm_ir
    const
    obj
$$

$section Print LLVM Intermediate Representation$$

$head Syntax$$
$icode%ir_obj%.print(%os%)%$$

$head ir_obj$$
This is a const $cref/llvm_ir/llvm_ir_ctor/$$ object.

$head os$$
Is the $code std::ostream$$ where the IR is printed.

$head Discussion$$
This function is included to help with using the $code llvm_ir$$ class.
The formatting of it's output is not part of the API; i.e., it may change
it the future.

$head IR Specifications$$
see $href%https://llvm.org/docs/LangRef.html%$$

$children%
    example/llvm/print_ir.cpp
%$$
$head Example$$
The file $cref llvm_print_ir.cpp$$ contains an example and test
of this operation.

$end
*/
# include <cppad/core/llvm/ir.hpp>
# include <llvm/Support/raw_os_ostream.h>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

void llvm_ir::print(std::ostream& os) const
{
    if( function_name_ == "" )
    {   CPPAD_ASSERT_UNKNOWN( n_dynamic_ind_ == 0 );
        CPPAD_ASSERT_UNKNOWN( n_variable_ind_ == 0 );
        CPPAD_ASSERT_UNKNOWN( n_variable_dep_ == 0 );
        CPPAD_ASSERT_UNKNOWN( context_ir_ == nullptr );
        CPPAD_ASSERT_UNKNOWN( module_ir_   == nullptr );
        //
        std::cout << "llvm_ir::print: empty function\n";
        return;
    }
    // llvm_os
    llvm::raw_os_ostream llvm_os( os );
    //
    // print module
    llvm_os << *module_ir_;
    //
    /* only print function
    llvm::Function* ir_function = module_ir_->getFunction(function_name_);
    CPPAD_ASSERT_UNKNOWN( ir_function != nullptr );
    llvm_os << *ir_function;
    */
    llvm_os.flush();
    return;
}

} // END_CPPAD_NAMESPACE
