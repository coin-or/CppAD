# ifndef CPPAD_CORE_ATOMIC_FOUR_FORWARD_HPP
# define CPPAD_CORE_ATOMIC_FOUR_FORWARD_HPP
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
$begin atomic_four_forward$$
$spell
    taylor
    ataylor
    af
    afun
    enum
    CppAD
    aparameter
$$

$section Atomic Function Forward Mode$$

$head Syntax$$

$subhead Base$$
$icode%ok% = %afun%.forward(
    %call_id%, %select_y%,
    %order_low%, %order_up%, %type_x%, %taylor_x%, %taylor_y%
)%$$

$subhead AD<Base>$$
$icode%ok% = %afun%.forward(
    %call_id%, %select_y%,
    %order_low%, %order_up%, %type_x%, %ataylor_x%, %ataylor_y%
)%$$

$head Prototype$$

$subhead Base$$
$srcthisfile%0%// BEGIN_PROTOTYPE_BASE%// END_PROTOTYPE_BASE%1
%$$

$subhead AD<Base>$$
$srcthisfile%0%// BEGIN_PROTOTYPE_AD_BASE%// END_PROTOTYPE_AD_BASE%1
%$$

$head Base$$
see $cref/Base/atomic_four_call/Base/$$.

$head vector$$
is the $cref CppAD_vector$$ template class.


$head Usage$$

$subhead Base$$
The $icode Base$$ syntax and prototype are used by a
$cref/call/atomic_four_call/$$ to the atomic function $icode afun$$.
They are also used by
$icode%f%.Forward%$$ and $icode%f%.new_dynamic%$$
where $icode f$$ has prototype
$codei%
    ADFun<%Base%> %f%
%$$
and $icode afun$$ is used during the recording of $icode f$$.

$subhead AD<Base>$$
The $codei%AD<%Base%>%$$ syntax and prototype are used by
$icode%af%.Forward%$$ and $icode%af%.new_dynamic%$$
where $icode af$$ has prototype
$codei%
    ADFun< AD<%Base%> , %Base% > %af%
%$$
and $icode afun$$ is used in a function $icode af$$,
created from $icode f$$ using $cref base2ad$$.

$head Implementation$$
The $icode taylor_x$$, $icode taylor_y$$ version of this function
must be defined by the
$cref/atomic_user/atomic_four_ctor/atomic_user/$$ class.
It can return $icode%ok% == false%$$
(and not compute anything) for values
of $icode%order_up%$$ that are greater than those used by your
$cref/forward/Forward/$$ mode calculations.
Order zero must be implemented.

$head call_id$$
See $cref/call_id/atomic_four_call/call_id/$$.

$head select_y$$
This argument has size equal to the number of results to this
atomic function; i.e. the size of $cref/ay/atomic_four_call/ay/$$.
It specifies which components of $icode y$$ the corresponding
Taylor coefficients must be computed.

$head order_low$$
This argument
specifies the lowest order Taylor coefficient that we are computing.

$subhead p$$
We sometimes use the notation $icode%p% = %order_low%$$ below.

$head order_up$$
This argument is the highest order Taylor coefficient that we
are computing ($icode%order_low% <= %order_up%$$).

$subhead q$$
We use the notation $icode%q% = %order_up% + 1%$$ below.
This is the number of Taylor coefficients for each
component of $icode x$$ and $icode y$$.

$head taylor_x$$
The size of $icode taylor_x$$ is $icode%q%*%n%$$.
For $latex j = 0 , \ldots , n-1$$ and $latex k = 0 , \ldots , q-1$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
    x_j^k    & = & \R{taylor\_x} [ j * q + k ]
    \\
    X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^{q-1} t^{q-1}
\end{array}
\] $$
Note that superscripts represent an index for $latex x_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex X(t)$$ correspond
to the derivatives of $latex X(t)$$ at $latex t = 0$$ in the following way:
$latex \[
    x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)
\] $$

$subhead parameters$$
If the $th j$$ component of $icode x$$ is a parameter,
$codei%
    %type_x%[%j%] < CppAD::variable_enum
%$$
In this case, for $icode%k% > 0%$$,
$codei%
    %taylor_x%[ %j% * %q% + %k% ] == 0
%$$

$head ataylor_x$$
The specifications for $icode ataylor_x$$ is the same as for $icode taylor_x$$
(only the type of $icode ataylor_x$$ is different).

$head taylor_y$$
The size of $icode taylor_y$$ is $icode%q%*%m%$$.
Upon return,
For $latex i = 0 , \ldots , m-1$$ and $latex k = 0 , \ldots , q-1$$,
if $icode select_y[i]$$ is true,
$latex \[
\begin{array}{rcl}
    Y_i (t)  & = & g_i [ X(t) ]
    \\
    Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^{q-1} t^{q-1} + o( t^{q-1} )
    \\
    \R{taylor\_y}  [ i * q + k ] & = & y_i^k
\end{array}
\] $$
where $latex o( t^{q-1} ) / t^{q-1} \rightarrow 0$$
as $latex t \rightarrow 0$$.
Note that superscripts represent an index for $latex y_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex Y(t)$$ correspond
to the derivatives of $latex Y(t)$$ at $latex t = 0$$ in the following way:
$latex \[
    y_j^k = \frac{1}{ k ! } Y_j^{(k)} (0)
\] $$
If $latex p > 0$$,
for $latex i = 0 , \ldots , m-1$$ and $latex k = 0 , \ldots , p-1$$,
the input of $icode taylor_y$$ satisfies
$latex \[
    \R{taylor\_y}  [ i * q + k ] = y_i^k
\]$$
These values do not need to be recalculated
and can be used during the computation of the higher order coefficients.

$head ataylor_y$$
The specifications for $icode ataylor_y$$ is the same as for $icode taylor_y$$
(only the type of $icode ataylor_y$$ is different).

$head ok$$
If this calculation succeeded, $icode ok$$ is true.
Otherwise, it is false.

$head Discussion$$
For example, suppose that $icode%order_up% == 2%$$,
and you know how to compute the function $latex g(x)$$,
its first derivative $latex g^{(1)} (x)$$,
and it component wise Hessian $latex g_i^{(2)} (x)$$.
Then you can compute $icode taylor_x$$ using the following formulas:
$latex \[
\begin{array}{rcl}
y_i^0 & = & Y(0)
        = g_i ( x^0 )
\\
y_i^1 & = & Y^{(1)} ( 0 )
        = g_i^{(1)} ( x^0 ) X^{(1)} ( 0 )
        = g_i^{(1)} ( x^0 ) x^1
\\
y_i^2
& = & \frac{1}{2 !} Y^{(2)} (0)
\\
& = & \frac{1}{2} X^{(1)} (0)^\R{T} g_i^{(2)} ( x^0 ) X^{(1)} ( 0 )
  +   \frac{1}{2} g_i^{(1)} ( x^0 ) X^{(2)} ( 0 )
\\
& = & \frac{1}{2} (x^1)^\R{T} g_i^{(2)} ( x^0 ) x^1
  +    g_i^{(1)} ( x^0 ) x^2
\end{array}
\] $$
For $latex i = 0 , \ldots , m-1$$, and $latex k = 0 , 1 , 2$$,
$latex \[
    \R{taylor\_y} [ i * q + k ] = y_i^k
\] $$


$head Example$$
The following is an example $code forward$$ definition taken from
$cref atomic_four_norm_sq.cpp$$:
$srcfile%
    example/atomic_four/norm_sq.cpp%
    0%// BEGIN FORWARD%// END FORWARD%0
%$$

$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE_BASE
template <class Base>
bool atomic_four<Base>::forward(
    size_t                       call_id     ,
    const vector<bool>&          select_y    ,
    size_t                       order_low   ,
    size_t                       order_up    ,
    const vector<Base>&          taylor_x    ,
    vector<Base>&                taylor_y    )
// END_PROTOTYPE_BASE
{   return false; }

// BEGIN_PROTOTYPE_AD_BASE
template <class Base>
bool atomic_four<Base>::forward(
    size_t                       call_id      ,
    const vector<bool>&          select_y    ,
    size_t                       order_low    ,
    size_t                       order_up     ,
    const vector< AD<Base> >&    ataylor_x    ,
    vector< AD<Base> >&          ataylor_y    )
// END_PROTOTYPE_AD_BASE
{   return false; }


} // END_CPPAD_NAMESPACE
# endif
