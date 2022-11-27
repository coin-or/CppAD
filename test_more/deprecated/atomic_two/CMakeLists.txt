# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# eigen_sources and CPPAD_HAS_EIGEN
IF( cppad_has_eigen )
ELSE( cppad_has_eigen )
   SET(eigen_sources "")
ENDIF( cppad_has_eigen )
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   atomic_sparsity.cpp
   atomic_two.cpp
   base2ad.cpp
   for_sparse_hes.cpp
   for_sparse_jac.cpp
   forward.cpp
   get_started.cpp
   mat_mul.cpp
   norm_sq.cpp
   reciprocal.cpp
   rev_sparse_hes.cpp
   rev_sparse_jac.cpp
   reverse.cpp
   set_sparsity.cpp
   tangent.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(test_more_deprecated_atomic_two
   "${cppad_debug_which}" "${source_list}"
)
#
ADD_EXECUTABLE(test_more_deprecated_atomic_two
   EXCLUDE_FROM_ALL ${source_list}
)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_deprecated_atomic_two
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_test_more_deprecated_atomic_two
add_check_executable(check_test_more_deprecated atomic_two)
