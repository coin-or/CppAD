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
# eigen_sources and CPPAD_HAS_EIGEN
IF( cppad_has_eigen )
	SET(eigen_sources eigen_mat_inv.cpp eigen_cholesky.cpp eigen_mat_mul.cpp)
ELSE( cppad_has_eigen )
	SET(eigen_sources "")
ENDIF( cppad_has_eigen )
#
SET(source_list
	${eigen_sources}
	atomic.cpp
	checkpoint.cpp
	extended_ode.cpp
	for_sparse_hes.cpp
	for_sparse_jac.cpp
	forward.cpp
	get_started.cpp
	mat_mul.cpp
	mul_level.cpp
	norm_sq.cpp
	ode.cpp
	reciprocal.cpp
	rev_sparse_hes.cpp
	rev_sparse_jac.cpp
	reverse.cpp
	set_sparsity.cpp
	tangent.cpp
)
set_compile_flags( example_atomic "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_atomic EXCLUDE_FROM_ALL ${source_list})
#
# Add the check_example_atomic target
ADD_CUSTOM_TARGET(check_example_atomic
	example_atomic
	DEPENDS example_atomic
)
MESSAGE(STATUS "make check_example_atomic: available")
#
# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_atomic)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
