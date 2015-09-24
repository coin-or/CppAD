# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# modify_cmake_cxx_flags(msg)
# If '/W[0-9]' appears in cppad_cxx_flags, it is removed from CMAKE_CXX_FLAGS.
#
# subdir:
# The sub-directory where the CMAKE_CXX_FLAGS values are set
# ${message} ${CMAKE_CXX_FLAGS}
#
MACRO(modify_cmake_cxx_flags subdir)
	IF ( cppad_cxx_flags MATCHES "/W[0-9]" )
		STRING( REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
	ENDIF ( cppad_cxx_flags MATCHES "/W[0-9]" )
	MESSAGE(STATUS "${subdir}: CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
ENDMACRO(modify_cmake_cxx_flags)
