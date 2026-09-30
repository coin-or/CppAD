# ifndef CPPAD_LOCAL_BASE_LIST_HPP
# define CPPAD_LOCAL_BASE_LIST_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2026 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin base_list dev}

Execute CPPAD_BASE_LIST_CASE for A List of Base Types
#####################################################

{xrst_literal ,
    BEGIN_BASE_LIST, END_BASE_LIST
}

{xrst_end base_list}
*/
// BEGIN_BASE_LIST
# define CPPAD_BASE_LIST \
    CPPAD_BASE_LIST_CASE(float) \
    CPPAD_BASE_LIST_CASE(double)
// END_BASE_LIST


# endif
