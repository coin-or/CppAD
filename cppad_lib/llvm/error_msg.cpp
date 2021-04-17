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
# include <llvm/Support/raw_os_ostream.h>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/llvm_error_msg.hpp>
//
namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
-------------------------------------------------------------------------------
$begin llvm_error_msg$$
$spell
    obj
    llvm
$$

$section Get Message From an LLVM Error Object$$

$head Syntax$$
$icode%msg% = local::llvm_error_msg(%error_obj%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head error_obj$$
Is a llvm error object that $codei%if( %errro_obj% )%$$ is true.
Upon return, the error has been cleared $icode error_obj$$
is no longer usable.


$head msg$$
The return value $icode msg$$ is the message corresponding to the error.

$end
*/
// BEGIN_PROTOTYPE
std::string llvm_error_msg(llvm::Error& error_obj)
// END_PROTOTYPE
{   // ss
    std::stringstream ss;
    //
    // put error messsag in ss
    llvm::raw_os_ostream os( ss );
    os << error_obj;
    os.flush();
    //
    // clear error_obj
    llvm::handleAllErrors(
        std::move(error_obj),
        [](const llvm::ErrorInfoBase& eib) {}
    );
    return ss.str();
}

} } // END_CPPAD_LOCAL_NAMESPACE
