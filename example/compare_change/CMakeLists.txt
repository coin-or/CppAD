# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/compare_change directory tests
#
# source_list
SET(source_list compare_change.cpp)
#
# compile_flags
set_compile_flags(
   example_compare_change "${cppad_debug_which}" "${source_list}"
)
#
# example_compare_change
ADD_EXECUTABLE(example_compare_change EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_compare_change
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_compare_change
add_check_executable(check_example compare_change)
