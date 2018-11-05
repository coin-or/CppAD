# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
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
IF( "${debug_which}" STREQUAL "debug_all" )
	SET(debug_extra    "${CMAKE_CXX_FLAGS_DEBUG}")
	SET(release_extra  "${CMAKE_CXX_FLAGS_DEBUG}")
ELSEIF( "${debug_which}" STREQUAL "debug_none" )
	SET(debug_extra    "${CMAKE_CXX_FLAGS_RELEASE}")
	SET(release_extra  "${CMAKE_CXX_FLAGS_RELEASE}")
ELSE( "${debug_which}" )
	SET(debug_extra    "${CMAKE_CXX_FLAGS_DEBUG}")
	SET(release_extra  "${CMAKE_CXX_FLAGS_RELEASE}")
ENDIF( "${debug_which}" STREQUAL "debug_all" )
#
SET_SOURCE_FILES_PROPERTIES(
	debug_rel.cpp debug.cpp PROPERTIES COMPILE_FLAGS
	"${cppad_cxx_flags} ${debug_extra} -DCPPAD_DEBUG_AND_RELEASE"
)
#
SET_SOURCE_FILES_PROPERTIES(
	release.cpp PROPERTIES COMPILE_FLAGS
	"${cppad_cxx_flags} ${release_extra} -DCPPAD_DEBUG_AND_RELEASE"
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
