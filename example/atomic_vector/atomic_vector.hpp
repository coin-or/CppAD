# ifndef CPPAD_EXAMPLE_ATOMIC_VECTOR_ATOMIC_VECTOR_HPP
# define CPPAD_EXAMPLE_ATOMIC_VECTOR_ATOMIC_VECTOR_HPP

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
$begin atomic_vector.hpp$$

$section Atomic Vector Operations$$

$childtable%
    example/atomic_vector/add_op.cpp
    %example/atomic_vector/sub_op.cpp
    %example/atomic_vector/mul_op.cpp
    %example/atomic_vector/div_op.cpp
%$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
class atomic_vector : public CppAD::atomic_three<double> {
//
public:
    // BEGIN op_enum_t
    // atomic_vector::op_enum_t
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
    atomic_vector(const std::string& name) :
    CppAD::atomic_three<double>(name)
    { }
private:
    // ------------------------------------------------------------------------
    // copy routines
    // ------------------------------------------------------------------------
    // au = atu^k_u
    // av = atv^k_v
    static void copy_atx_to_ax(
        size_t                                      n,
        size_t                                      q,
        size_t                                      k_u,
        size_t                                      k_v,
        const CppAD::vector< CppAD::AD<double> >&   atx,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( n % 2 == 1 );
        size_t m = (n - 1) / 2;
        //
        assert( atx.size() == n * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i)     * (q+1) + k_u;
            size_t v_index  = (1 + m + i) * (q+1) + k_v;
            ax[1 + i]       = atx[u_index];
            ax[1 + m +i]    = atx[v_index];
        }
    }
    // aty^k = ay
    static void copy_ay_to_aty(
        size_t                                      n,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   ay,
        CppAD::vector< CppAD::AD<double> >&         aty)
    {   assert( n % 2 == 1 );
        size_t m = (n - 1) / 2;
        //
        assert( aty.size() == m * (q+1) );
        assert( ay.size()  == m );
        for(size_t i = 0; i < m; ++i)
        {   size_t y_index  = i * (q+1) + k;
            aty[y_index]    = ay[i];
        }
    }
    // au = aty^k
    static void copy_aty_to_au(
        size_t                                      n,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   aty,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( n % 2 == 1 );
        size_t m = (n - 1) / 2;
        //
        assert( aty.size() == m * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t y_index  = i  * (q+1) + k;
            ax[1 + i]       = aty[y_index];
        }
    }
    // av = atv^k
    static void copy_atx_to_av(
        size_t                                      n,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   atx,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( n % 2 == 1 );
        size_t m = (n - 1) / 2;
        //
        assert( atx.size() == n * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t v_index  = (1 +  m + i) * (q+1) + k;
            ax[1 + m + i]   = atx[v_index];
        }
    }
    // au = atu^k
    static void copy_atx_to_au(
        size_t                                      n,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   atx,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( n % 2 == 1 );
        size_t m = (n - 1) / 2;
        //
        assert( atx.size() == n * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i) * (q+1) + k;
            ax[1 + i]       = atx[u_index];
        }
    }
    // ------------------------------------------------------------------------
    // for_type
    // ------------------------------------------------------------------------
    bool for_type(
        const CppAD::vector<double>&               parameter_x ,
        const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
        CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
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
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_add(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_sub
    // ----------------------------------------------------------------------
    void forward_sub(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_sub(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_mul
    // ----------------------------------------------------------------------
    void forward_mul(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_mul(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_div
    // ----------------------------------------------------------------------
    void forward_div(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_div(
        size_t                                           n,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward
    // forward mode routines called by ADFun<Base> objects
    // ----------------------------------------------------------------------
    bool forward(
        const CppAD::vector<double>&                     parameter_x,
        const CppAD::vector<CppAD::ad_type_enum>&        type_x,
        size_t                                           need_y,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty) override
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
            forward_add(n, q, p, tx, ty);
            break;

            // subtraction
            case sub_enum:
            forward_sub(n, q, p, tx, ty);
            break;

            // multiplication
            case mul_enum:
            forward_mul(n, q, p, tx, ty);
            break;

            // division
            case div_enum:
            forward_div(n, q, p, tx, ty);
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
    // forward mode routines called by ADFun< CppAD::AD<Base> , Base> objects
    // ----------------------------------------------------------------------
    bool forward(
        const CppAD::vector< CppAD::AD<double> >&        aparameter_x,
        const CppAD::vector<CppAD::ad_type_enum>&        type_x,
        size_t                                           need_y,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty         ) override
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
            forward_add(n, q, p, atx, aty);
            ok = true;
            break;

            // subtraction
            case sub_enum:
            forward_sub(n, q, p, atx, aty);
            ok = true;
            break;

            // multiplication
            case mul_enum:
            forward_mul(n, q, p, atx, aty);
            ok = true;
            break;

            // division
            case div_enum:
            forward_div(n, q, p, atx, aty);
            ok = true;
            break;

            // error
            case num_op:
            assert(false);
            ok = false;
            break;
        }
        return ok;
    }

};
// END C++

# endif
