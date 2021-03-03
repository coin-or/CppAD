# ifndef CPPAD_CORE_LLVM_COMPILED_HPP
# define CPPAD_CORE_LLVM_COMPILED_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cstdint>
/*
$begin llvm_compiled_t$$
$spell
    llvm
    ptr
    len
$$

$section C++ Function Type for LLVM Compiled Functions$$

$head Syntax$$
$codei%llvm_compiled_t %function_ptr%
%$$
$icode%error_no% = %function_ptr%(%len_input%, %input%, %len_output%, %output%)
%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head function_ptr$$
This is a function pointer returned by
$cref llvm_link_compiled$$.

$head len_input$$
This is the length of the input vector which is equal to
the sum of the number of independent dynamic parameters and
number of independent variables.

$head input$$
This is the input vector for the function.
The independent dynamic parameters come first,
and then the independent variables.

$head len_output$$
This is the length of the output vector which is equal to
the sum of the number of dependent variables.

$head output$$
The input value of the elements of this vector does not matter.
Upon return, it contains the dependent variable values corresponding
to the independent variables.

$end
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
typedef int32_t (*llvm_compiled_t)(int32_t, const double* , int32_t, double*);
// END_PROTOTYPE

} // END_CPPAD_NAMESPACE

# endif
