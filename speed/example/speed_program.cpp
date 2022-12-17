/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin speed_program.cpp$$
$spell
    speed_program.exe
    cppad.hpp
    std
    cpp
$$


$nospell
$bold This is old CppAD documentation:$$ Here is a link to its
$href%http://coin-or.github.io/CppAD%current documentation%$$.
$$
$section Example Use of SpeedTest$$

$head Program$$
$srccode%cpp% */
# include <cppad/utility/speed_test.hpp>

// Some compilers have gotten smarter and do not calucate somthing that is not used
// so make the result a global.
double global_result = 0.0;

std::string Test(size_t size, size_t repeat)
{   // setup
    double *a = new double[size];
    double *b = new double[size];
    size_t i  = size;;
    while(i)
    {   --i;
        a[i] = double(i);
        b[i] = double(2 * i);
    }
    // operations we are timing
    while(repeat--)
    {   i = size;;
        while(i)
        {   --i;
            global_result += a[i] * b[i];
        }
    }
    // teardown
    delete [] a;
    delete [] b;

    // return a test name that is valid for all sizes and repeats
    return "double: result = sum_i a[i] * b[i]";
}
int main(void)
{
    CppAD::SpeedTest(Test, 20, 20, 100);
    return 0;
}

/* %$$

$head Output$$
Executing of the program above generated the following output
(the rates will be different for each particular system):
$codep
    double: c[*] = a[*] + b[*]
    size = 20  rate = 7,157,515
    size = 40  rate = 3,887,214
    size = 60  rate = 2,685,214
    size = 80  rate = 2,032,124
    size = 100 rate = 1,657,828
$$

$end
*/
