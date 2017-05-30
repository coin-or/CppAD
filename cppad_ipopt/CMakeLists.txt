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

# Initialize list of tests as empty
SET(check_cppad_ipopt_depends "")

# assert ipopt_prefix is defined
IF ( NOT ipopt_prefix )
	MESSAGE(FATAL_ERROR
		"cppad_ipopt/CMakeLists.txt: ipopt_prefix = ${ipopt_prefix}"
	)
ENDIF ( NOT ipopt_prefix )

# The CMakeLists.txt file in the specified source directory is processed
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)
ADD_SUBDIRECTORY(speed)
ADD_SUBDIRECTORY(test)

# Add the check_cppad_ipopt target
ADD_CUSTOM_TARGET(check_cppad_ipopt DEPENDS ${check_cppad_ipopt_depends} )
MESSAGE(STATUS "make check_cppad_ipopt: available")

# Change check depends in parent environment
add_to_list(check_depends check_cppad_ipopt)
SET(check_depends "${check_depends}" PARENT_SCOPE)
