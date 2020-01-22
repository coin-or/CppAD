/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin cppadcg_det_minor_grad.c$$
$spell
    Cppadcg
    det
$$

$section Cppadcg Speed: Gradient of Determinant by Minor Expansion$$

$head Syntax$$
$icode%flag% = det_minor_grad(%size%, %x%, %y%)%$$

$head Prototype$$
$srccode%c%
extern "C" int det_minor_grad(int size, const double* x, double* y)
%$$

$head Purpose$$
Computes the gradient of the determinant for a square matrix.

$head size$$
Is the row and column dimension of the square matrix.
This function is only implemented for one value of $icode size$$.
You can create a version of this function for a different value of
$icode size$$ by calling $cref/cppadcg_det_minor/cppad_det_minor.cpp/$$.

$head x$$
Is the values in the matrix stored in either row or column major order.
The length of this vector is the product of its row and column dimension;
i.e., $icode%size%*size%$$.

$head y$$
Is the gradient of the determinant function.
It size is the same as $icode x$$.
The length of this vector is the same as $icode x$$.
The input value of it's elements does not matter.
Upon return the $th j$$ component of $icode y$$
is the partial of the determinant
with respect to the $th j$$ component of $icode x$$.

$head flag$$
This value is zero if the correct value of $icode size$$ is passed in.
Otherwise,
$icode y$$ is not changed and the return value of flag is $code 1$$.

$head Implementation$$
The following is an implementation for the case when $icode size$$
is one:
$srccode%c% */
int det_minor_grad(int size, const double* x, double* y)
{   if( size != 1 )
        return 1;
    y[0] = 1.0;
    return 0;
}
/* %$$
$end
*/
