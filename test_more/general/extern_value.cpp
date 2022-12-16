// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/cppad.hpp>
# include "extern_value.hpp"

# define INSTANTIATE(Type) template class extern_value< Type >

template <class Type>
extern_value<Type>::extern_value(Type value)
{  value_ = value; }

template <class Type>
void extern_value<Type>::set(Type value)
{  value_ = value; }

template <class Type>
Type extern_value<Type>::get(void)
{  return value_; }

INSTANTIATE( float );
INSTANTIATE( double );
INSTANTIATE( std::complex<float> );
INSTANTIATE( std::complex<double> );
//
INSTANTIATE( CppAD::AD< float > );
INSTANTIATE( CppAD::AD< double > );
INSTANTIATE( CppAD::AD< std::complex<float> > );
INSTANTIATE( CppAD::AD< std::complex<double> > );
