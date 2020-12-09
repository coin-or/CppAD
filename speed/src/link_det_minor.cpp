/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>
# include <cppad/utility/time_test.hpp>
// BEGIN PROTOTYPE
extern bool link_det_minor(
    const std::string&         job       ,
    size_t                     size      ,
    size_t                     repeat    ,
    CppAD::vector<double>&     matrix    ,
    CppAD::vector<double>&     gradient
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
$begin link_det_minor$$
$spell
    det
    bool
    CppAD
$$


$section Speed Testing Gradient of Determinant by Minor Expansion$$

$head Prototype$$
$srcthisfile%
    0%// BEGIN PROTOTYPE%// END PROTOTYPE%0
%$$


$head Purpose$$
Each $cref/package/speed_main/package/$$
must define a version of this routine as specified below.
This is used by the $cref speed_main$$ program
to run the corresponding speed and correctness tests.

$head Method$$
The same template class $cref det_by_minor$$
is used by the different AD packages.

$head Return Value$$
If this speed test is not yet
supported by a particular $icode package$$,
the corresponding return value for $code link_det_minor$$
should be $code false$$.

$head Thread Save$$
These speed tests may use static memory for setup and teardown
and hence my not be thread safe.

$head job$$
This argument has one of the following values:

$subhead setup$$
This is the first call with a new value for $icode size$$
and will not be used for timing.
It can be used to setup information that should not be included in timing
how long this routine takes.

$subhead run$$
This call will be used for timing how long this routine takes.
The previous call $icode job$$ was either $code setup$$ or $code run$$
and had the same value of $icode size$$.

$subhead teardown$$
This is the last call with this value of $icode size$$
and will not be used for timing.
It should be used to take down the setup; e.g., fee any memory that
was allocated.

$head size$$
The argument $icode size$$
is the number of rows and columns in the matrix.

$head repeat$$
The argument $icode repeat$$ is the number of different matrices
that the gradient (or determinant) is computed for.

$head matrix$$
The argument $icode matrix$$ is a vector with
$icode%size%*%size%$$ elements.
The input value of its elements does not matter.
The output value of its elements is the last matrix that the
gradient (or determinant) is computed for.

$head gradient$$
The argument $icode gradient$$ is a vector with
$icode%size%*%size%$$ elements.
The input value of its elements does not matter.
The output value of its elements is the gradient of the
determinant of $icode matrix$$ with respect to its elements.

$subhead double$$
In the case where $icode package$$ is $code double$$,
only the first element of $icode gradient$$ is used and it is actually
the determinant value (the gradient value is not computed).

$end
-----------------------------------------------------------------------------
*/
bool available_det_minor(void)
{
    size_t size   = 3;
    size_t repeat = 1;
    CppAD::vector<double> matrix(size * size);
    CppAD::vector<double> gradient(size * size);
    //
    std::string job = "setup";
    bool result = link_det_minor(job, size, repeat, matrix, gradient);
    //
    job = "teardown";
    link_det_minor(job, size, repeat, matrix, gradient);
    //
    return result;
}
// ---------------------------------------------------------------------------
bool correct_det_minor(bool is_package_double)
{   // cppadcg package assumes that that size = 3; see ../main.cpp
    size_t  size   = 3;
    size_t  repeat = 1;
    CppAD::vector<double> matrix(size * size);
    CppAD::vector<double> gradient(size * size);
    //
    std::string job = "setup";
    link_det_minor(job, size, repeat, matrix, gradient);
    //
    job = "run";
    link_det_minor(job, size, repeat, matrix, gradient);
    //
    bool ok = CppAD::det_grad_33(matrix, gradient);
    if( is_package_double )
        ok = CppAD::det_33(matrix, gradient);
    else
        ok = CppAD::det_grad_33(matrix, gradient);
    //
    job = "teardown";
    link_det_minor(job, size, repeat, matrix, gradient);
    //
    return ok;
}
// ---------------------------------------------------------------------------
void time_det_minor_callback(size_t size, size_t repeat)
{   // job
    std::string job = "run";
    CppAD::vector<double> matrix(size * size);
    CppAD::vector<double> gradient(size * size);
    //
    link_det_minor(job, size, repeat, matrix, gradient);
    return;
}
double time_det_minor(double time_min, size_t size)
{   CPPAD_ASSERT_UNKNOWN( size != 0 );
    //
    CppAD::vector<double> matrix(size * size);
    CppAD::vector<double> gradient(size * size);
    //
    std::string job    = "setup";
    size_t      repeat = 0;
    link_det_minor(job, size, repeat, matrix, gradient);
    //
    // job = run
    double time = CppAD::time_test(time_det_minor_callback, time_min, size);
    //
    job = "teardown";
    link_det_minor(job, size, repeat, matrix, gradient);
    //
    return time;
}
// ---------------------------------------------------------------------------
