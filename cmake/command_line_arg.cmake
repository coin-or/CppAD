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
# command_line_arg(variable default type description)
#
# variable: (out)
# is the variable we are setting to its default value.
# The varaiable can be changed on the cmake command line (or in the camke gui).
# The final value of the variable is printed with the cmake output.
#
# variable_01: (out)
# This value has value 1, if variable is true, and 0 otherwise.
#
# default: (in)
# is the default value for this variable; i.e.,
# if it is not set by the cmake command line or gui.
#
# type: (in)
# must be one of the following:
#	STRING, if the variable holds an arbitrary string.
#	PATH, if the variable holds a directory.
#	BOOL, if the variable only has the values true or false.
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
	SET(${variable} "${default}" CACHE ${type} "${description}")
	MESSAGE(STATUS "${variable} = ${${variable}}")
	#
	# convert BOOL variables to 0/1
	IF( ${variable} )
		SET( ${variable}_01 1)
	ELSE( ${variable} )
		SET( ${variable}_01 0)
	ENDIF( ${variable} )
ENDMACRO( command_line_arg )
