# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
ADD_EXECUTABLE(test_more_compare_c_c EXCLUDE_FROM_ALL ../det_by_minor.c)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_compare_c_c
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_test_more_compre_c_c
add_check_executable(check_test_more_compare_c c)
