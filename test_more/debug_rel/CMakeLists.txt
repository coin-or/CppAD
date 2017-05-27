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
# Build the test_more/general tests

# set compiler flags for debug_rel.cpp and debug.cpp
SET_SOURCE_FILES_PROPERTIES(
	debug_rel.cpp debug.cpp PROPERITIES COMPILE_FLAGS
	"${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_DEBUG} -DCPPAD_DEBUG_AND_RELEASE"
)

# et compiler flags for release,cpp
SET_SOURCE_FILES_PROPERTIES(
	release.cpp PROPERITIES COMPILE_FLAGS
	"${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_RELEASE} -DCPPAD_DEBUG_AND_RELEASE"
)

# now that we have the properties, add the executable
ADD_EXECUTABLE(test_more_debug_rel EXCLUDE_FROM_ALL
	debug_rel.cpp
	debug.cpp
	release.cpp
)

# Add the check_test_more_debug_rel target
ADD_CUSTOM_TARGET(check_test_more_debug_rel
	test_more_debug_rel
	DEPENDS
	test_more_debug_rel
)
MESSAGE(STATUS "make check_test_more_debug_rel: available")

# Change check depends in parent environment
add_to_list(check_test_more_depends check_test_more_debug_rel)
SET(check_test_more_depends "${check_test_more_depends}" PARENT_SCOPE)
