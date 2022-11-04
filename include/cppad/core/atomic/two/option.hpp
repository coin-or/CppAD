# ifndef CPPAD_CORE_ATOMIC_TWO_OPTION_HPP
# define CPPAD_CORE_ATOMIC_TWO_OPTION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin atomic_two_option$$
$spell
    sq
    enum
    afun
    bool
    CppAD
    std
    typedef
$$

$section Set Atomic Function Options$$

$head Syntax$$
$icode%afun%.option(%option_value%)%$$

$head Scope$$
These settings do not apply to individual $icode afun$$ calls,
but rather all subsequent uses of the corresponding atomic operation
in an $cref ADFun$$ object.

$head atomic_sparsity$$
Note that, if you use $cref optimize$$, these sparsity patterns are used
to determine the $cref/dependency/dependency.cpp/$$ relationship between
argument and result variables.

$subhead pack_sparsity_enum$$
If $icode option_value$$ is $codei%atomic_base<%Base%>::pack_sparsity_enum%$$,
then the type used by $icode afun$$ for
$cref/sparsity patterns/glossary/Sparsity Pattern/$$,
(after the option is set) will be
$codei%
    typedef CppAD::vectorBool %atomic_sparsity%
%$$
If $icode r$$ is a sparsity pattern
for a matrix $latex R \in \B{R}^{p \times q}$$:
$icode%r%.size() == %p% * %q%$$.

$subhead bool_sparsity_enum$$
If $icode option_value$$ is $codei%atomic_base<%Base%>::bool_sparsity_enum%$$,
then the type used by $icode afun$$ for
$cref/sparsity patterns/glossary/Sparsity Pattern/$$,
(after the option is set) will be
$codei%
    typedef CppAD::vector<bool> %atomic_sparsity%
%$$
If $icode r$$ is a sparsity pattern
for a matrix $latex R \in \B{R}^{p \times q}$$:
$icode%r%.size() == %p% * %q%$$.

$subhead set_sparsity_enum$$
If $icode option_value$$ is $icode%atomic_base<%Base%>::set_sparsity_enum%$$,
then the type used by $icode afun$$ for
$cref/sparsity patterns/glossary/Sparsity Pattern/$$,
(after the option is set) will be
$codei%
    typedef CppAD::vector< std::set<size_t> > %atomic_sparsity%
%$$
If $icode r$$ is a sparsity pattern
for a matrix $latex R \in \B{R}^{p \times q}$$:
$icode%r%.size() == %p%$$, and for $latex i = 0 , \ldots , p-1$$,
the elements of $icode%r%[%i%]%$$ are between zero and $latex q-1$$ inclusive.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_option.hpp
Setting atomic_base options.
*/

/*!
Setting atomic_base options.

\param option_value
new option value.
*/
template <class Base>
void atomic_base<Base>::option(enum option_enum option_value)
{   switch( option_value )
    {   case pack_sparsity_enum:
        case bool_sparsity_enum:
        case set_sparsity_enum:
        sparsity_ = option_value;
        break;

        default:
        CPPAD_ASSERT_KNOWN(
            false,
            "atoic_base::option: option_value is not valid"
        );
    }
    return;
}

} // END_CPPAD_NAMESPACE
# endif
