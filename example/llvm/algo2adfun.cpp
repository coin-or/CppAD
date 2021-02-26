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
$begin llvm_algo2adfun$$
$spell
    algo
    adfun
    np
    nx
$$

$section LLVM Example: Create an ADFun Corresponding to Algorithm$$

$head Syntax$$
$icode%f% = algo2adfun(%np%, %nx%)%$$

$head np$$
This is the size of the dynamic parameter vector $icode p$$ for this example.

$head nx$$
This is the size of the independent variable vector $icode x$$ for this example.

$head algo$$
The algorithm in $cref llvm_algo$$ is an input to this routine.

$head f$$
The return value $icode f$$ represent the same function as the algorithm.
Suppose that $icode x$$ and $icode p$$ are $code double$$ vectors and
$codei%
    f.new_dynamic(p);
    y = f.Forward(0, x);
%$$
If follows that we can compute the same $icode y$$ as follows:
$icode%
    %y% = algo(%x%, %p%)
%$$

$end
*/
# include "llvm.hpp"
CppAD::ADFun<double> algo2adfun(size_t np, size_t nx)
{   using CppAD::AD;

    // ap, ax
    CppAD::vector< AD<double> > ap(np), ax(nx);
    for(size_t i = 0; i < np; ++i)
        ap[i] = AD<double>( i + 1 );
    for(size_t i = 0; i < nx; ++i)
        ax[i] = AD<double>( i + np );
    //
    // start recording with ax independent variables and ap dynamic parameters
    CppAD::Independent(ax, ap);
    //
    // ay
    CppAD::vector< AD<double> > ay = algo(ap, ax);
    //
    // f : x -> y
    CppAD::ADFun<double> f(ax, ay);
    //
    f.function_name_set("cppad_llvm_example");
    //
    return f;
}
