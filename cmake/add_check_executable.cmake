# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
# add_check_executable(parent_target short_name)
# add_check_executable(parent_target short_name arguments)
#
# parent_target: (in)
# The variable containing the name of the parent check target,
# ${parent_target} must begin with "check".
#
# short_name: (in)
# Is the non-empty short name of the target we are adding.
# The full name of this target is ${parent_target}_${short_name}.
#
# arguments: (in)
# This argument is optional. If it is present, it is
# a string containing the arguments to the executable.
#
# 1. This macro creates the target ${parent_target}_${short_name}.
# 2. This target depends on an executable with the same name except that
#    the "check_" at the beginning is removed.
# 3. If the variable ${parent_target}_${short_name}_depends is defined,
#    it is a list that is included in the dependencies for this target.
# 4. This target is add to the list ${parent_target}_depends in both
#    the current scope and its parent scope.
#
# This macros uses temporary variables the name of which begin with
# add_check_executable.
#
MACRO(add_check_executable parent_target short_name)
    IF( NOT ${parent_target} MATCHES "^check" )
        MESSAGE(FATAL_ERROR "add_check_executable: "
            "parent_target does not begin with 'check'"
        )
    ENDIF( )
    IF( "${short_name}" STREQUAL "" )
        MESSAGE(FATAL_ERROR "add_check_target: short_name is empty")
    ENDIF( )
    #
    # add_check_executable_full_name
    SET(add_check_executable_full_name "${parent_target}_${short_name}" )
    #
    # add_check_executable_no_check
    STRING(REGEX REPLACE "^check_" "" add_check_executable_no_check
        "${add_check_executable_full_name}"
    )
    #
    # add_check_executable_arguments
    IF( ${ARGC} EQUAL 2 )
        SET(add_check_executable_arguments "")
    ELSEIF( ${ARGC} EQUAL 3 )
        STRING(
            REGEX REPLACE "[ ]" ";" add_check_executable_arguments "${ARGV2}"
        )
    ELSE( )
        MESSAGE(FATAL_ERROR "add_check_executable: "
            "number of arguments = ${ARGC}"
        )
    ENDIF( )
    #
    # add_check_executable_depends
    IF( DEFINED ${add_check_executable_full_name}_depends )
        SET(add_check_executable_depends
            ${${add_check_executable_full_name}_depends}
        )
        add_to_list(add_check_executable_depends
            ${add_check_executable_no_check}
        )
    ELSE ( )
        SET(add_check_executable_depends ${add_check_executable_no_check} )
    ENDIF( )
    #
    # add_check_executable_dot_slash
    IF( ${cmake_needs_dot_slash} )
        SET(add_check_executable_dot_slash "./")
    ELSE ( )
        SET(add_check_executable_dot_slash "")
    ENDIF( )
    #
    # create this target
    ADD_CUSTOM_TARGET(
        ${add_check_executable_dot_slash}${add_check_executable_full_name}
        ${add_check_executable_no_check} ${add_check_executable_arguments}
        DEPENDS ${add_check_executable_depends}
    )
    MESSAGE(STATUS "make ${add_check_executable_full_name}: available")
    #
    # add parent dependency
    add_to_list( ${parent_target}_depends ${add_check_executable_full_name} )
    SET( ${parent_target}_depends ${${parent_target}_depends} PARENT_SCOPE )
    #
ENDMACRO()
