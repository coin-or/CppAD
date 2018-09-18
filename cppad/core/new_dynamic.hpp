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
	dyn
	ind
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
$cref/dynamic/glossary/Parameter/Dynamic/$$ parameters.
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
It specifies a new value for the independent
$cref/dynamic/glossary/Parameter/Dynamic/$$ parameters.
It size must be the same as the size of the independent
$cref/dynamic/Independent/dynamic/$$ parameter vector
in the call to $code Independent$$ that started
the recording for $icode f$$; see
$cref/size_dyn_ind/seq_property/size_dyn_ind/$$.

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

$children%
	example/general/new_dynamic.cpp
%$$
$head Example$$
The file $cref new_dynamic.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
*/
# include <cppad/local/sweep/dynamic.hpp>

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
template <typename Base, typename RecBase>
template <typename VectorBase>
void ADFun<Base,RecBase>::new_dynamic(const VectorBase& dynamic)
{	using local::pod_vector;
	CPPAD_ASSERT_KNOWN(
		size_t( dynamic.size() ) == play_.num_dynamic_ind() ,
		"f.new_dynamic: dynamic.size() different from corresponding "
		"call to Independent"
	);
	// check VectorBase is Simple Vector class with Base elements
	CheckSimpleVector<Base, VectorBase>();

	// retrieve player information about the dynamic parameters
	local::pod_vector_maybe<Base>&     all_par_vec( play_.all_par_vec() );
	const pod_vector<bool>&            dyn_par_is ( play_.dyn_par_is()  );
	const pod_vector<local::opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
	const pod_vector<addr_t>&          dyn_par_arg( play_.dyn_par_arg() );
	const pod_vector<addr_t>&     dyn_ind2par_ind ( play_.dyn_ind2par_ind() );

	// set the dependent dynamic parameters
	local::sweep::dynamic(
		all_par_vec     ,
		dynamic         ,
		dyn_par_is      ,
		dyn_ind2par_ind ,
		dyn_par_op      ,
		dyn_par_arg
	);

	// the existing Taylor coefficients are no longer valid
	num_order_taylor_ = 0;

	return;
}


} // END_CPPAD_NAMESPACE
# endif
