# ifndef CPPAD_CORE_ATOMIC_THREE_JAC_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_THREE_JAC_SPARSITY_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_three_jac_sparsity$$
$spell
    Jacobian
    afun
    jac
$$

$section Atomic Function Jacobian Sparsity Patterns$$

$head Syntax$$
$icode%ok% = %afun%.jac_sparsity(
    %dependency%, %parameter_x%, %select_x%, %select_y%, %pattern_out%
)%$$

$head Prototype$$
$srcfile%include/cppad/core/atomic/three_jac_sparsity.hpp
    %0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head dependency$$
If $icode dependency$$ is true,
then $icode pattern_out$$ is a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
for this atomic function.
Otherwise it is a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ for the
derivative of the atomic function.

$head parameter_x$$
This argument contains the value of the parameters in
$cref/ax/atomic_three_afun/ax/$$ for the corresponding call to
$codei%
    %afun%(%ax%, %ay%)
%$$
These parameters can include information like the number of
arguments to this atomic function; e.g., by setting
$codei%
    %ax%[0] = %Base%( %ax%.size() )
%$$
Note that the value of dynamic parameters may have changed since
the call above; see $cref new_dynamic$$.

$head select_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
It specifies which domain components are included in
the calculation of $icode pattern_out$$.
If $icode%select_x%[%j%]%$$ is false, then there will be no indices
$icode k$$ such that
$codei%
    %pattern_out%.col()[%k%] == %j%
%$$.

$head select_y$$
This argument has size equal to the number of results to this
atomic function; i.e. the size of $icode ay$$.
It specifies which range components are included in
the calculation of $icode pattern_out$$.
If $icode%select_x%[%i%]%$$ is false, then there will be no indices
$icode k$$ such that
$codei%
    %pattern_out%.row()[%k%] == %i%
%$$.

$head pattern_out$$
This input value of $icode pattern_out$$ does not matter.
Upon return it is a
dependency or sparsity pattern for $latex g(x)$$,
the function corresponding to
$cref/afun/atomic_three_ctor/atomic_user/afun/$$;
$icode dependency$$ above.
To be specific, there are non-negative indices
$icode i$$, $icode j$$, $icode k$$ such that
$codei%
    %pattern_out%.row()[%k%] == %i%
    %pattern_out%.col()[%k%] == %j%
%$$
if and only if
$icode%select_x%[%j%]%$$ is true,
$icode%select_y%[%j%]%$$ is true,
and $latex g_i(x)$$ depends on the value of $latex x_j$$
(and the partial of $latex g_i(x)$$ with respect to
$latex x_j$$ is possibly non-zero).

$head ok$$
If this calculation succeeded, $icode ok$$ is true.
Otherwise it is false.


$children%
    example/atomic_three/jac_sparsity.cpp
%$$
$head Examples$$
The file $cref atomic_three_jac_sparsity.cpp$$ contains an example and test
that uses this routine.
$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_jac_sparsity.hpp
Third generation atomic Jacobian dependency and sparsity patterns.
*/
/*!
atomic_three to Jacobian dependency and sparsity calculations.

\param dependency [in]
if true, calculate dependency pattern,
otherwise calcuate sparsity pattern.

\param parameter_x [in]
contains the values for arguments that are parameters.

\param select_x [in]
which domain components to include in the dependency or sparsity pattern.

\param select_y [in]
which range components to include in the dependency or sparsity pattern.

\param pattern_out [out]
is the dependency or sparsity pattern.
*/
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_three<Base>::jac_sparsity(
    bool                                    dependency   ,
    const vector<Base>&                     parameter_x  ,
    const vector<bool>&                     select_x     ,
    const vector<bool>&                     select_y     ,
    sparse_rc< vector<size_t> >&            pattern_out  )
{   return false; }
// END_PROTOTYPE
/*!
Link from forward Jacobian sparsity calcuations to atomic_three

\tparam InternalSparsity
Is the type used for internal sparsity calculations; i.e.,
sparse_pack or sparse_list.

\param dependency
if true, calcuate dependency pattern,
otherwise calcuate sparsity pattern.

\param parameter_x
is parameter arguments to the function, other components are nan.

\param x_index
is the variable index, on the tape, for the arguments to this atomic function.
This size of x_index is n, the number of arguments to this atomic function.

\param y_index
is the variable index, on the tape, for the results for this atomic function.
This size of y_index is m, the number of results for this atomic function.

\param var_sparsity
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the j-th argument to this atomic function.
On output, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th result for this atomic function.

\return
is true if the computation succeeds.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_three<Base>::for_jac_sparsity(
    bool                             dependency   ,
    const vector<Base>&              parameter_x  ,
    const local::pod_vector<size_t>& x_index      ,
    const local::pod_vector<size_t>& y_index      ,
    InternalSparsity&                var_sparsity )
{   typedef typename InternalSparsity::const_iterator iterator;

    // number of arguments and resutls for this atomic function
    size_t n = x_index.size();
    size_t m = y_index.size();

    // selection vectors
    vector<bool> select_x(n), select_y(m);

    // 2DO: perhaps we should use type(type_x, type_y)
    // to reduce the true components in select_y
    for(size_t i = 0; i < m; ++i)
        select_y[i] = true;

    // determine select_x
    for(size_t j = 0; j < n; ++j)
    {   // check if x_j depends on any previous variable
        iterator itr(var_sparsity, x_index[j]);
        size_t ell = *itr;
        select_x[j] = ell < var_sparsity.end();
    }
    sparse_rc< vector<size_t> > pattern_out;
    bool ok = jac_sparsity(
        dependency, parameter_x, select_x, select_y, pattern_out
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

} // END_CPPAD_NAMESPACE
# endif
