# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list sparse.cpp
   colpack_hes.cpp
   colpack_hessian.cpp
   colpack_jac.cpp
   colpack_jacobian.cpp
   conj_grad.cpp
   dependency.cpp
   for_hes_sparsity.cpp
   for_jac_sparsity.cpp
   for_sparse_hes.cpp
   for_sparse_jac.cpp
   rc_sparsity.cpp
   rev_hes_sparsity.cpp
   rev_jac_sparsity.cpp
   rev_sparse_hes.cpp
   rev_sparse_jac.cpp
   sparse_hes.cpp
   sparse_hessian.cpp
   sparse_jac_for.cpp
   sparse_jac_rev.cpp
   sparse_jacobian.cpp
   sparse_sub_hes.cpp
   sparsity_sub.cpp
   sub_sparse_hes.cpp
   subgraph_hes2jac.cpp
   subgraph_jac_rev.cpp
   subgraph_reverse.cpp
   subgraph_sparsity.cpp
)
# END_SORT_THIS_LINE_MINUS_2

IF( cppad_has_eigen )
   SET(source_list sparse2eigen.cpp ${source_list} )
ENDIF( cppad_has_eigen )

set_compile_flags( example_sparse "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_sparse EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_sparse
   ${cppad_lib}
   ${colpack_libs}
)

# check_example_sparse
add_check_executable(check_example sparse)
