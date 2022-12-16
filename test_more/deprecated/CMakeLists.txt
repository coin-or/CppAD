# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the test_more/deprecated tests
#
# initialize check_test_more_deprecated
# This directory has both an executable and sub-directories
SET(check_test_more_deprecated_depends "")
#
ADD_SUBDIRECTORY(atomic_two)
ADD_SUBDIRECTORY(chkpoint_one)
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   deprecated.cpp
   old_mat_mul.cpp
   old_reciprocal.cpp
   old_tan.cpp
   old_usead_1.cpp
   old_usead_2.cpp
   omp_alloc.cpp
   track_new_del.cpp
   zdouble.cpp
)
# END_SORT_THIS_LINE_MINUS_2
#
set_compile_flags(
   test_more_deprecated "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(test_more_deprecated EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_deprecated
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_test_more_deprecated
add_check_executable(check_test_more deprecated)
