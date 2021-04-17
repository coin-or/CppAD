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
$begin llvm_link_lib.cpp$$
$spell
    llvm
    obj
    cos
$$

$section Example Linking a Library Function$$

$head Standard Math Library$$
is automatically included using
$codei%
    %link_obj%.dynamic_lib("%path%");
%$$
where $icode path$$ is the path to the standard math library; see
The $cref/llvm_link initialize/llvm_link_initialize/Standard Math Library/$$.
This example calls the $code cos$$ function which is in this library.

$head Source Code$$
$srcthisfile%8%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/configure.hpp>
# include <cppad/core/llvm/link.hpp>
//
bool link_lib(void)
{   bool ok = true;
    //
    // link_obj
    std::string msg;
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    std::string          fun_name = "cos";
    double (*fun_ptr)(double);
    msg = link_obj.function_ptr(fun_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    double cos2 = fun_ptr(2.0);
    ok         &= cos2 == std::cos(2.0);
    //
    return ok;
}
// END C++
