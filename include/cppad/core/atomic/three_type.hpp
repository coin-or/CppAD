# ifndef CPPAD_CORE_ATOMIC_THREE_TYPE_HPP
# define CPPAD_CORE_ATOMIC_THREE_TYPE_HPP
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
$begin atomic_three_type$$
$spell
    afun
    enum
$$

$section Atomic Function Type Calculation$$

$head Syntax$$
$icode%ok% = %afun%.type(%type_x%, %type_y%)%$$

$subhead Prototype$$
$srcfile%include/cppad/core/atomic/three_type.hpp
    %0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Usage$$
This syntax and prototype are used by
$codei%
    %afun%(%ax%, %ay%)
%$$
where $cref/afun/atomic_three_ctor/atomic_user/afun/$$
is a user defined atomic function.

$head Implementation$$
This virtual function must be defined by the
$cref/atomic_user/atomic_three_ctor/atomic_user/$$ class.

$head type_x$$
This vector has size equal to the number of arguments for this atomic function;
i.e. $icode%n%=%ax%.size()%$$.
For $latex j = 0 , \ldots , n-1$$,
$icode%type_x%[%j%]%$$ specifies if
$icode%ax%[%j%]%$$ is a
$cref/constant parameter/glossary/Parameter/Constant/$$
$cref/dynamic parameter/glossary/Parameter/Dynamic/$$
or $cref/variable/glossary/Variable/$$.

$head type_y$$
This vector has size equal to the number of results for this atomic function;
i.e. $icode%m%=%ay%.size()%$$.
The input values of the elements of $icode type_y$$
are not specified (must not matter).
Upon return, for $latex i = 0 , \ldots , m-1$$,
$icode%type_y%[%i%]%$$ is $code constant_enum$$ if it only depends on
the arguments that are constants.
It is $code dynamic_enum$$ if it depends on an argument
that is a dynamic parameter and does not depend on any variables.
It is $code variable_enum$$ if it depends on an argument that is a variable.

$head ok$$
If this calculation succeeded, $icode ok$$ is true.
Otherwise, it is false.

$head Examples$$

$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_type.hpp
Third generation atomic type computation.
*/
/*!
Link from atomic_three to type calculation

\param type_x [in]
specifies which components of x are
constants, dynamics, and variables

\param type_y [out]
specifies which components of y are
constants, dynamics, and variables
*/
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_three<Base>::type(
    const vector<ad_type_enum>&  type_x     ,
    vector<ad_type_enum>&        type_y     )
// END_PROTOTYPE
{   return false; }

} // END_CPPAD_NAMESPACE

# endif
