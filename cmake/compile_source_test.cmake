# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# compile_source_test(defined_ok source variable)
#
# defined_ok (in)
# If this is true, it is OK for variable to be defined on input
# (it will be replaced). Otherwise it is a fatal error if variable
# is defined on input.
#
# source: (in)
# contains the source for the program that will be compiled and linked.
#
# variable: (out)
# Upon return, the value of this variable is 1 (0) if the program compiles
# and links (does not compile and link).
#
# CMAKE_REQUIRED_name (in)
# For name equal to DEFINITIONS, INCLUDES, LIBRARIES, FLAGS, the variable
# CMAKE_REQUIRED_name is an input to routine; see CHECK_CXX_SOURCE_COMPILES
# documentation.
#
MACRO(compile_source_test defined_ok source variable)
   #
   #
   # check that variable is not yet defined
   IF( NOT ${defined_ok} )
      IF( DEFINED ${variable} )
         MESSAGE(FATAL_ERROR
            "compile_source_test: ${variable} is defined before expected"
         )
      ENDIF( DEFINED ${variable} )
   ENDIF( NOT ${defined_ok} )
   #
   # check that source code compiles
   CHECK_CXX_SOURCE_COMPILES("${source}" ${variable} )
   #
   # change result varialbe to 0 (1) for fail (succeed).
   IF( ${variable} )
      SET(${variable} 1)
   ELSE( ${variable} )
      SET(${variable} 0)
   ENDIF( ${variable} )
   #
   # check that varialbe is defined
   IF( NOT DEFINED ${variable} )
      MESSAGE(FATAL_ERROR
         "compile_source_test: error in CMake script."
      )
   ENDIF( NOT DEFINED ${variable} )
   #
   MESSAGE(STATUS "${variable} = ${${variable}}" )
ENDMACRO( compile_source_test )
