# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   atomic_three.cpp
   base2ad.cpp
   dynamic.cpp
   forward.cpp
   get_started.cpp
   hes_sparsity.cpp
   jac_sparsity.cpp
   mat_mul.cpp
   norm_sq.cpp
   reciprocal.cpp
   rev_depend.cpp
   reverse.cpp
   tangent.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_atomic_three "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_atomic_three EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_atomic_three
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_atomic_three
add_check_executable(check_example atomic_three)
