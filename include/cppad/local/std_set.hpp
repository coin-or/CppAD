# ifndef CPPAD_LOCAL_STD_SET_HPP
# define CPPAD_LOCAL_STD_SET_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-26 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/define.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file std_set.hpp
Two constant standard sets (currently used for concept checking).
*/

/*!
A standard set with one element.
*/
template <class Scalar>
std::set<Scalar> one_element_std_set(void)
{
    std::set<Scalar> one;
    one.insert(1);
    return one;
}
/*!
A standard set with a two elements.
*/
template <class Scalar>
std::set<Scalar> two_element_std_set(void)
{
    std::set<Scalar> two;
    two.insert(1);
    two.insert(2);
    return two;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
