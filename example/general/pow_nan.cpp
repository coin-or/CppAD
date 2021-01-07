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
$begin pow_nan.cpp$$
$spell
$$

$section pow: Avoiding Nan in Derivatives: Example and Test$$

$head Purpose$$
The $cref%pow(x, y)%pow%$$ function will work when $latex x < 0$$ and
$latex y$$  is integer valued. It will often generate nan or infinity when
$latex x = 0$$ and one tries to compute a derivatives
(even if $latex y$$ is a positive integer).
This is because the derivative of the log is $latex 1 / x$$
and the power function uses the representation
$latex \[
    \R{pow}(x, y) = \exp [ y \cdot \log(x) ]
\] $$
The example below  uses a $cref/conditional expression/condexp/$$
to avoid this problem.

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cmath>

bool pow_nan(void)
{   bool ok = true;

    using std::cout;
    using CppAD::AD;
    using CppAD::vector;
    using CppAD::NearEqual;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    vector< double>      x(1), y(1), z(1), dx(1), dz(1), w(1), dw(2);
    vector< AD<double> > ax(1), ay(1), az(1);
    AD<double> azero( 0.0 );
    //
    // variable vector
    x[0]  = 1.0;
    ax[0] = x[0];
    //
    // dynamic parameter vector
    y[0]  = 2.0;
    ay[0] = y[0];
    //
    CppAD::Independent(ax, ay);
    //
    // if x == 0 then z = 0 else z = pow(x, y)
    az[0] = CppAD::CondExpEq( ax[0], azero, azero,  pow(ax[0],  ay[0]) );
    //
    CppAD::ADFun<double> f(ax, az);
    f.check_for_nan(true);
    // -----------------------------------------------------------------------
    for(size_t ix = 0; ix < 2; ++ix)
    {   x[0] = 2.0 * double(ix);
        y[0] = 3.0 + double(ix);
        //
        double check;
        f.new_dynamic(y);
        z     = f.Forward(0, x);
        check = std::pow(x[0], y[0]);
        ok   &= NearEqual(z[0], check, eps99, eps99);
            //
        dx[0] = 1.0;
        dz    = f.Forward(1, dx);
        check = y[0] * pow(x[0], y[0] - 1.0 );
        ok   &= NearEqual(dz[0], check, eps99, eps99);
        //
        w[0]   = 1.0;
        dw     = f.Reverse(2, w);
        check = y[0] * pow(x[0], y[0] - 1.0 );
        ok    &= NearEqual(dw[0], check, eps99, eps99);
        check = y[0] * (y[0] - 1.0) * pow(x[0], y[0] - 2.0 );
        ok    &= NearEqual(dw[1], check, eps99, eps99);
    }
    //
    return ok;
}
// END C++
