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
$begin atomic_three_vector_op.cpp$$
$spell
    op
    enum
    mul
    div
    vec_op
$$

$section Atomic Vector Math Operators: Example and Test$$

$head Syntax$$
$codei%atomic_vector_op %vec_op%(%name%)
%$$
$icode%vec_op%(%x%, %y%)
%$$

$head Purpose$$
The atomic function implements
$codei%
    %y% = %u% %op% %v%
%$$
where $icode op$$, $icode u$$ and $icode v$$ are defined below.

$head x$$
We use $icode x$$ to denote the argument to the atomic function.
The length of $icode x$$ is denoted by $icode n$$ and must be odd.
We use the notation $icode%m% = (%n% - 1) / 2%$$.

$head op$$
The value $icode%x%[0]%$$
is a constant parameter with the following possible values:
$srcthisfile%0%// BEGIN op_enum_t%// END op_enum_t%1%$$
We use $icode op$$ for the corresponding operator.

$head u$$
We use $icode u$$ to denote the following sub-vector of $icode x$$:
$codei%
    %u% = ( %x%[1] , %...% , %x%[%m%] )
%$$

$head v$$
We use $icode v$$ to denote the following sub-vector of $icode x$$:
$codei%
    %v% = ( %x%[%m% + 1] , %...% , %x%[2 * %m%] )
%$$

$head y$$
We use $icode y$$ to denote the atomic function return value.
The length of $icode y$$ is equal to $icode m$$.


$head Source Code$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace {          // isolate items below to this file
//
// abbreviations
using CppAD::AD;
using CppAD::vector;
//
class atomic_vector_op : public CppAD::atomic_three<double> {
//
public:
    // BEGIN op_enum_t
    // atomic_vector_op::op_enum_t
    typedef enum {
        add_enum,
        sub_enum,
        mul_enum,
        div_enum,
        num_op,
    } op_enum_t;
    // END op_enum_t
    //
    // ctor
    atomic_vector_op(const std::string& name) :
    CppAD::atomic_three<double>(name)
    { }
private:
    // ------------------------------------------------------------------------
    static void copy_atx_to_ax(
        size_t                        n,
        size_t                        m,
        size_t                        q,
        size_t                        k_u,
        size_t                        k_v,
        const vector< AD<double> >&   atx,
        vector< AD<double> >&         ax)
    {   assert( atx.size() == n * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i)     * (q+1) + k_u;
            size_t v_index  = (1 + m + i) * (q+1) + k_v;
            ax[1+i]         = atx[u_index];
            ax[1+ m+i]      = atx[v_index];
        }
    }
    // ------------------------------------------------------------------------
    static void copy_ay_to_aty(
        size_t                        n,
        size_t                        m,
        size_t                        q,
        size_t                        k,
        const vector< AD<double> >&   ay,
        vector< AD<double> >&         aty)
    {   assert( aty.size() == m * (q+1) );
        assert( ay.size()  == m );
        for(size_t i = 0; i < m; ++i)
        {   size_t y_index  = i * (q+1) + k;
            aty[y_index]    = ay[i];
        }
    }
    // ------------------------------------------------------------------------
    // for_type
    bool for_type(
        const vector<double>&               parameter_x ,
        const vector<CppAD::ad_type_enum>&  type_x      ,
        vector<CppAD::ad_type_enum>&        type_y      ) override
    {   // n, m
        size_t n = parameter_x.size();
        size_t m = (n - 1) / 2;
        //
        // ok
        bool ok  = type_x.size() == n;
        ok      &= type_y.size() == m;
        if( ! ok )
            return false;
        //
        // type_y
        for(size_t i = 0; i < m; ++i)
            type_y[i] = std::max( type_x[1 + i] , type_x[1 + m + i] );
        //
        return true;
    }
    // ----------------------------------------------------------------------
    // forward_add
    // ----------------------------------------------------------------------
    void forward_add(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<double>&              tx          ,
        vector<double>&                    ty          )
    {
        for(size_t k = p; k <= q; ++k)
        {   for(size_t i = 0; i < m; ++i)
            {   size_t u_index  = (1 + i)     * (q+1) + k;
                size_t v_index  = (1 + m + i) * (q+1) + k;
                size_t y_index  = i *           (q+1) + k;
                ty[y_index]     = tx[u_index] + tx[v_index];
            }
        }
    }
    void forward_add(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector< AD<double> >&        atx         ,
        vector< AD<double> >&              aty         )
    {   vector< AD<double> > ax(n), ay(m);
        ax[0] = AD<double>( add_enum );
        for(size_t k = p; k <= q; ++k)
        {   copy_atx_to_ax(n, m, q, k, k, atx, ax);
            (*this)(ax, ay); // atomic add operation
            copy_ay_to_aty(n, m, q, k, ay, aty);
        }
    }
    // ----------------------------------------------------------------------
    // forward_sub
    // ----------------------------------------------------------------------
    void forward_sub(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<double>&              tx          ,
        vector<double>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t u_index  = (1 + i)     * (q+1) + k;
                size_t v_index  = (1 + m + i) * (q+1) + k;
                size_t y_index  = i           * (q+1) + k;
                ty[y_index]     = tx[u_index] - tx[v_index];
            }
        }
    }
    void forward_sub(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector< AD<double> >&        atx         ,
        vector< AD<double> >&              aty         )
    {   vector< AD<double> > ax(n), ay(m);
        ax[0] = AD<double>( sub_enum );
        for(size_t k = p; k <= q; ++k)
        {   copy_atx_to_ax(n, m, q, k, k, atx, ax);
            (*this)(ax, ay); // atomic add operation
            copy_ay_to_aty(n, m, q, k, ay, aty);
        }
    }
    // ----------------------------------------------------------------------
    // forward_mul
    // ----------------------------------------------------------------------
    void forward_mul(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<double>&              tx          ,
        vector<double>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t y_index = i * (q+1) + k;
                ty[y_index]    = 0.0;
                for(size_t d = 0; d <= k; d++)
                {   size_t u_index  = (1 + i)     * (q+1) + (k-d);
                    size_t v_index  = (1 + m + i) * (q+1) + d;
                    ty[y_index]    += tx[u_index] * tx[v_index];
                }
            }
        }
    }
    // ----------------------------------------------------------------------
    // forward_div
    // ----------------------------------------------------------------------
    void forward_div(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<double>&              tx          ,
        vector<double>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t y_index  = i *       (q+1) + k;
                size_t u_index  = (1 + i) * (q+1) + k;
                ty[y_index]     = tx[u_index];
                for(size_t d = 1; d <= k; d++)
                {   size_t y_other      = i       * (q+1) + (k-d);
                    size_t v_index  = (1 + m + i) * (q+1) + d;
                    ty[y_index] -= ty[y_other] * tx[v_index];
                }
                size_t v_index = (1 + m + i ) * (q+1) + 0;
                ty[y_index] /= tx[v_index];
            }
        }
    }
    // ----------------------------------------------------------------------
    // forward
    // forward mode routines called by ADFun<Base> objects
    // ----------------------------------------------------------------------
    bool forward(
        const vector<double>&              parameter_x ,
        const vector<CppAD::ad_type_enum>& type_x      ,
        size_t                             need_y      ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<double>&              tx          ,
        vector<double>&                    ty          ) override
    {
        // op, n, m
        op_enum_t op = op_enum_t( parameter_x[0] );
        size_t n     = parameter_x.size();
        size_t m     = (n - 1) / 2;
        //
        assert( tx.size() == (q+1) * n );
        assert( ty.size() == (q+1) * m );
        //
        switch(op)
        {
            // addition
            case add_enum:
            forward_add(n, m, q, p, tx, ty);
            break;

            // subtraction
            case sub_enum:
            forward_sub(n, m, q, p, tx, ty);
            break;

            // multiplication
            case mul_enum:
            forward_mul(n, m, q, p, tx, ty);
            break;

            // division
            case div_enum:
            forward_div(n, m, q, p, tx, ty);
            break;

            // error
            case num_op:
            assert(false);
            break;
        }
        return true;
    }
    // ----------------------------------------------------------------------
    // forward
    // forward mode routines called by ADFun< AD<Base> , Base> objects
    // ----------------------------------------------------------------------
    bool forward(
        const vector< AD<double> >&        aparameter_x ,
        const vector<CppAD::ad_type_enum>& type_x      ,
        size_t                             need_y      ,
        size_t                             p           ,
        size_t                             q           ,
        const vector< AD<double> >&        atx         ,
        vector< AD<double> >&              aty         ) override
    {   //
        // op, n, m
        op_enum_t op = op_enum_t( Value( aparameter_x[0] ) );
        size_t n     = aparameter_x.size();
        size_t m     = (n - 1) / 2;
        //
        assert( atx.size() == (q+1) * n );
        assert( aty.size() == (q+1) * m );
        //
        bool ok;
        switch(op)
        {
            // addition
            case add_enum:
            forward_add(n, m, q, p, atx, aty);
            ok = true;
            break;

            // subtraction
            case sub_enum:
            forward_sub(n, m, q, p, atx, aty);
            ok = true;
            break;

            // multiplication
            case mul_enum:
            ok = false;
            break;

            // division
            case div_enum:
            ok = false;
            break;

            // error
            case num_op:
            assert(false);
            ok = false;
            break;
        }
        return ok;
    }

}; // End of atomic_vector_op class
}  // End empty namespace

bool vector_op(void)
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
    for(size_t i_op = 0; i_op < num_op; ++i_op)
    {   //
        // op
        op_enum_t op = op_enum_t(i_op);
        //
        // Create the function f(x) = u op v
        CPPAD_TESTVECTOR( AD<double> ) auv(2 * m);
        for(size_t i = 0; i < m; i++)
        {   auv[i]     = double(i+1);   // u[i]
            auv[m + i] = double(m+2*i); // v[i]
        }
        // declare independent variables and start tape recording
        CppAD::Independent(auv);
        //
        // ax, ay
        CPPAD_TESTVECTOR( AD<double> ) ax(n), ay(m);
        ax[0] = AD<double>(op); // code for this operator
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
                case atomic_vector_op::add_enum:
                check_y  =   uv[i] +  uv[m + i];
                check_dy =  duv[i] + duv[m + i];
                break;

                case atomic_vector_op::sub_enum:
                check_y  =  uv[i] -  uv[m + i];
                check_dy = duv[i] - duv[m + i];
                break;

                case atomic_vector_op::mul_enum:
                check_y  =  uv[i] *  uv[m + i];
                check_dy = duv[i + 1] *  uv[m + i]
                         +  uv[i] * duv[m + i];
                break;

                case atomic_vector_op::div_enum:
                den_sq   =  uv[m + i] *  uv[m + i];
                check_y  =  uv[i] /  uv[m + i];
                check_dy = duv[i] /  uv[m + i]
                         -  uv[i] * duv[m + i] / den_sq;
                break;

                case atomic_vector_op::num_op:
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
