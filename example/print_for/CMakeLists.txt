# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/print_for directory tests
#
SET(source_list print_for.cpp)
#
set_compile_flags( example_print_for "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_print_for EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_print_for
   ${cppad_lib}
   ${colpack_libs}
)
# check_example_print_for
add_check_executable(check_example print_for)
