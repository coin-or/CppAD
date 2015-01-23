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
# check_source_runs(source variable)
#
# source: (in)
# contains the source for the program that will be run.
# 
# variable: (out)
# the value of this variable is 1 if the program runs and 
# returns a zero status. Otherwise its value is 0.
# Note that this is the reverse of the status flag returned by the program.
# 
MACRO(check_source_runs source variable)
	SET(CMAKE_REQUIRED_INCLUDES    "" )
	SET(CMAKE_REQUIRED_LIBRARIES   "" )
	SET(CMAKE_REQUIRED_DEFINITIONS "" )
	IF( cppad_cxx_flags )
		SET(CMAKE_REQUIRED_FLAGS   "${cppad_cxx_flags}" )
	ELSE( cppad_cxx_flags )
		SET(CMAKE_REQUIRED_FLAGS   "" )
	ENDIF( cppad_cxx_flags )
	CHECK_CXX_SOURCE_RUNS("${source}" ${variable}_result)
	IF( ${variable}_result )
		SET(${variable} 1)
	ELSE( ${variable}_result )
		SET(${variable} 0)
	ENDIF( ${variable}_result )
	MESSAGE(STATUS "${variable} = ${${variable}}" )
ENDMACRO( check_source_runs )
