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
$begin cppadcg_sparse_jacobian.c$$
$spell
    Cppadcg
    Jacobian
    subgraph
    nnz
$$

$section Cppadcg Speed: Sparse Jacobian$$

$head Syntax$$
$icode%flag% = sparse_jacobian(
    %subgraph%, %optimize%, %size%, %nnz%, %x%, %y%
)%$$

$head Prototype$$
$srccode%c%
extern "C" int sparse_jacobian(
    int optimize, int size, int nnz, const double* x, double* y
)%$$

$head Purpose$$
Computes a sparse Jacobian corresponding to
the function $cref sparse_jac_fun$$.

$head subgraph$$
If it is one (zero), the $cref subgraph_jac_rev$$
method is used (is not used) to compute the sparse Jacobian

$head optimize$$
If it is one (zero), the optimized (non-optimized) version
of the computation is used.

$head size$$
Is the dimension of the argument space for the function.
This function is only implemented for one value of $icode size$$.
You can create a version of this function for a different value of
$icode size$$ by calling $cref cppadcg_sparse_jacobian_cg.cpp$$.

$head x$$
This is a vector of length $icode size$$ containing the
point at which to evaluate the derivative.

$head y$$
Is the sparse representation of the Jacobian function.
It size is the number of non-zeros in the Jacobian; $icode nnz$$.
The input value of it's elements does not matter.
Upon return,
for $icode%k% = 0 , %...%, %nnz%-1%$$.
the $th k$$ component of $icode y$$
is the partial of component $icode%row%[%k%]%$$ of the function
with respect to the $icode%col%[%k%]%$$ component of $icode x$$.

$subhead row$$
is the same as $cref/row/link_sparse_jacobian/row/$$ in the call to
$code link_sparse_jacobian$$.

$subhead col$$
is the same as $cref/col/link_sparse_jacobian/col/$$ in the call to
$code link_sparse_jacobian$$.

$head flag$$
This value is zero if the value of $icode size$$ is passed in
can be handled by this source code; see $cref cppadcg_sparse_jacobian_cg.cpp$$.
Otherwise,
$icode y$$ is not used and the return value of flag is $code 1$$.

$head Implementation$$
The following is an implementation for the case when $icode size$$
is one:
$srccode%c% */
# include <math.h>
# include <assert.h>
int sparse_jacobian(
    int subgraph, int optimized, int size, int nnz, const double* x, double* y
)
{   if( size != 1 )
        return 1;
    assert( nnz = 1 );
    // f(x) = exp( x * x )
    y[0] = 2.0 * x[0] * exp( x[0] * x[0] );
    return 0;
}
/* %$$
$end
*/
