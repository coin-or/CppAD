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
EXTRA_DIST        = test_more.sln test_more.vcproj one_test.sh
#
noinst_PROGRAMS   = test_more
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
test_more_SOURCES  = test_more.cpp \
	abs.cpp \
	acos.cpp \
	asin.cpp \
	add.cpp \
	add_eq.cpp \
	add_zero.cpp \
	atan.cpp \
	atan_2.cpp \
	compare.cpp \
	compare_change.cpp \
	cond_exp.cpp \
	cond_exp_ad.cpp \
	copy.cpp \
	cos.cpp \
	cosh.cpp \
	div.cpp \
	div_eq.cpp \
	div_zero_one.cpp \
	erf.cpp \
	exp.cpp \
	for_hess.cpp \
	for_sparse_jac.cpp \
	forward.cpp \
	from_base.cpp \
	log.cpp \
	log_10.cpp \
	mul.cpp \
	mul_eq.cpp \
	mul_zero_one.cpp \
	near_equal_ext.cpp \
	neg.cpp \
	ode_err_control.cpp \
	poly.cpp \
	pow.cpp \
	pow_int.cpp \
	romberg_one.cpp \
	rosen_34.cpp \
	runge_45.cpp \
	reverse.cpp \
	rev_sparse_hes.cpp \
	rev_sparse_jac.cpp \
	rev_two.cpp \
	simple_vector.cpp \
	sin.cpp \
	sin_cos.cpp \
	sinh.cpp \
	sparse_vec_ad.cpp \
	sqrt.cpp \
	sub.cpp \
	sub_eq.cpp \
	sub_zero.cpp \
	value.cpp \
	vec_ad.cpp \
	vec_ad_par.cpp \
	vec_unary.cpp 
