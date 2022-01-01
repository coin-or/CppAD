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
$begin atomic_vector_ad_double.cpp$$

$section Atomic Vector Operations in AD<double>$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "atomic_vector.hpp"
bool ad_double(void)
{   bool ok = true;
    using CppAD::NearEqual;
    double eps99 = 99.0 * CppAD::numeric_limits<double>::epsilon();
    //
    // vec_op
    // atomic vector_op object
    atomic_vector_op vec_op("atomic_vector_op");
    //
    // m, n
    // size of x and y
    size_t m = 2;
    size_t n = 1 + 2 * m;
    //
    // op_enum_t
    typedef atomic_vector_op::op_enum_t op_enum_t;
    //
    // num_op
    size_t num_op = size_t( atomic_vector_op::num_op );
    //
    // i_op
    for(size_t i_op = 0; i_op < num_op - 1; ++i_op)
    {   //
        // op
        op_enum_t op = op_enum_t(i_op);
        //
        // Create the function f(x) = u op v
        CPPAD_TESTVECTOR( CppAD::AD<double> ) auv(2 * m);
        for(size_t j = 0; j < 2 * m; ++j)
            auv[j] = double(1 + j);
        //
        // declare independent variables and start tape recording
        CppAD::Independent(auv);
        //
        // ax, ay
        CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(n), ay(m);
        ax[0] = CppAD::AD<double>(op); // code for this operator
        for(size_t i = 0; i < m; ++i)
        {   ax[1 + i]     = auv[i];
            ax[1 + m + i] = auv[m + i];
        }
        //
        // ay = u op v
        vec_op(ax, ay);
        //
        // create f: x -> y and stop tape recording
        CppAD::ADFun<double> f(auv, ay);
        //
        // af
        CppAD::ADFun< CppAD::AD<double>, double> af = f.base2ad();
        //
        // Create the function g(x) where g_i(x) = d/dv[i] f_i(x) using the
        // fact that d/dv[j] f_i (x) is zero when i is not equal to j
        CppAD::Independent(auv);
        CPPAD_TESTVECTOR( CppAD::AD<double> ) aduv(2 * m), az(m);
        for(size_t i = 0; i < m; ++i)
        {   aduv[i]     = 0.0; // du[i]
            aduv[m + i] = 1.0; // dv[i]
        }
        af.Forward(0, auv);
        az = af.Forward(1, aduv);
        CppAD::ADFun<double> g(auv, az);
        //
        // uv, y
        CPPAD_TESTVECTOR(double) uv(2 * m), z(m);
        for(size_t j = 0; j < 2 * m; ++j)
            uv[j]  = double(1 + j);
        z  = g.Forward(0, uv);
        //
        for(size_t i = 0; i < m; ++i)
        {   double check_z;
            switch(op)
            {
                case atomic_vector_op::add_enum:
                check_z =  1.0;
                break;

                case atomic_vector_op::sub_enum:
                check_z  =  - 1.0;
                break;

                case atomic_vector_op::mul_enum:
                check_z  = uv[i];
                break;

                case atomic_vector_op::div_enum:
                check_z = - uv[i] / (uv[m + i] * uv[m + i]);
                break;

                case atomic_vector_op::num_op:
                assert( false );
                break;
            }
            ok &= NearEqual( z[i] ,  check_z,  eps99, eps99);
        }
    }
    return ok;
}
// END C++
