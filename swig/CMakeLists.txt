# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# used debug during initial development of swig interface
SET(CMAKE_BUILD_TYPE DEBUG)

# Initialize list of tests as empty (added to by sub-directories below)
SET(check_swig_depends "")

# example runs with out environment used for other subdirectories
ADD_SUBDIRECTORY(example)

# check if we have swig
FIND_PACKAGE(SWIG)
IF( SWIG_FOUND )

	# include the swig specific macros
	INCLUDE( ${CMAKE_ROOT}/Modules/UseSWIG.cmake )

	# swig_control_file: used by the subdirectories below
	SET(swig_control_file "${CMAKE_CURRENT_SOURCE_DIR}/cppad_swig.i" )

	# cppad_swig_lib
	# library used by subdirectories below
	ADD_LIBRARY(cppad_swig_lib SHARED EXCLUDE_FROM_ALL
		a_double.cpp
		a_fun.cpp
		function.cpp
	)
	add_cppad_cxx_flags( cppad_swig_lib )

	# SWIG_MODULE_cppad_swig_EXTRA_DEPS
	# swig extra dependencies used by subdirectories below
	SET(SWIG_MODULE_cppad_swig_EXTRA_DEPS
		"${CMAKE_CURRENT_SOURCE_DIR}/a_double.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/a_fun.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/function.hpp"
	)

	# subdirectories that use swig_cpapd_files
	ADD_SUBDIRECTORY(python)
	ADD_SUBDIRECTORY(octave)
	ADD_SUBDIRECTORY(perl)

ENDIF( SWIG_FOUND )

IF( NOT( "${check_swig_depends}" STREQUAL "" ) )
	# Add the check_swig target
	ADD_CUSTOM_TARGET(check_swig DEPENDS ${check_swig_depends})
	MESSAGE(STATUS "make check_swig: available")

	# Change check depends in parent environment
	add_to_list(check_depends check_swig)
	SET(check_depends "${check_depends}" PARENT_SCOPE)
ENDIF( NOT( "${check_swig_depends}" STREQUAL "" ) )
