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
# compile_flags
SET(compile_flags "${CMAKE_CXX_FLAGS_DEBUG}" )
IF( "${cppad_debug_which}" STREQUAL "debug_all" )
	SET(compile_flags "${CMAKE_CXX_FLAGS_RELEASE}" )
ENDIF( "${cppad_debug_which}" STREQUAL "debug_all" )
IF( "${cppad_debug_which}" STREQUAL "debug_odd" )
	SET(compile_flags "${CMAKE_CXX_FLAGS_RELEASE}" )
ENDIF( "${cppad_debug_which}" STREQUAL "debug_odd" )
SET(compile_flags
	"${compile_flags} ${cppad_cxx_flags} ${OpenMP_CXX_FLAGS} -DCPPAD_FOR_TMB"
)
#
# name
SET(name cppad_for_tmb)
#
# set compiler flags
SET_SOURCE_FILES_PROPERTIES(
	${name}.cpp PROPERTIES COMPILE_FLAGS "${compile_flags}"
)
#
# now that we have the properties, add the executable
ADD_EXECUTABLE( test_more_${name} EXCLUDE_FROM_ALL ${name}.cpp )
#
# Extra flags used by linker for openmp support
SET(CMAKE_EXE_LINKER_FLAGS ${OpenMP_CXX_FLAGS} )
#
# check_test_more_cppad_for_tmb target
ADD_CUSTOM_TARGET(
	check_test_more_${name} test_more_${name} DEPENDS test_more_${name}
)
MESSAGE(STATUS "make check_test_more_${name}: available")

# Add check_test_more_cppad_for_tmb to check depends in parent environment
add_to_list(check_test_more_depends check_test_more_${name})
SET(check_test_more_depends "${check_test_more_depends}" PARENT_SCOPE)
