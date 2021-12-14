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
$$

$section Atomic Vector Math Operators: Example and Test$$

$head Purpose$$
The atomic function implements
$codei%
    %y% = %left% %op% %right%
%$$
where $icode op$$, $icode left$$ and $icode right$$ are defined below.

$head x$$
We use $icode x$$ to denote the argument to the atomic function.
The length of $icode x$$ is denoted by $icode n$$ and must be odd.
We use the notation $icode%m% = (%n% - 1) / 2%$$.

$head op$$
The value $icode%x%[0]%$$
is a constant parameter with the following possible values:
$code add_enum$$,
$code sub_enum$$,
$code mul_enum$$,
$code div_enum$$,
(These enum value are defined in the $code atomic_vector_op$$ class.)
We use $icode op$$ for the corresponding operator.

$head left$$
We use $icode left$$ to denote the following sub-vector of $icode x$$:
$codei%
    %left% = ( %x%[1] , %...% , %x%[%m%] )
%$$

$head right$$
We use $icode right$$ to denote the following sub-vector of $icode x$$:
$codei%
    %left% = ( %x%[%m% + 1] , %...% , %x%[2 * %m%] )
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
    // op_enum_t
    typedef enum {
        add_enum,
        sub_enum,
        mul_enum,
        div_enum,
        num_op,
    } op_enum_t;
    //
    // atomic_vector_op
    atomic_vector_op(const std::string& name) :
    CppAD::atomic_three<double>(name)
    { }
private:
    // ------------------------------------------------------------------------
    // for_type
    virtual bool for_type(
        const vector<double>&               parameter_x ,
        const vector<CppAD::ad_type_enum>&  type_x      ,
        vector<CppAD::ad_type_enum>&        type_y      )
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
            type_y[i] = std::max( type_x[1 + i] , type_x[m + i] );
        //
        return true;
    }
    // ----------------------------------------------------------------------
    // template_forward_op
    // ----------------------------------------------------------------------
    template <class Scalar>
    void template_forward_add(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<Scalar>&              tx          ,
        vector<Scalar>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t left_index   = (1 + i) * (q+1) + k;
                size_t right_index  = (1 + m + i) * (q+1) + k;
                size_t y_index      = i * (q+1) + k;
                ty[y_index]         = tx[left_index] + tx[right_index];
            }
        }
    }
    template <class Scalar>
    void template_forward_sub(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<Scalar>&              tx          ,
        vector<Scalar>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t left_index   = (1 + i) * (q+1) + k;
                size_t right_index  = (1 + m + i) * (q+1) + k;
                size_t y_index      = i * (q+1) + k;
                ty[y_index]         = tx[left_index] - tx[right_index];
            }
        }
    }
    template <class Scalar>
    void template_forward_mul(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<Scalar>&              tx          ,
        vector<Scalar>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t y_index = i * (q+1) + k;
                ty[y_index]    = 0.0;
                for(size_t d = 0; d <= k; d++)
                {   size_t left_index   = (1 + i) * (q+1) + (k-d);
                    size_t right_index  = (1 + m + i) * (q+1) + k;
                    ty[y_index]        += tx[left_index] * tx[right_index];
                }
            }
        }
    }
    template <class Scalar>
    void template_forward_div(
        size_t                             n           ,
        size_t                             m           ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<Scalar>&              tx          ,
        vector<Scalar>&                    ty          )
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t y_index     = i * (q+1) + k;
                size_t left_index  = (1 + i) * (q+1) + k;
                ty[y_index]        = tx[left_index];
                for(size_t d = 1; d <= k; d++)
                {   size_t y_other      = i * (q+1) + (k-d);
                    size_t right_index  = (1 + m + i) * (q+1) + d;
                    ty[y_index]        -= ty[y_other] * tx[right_index];
                }
                size_t right_index = (1 + m + i ) * (q+1) + 0;
                ty[y_index] /= tx[right_index];
            }
        }
    }
    // ----------------------------------------------------------------------
    // template_forward
    // ----------------------------------------------------------------------
    template <class Scalar>
    bool template_forward(
        size_t                             n           ,
        op_enum_t                          op          ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<Scalar>&              tx          ,
        vector<Scalar>&                    ty          )
    {
        // m
        size_t m = (n - 1) / 2;
        //
        assert( tx.size() == (q+1) * n );
        assert( ty.size() == (q+1) * m );
        //
        switch(op)
        {
            // addition
            case add_enum:
            template_forward_add(n, m, q, p, tx, ty);
            break;

            // subtraction
            case sub_enum:
            template_forward_sub(n, m, q, p, tx, ty);
            break;

            // multiplication
            case mul_enum:
            template_forward_mul(n, m, q, p, tx, ty);
            break;

            // division
            case div_enum:
            template_forward_div(n, m, q, p, tx, ty);
            break;

            // error
            default:
            assert(false);
            break;
        }
        //
        return true;
    }
    // ----------------------------------------------------------------------
    // forward
    // forward mode routines called by ADFun<Base> objects
    // ----------------------------------------------------------------------
    virtual bool forward(
        const vector<double>&              parameter_x ,
        const vector<CppAD::ad_type_enum>& type_x      ,
        size_t                             need_y      ,
        size_t                             p           ,
        size_t                             q           ,
        const vector<double>&              tx          ,
        vector<double>&                    ty          )
    {   op_enum_t op = op_enum_t( parameter_x[0] );
        size_t n     = parameter_x.size();
        return template_forward(n, op, p, q, tx, ty);
    }
    // forward mode routines called by ADFun< AD<Base> , Base> objects
    virtual bool forward(
        const vector< AD<double> >&        aparameter_x ,
        const vector<CppAD::ad_type_enum>& type_x      ,
        size_t                             need_y      ,
        size_t                             p           ,
        size_t                             q           ,
        const vector< AD<double> >&        atx         ,
        vector< AD<double> >&              aty         )
    {   op_enum_t op = op_enum_t( Value( aparameter_x[0] ) );
        size_t n     = aparameter_x.size();
        return template_forward(n, op, p, q, atx, aty);
    }
}; // End of atomic_vector_op class
}  // End empty namespace

bool vector_op(void)
{   bool ok = true;
    using CppAD::NearEqual;
    double eps99 = 99.0 * CppAD::numeric_limits<double>::epsilon();
    //
    // afun
    // atomic vector_op object
    atomic_vector_op afun("atomic_vector_math");
    //
    // m, n
    // size of x and y
    size_t m = 2;
    size_t n = 1 + 2 * m;
    //
    // x
    CPPAD_TESTVECTOR( AD<double> ) ax(n);
    typedef atomic_vector_op::op_enum_t op_enum_t;
    size_t num_op = size_t( atomic_vector_op::num_op );
    for(size_t i_op = 0; i_op < num_op; ++i_op)
    {   //
        // op
        op_enum_t op = op_enum_t(i_op);
        //
        // Create the function f(x) = left op right
        //
        ax[0] = AD<double>(op); // code for this operator
        for(size_t i = 0; i < m; i++)
        {   ax[1 + i]     = double(i+1);   // left[i]
            ax[1 + m + i] = double(m+2*i); // right[i]
        }
        // declare independent variables and start tape recording
        CppAD::Independent(ax);
        //
        // y
        CPPAD_TESTVECTOR( AD<double> ) ay(m);
        //
        // ay = left op right
        afun(ax, ay);
        //
        // create f: x -> y and stop tape recording
        CppAD::ADFun<double> f;
        f.Dependent (ax, ay);
        //
        // check ay
        for(size_t i = 0; i < m; ++i)
        {   AD<double> check;
            switch(op)
            {
                case atomic_vector_op::add_enum:
                check = ax[1 + i] + ax[1 + m + i];
                break;

                case atomic_vector_op::sub_enum:
                check = ax[1 + i] - ax[1 + m + i];
                break;

                case atomic_vector_op::mul_enum:
                check = ax[1 + i] * ax[1 + m + i];
                break;

                case atomic_vector_op::div_enum:
                check = ax[1 + i] / ax[1 + m + i];
                break;
            }
            ok &= NearEqual( ay[i] , check,  eps99, eps99);
        }
    }
    return ok;
}
// END C++
