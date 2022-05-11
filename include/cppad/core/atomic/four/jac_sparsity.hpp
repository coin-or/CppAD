# ifndef CPPAD_CORE_ATOMIC_FOUR_JAC_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_FOUR_JAC_SPARSITY_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/atomic/four/devel/jac_sparsity.hpp>

/*
$begin atomic_four_jac_sparsity$$
$spell
    Jacobian
    afun
    jac
    ident
$$

$section Atomic Function Jacobian Sparsity Patterns$$

$head Syntax$$
You can define one or the other of the following callbacks,
but you should not define both.

$subhead Preferred$$
$icode%ok% = %afun%.jac_sparsity( %call_id%,
    %dependency%, %ident_zero_x%, %select_x%, %select_y%, %pattern_out%
)%$$

$subhead Deprecated 2022-05-10$$
$icode%ok% = %afun%.jac_sparsity(
    %dependency%, %call_id%, %select_x%, %select_y%, %pattern_out%
)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Implementation$$
This function must be defined if
$cref/afun/atomic_four_ctor/atomic_user/afun/$$ is
used to define an $cref ADFun$$ object $icode f$$,
and Jacobian sparsity patterns are computed for $icode f$$.
(Computing Hessian sparsity patterns
requires Jacobian sparsity patterns.)

$head Base$$
See $cref/Base/atomic_four_call/Base/$$.

$head vector$$
is the $cref CppAD_vector$$ template class.

$head call_id$$
See $cref/call_id/atomic_four_call/call_id/$$.

$head dependency$$
If $icode dependency$$ is true,
then $icode pattern_out$$ is a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
for this atomic function.
Otherwise it is a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ for the
derivative of the atomic function.

$head ident_zero_x$$
This can sometimes be used to create more efficient sparsity patterns.
If you do not see a way to do this, you can just ignore it.
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
If $icode%ident_zero_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a constant parameter that is identically zero.
For example, an identically zero value times any other value can be treated
as being identically zero.

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
If $icode%select_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a variable and $icode%ident_zero_x%[%j%]%$$ will be false.

$head select_y$$
This argument has size equal to the number of results to this
atomic function; i.e. the size of $icode ay$$.
It specifies which range components are included in
the calculation of $icode pattern_out$$.
If $icode%select_y%[%i%]%$$ is false, then there will be no indices
$icode k$$ such that
$codei%
    %pattern_out%.row()[%k%] == %i%
%$$.

$head pattern_out$$
This input value of $icode pattern_out$$ does not matter.
Upon return it is a
dependency or sparsity pattern for the Jacobian of $latex g(x)$$,
the function corresponding to
$cref/afun/atomic_four_ctor/atomic_user/afun/$$.
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

$head Example$$
The following is an example $code jac_sparsity$$ definition taken from
$cref atomic_four_norm_sq.cpp$$:
$srcfile%
    example/atomic_four/norm_sq.cpp%
    0%// BEGIN JAC_SPARSITY%// END JAC_SPARSITY%0
%$$

$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::jac_sparsity(
    size_t                                  call_id      ,
    bool                                    dependency   ,
    const vector<bool>&                     ident_zero_x ,
    const vector<bool>&                     select_x     ,
    const vector<bool>&                     select_y     ,
    sparse_rc< vector<size_t> >&            pattern_out  )
// END_PROTOTYPE
{   return false; }
//
// deprecated version of jac_sparsity callback
template <class Base>
bool atomic_four<Base>::jac_sparsity(
    size_t                                  call_id      ,
    bool                                    dependency   ,
    const vector<bool>&                     select_x     ,
    const vector<bool>&                     select_y     ,
    sparse_rc< vector<size_t> >&            pattern_out  )
{   return false; }
} // END_CPPAD_NAMESPACE

# endif
