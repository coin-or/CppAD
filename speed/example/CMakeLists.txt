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
# Build the speed/cppad directory tests 
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(speed_example example.cpp
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
)

# Compiler flags for cppad source
SET_TARGET_PROPERTIES(
	speed_example PROPERTIES COMPILE_FLAGS "${cppad_cxx_flags}"
)

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_example speed )

# add_test(testname Exename arg1 arg2 ... )
# ADD_TEST( speed_example speed_example )
