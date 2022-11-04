# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# eigen_sources and CPPAD_HAS_EIGEN
IF( cppad_has_eigen )
   SET(eigen_sources eigen_mat_inv.cpp eigen_cholesky.cpp eigen_mat_mul.cpp)
ELSE( cppad_has_eigen )
   SET(eigen_sources "")
ENDIF( cppad_has_eigen )
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   ${eigen_sources}
   atomic_two.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( example_atomic_two "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_atomic_two EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_atomic_two
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_atomic_two
add_check_executable(check_example atomic_two)
