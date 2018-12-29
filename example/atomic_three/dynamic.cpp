/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
$begin atomic_three_dynamic.cpp$$

$section Atomic Functions with Dynamic Parameters: Example and Test$$

$head Purpose$$
This example demonstrates using dynamic parameters with an
$cref atomic_three$$ function.

$head Function$$
For this example, the atomic function
$latex f : \B{R}^3 \rightarrow \B{R}^3$$ is defined by
$latex f_0 (x) = x_0 * x_ 0$$,
$latex f_1 (x) = x_0 * x_ 1$$,
$latex f_2 (x) = x_1 * x_ 2$$.

$nospell

$head Start Class Definition$$
$srccode%cpp% */
# include <cppad/cppad.hpp>  // CppAD include file
namespace {                  // start empty namespace
using CppAD::vector;         // abbreviate CppAD::vector using vector
// start definition of atomic derived class using atomic_three interface
class atomic_dynamic : public CppAD::atomic_three<double> {
/* %$$
$head Constructor$$
$srccode%cpp% */
public:
    // can use const char* name when calling this constructor
    atomic_dynamic(const std::string& name) : // can have more arguments
    CppAD::atomic_three<double>(name)             // inform base class of name
    { }

private:
/* %$$
$head type$$
$srccode%cpp% */
    // calculate type_x
    virtual bool type(
        const vector<CppAD::ad_type_enum>&  type_x    ,
        vector<CppAD::ad_type_enum>&        type_y    )
    {   bool ok = type_x.size() == 3; // n
        ok     &= type_y.size() == 3; // m
        if( ! ok )
            return false;
        type_y[0] = type_x[0];
        type_y[1] = std::max( type_x[0], type_x[1] );
        type_y[2] = std::max( type_x[1], type_x[2] );
        return true;
    }
/* %$$
$head forward$$
$srccode%cpp% */
    // forward mode routine called by CppAD
    virtual bool forward(
        size_t                              need_y    ,
        size_t                              order_low ,
        size_t                              order_up  ,
        const vector<CppAD::ad_type_enum>&  type_x    ,
        const vector<double>&               taylor_x  ,
        vector<double>&                     taylor_y
    )
    {
# ifndef NDEBUG
        size_t n = taylor_x.size() / (order_up + 1);
        size_t m = taylor_y.size() / (order_up + 1);
# endif
        assert( n == 3 );
        assert( m == 3 );
        assert( order_low <= order_up );

        // return flag
        bool ok = order_up == 0;
        if( ! ok )
            return ok;

        // Order zero forward mode.
        // This case must always be implemented
        if( need_y > size_t(CppAD::variable_enum) )
        {   // f_0 = x_0 * x_0
            taylor_y[0] = taylor_x[0] * taylor_x[0];
            // f_1 = x_0 * x_1
            taylor_y[1] = taylor_x[0] * taylor_x[1];
            // f_2 = x_1 * x_2
            taylor_y[2] = taylor_x[1] * taylor_x[2];
        }
        else
        {   // This uses need_y to reduce amount of computation.
            // It is probably faster, for this case, to ignore need_y.
            vector<CppAD::ad_type_enum> type_y( taylor_y.size() );
            type(type_x, type_y);
            // f_0 = x_0 * x_0
            if( size_t(type_y[0]) == need_y )
                taylor_y[0] = taylor_x[0] * taylor_x[0];
            // f_1 = x_0 * x_1
            if( size_t(type_y[1]) == need_y )
                taylor_y[1] = taylor_x[0] * taylor_x[1];
            // f_2 = x_1 * x_2
            if( size_t(type_y[2]) == need_y )
                taylor_y[2] = taylor_x[1] * taylor_x[2];
        }

        return ok;
    }
/* %$$
$head End Class Definition$$
$srccode%cpp% */
}; // End of atomic_dynamic class
}  // End empty namespace

/* %$$
$head Use Atomic Function$$
$srccode%cpp% */
bool dynamic(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::NearEqual;
    double eps = 10. * CppAD::numeric_limits<double>::epsilon();
/* %$$
$subhead Constructor$$
$srccode%cpp% */
    // Create the atomic dynamic object
    atomic_dynamic afun("atomic_dynamic");
/* %$$
$subhead Recording$$
$srccode%cpp% */
    // Create the function f(x)
    //
    // constant parameter
    double c0 = 2.0;
    //
    // indepndent dynamic parameter vector
    size_t np = 1;
    CPPAD_TESTVECTOR(double) p(np);
    CPPAD_TESTVECTOR( AD<double> ) ap(np);
    ap[0] = p[0] = 3.0;
    //
    // independent variable vector
    size_t  nx = 1;
    double  x0 = 0.5;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx);
    ax[0]     = x0;

    // declare independent variables and start tape recording
    size_t abort_op_index = 0;
    bool   record_compare = true;
    CppAD::Independent(ax, abort_op_index, record_compare, ap);

    // range space vector
    size_t ny = 3;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);

    // call atomic function and store result in ay
    // y = ( c * c, c * p, p * x )
    CPPAD_TESTVECTOR( AD<double> ) au(3);
    au[0] = c0;
    au[1] = ap[0];
    au[2] = ax[0];
    afun(au, ay);

    // check type of result
    ok &= Constant( ay[0] );
    ok &= Dynamic(  ay[1] );
    ok &= Variable( ay[2] );

    // create f: x -> y and stop tape recording
    CppAD::ADFun<double> f;
    f.Dependent (ax, ay);  // f(x) = (c * c, c * p, p * x)
/* %$$
$subhead forward$$
$srccode%cpp% */
    // check function value
    double check = c0 * c0;
    ok &= NearEqual( Value(ay[0]) , check,  eps, eps);
    check = c0 * p[0];
    ok &= NearEqual( Value(ay[1]) , check,  eps, eps);
    check = p[0] * x0;
    ok &= NearEqual( Value(ay[2]) , check,  eps, eps);

    // check zero order forward mode
    size_t q;
    CPPAD_TESTVECTOR( double ) x_q(nx), y_q(ny);
    q      = 0;
    x_q[0] = x0;
    y_q    = f.Forward(q, x_q);
    check = c0 * c0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = c0 * p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);
    check = p[0] * x0;
    ok    &= NearEqual(y_q[2] , check,  eps, eps);

    // set new value for dynamic parameters
    p[0]   = 2.0 * p[0];
    f.new_dynamic(p);
    y_q    = f.Forward(q, x_q);
    check = c0 * c0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = c0 * p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);
    check = p[0] * x0;
    ok    &= NearEqual(y_q[2] , check,  eps, eps);

/* %$$
$subhead Return Test Result$$
$srccode%cpp% */
    return ok;
}
/* %$$
$$ $comment end nospell$$
$end
*/
