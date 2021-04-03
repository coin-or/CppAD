# ifndef CPPAD_CORE_LLVM_COMPILED_IR_HPP
# define CPPAD_CORE_LLVM_COMPILED_IR_HPP
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
$begin compiled_ir_t$$
$spell
    llvm
    len
    ir
    ptr
    cpp
$$

$section C++ Function Type for a Compiled llvm_ir Object$$

$head Syntax$$
$codei%compiled_ir_t %fun_ptr%
%$$
$icode%error_no% = %fun_ptr%(%len_input%, %input%, %len_output%, %output%)
%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head fun_ptr$$
This is a pointer to a function compiled, and stored in an object file,
using $cref llvm_ir_to_object_file$$.

$head len_input$$
This is the length of the input vector which is equal to
the sum of the number of independent dynamic parameters and
number of independent variables.

$head input$$
This is the input vector for the function.
The independent dynamic parameters come first,
and then the independent variables.
This vector is not modified.

$head len_output$$
This is the length of the output vector which is equal to
the sum of the number of dependent variables.

$head output$$
The input value of the elements of this vector does not matter.
Upon return, it contains the dependent variable values corresponding
to the independent variables and dynamic parameters.

$head error_no$$

$subhead Zero$$
If the return value $icode error_no$$ is zero, no error was detected.

$subhead One$$
If the return value $icode error_no$$ is one,
one or more of the comparisons did not match its operator value.

$subhead Two$$
If the return value $icode error_no$$ is two,
the value of $icode len_input$$ is not correct.
In this case $icode output$$ is not changed.

$subhead Three$$
If the return value $icode error_no$$ is three,
the value of $icode len_output$$ is not correct.
In this case $icode output$$ is not changed.

$table
$icode error_no$$ $pre $$ $cnext Meaning  $rnext
0  $cnext No error occurred                                         $rnext
1  $cnext One (or more) comparison did not match its operator value $rnext
2  $cnext $icode len_input$$ is incorrect                           $rnext
3  $cnext $icode len_output$$ is incorrect
$tend

$head Atomic Functions$$
The function type $code compiled_ir_t$$
is also used for the $code double$$ implementation of
zero order forward mode for atomic functions; e.g.,
$cref/atomic.cpp/llvm_link_atomic.cpp/atomic.cpp/$$.
The difference is that $icode input$$ and $icode output$$
are relative to the atomic function,
not variables and parameters in a corresponding ADFun object.

$end
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
typedef int32_t (*compiled_ir_t)(int32_t, const double* , int32_t, double*);
// END_PROTOTYPE

} // END_CPPAD_NAMESPACE

# endif
