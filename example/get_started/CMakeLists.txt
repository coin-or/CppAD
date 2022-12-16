# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/get_started directory tests
#
SET(source_list get_started.cpp)
#
set_compile_flags( example_get_started "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_get_started EXCLUDE_FROM_ALL ${source_list})
#
TARGET_LINK_LIBRARIES(example_get_started ${cppad_lib} ${colpack_libs})
#
# check_example_get_started
add_check_executable(check_example get_started)
