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
#
# Name for this text
SET(name cppad_for_tmb)
#
# Add extra compiler flags to cppad_cxx_flags so set_compile_flags uses them.
# This change only affects the cppad_for_tmb tests (NO PARENT_SCOPE).
SET(cppad_cxx_flags "${cppad_cxx_flags} ${OpenMP_CXX_FLAGS} -DCPPAD_FOR_TMB")
#
SET(source_list
	${name}.cpp
	implicit_ctor.cpp
	perfer_reverse.cpp
	multi_atomic.cpp
	multi_checkpoint.cpp
)
set_compile_flags(
	test_more_${name} "${cppad_debug_which}" "${source_list}"
)
#
# now that we have the properties, add the executable
ADD_EXECUTABLE( test_more_${name} EXCLUDE_FROM_ALL ${source_list} )
#
# Extra flags used by linker for openmp support
SET(CMAKE_EXE_LINKER_FLAGS ${OpenMP_CXX_FLAGS} )
#
# check_test_more_${name} target
ADD_CUSTOM_TARGET(
	check_test_more_${name} test_more_${name} DEPENDS test_more_${name}
)
MESSAGE(STATUS "make check_test_more_${name}: available")

# Add check_test_more_${name} to check depends in parent environment
add_to_list(check_test_more_depends check_test_more_${name})
SET(check_test_more_depends "${check_test_more_depends}" PARENT_SCOPE)
