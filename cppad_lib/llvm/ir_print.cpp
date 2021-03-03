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
    llvm_ir
    const
    obj
$$

$section Print LLVM Intermediate Representation$$

$head Syntax$$
$icode%ir_obj%.print()%$$

$head ir_obj$$
This is a $code const$$  $cref/llvm_ir/llvm_ir_ctor/$$ object.

$end
*/
# include <cppad/core/llvm_ir.hpp>
# include <llvm/Support/raw_os_ostream.h>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

void llvm_ir::print(void) const
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
    // os
    llvm::raw_os_ostream os( std::cout );
    // function_ir
    llvm::Function* function_ir = module_ir_->getFunction(function_name_);
    CPPAD_ASSERT_UNKNOWN( function_ir != nullptr );
    // output
    os << *function_ir;
    os.flush();
    return;
}

} // END_CPPAD_NAMESPACE
