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

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
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
    // copy routines
    // ------------------------------------------------------------------------
    static void copy_atx_to_ax(
        size_t                                      n,
        size_t                                      m,
        size_t                                      q,
        size_t                                      k_u,
        size_t                                      k_v,
        const CppAD::vector< CppAD::AD<double> >&   atx,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( atx.size() == n * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i)     * (q+1) + k_u;
            size_t v_index  = (1 + m + i) * (q+1) + k_v;
            ax[1 + i]       = atx[u_index];
            ax[1 + m +i]    = atx[v_index];
        }
    }
    static void copy_ay_to_aty(
        size_t                                      n,
        size_t                                      m,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   ay,
        CppAD::vector< CppAD::AD<double> >&         aty)
    {   assert( aty.size() == m * (q+1) );
        assert( ay.size()  == m );
        for(size_t i = 0; i < m; ++i)
        {   size_t y_index  = i * (q+1) + k;
            aty[y_index]    = ay[i];
        }
    }
    static void copy_aty_to_au(
        size_t                                      n,
        size_t                                      m,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   aty,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( aty.size() == m * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t y_index  = i  * (q+1) + k;
            ax[1 + i]       = aty[y_index];
        }
    }
    static void copy_atx_to_av(
        size_t                                      n,
        size_t                                      m,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   atx,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( atx.size() == n * (q+1) );
        assert( ax.size()  == n );
        for(size_t i = 0; i < m; ++i)
        {   size_t v_index  = (1 +  m + i) * (q+1) + k;
            ax[1 + m + i]   = atx[v_index];
        }
    }
    static void copy_atx_to_au(
        size_t                                      n,
        size_t                                      m,
        size_t                                      q,
        size_t                                      k,
        const CppAD::vector< CppAD::AD<double> >&   atx,
        CppAD::vector< CppAD::AD<double> >&         ax)
    {   assert( atx.size() == n * (q+1) );
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
    // BEGIN forward_add
    void forward_add(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty)
    {
        for(size_t k = p; k <= q; ++k)
        {   for(size_t i = 0; i < m; ++i)
            {   size_t u_index  = (1 + i)     * (q+1) + k;
                size_t v_index  = (1 + m + i) * (q+1) + k;
                size_t y_index  = i *           (q+1) + k;
                // y_i^k = u_i^k + v_i^k
                ty[y_index]     = tx[u_index] + tx[v_index];
            }
        }
    }
    void forward_add(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty)
    {   CppAD::vector< CppAD::AD<double> > ax(n), ay(m);
        ax[0] = CppAD::AD<double>( add_enum );
        for(size_t k = p; k <= q; ++k)
        {   // ax = (op, u^k, v^k)
            copy_atx_to_ax(n, m, q, k, k, atx, ax);
            // ay = u^k + v^k
            (*this)(ax, ay); // atomic vector add
            // y^k = ay
            copy_ay_to_aty(n, m, q, k, ay, aty);
        }
    }
    // END forward_add
    // ----------------------------------------------------------------------
    // forward_sub
    // ----------------------------------------------------------------------
    void forward_sub(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty)
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t u_index  = (1 + i)     * (q+1) + k;
                size_t v_index  = (1 + m + i) * (q+1) + k;
                size_t y_index  = i           * (q+1) + k;
                // y_i^k = u_i^k - v_i^k
                ty[y_index]     = tx[u_index] - tx[v_index];
            }
        }
    }
    void forward_sub(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty)
    {   CppAD::vector< CppAD::AD<double> > ax(n), ay(m);
        ax[0] = CppAD::AD<double>( sub_enum );
        for(size_t k = p; k <= q; ++k)
        {   // ax = (op, u^k, v^k)
            copy_atx_to_ax(n, m, q, k, k, atx, ax);
            // ay = u^k - v^k
            (*this)(ax, ay); // atomic vector subtract
            // y^k = ay
            copy_ay_to_aty(n, m, q, k, ay, aty);
        }
    }
    // ----------------------------------------------------------------------
    // forward_mul
    // ----------------------------------------------------------------------
    void forward_mul(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty)
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t y_index = i * (q+1) + k;
                // y^k = 0
                ty[y_index]    = 0.0;
                for(size_t d = 0; d <= k; d++)
                {   size_t u_index  = (1 + i)     * (q+1) + (k-d);
                    size_t v_index  = (1 + m + i) * (q+1) + d;
                    // y^k += u^{k-d} * v^d
                    ty[y_index]    += tx[u_index] * tx[v_index];
                }
            }
        }
    }
    void forward_mul(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty)
    {   CppAD::vector< CppAD::AD<double> > ax_mul(n), ax_add(n), ay(m);
        ax_mul[0] = CppAD::AD<double>( mul_enum );
        ax_add[0] = CppAD::AD<double>( add_enum );
        for(size_t k = p; k <= q; ++k)
        {   // ay = 0
            for(size_t i = 0; i < m; ++i)
                ay[i] = 0.0;
            for(size_t d = 0; d <= k; d++)
            {   // u_add = ay
                for(size_t i = 0; i < m; ++i)
                    ax_add[1 + i] = ay[i];
                //
                // ax_mul = (op, u^{k-d},  v^d)
                copy_atx_to_ax(n, m, q, k-d, d, atx, ax_mul);
                //
                // ay = u^{k-d} * v^d
                (*this)(ax_mul, ay); // atomic vector multiply
                //
                // v_add = ay
                for(size_t i = 0; i < m; ++i)
                    ax_add[1 + m + i] = ay[i];
                //
                // ay = u_add + v_add
                (*this)(ax_add, ay); // atomic vector add
            }
            // y^k = ay
            copy_ay_to_aty(n, m, q, k, ay, aty);
        }
    }
    // ----------------------------------------------------------------------
    // forward_div
    // ----------------------------------------------------------------------
    void forward_div(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty)
    {
        for(size_t i = 0; i < m; ++i)
        {   for(size_t k = p; k <= q; ++k)
            {   size_t y_index  = i *       (q+1) + k;
                size_t u_index  = (1 + i) * (q+1) + k;
                // y^k = u^k
                ty[y_index]     = tx[u_index];
                for(size_t d = 1; d <= k; d++)
                {   size_t y_other      = i       * (q+1) + (k-d);
                    size_t v_index  = (1 + m + i) * (q+1) + d;
                    // y^k -= y^{k-d} * v^d
                    ty[y_index] -= ty[y_other] * tx[v_index];
                }
                size_t v_index = (1 + m + i ) * (q+1) + 0;
                // y^k /= v^0
                ty[y_index] /= tx[v_index];
            }
        }
    }
    void forward_div(
        size_t                                           n,
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty)
    {   CppAD::vector< CppAD::AD<double> > ax_div(n), ax_mul(n), ax_sub(n), ay(m);
        ax_div[0] = CppAD::AD<double>( div_enum );
        ax_mul[0] = CppAD::AD<double>( mul_enum );
        ax_sub[0] = CppAD::AD<double>( sub_enum );
        for(size_t k = p; k <= q; ++k)
        {   // u_sub = u^k
            copy_atx_to_au(n, m, q, k, atx, ax_sub);
            for(size_t d = 1; d <= k; d++)
            {   // u_mul = y^{k-d}
                copy_aty_to_au(n, m, q, k-d, aty, ax_mul);
                // v_mul = v^d
                copy_atx_to_av(n, m, q, d, atx, ax_mul);
                // ay = y^{k-d} * v^d
                (*this)(ax_mul, ay); // atomic vector multiply
                // v_sub = ay
                for(size_t i = 0; i < m; ++i)
                    ax_sub[1 + m + i] = ay[i];
                // ay = u_sub - v_sub
                (*this)(ax_sub, ay); // atomic vector subtract
                // u_sub = ay
                for(size_t i = 0; i < m; ++i)
                    ax_sub[1 + i] = ay[i];
            }
            // u_div = u_sub
            for(size_t i = 0; i < m; ++i)
                ax_div[1 + i] = ax_sub[1 + i];
            // v_div = v^0
            copy_atx_to_av(n, m, q, 0, atx, ax_div);
            // ay = u_div / v_div
            (*this)(ax_div, ay); // atomic vector divide
            // y^k = ay
            copy_ay_to_aty(n, m, q, k, ay, aty);
        }
    }
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
            forward_mul(n, m, q, p, atx, aty);
            ok = true;
            break;

            // division
            case div_enum:
            forward_div(n, m, q, p, atx, aty);
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
