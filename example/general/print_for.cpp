/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
$begin print_for_string.cpp$$
$spell
    Cpp
$$

$section Print During Zero Order Forward Mode: Example and Test$$

$srcfile%example/general/print_for.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
    using std::endl;
    using CppAD::AD;

    // use of PrintFor to check for invalid function arguments
    AD<double> check_log(const AD<double>& y, std::ostream& s_out)
    {   // check AD<double> value during recording
        if( y <= 0 )
            s_out << "check_log: y == " << y << " which is <= 0\n";

        // check double value during zero order forward calculation
        PrintFor(y, "check_log: y == ", y , " which is <= 0\n");

        return log(y);
    }
}

bool print_for(void)
{   bool ok = true;
    using CppAD::PrintFor;
    std::stringstream stream_out;

    // independent variable vector
    size_t n = 1;
    CPPAD_TESTVECTOR(AD<double>) ax(n);
    ax[0] = 1.;         // value of the independent variable during recording
    Independent(ax);

    // A log evaluations that is OK when x[0] = 1 but not when x[0] = 2.
    AD<double> var     = 2. - ax[0];
    AD<double> log_var = check_log(var, stream_out);
    ok &= stream_out.str() == "";

    // dependent variable vector
    size_t m = 1;
    CPPAD_TESTVECTOR(AD<double>) ay(m);
    ay[0]    = log_var;

    // define f: x -> y and stop tape recording
    CppAD::ADFun<double> f(ax, ay);

    // zero order forward with x[0] = 2
    CPPAD_TESTVECTOR(double) x(n);
    x[0] = 2.;
    f.Forward(0, x, stream_out);

    std::string string_out = stream_out.str();
    ok &= stream_out.str() == "check_log: y == 0 which is <= 0\n";

    return ok;
}
// END C++
