# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the speed/cppad directory tests 
# Inherit environment from ../CMakeList.txt
# CMAKE_BUILD_TYPE determined by parent directory

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
# We do not add ../src/speed_src library to avoid undefined externals.
# Instead we build our own copy of ../src/microsoft_timer.cpp.
ADD_EXECUTABLE(speed_example EXCLUDE_FROM_ALL example.cpp
	det_by_lu.cpp
	det_by_minor.cpp
	det_of_minor.cpp
	elapsed_seconds.cpp
	mat_sum_sq.cpp
	ode_evaluate.cpp
	sparse_hes_fun.cpp
	sparse_jac_fun.cpp
	speed_test.cpp
	time_test.cpp
	"../src/microsoft_timer.cpp"
)

# Compiler flags for cppad source
add_cppad_cxx_flags( speed_example )

# Add the check_speed_example target
ADD_CUSTOM_TARGET(check_speed_example 
	speed_example 
	DEPENDS speed_example speed_src
)
