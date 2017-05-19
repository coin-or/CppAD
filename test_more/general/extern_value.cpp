/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>
# include "extern_value.hpp"

# define INSTANTIATE(Type) template class extern_value< Type >

template <class Type>
extern_value<Type>::extern_value(Type value)
{	value_ = value; }

template <class Type>
void extern_value<Type>::set(Type value)
{	value_ = value; }

template <class Type>
Type extern_value<Type>::get(void)
{	return value_; }

INSTANTIATE( float );
INSTANTIATE( double );
INSTANTIATE( std::complex<float> );
INSTANTIATE( std::complex<double> );
//
INSTANTIATE( CppAD::AD< float > );
INSTANTIATE( CppAD::AD< double > );
INSTANTIATE( CppAD::AD< std::complex<float> > );
INSTANTIATE( CppAD::AD< std::complex<double> > );
