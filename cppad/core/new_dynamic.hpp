# ifndef CPPAD_CORE_NEW_DYNAMIC_HPP
# define CPPAD_CORE_NEW_DYNAMIC_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin new_dynamic$$
$spell
	const
	Taylor
	cpp
$$

$section Change the Dynamic Parameters$$

$head Syntax$$
$icode%f%.new_dynamic(%dynamic%)%$$

$head Purpose$$
Often one is only interested in computing derivatives with respect
to a subset of arguments to a function.
In this case, it is easier to make all the arguments to the function
$cref/independent variables/glossary/Tape/Independent Variable/$$.
It is more efficient,
will use less memory and be faster,
if the only the argument were are computing derivatives with respect to
are independent variables and the other arguments are
$cref/dynamic parameters/glossary/Dynamic Parameter/$$.
The $code new_dynamic$$ method is used to change the value
of the dynamic parameters in $icode f$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the $cref ADFun$$ object $icode f$$ is not $code const$$.

$head dynamic$$
This argument has prototype
$codei%
	const %VectorBase%& %dynamic%
%$$
(see $icode VectorBase$$ below).
It specifies a new value for the
$cref/dynamic parameter/glossary/Dynamic Parameter/$$ vector.
It size must be the same as the size of the
$cref/dynamic/Independent/dynamic/$$ parameter vector
in the call to $code Independent$$ that started
the recording for $icode f$$; see
$cref/size_dynamic/seq_property/size_dynamic/$$.

$head VectorBase$$
The type $icode VectorBase$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$icode Base$$.

$head Taylor Coefficients$$
The Taylor coefficients computed by previous calls to
$cref/f.Forward/Forward/$$ are lost after this operation; including the
order zero coefficients (because they may depend on the dynamic parameters).
In order words;
$cref/f.size_order/size_order/$$ returns zero directly after
$icode%f%.new_dynamic%$$ is called.

$head Restrictions$$
Dynamic parameters are only allowed in operations where one of the other
operands is a $cref/Variable/glossary/Variable/$$.
You must compute all the parameters that depend on dynamic parameters
before the call to $cref Independent$$ and include them as part of the
$cref/dynamic/Independent/dynamic/$$ parameter vector.

$children%
	example/general/new_dynamic.cpp
%$$
$head Example$$
The file $cref new_dynamic.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file new_dynamic.hpp
User interface to ADFun dynamic_parameter member function.
*/

/*!
Change the dynamic parameters in this ADFun object

\param dynamic
is the vector of new values for the dynamic parameters.
*/
template <typename Base>
template <typename VectorBase>
void ADFun<Base>::new_dynamic(const VectorBase& dynamic)
{	// num_dynamic
	size_t num_dynamic = play_.num_dynamic();
	CPPAD_ASSERT_UNKNOWN( size_t( dynamic.size() ) == num_dynamic );
	CPPAD_ASSERT_UNKNOWN( num_dynamic <= play_.num_par_rec() );

	// check VectorBase is Simple Vector class with Base elements
	CheckSimpleVector<Base, VectorBase>();

	// set the dynamic parameters
	for(size_t j = 0; j < num_dynamic; ++j)
		play_.set_dynamic(j, dynamic[j]);

	// we now have zero taylor_ coefficient orders per variable
	num_order_taylor_ = 0;

	return;
}


} // END_CPPAD_NAMESPACE
# endif
