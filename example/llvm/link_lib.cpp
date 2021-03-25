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

$section Example Linking a Library Function$$

$head CPPAD_STD_MATH_LIBRARY_PATH$$
This is the path to the standard math library which is used
by this example (to access the $code sin$$ function).

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
    CppAD::llvm_link link_obj(msg);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // load the standard math library (for cosine function)
    msg = link_obj.dynamic_lib(CPPAD_STD_MATH_LIBRARY_PATH);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
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
