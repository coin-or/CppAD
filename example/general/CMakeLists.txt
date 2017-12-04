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
# Build the example/general directory tests
#
# adolc_sources, adolc_libs
IF( cppad_has_adolc )
	SET(adolc_sources mul_level_adolc.cpp mul_level_adolc_ode.cpp)
	SET(adolc_libs adolc)
ELSE( cppad_has_adolc )
	SET(adolc_sources "")
	SET(adolc_libs "")
ENDIF( cppad_has_adolc )
#
# eigen_sources
IF( cppad_has_eigen )
	SET(eigen_sources eigen_det.cpp eigen_array.cpp)
ELSE( cppad_has_eigen )
	SET(eigen_sources "")
ENDIF( cppad_has_eigen )
#
SET(source_list
	general.cpp
	${adolc_sources}
	${eigen_sources}
	abort_recording.cpp
	fabs.cpp
	acos.cpp
	acosh.cpp
	ad_assign.cpp
	ad_ctor.cpp
	add.cpp
	add_eq.cpp
	ad_fun.cpp
	ad_in_c.cpp
	ad_input.cpp
	ad_output.cpp
	asin.cpp
	asinh.cpp
	atan2.cpp
	atan.cpp
	atanh.cpp
	azmul.cpp
	base_alloc.hpp
	base_require.cpp
	bender_quad.cpp
	bool_fun.cpp
	capacity_order.cpp
	change_param.cpp
	check_for_nan.cpp
	compare_change.cpp
	compare.cpp
	complex_poly.cpp
	cond_exp.cpp
	cos.cpp
	cosh.cpp
	div.cpp
	div_eq.cpp
	equal_op_seq.cpp
	erf.cpp
	exp.cpp
	expm1.cpp
	for_one.cpp
	for_two.cpp
	forward.cpp
	forward_dir.cpp
	forward_order.cpp
	fun_assign.cpp
	fun_check.cpp
	hes_lagrangian.cpp
	hes_lu_det.cpp
	hes_minor_det.cpp
	hessian.cpp
	hes_times_dir.cpp
	independent.cpp
	integer.cpp
	interface2c.cpp
	interp_onetape.cpp
	interp_retape.cpp
	jac_lu_det.cpp
	jac_minor_det.cpp
	jacobian.cpp
	log10.cpp
	log1p.cpp
	log.cpp
	lu_ratio.cpp
	lu_vec_ad.cpp
	lu_vec_ad.hpp
	lu_vec_ad_ok.cpp
	mul.cpp
	mul_eq.cpp
	mul_level.cpp
	mul_level_ode.cpp
	near_equal_ext.cpp
	number_skip.cpp
	numeric_type.cpp
	num_limits.cpp
	ode_stiff.cpp
	ode_taylor.cpp
	opt_val_hes.cpp
	par_var.cpp
	poly.cpp
	pow.cpp
	pow_int.cpp
	print_for.cpp
	reverse_checkpoint.cpp
	reverse_one.cpp
	reverse_three.cpp
	reverse_two.cpp
	rev_one.cpp
	rev_two.cpp
	rosen_34.cpp
	runge45_2.cpp
	seq_property.cpp
	sign.cpp
	sin.cpp
	sinh.cpp
	sqrt.cpp
	stack_machine.cpp
	sub.cpp
	sub_eq.cpp
	tan.cpp
	tanh.cpp
	tape_index.cpp
	unary_minus.cpp
	unary_plus.cpp
	value.cpp
	var2par.cpp
	vec_ad.cpp
)
set_compile_flags( example_general "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_general EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_general
	${adolc_libs}
)
#
# Add the check_example_general target
ADD_CUSTOM_TARGET(check_example_general
	example_general
	DEPENDS example_general
)
MESSAGE(STATUS "make check_example_general: available")
#
# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_general)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
