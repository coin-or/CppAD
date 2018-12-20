# ifndef CPPAD_CORE_ATOMIC_THREE_FORWARD_HPP
# define CPPAD_CORE_ATOMIC_THREE_FORWARD_HPP
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
$begin atomic_three_forward$$
$spell
    taylor
    ataylor
    af
    afun
    enum
    CppAD
$$

$section Atomic Function Forward Mode Callbacks$$

$head Base$$
This syntax and prototype are used by
$codei%
    %afun%(%ax%, %ay%)
%$$
where $cref/afun/atomic_three_ctor/atomic_user/afun/$$
is a user defined atomic function.
They are also used by
$icode%f%.Forward%$$ where $icode f$$ has prototype
$codei%
    ADFun<%Base%> %f%
%$$
and $icode afun$$ is used during the recording of $icode f$$.

$subhead Syntax$$
$icode%ok% = %afun%.forward(
    %order_low%, %order_up%, %type_x%, %type_y%, %taylor_x%, %taylor_y%
)%$$

$subhead Prototype$$
$srcfile%include/cppad/core/atomic/three_forward.hpp
    %0%// BEGIN_PROTOTYPE_BASE%// END_PROTOTYPE_BASE%1
%$$

$head AD<Base>$$
This syntax and prototype are used by
$icode%af%.Forward%$$ where $icode af$$ has prototype
$codei%
    ADFun< AD<%Base%> , %Base% > %af%
%$$
and $icode afun$$ is used in $icode af$$ (see $cref base2ad$$).

$subhead Syntax$$
$icode%ok% = %afun%.forward(
    %order_low%, %order_up%, %type_x%, %type_y%, %ataylor_x%, %ataylor_y%
)%$$

$subhead Prototype$$
$srcfile%include/cppad/core/atomic/three_forward.hpp
    %0%// BEGIN_PROTOTYPE_AD_BASE%// END_PROTOTYPE_AD_BASE%1
%$$

$head Implementation$$
This virtual function must be defined by the
$cref/atomic_user/atomic_three_ctor/atomic_user/$$ class.
It can just return $icode%ok% == false%$$
(and not compute anything) for values
of $icode%order_up%$$ that are greater than those used by your
$cref/forward/Forward/$$ mode calculations
(order zero must be implemented).

$head order_low$$
The argument $icode order_low$$
specifies the lowest order Taylor coefficient that we are computing.

$subhead p$$
We sometimes use the notation $icode%p% = %order_low%$$ below.

$head order_up$$
The argument $icode order_up$$
specifies the highest order Taylor coefficient that we are computing
($icode%order_low% <= %order_up%$$).

$subhead q$$
We sometimes use the notation $icode%q% = %order_up%$$ below.

$head type_x$$
If $icode%type_x%.size() == 0%$$, and it should not be used.
The case $icode%type_x%.size() > 0%$$ only occurs while evaluating a call
$codei%
    %afun%(%ax%, %ay%)
%$$
where $cref/afun/atomic_three_ctor/atomic_user/afun/$$
is a user defined atomic function.
In this case,
$icode%order_up% == 0%$$,
$icode%type_x%.size() == %n%$$, and
for $latex j = 0 , \ldots , n-1$$,
$icode%type_x%[%j%]%$$ specifies if
$icode%ax%[%j%]%$$ is a
$cref/constant parameter/glossary/Parameter/Constant/$$
$cref/dynamic parameter/glossary/Parameter/Dynamic/$$
or $cref/variable/glossary/Variable/$$.

$head type_y$$
If $icode%type_y%.size() == 0%$$, it should not be used.
Otherwise,
$icode%type_x%.size() > 0%$$,
$icode%order_up% == 0%$$,
and $icode%type_y%.size() == %m%$$.
The input values of the elements of $icode type_y$$
are not specified (must not matter).
Upon return, for $latex j = 0 , \ldots , m-1$$,
$icode%type_y%[%i%]%$$ is
$code constant_enum$$ ($code dynamic_enum$$) [$code variable_enum$$]
if $icode%ay%[%i%]%$$ is a
constant parameter (dynamic parameter) [variable].
CppAD uses this information to reduce the necessary computations.

$head taylor_x$$
The size of $icode taylor_x$$ is $codei%(%q%+1)*%n%$$.
For $latex j = 0 , \ldots , n-1$$ and $latex k = 0 , \ldots , q$$,
we use the Taylor coefficient notation
$latex \[
\begin{array}{rcl}
    x_j^k    & = & \R{taylor\_x} [ j * ( q + 1 ) + k ]
    \\
    X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^q t^q
\end{array}
\] $$
Note that superscripts represent an index for $latex x_j^k$$
and an exponent for $latex t^k$$.
Also note that the Taylor coefficients for $latex X(t)$$ correspond
to the derivatives of $latex X(t)$$ at $latex t = 0$$ in the following way:
$latex \[
    x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)
\] $$

$head ataylor_x$$
The specifications for $icode ataylor_x$$ is the same as for $icode taylor_x$$
(only the type of $icode ataylor_x$$ is different).

$head taylor_y$$
The size of $icode taylor_y$$ is $codei%(%q%+1)*%m%$$.
Upon return,
For $latex i = 0 , \ldots , m-1$$ and $latex k = 0 , \ldots , q$$,
$latex \[
\begin{array}{rcl}
    Y_i (t)  & = & f_i [ X(t) ]
    \\
    Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^q t^q + o ( t^q )
    \\
    \R{taylor\_y}  [ i * ( q + 1 ) + k ] & = & y_i^k
\end{array}
\] $$
where $latex o( t^q ) / t^q \rightarrow 0$$ as $latex t \rightarrow 0$$.
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
    \R{taylor\_y}  [ i * ( q + 1 ) + k ] = y_i^k
\]$$
These values do not need to be recalculated
and can be used during the computation of the higher order coefficients.

$head ataylor_y$$
The specifications for $icode ataylor_y$$ is the same as for $icode taylor_y$$
(only the type of $icode ataylor_y$$ is different).

$head ok$$
If the required results are calculated, $icode ok$$ should be true.
Otherwise, it should be false.

$head Discussion$$
For example, suppose that $icode%order_up% == 2%$$,
and you know how to compute the function $latex f(x)$$,
its first derivative $latex f^{(1)} (x)$$,
and it component wise Hessian $latex f_i^{(2)} (x)$$.
Then you can compute $icode taylor_x$$ using the following formulas:
$latex \[
\begin{array}{rcl}
y_i^0 & = & Y(0)
        = f_i ( x^0 )
\\
y_i^1 & = & Y^{(1)} ( 0 )
        = f_i^{(1)} ( x^0 ) X^{(1)} ( 0 )
        = f_i^{(1)} ( x^0 ) x^1
\\
y_i^2
& = & \frac{1}{2 !} Y^{(2)} (0)
\\
& = & \frac{1}{2} X^{(1)} (0)^\R{T} f_i^{(2)} ( x^0 ) X^{(1)} ( 0 )
  +   \frac{1}{2} f_i^{(1)} ( x^0 ) X^{(2)} ( 0 )
\\
& = & \frac{1}{2} (x^1)^\R{T} f_i^{(2)} ( x^0 ) x^1
  +    f_i^{(1)} ( x^0 ) x^2
\end{array}
\] $$
For $latex i = 0 , \ldots , m-1$$, and $latex k = 0 , 1 , 2$$,
$latex \[
    \R{taylor\_y} [ i * (q + 1) + k ] = y_i^k
\] $$

$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_forward.hpp
Atomic forward mode
*/
/*!
Link from atomic_three to forward mode

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param type_x [in]
if size not zero, which components of x are
constants, dynamics, and variables

\param type_y [out]
if size not zero, which components of y are
constants, dynamics, and variables

\param taylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param taylor_y [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for base_three
*/
// BEGIN_PROTOTYPE_BASE
template <class Base>
bool atomic_three<Base>::forward(
    size_t                    order_low  ,
    size_t                    order_up   ,
    const vector<type_enum>&  type_x     ,
    vector<type_enum>&        type_y     ,
    const vector<Base>&       taylor_x   ,
    vector<Base>&             taylor_y   )
// END_PROTOTYPE_BASE
{   return false; }

/*!
Link from atomic_three to forward mode

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param type_x [in]
if size not zero, which components of x are
constants, dynamics, and variables

\param type_y [out]
if size not zero, which components of y are
constants, dynamics, and variables

\param ataylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param ataylor_y [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for base_three
*/
// BEGIN_PROTOTYPE_AD_BASE
template <class Base>
bool atomic_three<Base>::forward(
    size_t                    order_low  ,
    size_t                    order_up   ,
    const vector<type_enum>&  type_x     ,
    vector<type_enum>&        type_y     ,
    const vector< AD<Base> >& ataylor_x  ,
    vector< AD<Base> >&       ataylor_y  )
// END_PROTOTYPE_AD_BASE
{   return false; }


} // END_CPPAD_NAMESPACE
# endif
