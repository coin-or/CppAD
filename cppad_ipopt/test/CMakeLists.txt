# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the cppad_ipopt/test directory tests
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../src ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(cppad_ipopt_test EXCLUDE_FROM_ALL test_more.cpp
	k_gt_one.cpp
	multiple_solution.cpp
	retape_k1_l1.cpp
	retape_k1_l2.cpp
)
add_cppad_cxx_flags(cppad_ipopt_test)

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(cppad_ipopt_test
	cppad_ipopt
	${cppad_lib}
	${ipopt_LIBRARIES}
	${colpack_libs}
)

# Add the check_cppad_ipopt_test target
ADD_CUSTOM_TARGET(check_cppad_ipopt_test
	cppad_ipopt_test
	DEPENDS cppad_ipopt_test cppad_ipopt
)
