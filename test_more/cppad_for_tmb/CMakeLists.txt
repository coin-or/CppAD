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
# use cppad_debug_which to determine build type
IF( "${cppad_debug_which}" STREQUAL debug_all )
	SET(CMAKE_BUILD_TYPE DEBUG)
ELSEIF( "${cppad_debug_which}" STREQUAL debug_odd )
	SET(CMAKE_BUILD_TYPE DEBUG)
ELSE( "${cppad_debug_which}" STREQUAL debug_odd )
	SET(CMAKE_BUILD_TYPE RELEASE)
ENDIF( "${cppad_debug_which}" STREQUAL debug_all )
#
SET(name cppad_for_tmb)
#
ADD_EXECUTABLE(
	${name} EXCLUDE_FROM_ALL ${name}.cpp
)
#
# Add extra definitions to the complilation flags
SET(cppad_cxx_flags "${cppad_cxx_flags} ${OpenMP_CXX_FLAGS} -D CPPAD_FOR_TMB")
#
# set compiler flags for this executable
SET_TARGET_PROPERTIES(
	${name} PROPERTIES COMPILE_FLAGS "${cppad_cxx_flags}"
)
# extra flags used by linker for openmp support
SET(CMAKE_EXE_LINKER_FLAGS ${OpenMP_CXX_FLAGS} )
#
# check_test_more_cppad_for_tmb target
ADD_CUSTOM_TARGET(
	check_test_more_${name} ${name} DEPENDS ${name}
)
MESSAGE(STATUS "make check_test_more_${name}: available")

# Add check_test_more_cppad_for_tmb to check depends in parent environment
add_to_list(check_test_more_depends check_test_more_${name})
SET(check_test_more_depends "${check_test_more_depends}" PARENT_SCOPE)
