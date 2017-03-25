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
# Build the test_more directory tests
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

# This directory now contains omp_alloc.cpp which uses multi-threading, so
# so no longer can set CPPAD_MAX_NUM_THREADS equal to one (its smallest value)
# ADD_DEFINITIONS("-DCPPAD_MAX_NUM_THREADS=1")

# adolc_sources, adolc_libs and CPPAD_ADOLC_TEST
SET(sources base_adolc.cpp)
sources_libs_define(adolc "${sources}" adolc ADOLC_TEST)

# eigen_sources and CPPAD_EIGEN_TEST
SET(sources cppad_eigen.cpp eigen_mat_inv.cpp)
sources_libs_define(eigen "${sources}" "" EIGEN_TEST)

# ipopt_prefix
SET(sources ipopt_solve.cpp)
sources_libs_define(ipopt "${sources}" "${ipopt_LIBRARIES}" IPOPT_TEST)

# )
ADD_EXECUTABLE(test_more EXCLUDE_FROM_ALL test_more.cpp
	${adolc_sources}
	${eigen_sources}
	${ipopt_sources}
	old_usead_2.cpp
	old_usead_1.cpp
	old_tan.cpp
	old_reciprocal.cpp
	old_mat_mul.cpp
	fabs.cpp
	acos.cpp
	acosh.cpp
	adfun_copy.cpp
	asin.cpp
	asinh.cpp
	assign.cpp
	add.cpp
	add_eq.cpp
	add_zero.cpp
	atan.cpp
	atanh.cpp
	atan2.cpp
	atomic_sparsity.cpp
	azmul.cpp
	base_alloc.cpp
	bool_sparsity.cpp
	check_simple_vector.cpp
	checkpoint.cpp
	compare.cpp
	compare_change.cpp
	cond_exp.cpp
	cond_exp_ad.cpp
	cond_exp_rev.cpp
	copy.cpp
	cos.cpp
	cosh.cpp
	dbl_epsilon.cpp
	dependency.cpp
	div.cpp
	div_eq.cpp
	div_zero_one.cpp
	erf.cpp
	exp.cpp
	expm1.cpp
	extern_value.cpp
	for_hess.cpp
	for_sparse_hes.cpp
	for_sparse_jac.cpp
	forward.cpp
	forward_dir.cpp
	forward_order.cpp
	from_base.cpp
	fun_check.cpp
	hes_sparsity.cpp
	jacobian.cpp
	num_limits.cpp
	log.cpp
	log1p.cpp
	log10.cpp
	mul.cpp
	mul_cskip.cpp
	mul_eq.cpp
	mul_level.cpp
	mul_cond_rev.cpp
	mul_zdouble.cpp
	mul_zero_one.cpp
	near_equal_ext.cpp
	neg.cpp
	ode_err_control.cpp
	omp_alloc.cpp
	optimize.cpp
	parameter.cpp
	poly.cpp
	pow.cpp
	pow_int.cpp
	print_for.cpp
	romberg_one.cpp
	rosen_34.cpp
	runge_45.cpp
	reverse.cpp
	rev_sparse_jac.cpp
	rev_two.cpp
	simple_vector.cpp
	sin.cpp
	sin_cos.cpp
	sinh.cpp
	sparse_hessian.cpp
	sparse_jacobian.cpp
	sparse_sub_hes.cpp
	sparse_vec_ad.cpp
	sqrt.cpp
	std_math.cpp
	sub.cpp
	sub_eq.cpp
	sub_zero.cpp
	tan.cpp
	test_vector.cpp
	to_string.cpp
	track_new_del.cpp
	value.cpp
	vec_ad.cpp
	vec_ad_par.cpp
	vec_unary.cpp
	zdouble.cpp

	local/vector_set.cpp
)

# Compiler flags for cppad source
add_cppad_cxx_flags(test_more)

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more
	${cppad_lib}
	${adolc_libs}
	${ipopt_libs}
	${colpack_libs}
)

# Add the check_test_more target
ADD_CUSTOM_TARGET( check_test_more test_more DEPENDS test_more )
MESSAGE(STATUS "make check_test_more: available")

# Change check depends in parent environment
add_to_list(check_depends check_test_more)
SET(check_depends "${check_depends}" PARENT_SCOPE)
