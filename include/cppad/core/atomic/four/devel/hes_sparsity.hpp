# ifndef CPPAD_CORE_ATOMIC_FOUR_DEVEL_HES_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_FOUR_DEVEL_HES_SPARSITY_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*
-----------------------------------------------------------------------------
$begin atomic_four_for_hes_sparsity$$

$section Link from Forward Hesswian Sparsity Sweep to atomic_four Callback$$

$head Prototype$$
$srcthisfile%0%// BEGIN_FOR_HES_SPARSITY%// END_FOR_HES_SPARSITY%1%$$

$head InternalSparsity$$
Is the used internaly for sparsity calculations; i.e.,
sparse_pack or sparse_list.

$head call_id [in]$$
see $cref/call_id/atomic_four_call/call_id/$$.

$head ident_zero_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
If $icode%ident_zero_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a constant parameter that is identically zero.

$head x_index$$
is the variable index, on the tape, for the arguments to this function.
This size of x_index is n, the number of arguments to this function.
The index zero is used for parameters.

$head y_index$$
is the variable index, on the tape, for the results for this function.
This size of y_index is m, the number of results for this function.
The index zero is used for parameters.

$head np1$$
is the number of components of x plus one; i.e. $icode%n% + 1%$$.

$head numvar$$
is the total number of variables in the tape; i.e.,
$icode%play%->num_var_rec()%$$.

$head for_sparsity$$
The sparsity patterns with index zero and index $icode np1$$ are empty
(because they correspond to parameters).

$subhead On Input$$
On input, for j = 0, ... , n-1,
the forward Jacobian sparsity for the $th j$$ arguemnt to the atomic fucntion
is the sparsity pattern with index $icode%np1% + %x_index[j]%$$.
In addition, the sparsity pattern with index $th j+1$$ contains
the non-zero cross partial indices where $th j+1$$ is the other index.
This Hessian does not include the atomic operation.

$subhead On Output$$
On output, for i = 0, ... , m-1,
the forward Jacobian sparsity for the $th i$$ result of the atomic fucntion
is the sparsity pattern with index $icode%np1% + %y_index[i]%$$.
In addition, the sparsity pattern with index $th j+1$$ contains
the non-zero cross partial indices where $th j+1$$ is the other index.
This Hessian includes the atomic operation.

$head rev_jac_pattern$$
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the reverse Jacobian sparsity for the i-th result to this atomic function.
This shows which components of the result affect the function we are
computing the Hessian of.

$head hes_sparsity_for$$
This is the sparsity pattern for the Hessian. On input, the non-linear
terms in the atomic fuction have not been included. Upon return, they
have been included.

$end
*/
template <class Base>
template <class InternalSparsity>
bool atomic_four<Base>::for_hes_sparsity(
    size_t                           call_id          ,
    const vector<bool>&              ident_zero_x     ,
    const local::pod_vector<size_t>& x_index          ,
    const local::pod_vector<size_t>& y_index          ,
    size_t                           np1              ,
    size_t                           numvar           ,
    const InternalSparsity&          rev_jac_pattern  ,
    InternalSparsity&                for_sparsity     )
{   typedef typename InternalSparsity::const_iterator const_iterator;
    //
    CPPAD_ASSERT_UNKNOWN( rev_jac_pattern.end() == 1 );
    CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
    CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + numvar );
    CPPAD_ASSERT_UNKNOWN( for_sparsity.number_elements(0) == 0 );
    CPPAD_ASSERT_UNKNOWN( for_sparsity.number_elements(np1) == 0 );
    //
    size_t n      = x_index.size();
    size_t m      = y_index.size();
    //
    // select_x
    vector<bool> select_x(n);
    for(size_t j = 0; j < n; j++)
    {   // check if should compute pattern w.r.t x[j]
        select_x[j] = for_sparsity.number_elements(np1 + x_index[j]) > 0;
    }
    //
    // bool select_y
    vector<bool> select_y(m);
    for(size_t i = 0; i < m; i++)
    {   // check if we should include y[i]
        select_y[i] = rev_jac_pattern.number_elements(y_index[i]) > 0;
    }
    // ------------------------------------------------------------------------
    // call user's version of atomic function for Jacobian
    sparse_rc< vector<size_t> > pattern_out;
    bool dependency = false;
    bool ok = jac_sparsity( call_id,
        dependency, ident_zero_x, select_x, select_y, pattern_out
    );
    if(! ok) ok = jac_sparsity(
        call_id, dependency, select_x, select_y, pattern_out
    );
    if( ! ok )
        return false;
    //
    // transfer sparsity patterns from pattern_out to var_sparsity
    size_t                nnz = pattern_out.nnz();
    const vector<size_t>& row( pattern_out.row() );
    const vector<size_t>& col( pattern_out.col() );
    for(size_t k = 0; k < nnz; ++k)
    {   size_t i = row[k];
        size_t j = col[k];
        CPPAD_ASSERT_KNOWN(
            select_y[i] & select_x[j],
            "atomic: jac_sparsity: pattern_out not in "
            "select_x or select_y range"
        );
        const_iterator itr(for_sparsity, np1 + x_index[j]);
        size_t ell = *itr;
        while( ell < np1 )
        {   for_sparsity.post_element(np1 + y_index[i], ell );
            ell = *(++itr);
        }
    }
    for(size_t i = 0; i < m; ++i)
        for_sparsity.process_post( np1 + y_index[i] );
    // ------------------------------------------------------------------------
    // call user's version of atomic function for Hessian
    ok = hes_sparsity(
        call_id, ident_zero_x, select_x, select_y, pattern_out
    );
    if(! ok ) ok = hes_sparsity(
        call_id, select_x, select_y, pattern_out
    );
    if( ! ok )
        return ok;
    //
    // add new elements to Hessian sparisty in calling routine
    nnz = pattern_out.nnz();
    for(size_t k = 0; k < nnz; ++k)
    {   size_t r = row[k];
        size_t c = col[k];
        CPPAD_ASSERT_KNOWN(
            select_x[r] & select_x[c],
            "atomic: hes_sparsity: pattern_out not in select_x range"
        );
        const_iterator itr_1(for_sparsity, np1 + x_index[r]);
        size_t v1 = *itr_1;
        while( v1 < np1 )
        {   for_sparsity.binary_union(
                v1, v1, np1 + x_index[c], for_sparsity
             );
             v1 = *(++itr_1);
        }
        // no need to add same elements twice
        if( c != r )
        {   const_iterator itr_2(for_sparsity, np1 + x_index[c]);
            size_t v2 = *itr_2;
            while( v2 < np1 )
            {   for_sparsity.binary_union(
                    v2, v2, np1 + x_index[r], for_sparsity
                );
                v2 = *(++itr_2);
            }
        }
    }
    return ok;
}
/*
$begin atomic_four_rev_hes_sparsity$$

$section Link from Reverse Hessian Sparsity Sweep to atomic_four Callback$$

$head Prototype$$
$srcthisfile%0%// BEGIN_REV_HES_SPARSITY%// END_REV_HES_SPARSITY%1%$$

$head InternalSparsity$$
Is the used internaly for sparsity calculations; i.e.,
sparse_pack or sparse_list.

$head call_id [in]$$
see $cref/call_id/atomic_four_call/call_id/$$.

$head ident_zero_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
If $icode%ident_zero_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a constant parameter that is identically zero.

$head x_index$$
is the variable index, on the tape, for the arguments to this function.
This size of x_index is n, the number of arguments to this function.
The index zero is used for parameters.

$head y_index$$
is the variable index, on the tape, for the results for this function.
This size of y_index is m, the number of results for this function.
The index zero is used for parameters.

$head for_jac_pattern$$
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the forward Jacobian pattern for the j-th argument to this atomic function.

$head rev_jac_flag$$
On input, for i = 0, ... , m-1, rev_jac_flag[ y_index[i] ] is true
if the function we are computing the Hessian of has possibly non-zero Jacobian
w.r.t varialbe y_index[i].
On output, for j = 0, ... , n, rev_jac_flag[ x_index[j] ] is set to true
if the varialbe with index x_index[j] has possible non-zero Jacobian
with repect to one of the true y_index[i] cases.
Otherwise, rev_jac_flag [ x_inde[j] ] is not changed.

$head hes_sparsity_rev$$
Is the reverse mode sparsity pattern for the Hessian. On input, the non-linear
terms in the atomic fuction have not been included. Upon return, they
have been included.

$end
*/
template <class Base>
template <class InternalSparsity>
bool atomic_four<Base>::rev_hes_sparsity(
    size_t                           call_id          ,
    const vector<bool>&              ident_zero_x     ,
    const local::pod_vector<size_t>& x_index          ,
    const local::pod_vector<size_t>& y_index          ,
    const InternalSparsity&          for_jac_pattern  ,
    bool*                            rev_jac_flag     ,
    InternalSparsity&                hes_sparsity_rev )
{   CPPAD_ASSERT_UNKNOWN( for_jac_pattern.number_elements(0) == 0 );
    CPPAD_ASSERT_UNKNOWN( ! rev_jac_flag[0] );
    //
    size_t n      = x_index.size();
    size_t m      = y_index.size();
    //
    // select_x
    vector<bool> select_x(n);
    for(size_t j = 0; j < n; j++)
        select_x[j] = for_jac_pattern.number_elements( x_index[j] ) > 0;
    //
    // select_y
    vector<bool> select_y(m);
    for(size_t i = 0; i < m; i++)
        select_y[i] = rev_jac_flag[ y_index[i] ];
    //
    // call atomic function for Jacobain sparsity
    bool dependency = false;
    sparse_rc< vector<size_t> > pattern_jac;
    bool ok = jac_sparsity( call_id,
        dependency, ident_zero_x, select_x, select_y, pattern_jac
    );
    if(! ok) ok = jac_sparsity(
        call_id, dependency, select_x, select_y, pattern_jac
    );
    const vector<size_t>& row_jac( pattern_jac.row() );
    const vector<size_t>& col_jac( pattern_jac.col() );
    size_t nnz_jac = pattern_jac.nnz();
    if( ! ok )
        return ok;
    //
    // call atomic function for Hessian sparsity
    sparse_rc< vector<size_t> > pattern_hes;
    ok = hes_sparsity(
        call_id, ident_zero_x, select_x, select_y, pattern_hes
    );
    if( ! ok ) ok = hes_sparsity(
        call_id, select_x, select_y, pattern_hes
    );
    if( ! ok )
        return ok;
    //
    // row_hes, col_hes, nnz_hes
    const vector<size_t>& row_hes( pattern_hes.row() );
    const vector<size_t>& col_hes( pattern_hes.col() );
    size_t nnz_hes = pattern_hes.nnz();
    //
    // propagate Hessian sparsity through the Jacobian
    for(size_t k = 0; k < nnz_jac; ++k)
    {   size_t i = row_jac[k];
        size_t j = col_jac[k];
        CPPAD_ASSERT_KNOWN(
            select_y[i] & select_x[j] ,
            "atomic: jac_sparsity: pattern_out not in "
            "select_x or select_y range"
        );
        // from y_index[i] to x_index[j]
        hes_sparsity_rev.binary_union(
            x_index[j], x_index[j], y_index[i], hes_sparsity_rev
        );
    }
    //
    // propagate rev_jac_flag through the Jacobian
    // (seems OK to exclude variables with zero forward jacobian)
    for(size_t k = 0; k < nnz_jac; ++k)
    {   size_t j = col_jac[k];
        rev_jac_flag[ x_index[j] ] = true;
    }
    //
    // new hessian sparsity terms between y and x
    for(size_t k = 0; k < nnz_hes; ++k)
    {   size_t r = row_hes[k];
        size_t c = col_hes[k];
        CPPAD_ASSERT_KNOWN(
            select_x[r] & select_x[c] ,
            "atomic: hes_sparsity: pattern_out not in select_x range"
        );
        hes_sparsity_rev.binary_union(
            x_index[r], x_index[r], x_index[c], for_jac_pattern
        );
        hes_sparsity_rev.binary_union(
            x_index[c], x_index[c], x_index[r], for_jac_pattern
        );
    }
    return ok;
}

} // END_CPPAD_NAMESPACE

# endif
