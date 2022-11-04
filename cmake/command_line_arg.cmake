# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# command_line_arg(variable default type description)
#
# variable: (out)
# is the variable we are setting to its default value.
# The varaiable can be changed on the cmake command line (or in the camke gui).
# The final value of the variable is printed with the cmake output.
#
# default: (in)
# is the default value for this variable; i.e.,
# if it is not set by the cmake command line or gui.
#
# type: (in)
# must be one of the following:
#  STRING, if the variable holds an arbitrary string.
#  PATH, if the variable holds a directory.
#  BOOL, if the variable only has the values true or false.
#
# description: (in)
# Is a description of how the variable affects the CppAD install procedure.
#
MACRO(command_line_arg variable default type description)
   IF( NOT ( ${type} STREQUAL "STRING" ) )
   IF( NOT ( ${type} STREQUAL "PATH" ) )
   IF( NOT ( ${type} STREQUAL "BOOL" ) )
      MESSAGE(FATAL_ERROR, "command_line_arg: bug in CppAD cmake commands")
   ENDIF( NOT ( ${type} STREQUAL "BOOL" ) )
   ENDIF( NOT ( ${type} STREQUAL "PATH" ) )
   ENDIF( NOT ( ${type} STREQUAL "STRING" ) )
   #
   IF( NOT ${variable} )
      SET(${variable} "${default}" CACHE ${type} "${description}")
   ENDIF( NOT ${variable} )
   MESSAGE(STATUS "${variable} = ${${variable}}")
   #
ENDMACRO( command_line_arg )
