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

# subdirectories
ADD_SUBDIRECTORY(example)

IF( NOT( "${check_swig_depends}" STREQUAL "" ) )
	# check_swig
	ADD_CUSTOM_TARGET(check_swig DEPENDS ${check_swig_depends} )
	MESSAGE(STATUS "make check_swig: available")

	# Change check depends in parent environment
	add_to_list(check_depends check_swig)
	SET(check_depends "${check_depends}" PARENT_SCOPE)
ENDIF( NOT( "${check_swig_depends}" STREQUAL "" ) )
