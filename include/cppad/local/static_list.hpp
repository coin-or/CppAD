# ifndef CPPAD_LOCAL_STATIC_LIST_HPP
# define CPPAD_LOCAL_STATIC_LIST_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2026 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin static_list dev}

Execute CPPAD_STATIC_LIST_CASE for A List of Base Types
#######################################################

{xrst_literal ,
    BEGIN_STATIC_LIST, END_STATIC_LIST
}

{xrst_end static_list}
*/
// BEGIN_STATIC_LIST
# define CPPAD_STATIC_LIST \
    CPPAD_STATIC_LIST_CASE(float) \
    CPPAD_STATIC_LIST_CASE(double) \
    CPPAD_STATIC_LIST_CASE( std::complex<double> )
// END_STATIC_LIST


# endif
