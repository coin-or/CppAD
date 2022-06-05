# ifndef CPPAD_CORE_ATOMIC_FOUR_DEVEL_JAC_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_FOUR_DEVEL_JAC_SPARSITY_HPP
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
------------------------------------------------------------------------------
$begin atomic_four_for_jac_sparsity$$
$spell
    Jacobian
    var
    ident
$$

$section Link from Forward Jacobian Sparsity Sweep to atomic_four Callback$$

$head Prototype$$
$srcthisfile%0%// BEGIN_FOR_JAC_SPARSITY%// END_FOR_JAC_SPARSITY%1%$$

$head InternalSparsity$$
is the type used for internal sparsity calculations; i.e.,
sparse_pack or sparse_list.

$head call_id$$
see $cref/call_id/atomic_four_call/call_id/$$.

$head dependency$$
if true, calculate dependency pattern,
otherwise calculate sparsity pattern.

$head ident_zero_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
If $icode%ident_zero_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a constant parameter that is identically zero.

$head x_index$$
is the variable index, on the tape, for the arguments to this atomic function.
This size of x_index is, the number of arguments to this atomic function.
The index zero is used for parameters.

$head y_index$$
is the variable index, on the tape, for the results for this atomic function.
This size of y_index is m, the number of results for this atomic function.
The index zero is used for parameters.

$head var_sparsity$$
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the j-th argument to this atomic function.
On output, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th result for this atomic function.

$head Return Value$$
is true if the computation succeeds.

$end
*/
// BEGIN_FOR_JAC_SPARSITY
template <class Base>
template <class InternalSparsity>
bool atomic_four<Base>::for_jac_sparsity(
    size_t                           call_id      ,
    bool                             dependency   ,
    const vector<bool>&              ident_zero_x ,
    const local::pod_vector<size_t>& x_index      ,
    const local::pod_vector<size_t>& y_index      ,
    InternalSparsity&                var_sparsity )
// END_FOR_JAC_SPARSITY
{   typedef typename InternalSparsity::const_iterator iterator;

    // number of arguments and resutls for this atomic function
    size_t n = x_index.size();
    size_t m = y_index.size();

    // select_y
    vector<bool> select_y(m);
    for(size_t i = 0; i < m; ++i)
        select_y[i] = y_index[i] != 0;

    // determine select_x
    vector<bool> select_x(n);
    for(size_t j = 0; j < n; ++j)
    {   if( x_index[j] == 0 )
            select_x[j] = false;
        else
        {   // check if x_j depends on any previous variable
            iterator itr(var_sparsity, x_index[j]);
            size_t ell = *itr;
            select_x[j] = ell < var_sparsity.end();
        }
    }
    sparse_rc< vector<size_t> > pattern_out;
    bool ok = jac_sparsity( call_id,
        dependency, ident_zero_x, select_x, select_y, pattern_out
    );
    if( ! ok ) ok = jac_sparsity(
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
        iterator itr(var_sparsity, x_index[j]);
        size_t ell = *itr;
        while( ell < var_sparsity.end() )
        {   var_sparsity.post_element( y_index[i], ell );
            ell = *(++itr);
        }
    }
    for(size_t i = 0; i < m; ++i)
        var_sparsity.process_post( y_index[i] );
    //
    return true;
}

/*
------------------------------------------------------------------------------
$begin atomic_four_rev_jac_sparsity$$
$spell
    Jacobian
    var
    ident
$$

$section Link from Reverse Jacobian Sparsity Sweep to atomic_four Callback$$

$head Prototype$$
$srcthisfile%0%// BEGIN_REV_JAC_SPARSITY%// END_REV_JAC_SPARSITY%1%$$

$head InternalSparsity$$
Is the type used for internal sparsity calculations; i.e.,
sparse_pack or sparse_list.

$head call_id$$
see $cref/call_id/atomic_four_call/call_id/$$

$head dependency$$
if true, calculate dependency pattern,
otherwise calculate sparsity pattern.

$head ident_zero_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
If $icode%ident_zero_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a constant parameter that is identically zero.

$head x_index$$
is the variable index, on the tape, for the arguments to this atomic function.
This size of x_index is n, the number of arguments to this atomic function.
The index zero is used for parameters.

$head y_index$$
is the variable index, on the tape, for the results for this atomic function.
This size of y_index is m, the number of results for this atomic function.
The index zero is used for parameters.

$head var_sparsity$$
We are given a sparsity pattern an outer function G(y, x) and compute
the pattern for an inner function H(x), which is the outer functions
with the components of y treated as functions of x; i.e.
H(x) = G( Y(x), x).

$subhead y_index$$
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity of the outer function with respect to the i-th
result for this atomic function.

$subhead x_index$$
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the outer function with respect to the j-th
argument to this atomic function.
On output, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the inner function with respect to the j-th
argument to this atomic function.

$head Return Value$$
is true if the computation succeeds.

$end
*/
// BEGIN_REV_JAC_SPARSITY
template <class Base>
template <class InternalSparsity>
bool atomic_four<Base>::rev_jac_sparsity(
    size_t                           call_id      ,
    bool                             dependency   ,
    const vector<bool>&              ident_zero_x ,
    const local::pod_vector<size_t>& x_index      ,
    const local::pod_vector<size_t>& y_index      ,
    InternalSparsity&                var_sparsity )
// END_REV_JAC_SPARSITY
{   typedef typename InternalSparsity::const_iterator iterator;

    // number of arguments and resutls for this atomic function
    size_t n = x_index.size();
    size_t m = y_index.size();

    // selection vectors
    vector<bool> select_x(n), select_y(m);

    // select_x
    for(size_t j = 0; j < n; ++j)
        select_x[j] = x_index[j] != 0;

    // determine select_y
    for(size_t i = 0; i < m; ++i)
    {   if( y_index[i] == 0 )
            select_y[i] = false;
        else
        {   // check if y_i has sparsity is non-empty
            iterator itr(var_sparsity, y_index[i]);
            size_t ell = *itr;
            select_y[i] = ell < var_sparsity.end();
        }
    }
    sparse_rc< vector<size_t> > pattern_out;
    bool ok = jac_sparsity( call_id,
        dependency, ident_zero_x, select_x, select_y, pattern_out
    );
    if( ! ok ) ok = jac_sparsity(
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
        iterator itr(var_sparsity, y_index[i]);
        size_t ell = *itr;
        while( ell < var_sparsity.end() )
        {   var_sparsity.post_element( x_index[j], ell );
            ell = *(++itr);
        }
    }
    for(size_t j = 0; j < n; ++j)
        var_sparsity.process_post( x_index[j] );
    //
    return true;
}

} // END_CPPAD_NAMESPACE
# endif
