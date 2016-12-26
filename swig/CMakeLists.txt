# $Id$
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

# Initialize list of tests as empty
SET(check_swig_depends "")

# example subdirectory
ADD_SUBDIRECTORY(example)

# swig_congrol_file: used by the subdirectories below
SET(swig_control_file "${CMAKE_CURRENT_SOURCE_DIR}/cppad_swig.i" )

# swig_module_files: used by the subdirectories below
SET(swig_module_files "${swig_control_file}" )
FOREACH(root_name a_double a_fun function)
	add_to_list(swig_module_files "${CMAKE_CURRENT_SOURCE_DIR}/${root_name}" )
ENDFOREACH(root_name)

# subdirectories that use swig_cpapd_files
ADD_SUBDIRECTORY(python)
ADD_SUBDIRECTORY(octave)

IF( NOT( "${check_swig_depends}" STREQUAL "" ) )
	# Add the check_swig target
	ADD_CUSTOM_TARGET(check_swig DEPENDS ${check_swig_depends})
	MESSAGE(STATUS "make check_swig: available")

	# Change check depends in parent environment
	add_to_list(check_depends check_swig)
	SET(check_depends "${check_depends}" PARENT_SCOPE)
ENDIF( NOT( "${check_swig_depends}" STREQUAL "" ) )
