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
$begin atomic_four_dynamic.cpp$$

$nospell
$bold This is old CppAD documentation:$$ Here is a link to its
$href%http://coin-or.github.io/CppAD%current documentation%$$.
$$
$section Atomic Functions with Dynamic Parameters: Example and Test$$

$head Purpose$$
This example demonstrates using dynamic parameters with an
$cref atomic_four$$ function.

$head Function$$
For this example, the atomic function
$latex g : \B{R}^3 \rightarrow \B{R}^3$$ is defined by
$latex g_0 (x) = x_0 * x_ 0$$,
$latex g_1 (x) = x_0 * x_ 1$$,
$latex g_2 (x) = x_1 * x_ 2$$.

$head Define Atomic Function$$
$srcthisfile%0
    %// BEGIN_DEFINE_ATOMIC_FUNCTION%// END_DEFINE_ATOMIC_FUNCTION%
1%$$

$head Use Atomic Function$$
$srcthisfile%0
    %// BEGIN_USE_ATOMIC_FUNCTION%// END_USE_ATOMIC_FUNCTION%
1%$$

$end
*/

# include <cppad/cppad.hpp>

// BEGIN_DEFINE_ATOMIC_FUNCTION
// empty namespace
namespace {
    // atomic_dynamic
    class atomic_dynamic : public CppAD::atomic_four<double> {
    public:
        atomic_dynamic(const std::string& name) :
        CppAD::atomic_four<double>(name)
        { }
    private:
        // for_type
        bool for_type(
        size_t                                     call_id     ,
        const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
        CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
        {   assert( call_id == 0 );       // default value
            assert( type_x.size() == 3 ); // n
            assert( type_y.size() == 3 ); // m
            //
            // type_y
            type_y[0] = type_x[0];
            type_y[1] = std::max( type_x[0], type_x[1] );
            type_y[2] = std::max( type_x[1], type_x[2] );
            return true;
        }
        // forward
        bool forward(
            size_t                        call_id      ,
            const CppAD::vector<bool>&    select_y     ,
            size_t                        order_low    ,
            size_t                        order_up     ,
            const CppAD::vector<double>&  taylor_x     ,
            CppAD::vector<double>&        taylor_y     ) override
        {
# ifndef NDEBUG
            size_t q = order_up + 1;
            size_t n = taylor_x.size() / q;
            size_t m = taylor_y.size() / q;
            assert( n == 3 );
            assert( m == 3 );
# endif
            // ok
            bool ok = order_low == 0 && order_up == 0;
            if( ! ok )
                return ok;
            //
            // taylor_y[0] = g_0 = x_0 * x_0
            if( select_y[0] )
                taylor_y[0] = taylor_x[0] * taylor_x[0];
            //
            // taylor_y[1] = g_1 = x_0 * x_1
            if( select_y[1] )
                taylor_y[1] = taylor_x[0] * taylor_x[1];
            //
            // taylor_y[2] = g_2 = x_1 * x_2
            if( select_y[2] )
                taylor_y[2] = taylor_x[1] * taylor_x[2];
            //
            return ok;
        }
    };
}
// END_DEFINE_ATOMIC_FUNCTION

// BEGIN_USE_ATOMIC_FUNCTION
bool dynamic(void)
{   // ok, eps
    bool ok = true;
    double eps = 10. * CppAD::numeric_limits<double>::epsilon();
    //
    // afun
    atomic_dynamic afun("atomic_dynamic");
    //
    // c, p, u
    CPPAD_TESTVECTOR(double) c(1), p(1), u(1);
    c[0] = 2.0;
    p[0] = 3.0;
    u[0] = 4.0;
    //
    //
    // np, nu, ny
    size_t np = 1;
    size_t nu = 1;
    size_t nx = 3;
    size_t ny = 3;
    //
    // ap
    // indepndent dynamic parameter vector
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ap(np);
    ap[0] = p[0];
    //
    // au
    // independent variable vector
    CPPAD_TESTVECTOR( CppAD::AD<double> ) au(nu);
    au[0] = u[0];
    //
    // Independent
    CppAD::Independent(au, ap);
    //
    // ay
    // y = ( c * c, c * p, p * x )
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(nx), ay(ny);
    ax[0] = c[0];  // x_0
    ax[1] = ap[0]; // x_1
    ax[2] = au[0]; // x_2
    afun(ax, ay);
    //
    // ay
    // check type of result
    ok &= Constant( ay[0] );
    ok &= Dynamic(  ay[1] );
    ok &= Variable( ay[2] );
    //
    // f
    // f(u) = (c * c, c * p, p * u)
    CppAD::ADFun<double> f;
    f.Dependent (au, ay);
    //
    // ay[0]
    double check = c[0] * c[0];
    ok &= CppAD::NearEqual( Value(ay[0]) , check,  eps, eps);
    //
    // ay[1]
    check = c[0] * p[0];
    ok &= CppAD::NearEqual( Value(ay[1]) , check,  eps, eps);
    //
    // ay[2]
    check = p[0] * u[0];
    ok &= CppAD::NearEqual( Value(ay[2]) , check,  eps, eps);
    //
    // y = f.Foward(0, u)
    CPPAD_TESTVECTOR(double) y(ny);
    y     = f.Forward(0, u);
    check = c[0] * c[0];
    ok    &= CppAD::NearEqual(y[0] , check,  eps, eps);
    check = c[0] * p[0];
    ok    &= CppAD::NearEqual(y[1] , check,  eps, eps);
    check = p[0] * u[0];
    ok    &= CppAD::NearEqual(y[2] , check,  eps, eps);
    //
    // p
    p[0]   = 2.0 * p[0];
    f.new_dynamic(p);
    //
    // y = f.Foward(0, u)
    y     = f.Forward(0, u);
    check = c[0] * c[0];
    ok    &= CppAD::NearEqual(y[0] , check,  eps, eps);
    check = c[0] * p[0];
    ok    &= CppAD::NearEqual(y[1] , check,  eps, eps);
    check = p[0] * u[0];
    ok    &= CppAD::NearEqual(y[2] , check,  eps, eps);
    //
    return ok;
}
// END_USE_ATOMIC_FUNCTION
