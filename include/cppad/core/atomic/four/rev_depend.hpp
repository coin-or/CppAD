# ifndef CPPAD_CORE_ATOMIC_FOUR_REV_DEPEND_HPP
# define CPPAD_CORE_ATOMIC_FOUR_REV_DEPEND_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin atomic_four_rev_depend$$
$spell
    afun
    enum
    cpp
    taylor
    ident
$$

$section Atomic Function Reverse Dependency$$

$head Syntax$$
You can define one or the other of the following callbacks,
but you should not define both.

$subhead Preferred$$
$icode%ok% = %afun%.rev_depend(%call_id%,
    %ident_zero_x%, %depend_x%, %depend_y%
)%$$

$subhead Deprecated 2022-05-10$$
$icode%ok% = %afun%.rev_depend(%call_id%,
    %depend_x%, %depend_y%
)%$$

$subhead Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Dependency Analysis$$
This calculation is sometimes referred to as a reverse dependency analysis.

$head Implementation$$
This function must be defined if
$cref/afun/atomic_four_ctor/atomic_user/afun/$$ is
used to define an $cref ADFun$$ object $icode f$$,
and $cref/f.optimize()/optimize/$$ is used.

$head Base$$
See $cref/Base/atomic_four_call/Base/$$.

$head vector$$
is the $cref CppAD_vector$$ template class.

$head call_id$$
See $cref/call_id/atomic_four_call/call_id/$$.

$head ident_zero_x$$
This can sometimes be used to create more efficient dependency
(fewer true values in $icode depend_y$$).
If you do not see a way to do this, you can just ignore it.
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
If $icode%ident_zero_x%[%j%]%$$ is true, the argument $icode%ax%[%j%]%$$
is a constant parameter that is identically zero.
An identically zero value times any other value can be treated
as being identically zero.

$head depend_x$$
This vector has size equal to the number of arguments for this atomic function;
i.e. $icode%n%=%ax%.size()%$$ (see $cref/ax/atomic_four_call/ax/$$).
The input values of the elements of $icode depend_x$$
are not specified (must not matter).
Upon return, for $latex j = 0 , \ldots , n-1$$,
$icode%depend_x%[%j%]%$$ is true if the values of interest depend
on the value of $icode ax[j]$$ in the corresponding atomic function call.

$subhead Optimize$$
Parameters and variables,
that the values of interest do not depend on,
may get removed by $cref/optimization/optimize/$$.
The corresponding values in
$cref/taylor_x/atomic_four_forward/taylor_x/$$
(after optimization has removed them) are currently zero,
but perhaps these should be changed back to nan.

$head depend_y$$
This vector has size equal to the number of results for this atomic function;
i.e. $icode%m%=%ay%.size()%$$ (see $cref/ay/atomic_four_call/ay/$$).
For $latex i = 0 , \ldots , m-1$$,
$icode%depend_y%[%i%]%$$ is true if the values of interest depend
on the value of $icode ay[i]$$ in the corresponding atomic function call.

$head ok$$
If this calculation succeeded, $icode ok$$ is true.
Otherwise, it is false.

$head Example$$
The following is an example $code rev_depend$$ definition taken from
$cref atomic_four_norm_sq.cpp$$:
$srcfile%
    example/atomic_four/norm_sq.cpp%
    0%// BEGIN REV_DEPEND%// END REV_DEPEND%0
%$$

$end
-----------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::rev_depend(
    size_t                      call_id      ,
    vector<bool>&               ident_zero_x ,
    vector<bool>&               depend_x     ,
    const vector<bool>&         depend_y     )
// END_PROTOTYPE
{   return false; }

// deprecated version
template <class Base>
bool atomic_four<Base>::rev_depend(
    size_t                      call_id     ,
    vector<bool>&               depend_x    ,
    const vector<bool>&         depend_y    )
// END_PROTOTYPE
{   return false; }

} // END_CPPAD_NAMESPACE

# endif
