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
# Build the introduction/exp_apx directory tests

SET(source_list
	introduction.cpp
	exp_2.cpp
	exp_2.hpp
	exp_2_cppad.cpp
	exp_2_for1.cpp
	exp_2_for2.cpp
	exp_2_rev1.cpp
	exp_2_rev2.cpp
	exp_2_for0.cpp
	exp_eps.cpp
	exp_eps.hpp
	exp_eps_cppad.cpp
	exp_eps_for1.cpp
	exp_eps_for2.cpp
	exp_eps_for0.cpp
	exp_eps_rev1.cpp
	exp_eps_rev2.cpp
)
set_compile_flags( introduction "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( introduction EXCLUDE_FROM_ALL ${source_list} )

# Add this check target
ADD_CUSTOM_TARGET(check_introduction
	introduction
	DEPENDS introduction
)
MESSAGE(STATUS "make check_introduction: available")

# Change check depends in parent environment
add_to_list(check_depends check_introduction)
SET(check_depends "${check_depends}" PARENT_SCOPE)
