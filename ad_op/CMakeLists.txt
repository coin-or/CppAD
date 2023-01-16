# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the ad_op directory tests
#
SET(source_list ad_op.cpp)
#
set_compile_flags( ad_op "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(ad_op EXCLUDE_FROM_ALL ${source_list})
#
# check_example_print_for
add_check_executable(check ad_op)
