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
EXTRA_DIST        = TestMore.sln TestMore.vcproj OneTest.sh
#
noinst_PROGRAMS   = TestMore
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
TestMore_SOURCES  = test_more.cpp \
	abs_.cpp \
	acos_.cpp \
	asin_.cpp \
	add_.cpp \
	add_eq.cpp \
	add_zero.cpp \
	atan_.cpp \
	atan_2.cpp \
	compare_.cpp \
	compare_change.cpp \
	cond_exp.cpp \
	cond_exp_ad.cpp \
	copy_.cpp \
	cos_.cpp \
	cosh_.cpp \
	div_.cpp \
	div_eq.cpp \
	div_zero_one.cpp \
	erf_.cpp \
	exp_.cpp \
	for_hess.cpp \
	for_sparse_jac.cpp \
	forward_.cpp \
	from_base.cpp \
	log_.cpp \
	log_10.cpp \
	mul_.cpp \
	mul_eq.cpp \
	mul_zero_one.cpp \
	near_equal_ext.cpp \
	neg_.cpp \
	ode_err_control.cpp \
	poly_.cpp \
	pow_.cpp \
	pow_int.cpp \
	rosen_34.cpp \
	runge_45.cpp \
	reverse_.cpp \
	rev_sparse_hes.cpp \
	rev_sparse_jac.cpp \
	rev_two.cpp \
	simple_vector.cpp \
	sin_.cpp \
	sin_cos.cpp \
	sinh_.cpp \
	sparse_vec_ad.cpp \
	sqrt_.cpp \
	sub_.cpp \
	sub_eq.cpp \
	sub_zero.cpp \
	value_.cpp \
	vec_ad.cpp \
	vec_ad_par.cpp \
	vec_unary.cpp 
