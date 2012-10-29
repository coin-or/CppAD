# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Run the test_more directory tests 
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

IF ( adolc_prefix )
	# Extra source files if we are including adolc
	SET(adolc_sources base_adolc.cpp)
	# Adds -D define flags to the compilation of source files.
	ADD_DEFINITIONS("-DCPPAD_ADOLC_TEST")
ELSE ( adolc_prefix )
	SET(adolc_sources "")
ENDIF ( adolc_prefix )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(test_more test_more.cpp
	${adolc_sources}
	abs.cpp
	acos.cpp
	asin.cpp
	assign.cpp
	add.cpp
	add_eq.cpp
	add_zero.cpp
	atan.cpp
	atan2.cpp
	base_alloc.cpp
	check_simple_vector.cpp
	compare.cpp
	compare_change.cpp
	cond_exp.cpp
	cond_exp_ad.cpp
	copy.cpp
	cos.cpp
	cosh.cpp
	dbl_epsilon.cpp
	div.cpp
	div_eq.cpp
	div_zero_one.cpp
	erf.cpp
	epsilon.cpp
	exp.cpp
	for_hess.cpp
	for_sparse_jac.cpp
	forward.cpp
	from_base.cpp
	fun_check.cpp
	jacobian.cpp
	log.cpp
	log10.cpp
	mul.cpp
	mul_eq.cpp
	mul_level.cpp
	mul_zero_one.cpp
	ndebug.cpp
	near_equal_ext.cpp
	neg.cpp
	ode_err_control.cpp
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
	rev_sparse_hes.cpp
	rev_sparse_jac.cpp
	rev_two.cpp
	simple_vector.cpp
	sin.cpp
	sin_cos.cpp
	sinh.cpp
	sparse_hessian.cpp
	sparse_jacobian.cpp
	sparse_vec_ad.cpp
	sqrt.cpp
	std_math.cpp
	sub.cpp
	sub_eq.cpp
	sub_zero.cpp
	tan.cpp
	test_vector.cpp
	track_new_del.cpp
	value.cpp
	vec_ad.cpp
	vec_ad_par.cpp
	vec_unary.cpp 
)
IF ( adolc_prefix )
	# List of libraries to be linked into the specified target
	TARGET_LINK_LIBRARIES(test_more adolc)
ENDIF ( adolc_prefix )

# add_test(testname Exename arg1 arg2 ... )
# ADD_TEST( test_more test_more )
