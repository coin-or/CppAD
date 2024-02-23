# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   cppad_code_gen.cpp
   file.cpp
   function.cpp
   jac_as_fun.cpp
   jacobian.cpp
   sparse_jac_as_fun.cpp
   sparse_jacobian.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(
   example_cppad_code_gen "${cppad_debug_which}" "${source_list}" 
)
#
ADD_EXECUTABLE(example_cppad_code_gen EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_cppad_code_gen
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_cppad_code_gen
add_check_executable(check_example cppad_code_gen)
