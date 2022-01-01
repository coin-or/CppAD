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
$begin atomic_vector_in_double.cpp$$

$section Atomic Vector Operations in double$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "atomic_vector.hpp"
bool in_double(void)
{   bool ok = true;
    using CppAD::NearEqual;
    double eps99 = 99.0 * CppAD::numeric_limits<double>::epsilon();
    //
    // vec_op
    // atomic vector_op object
    atomic_vector vec_op("atomic_vector");
    //
    // m, n
    // size of x and y
    size_t m = 2;
    size_t n = 1 + 2 * m;
    //
    // op_enum_t
    typedef atomic_vector::op_enum_t op_enum_t;
    //
    // num_op
    size_t num_op = size_t( atomic_vector::num_op );
    //
    // i_op
    for(size_t i_op = 0; i_op < num_op; ++i_op)
    {   //
        // op
        op_enum_t op = op_enum_t(i_op);
        //
        // Create the function f(x) = u op v
        CPPAD_TESTVECTOR( CppAD::AD<double> ) auv(2 * m);
        for(size_t i = 0; i < m; i++)
        {   auv[i]     = double(i+1); // u[i]
            auv[m + i] = double(i+2); // v[i]
        }
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
        CppAD::ADFun<double> f;
        f.Dependent (auv, ay);
        //
        // uv, y
        CPPAD_TESTVECTOR(double) uv(2*m), duv(2*m), y(m), dy(m);
        for(size_t j = 0; j < 2*m; ++j)
        {   uv[j]  = double(1 + j);
            duv[j] = double(1);
        }
        y  = f.Forward(0, uv);
        dy = f.Forward(1, duv);
        //
        for(size_t i = 0; i < m; ++i)
        {   double check_y, check_dy, den_sq;
            switch(op)
            {
                case atomic_vector::add_enum:
                check_y  =   uv[i] +  uv[m + i];
                check_dy =  duv[i] + duv[m + i];
                break;

                case atomic_vector::sub_enum:
                check_y  =  uv[i] -  uv[m + i];
                check_dy = duv[i] - duv[m + i];
                break;

                case atomic_vector::mul_enum:
                check_y  =  uv[i] *  uv[m + i];
                check_dy = duv[i + 1] *  uv[m + i]
                         +  uv[i] * duv[m + i];
                break;

                case atomic_vector::div_enum:
                den_sq   =  uv[m + i] *  uv[m + i];
                check_y  =  uv[i] /  uv[m + i];
                check_dy = duv[i] /  uv[m + i]
                         -  uv[i] * duv[m + i] / den_sq;
                break;

                case atomic_vector::num_op:
                assert( false );
                break;
            }
            ok &= NearEqual( y[i] ,  check_y,  eps99, eps99);
            ok &= NearEqual( dy[i] , check_dy,  eps99, eps99);
        }
    }
    return ok;
}
// END C++
