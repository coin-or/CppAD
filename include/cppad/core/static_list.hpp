# ifndef CPPAD_CORE_STATIC_LIST_HPP
# define CPPAD_CORE_STATIC_LIST_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2026 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin static_list}

The Base Types with Static Variables in the CppAD Library
#########################################################

Purpose
*******
Shared libraries in windows Visual C++ do not handle static variables
the same as other compilers.
If you are using Visual C++ and :ref:`cmake@cppad_static_lib` is false,
you can only use AD< *Base* > for the base types listed below; e.g.,
you can use AD<double> and AD< AD<double> > .


CPPAD_STATIC_LIST
*****************
{xrst_literal ,
    BEGIN_STATIC_LIST, END_STATIC_LIST
}

{xrst_end static_list}
*/
// BEGIN_STATIC_LIST
# define CPPAD_STATIC_LIST \
    CPPAD_STATIC_LIST_CASE(float) \
    CPPAD_STATIC_LIST_CASE(double) \
    CPPAD_STATIC_LIST_CASE( std::complex<double> ) \
    CPPAD_STATIC_LIST_CASE( AD<float> ) \
    CPPAD_STATIC_LIST_CASE( AD<double> ) \
    CPPAD_STATIC_LIST_CASE( AD< std::complex<double> > )
// END_STATIC_LIST


# endif
