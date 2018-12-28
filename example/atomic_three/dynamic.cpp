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
$head forward$$
$srccode%cpp% */
    // forward mode routine called by CppAD
    virtual bool forward(
        size_t                              order_low ,
        size_t                              order_up  ,
        const vector<CppAD::ad_type_enum>&  type_x    ,
        vector<CppAD::ad_type_enum>&        type_y    ,
        const vector<double>&               taylor_x  ,
        vector<double>&                     taylor_y
    )
    {
# ifndef NDEBUG
        size_t n = taylor_x.size() / (order_up + 1);
        size_t m = taylor_y.size() / (order_up + 1);
# endif
        assert( n == 2 );
        assert( m == 2 );
        assert( order_low <= order_up );

        // return flag
        bool ok = order_up == 0;
        if( ! ok )
            return ok;

        // Check if we are setting ad_type for y
        // (must always be implemented).
        if( type_x.size() > 0 )
        {   assert( type_x.size() == n );
            assert( type_y.size() == m );
            type_y[0] = std::max( type_x[0], type_x[1] );
            type_y[1] = type_x[0];
        }

        // Order zero forward mode.
        // This case must always be implemented
        // y_0^0 = f_0( x^0 ) = x_0^0 * x_1^0
        taylor_y[0] = taylor_x[0] * taylor_x[1];
        // y_1^0 = f_1( x^0 ) = x_0^0 * x_0^0
        taylor_y[1] = taylor_x[0] * taylor_x[0];
        //
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
    // indepndent dynamic parameter vector
    size_t np = 1;
    CPPAD_TESTVECTOR(double) p(np);
    CPPAD_TESTVECTOR( AD<double> ) ap(np);
    ap[0] = p[0] = 3.0;
    //
    // domain space vector
    size_t  nx = 1;
    double  x0 = 0.5;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx);
    ax[0]     = x0;

    // declare independent variables and start tape recording
    size_t abort_op_index = 0;
    bool   record_compare = true;
    CppAD::Independent(ax, abort_op_index, record_compare, ap);

    // range space vector
    size_t ny = 2;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);

    // call atomic function and store result in au[0]
    // y = ( p * x , p * p )
    CPPAD_TESTVECTOR( AD<double> ) au(2);
    au[0] = ap[0];
    au[1] = ax[0];
    afun(au, ay);

    // check type of result
    ok &= Variable( ay[0] );
    ok &= Dynamic( ay[1] );

    // create f: x -> y and stop tape recording
    CppAD::ADFun<double> f;
    f.Dependent (ax, ay);  // f(x) = ( p * x , p * p )
/* %$$
$subhead forward$$
$srccode%cpp% */
    // check function value
    double check = p[0] * x0;
    ok &= NearEqual( Value(ay[0]) , check,  eps, eps);
    check = p[0] *  p[0];
    ok &= NearEqual( Value(ay[1]) , check,  eps, eps);

    // check zero order forward mode
    size_t q;
    CPPAD_TESTVECTOR( double ) x_q(nx), y_q(ny);
    q      = 0;
    x_q[0] = x0;
    y_q    = f.Forward(q, x_q);
    check = p[0] * x0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check = p[0] *  p[0];
    ok    &= NearEqual(y_q[1] , check,  eps, eps);

    // set new value for dynamic parameters
    p[0]   = 2.0 * p[0];
    f.new_dynamic(p);
    y_q    = f.Forward(q, x_q);

    // check forward using new parameter value
    x_q[0] = x0;
    y_q    = f.Forward(q, x_q);
    check  = p[0] * x0;
    ok    &= NearEqual(y_q[0] , check,  eps, eps);
    check  = p[0] *  p[0];
    ok &= NearEqual(y_q[1] , check,  eps, eps);

/* %$$
$subhead Return Test Result$$
$srccode%cpp% */
    return ok;
}
/* %$$
$$ $comment end nospell$$
$end
*/
