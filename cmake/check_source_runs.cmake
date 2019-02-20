# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
# check_source_runs(source variable)
#
# source: (in)
# contains the source for the program that will be run.
#
# variable: (out)
# This variable must not be defined when this macro is called.
# Upon return, the value of this variable is 1 if the program runs and
# returns a zero status. Otherwise its value is 0.
# Note that this is the reverse of the status flag returned by the program.
#
MACRO(check_source_runs source variable)
    IF( DEFINED ${variable} )
        MESSAGE(ERROR
            "check_source_runs: ${variable} is defined before expected"
        )
    ENDIF( DEFINED ${variable} )
    SET(CMAKE_REQUIRED_DEFINITIONS "" )
    SET(CMAKE_REQUIRED_INCLUDES    "" )
    SET(CMAKE_REQUIRED_LIBRARIES   "" )
    IF( cppad_cxx_flags )
        SET(CMAKE_REQUIRED_FLAGS   "${cppad_cxx_flags}" )
    ELSE( cppad_cxx_flags )
        SET(CMAKE_REQUIRED_FLAGS   "" )
    ENDIF( cppad_cxx_flags )
    CHECK_CXX_SOURCE_RUNS("${source}" ${variable} )
    IF( ${variable} )
        SET(${variable} 1)
    ELSE( ${variable} )
        SET(${variable} 0)
    ENDIF( ${variable} )
    MESSAGE(STATUS "${variable} = ${${variable}}" )
ENDMACRO( check_source_runs )
