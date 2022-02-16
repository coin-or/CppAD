# ifndef CPPAD_CORE_ATOMIC_FOUR_REV_DEPEND_HPP
# define CPPAD_CORE_ATOMIC_FOUR_REV_DEPEND_HPP
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
$begin atomic_four_rev_depend$$
$spell
    afun
    enum
    cpp
    taylor.hpp
$$

$section Atomic Function Reverse Dependency$$

$head Syntax$$
$icode%ok% = %afun%.rev_depend(%call_id%, %depend_x%, %depend_y%)%$$

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
    size_t                      call_id     ,
    vector<bool>&               depend_x    ,
    const vector<bool>&         depend_y    )
// END_PROTOTYPE
{   return false; }

} // END_CPPAD_NAMESPACE

# endif
