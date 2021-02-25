# ifndef CPPAD_EXAMPLE_LLVM_ALGORITHM_HPP
# define CPPAD_EXAMPLE_LLVM_ALGORITHM_HPP
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
$begin llvm_algorithm$$
$spell
$$

$section LLVM Example: Template Function that Evaluates Algorithm$$

$head Syntax$$
$icode%y% = algorithm(%p%, %x%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head VectorFloat$$
This is a $cref SimpleVector$$

$head value_type$$
We use this as an abbreviation for the type of the elements; i.e.,
$icode%VectorFloat%::value_type%$$.
This type must support binary addition.
More possible operators will be included in the future.

$head p$$
This input vector
is intended to represent the dynamic parameters in the function.
We use $latex \ell$$ to denote the size of this vector.

$head x$$
This input vector
is intended to represent the independent variables in the function.
We use $latex n$$ to denote the size of this vector.

$head y$$
This output vector
is the result of applying the algorithm with inputs $icode p$$ and $icode x$$.
We use $latex m$$ to denote the size of this vector.
The algorithm evaluates a function
$latex \[
    f : \B{R}^\ell \times \B{R}^n  \rightarrow \B{R}^m
\] $$.
Thus the size of $icode y$$ does not depend on the value of
$icode x$$ or $icode p$$.
Thus the size of $icode y$$ does not depend on the value of
$icode x$$ or $icode p$$.

$end
*/
// BEGIN_PROTOTYPE
template <class VectorFloat>
VectorFloat algorithm(const VectorFloat& p, const VectorFloat& x)
// END_PROTOTYPE
{   size_t nx = x.size();
    size_t np = p.size();
    size_t ny = nx + np;
    //
    // define the actual function
    VectorFloat y(ny);
    for(size_t i = 0; i < np; ++i)
        y[i] = p[i] + double(i);
    for(size_t i = 0; i < nx; ++i)
        y[i + np] = x[i] + double(i + np);
    //
    // some operations that optimizer should remove
    typename VectorFloat::value_type sum = 0.0;
    for(size_t i = 0; i < ny; i++)
        sum += y[i];
    //
    return y;
}
# endif
