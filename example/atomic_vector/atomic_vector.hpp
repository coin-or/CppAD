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

$section Implementing Atomic Vector Operations$$

$childtable%
    example/atomic_vector/forward_op.cpp
    %example/atomic_vector/reverse_op.cpp
    %example/atomic_vector/add_op.cpp
    %example/atomic_vector/sub_op.cpp
    %example/atomic_vector/mul_op.cpp
    %example/atomic_vector/div_op.cpp
    %example/atomic_vector/neg_op.cpp
%$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
class atomic_vector : public CppAD::atomic_three<double> {
//
public:
    // BEGIN_SORT_THIS_LINE_PLUS_4
    // BEGIN op_enum_t
    // atomic_vector::op_enum_t
    typedef enum {
        add_enum,
        div_enum,
        mul_enum,
        neg_enum,
        sub_enum,
        number_op_enum
    } op_enum_t;
    // END op_enum_t
    // END_SORT_THIS_LINE_MINUS_4
    //
    // ctor
    atomic_vector(const std::string& name) :
    CppAD::atomic_three<double>(name)
    { }
private:
    typedef CppAD::vector< CppAD::AD<double> >    ad_vector;
    //
    static bool is_unary(op_enum_t op)
    {   bool result = true;
        switch(op)
        {   case add_enum:
            case sub_enum:
            case mul_enum:
            case div_enum:
            result = false;
            break;

            default:
            break;
        }
        return result;
    }
    // ------------------------------------------------------------------------
    // copy routines
    // ------------------------------------------------------------------------
    static void copy_vec_to_mat(
        size_t                    m,
        size_t                    q,
        size_t                    k ,
        ad_vector::const_iterator vec,
        ad_vector::iterator       mat)
    {   for(size_t i = 0; i < m; ++i)
        {   size_t index  = i * (q+1) + k;
            *(mat + index) = *(vec + i);
        }
    }
    // copy_mat_to_vec
    static void copy_mat_to_vec(
        size_t                    m,
        size_t                    q,
        size_t                    k,
        ad_vector::const_iterator mat,
        ad_vector::iterator       vec)
    {   for(size_t i = 0; i < m; ++i)
        {   size_t index  = i * (q+1) + k;
            *(vec + i)    = *(mat + index);
        }
    }
    // ------------------------------------------------------------------------
    // for_type
    // ------------------------------------------------------------------------
    bool for_type(
        const CppAD::vector<double>&               parameter_x ,
        const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
        CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
    {
        // op, m
        op_enum_t op = op_enum_t( parameter_x[0] );
        size_t n     = parameter_x.size();
        size_t m = (n - 1) / 2;
        if( is_unary(op) )
            m = n - 1;
        //
        // ok
        bool ok  = type_x.size() == n;
        ok      &= type_y.size() == m;
        if( ! ok )
            return false;
        //
        // type_y
        if( is_unary(op) )
            for(size_t i = 0; i < m; ++i)
                type_y[i] = type_x[1 + i];
        else
            for(size_t i = 0; i < m; ++i)
                type_y[i] = std::max( type_x[1 + i] , type_x[1 + m + i] );
        //
        return true;
    }
    // =====================================================================
    // Forward Routines
    // =====================================================================
    // forward
    bool forward(
        const CppAD::vector<double>&                     parameter_x,
        const CppAD::vector<CppAD::ad_type_enum>&        type_x,
        size_t                                           need_y,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    ) override;
    bool forward(
        const CppAD::vector< CppAD::AD<double> >&        aparameter_x,
        const CppAD::vector<CppAD::ad_type_enum>&        type_x,
        size_t                                           need_y,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    ) override;
    // ----------------------------------------------------------------------
    // forward_add
    void forward_add(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_add(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_sub
    void forward_sub(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_sub(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_mul
    void forward_mul(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_mul(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_div
    void forward_div(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_div(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // ----------------------------------------------------------------------
    // forward_neg
    void forward_neg(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        CppAD::vector<double>&                           ty
    );
    void forward_neg(
        size_t                                           m,
        size_t                                           p,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        CppAD::vector< CppAD::AD<double> >&              aty
    );
    // =====================================================================
    // Reverse Routines
    // =====================================================================
    // reverse
    bool reverse(
        const CppAD::vector<double>&                     parameter_x,
        const CppAD::vector<CppAD::ad_type_enum>&        type_x,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        const CppAD::vector<double>&                     ty,
        CppAD::vector<double>&                           px,
        const CppAD::vector<double>&                     py
    ) override;
    bool reverse(
        const CppAD::vector< CppAD::AD<double> >&        aparameter_x,
        const CppAD::vector<CppAD::ad_type_enum>&        type_x,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        const CppAD::vector< CppAD::AD<double> >&        aty,
        CppAD::vector< CppAD::AD<double> >&              apx,
        const CppAD::vector< CppAD::AD<double> >&        apy
    ) override;
    // ----------------------------------------------------------------------
    // reverse_add
    void reverse_add(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        const CppAD::vector<double>&                     ty,
        CppAD::vector<double>&                           px,
        const CppAD::vector<double>&                     py
    );
    void reverse_add(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        const CppAD::vector< CppAD::AD<double> >&        aty,
        CppAD::vector< CppAD::AD<double> >&              apx,
        const CppAD::vector< CppAD::AD<double> >&        apy
    );
    // ----------------------------------------------------------------------
    // reverse_sub
    void reverse_sub(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        const CppAD::vector<double>&                     ty,
        CppAD::vector<double>&                           px,
        const CppAD::vector<double>&                     py
    );
    void reverse_sub(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        const CppAD::vector< CppAD::AD<double> >&        aty,
        CppAD::vector< CppAD::AD<double> >&              apx,
        const CppAD::vector< CppAD::AD<double> >&        apy
    );
    // ----------------------------------------------------------------------
    // reverse_mul
    void reverse_mul(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        const CppAD::vector<double>&                     ty,
        CppAD::vector<double>&                           px,
        const CppAD::vector<double>&                     py
    );
    void reverse_mul(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        const CppAD::vector< CppAD::AD<double> >&        aty,
        CppAD::vector< CppAD::AD<double> >&              apx,
        const CppAD::vector< CppAD::AD<double> >&        apy
    );
    // ----------------------------------------------------------------------
    // reverse_div
    void reverse_div(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        const CppAD::vector<double>&                     ty,
        CppAD::vector<double>&                           px,
        const CppAD::vector<double>&                     py
    );
    void reverse_div(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        const CppAD::vector< CppAD::AD<double> >&        aty,
        CppAD::vector< CppAD::AD<double> >&              apx,
        const CppAD::vector< CppAD::AD<double> >&        apy
    );
    // ----------------------------------------------------------------------
    // reverse_neg
    void reverse_neg(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector<double>&                     tx,
        const CppAD::vector<double>&                     ty,
        CppAD::vector<double>&                           px,
        const CppAD::vector<double>&                     py
    );
    void reverse_neg(
        size_t                                           m,
        size_t                                           q,
        const CppAD::vector< CppAD::AD<double> >&        atx,
        const CppAD::vector< CppAD::AD<double> >&        aty,
        CppAD::vector< CppAD::AD<double> >&              apx,
        const CppAD::vector< CppAD::AD<double> >&        apy
    );
};
// END C++

# endif
