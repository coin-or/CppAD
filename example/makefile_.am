# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# automake input file
#
# Microsoft project files
EXTRA_DIST        = example.sln example.vcproj one_test.sh
#
noinst_PROGRAMS   = example 
#
if CppAD_BOOSTVECTOR
AM_CPPFLAGS       = -I$(BOOST_DIR) \
                    -I. -I$(top_builddir) 
else
AM_CPPFLAGS       = -I. -I$(top_builddir)
endif
#
AM_CXXFLAGS       = -g $(CPP_ERROR_WARN)
#
example_SOURCES   = \
	example.cpp \
	abs.cpp \
	acos.cpp \
	add.cpp \
	add_eq.cpp \
	asin.cpp \
	atan.cpp \
	atan_2.cpp \
	bender_quad.cpp \
	bool_fun.cpp \
	vector_bool.cpp \
	check_numeric_type.cpp \
	check_simple_vector.cpp \
	compare.cpp \
	compare_change.cpp \
	complex_poly.cpp \
	cond_exp.cpp \
	copy_ad.cpp \
	copy_base.cpp \
	cos.cpp \
	cosh.cpp \
	cpp_ad_vector.cpp \
	default.cpp \
	det_by_lu.cpp \
	det_by_lu.hpp \
	det_by_minor.cpp \
	det_by_minor.hpp \
	det_of_minor.cpp \
	det_of_minor.hpp \
	div.cpp \
	div_eq.cpp \
	eq.cpp \
	equal_op_seq.cpp \
	erf.cpp \
	error_handler.cpp \
	exp.cpp \
	for_one.cpp \
	for_two.cpp \
	forward.cpp \
	for_sparse_jac.cpp \
	fun_check.cpp \
	hes_lagrangian.cpp \
	hes_lu_det.cpp \
	hes_minor_det.cpp \
	hessian.cpp \
	hes_times_dir.cpp \
	independent.cpp \
	integer.cpp \
	interface_2c.cpp\
	jac_lu_det.cpp \
	jac_minor_det.cpp \
	jacobian.cpp \
	log.cpp \
	log_10.cpp \
	lu_factor.cpp \
	lu_invert.cpp \
	lu_ratio.cpp \
	lu_solve.cpp \
	lu_vec_ad.cpp \
	lu_vec_ad.hpp \
	lu_vec_ad_ok.cpp \
	mul.cpp \
	mul_eq.cpp \
	mul_tape.cpp \
	near_equal.cpp \
	near_equal_ext.cpp \
	numeric_type.cpp \
	ode_err_control.cpp \
	ode_err_maxabs.cpp \
	ode_gear.cpp \
	ode_gear_control.cpp\
	ode_stiff.cpp \
	ode_taylor.cpp \
	output.cpp \
	par_var.cpp \
	piecewise.cpp \
	poly.cpp \
	pow.cpp \
	pow_int.cpp \
	reverse_any.cpp \
	reverse_one.cpp \
	rev_one.cpp \
	rev_sparse_hes.cpp \
	rev_sparse_jac.cpp \
	rev_two.cpp \
	romberg_mul.cpp \
	romberg_one.cpp \
	rosen_34.cpp \
	runge_45.cpp \
	seq_property.cpp \
	simple_vector.cpp \
	sin.cpp \
	sinh.cpp \
	sqrt.cpp \
	stack_machine.cpp \
	sub.cpp \
	sub_eq.cpp \
	tan.cpp \
	tape_index.cpp \
	track_new_del.cpp \
	unary_minus.cpp \
	unary_plus.cpp \
	value.cpp \
	var_2par.cpp \
	vec_ad.cpp 
