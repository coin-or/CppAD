# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
SET(source_list
	sparse.cpp
	sub_sparse_hes.cpp
	sparsity_sub.cpp
	sparse_sub_hes.cpp
	sparse_hes.cpp
	sparse_jac_for.cpp
	sparse_jac_rev.cpp
	sparse_jacobian.cpp
	sparse_hessian.cpp
	rev_hes_sparsity.cpp
	rev_jac_sparsity.cpp
	rev_sparse_jac.cpp
	rev_sparse_hes.cpp
	for_hes_sparsity.cpp
	for_jac_sparsity.cpp
	for_sparse_jac.cpp
	for_sparse_hes.cpp
	dependency.cpp
	conj_grad.cpp
	colpack_jac.cpp
	colpack_hes.cpp
	rc_sparsity.cpp
)
set_compile_flags( example_sparse "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_sparse EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_sparse
	${cppad_lib}
	${colpack_libs}
)

# Add the check_example_sparse target
ADD_CUSTOM_TARGET(check_example_sparse
	example_sparse
	DEPENDS example_sparse
)
MESSAGE(STATUS "make check_example_sparse: available")

# add to check_example_depends in parent environment
add_to_list(check_example_depends check_example_sparse)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
