# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-26 Bradley M. Bell
# ----------------------------------------------------------------------------
# ipopt_info()
#
# cppad_has_ipopt: (out)
# is 1 (0) if ipopt is found (not found)
# If this is 1, the include directory for ipopt will automatically be
# added using INCLUDE_DIRECTORIES with the SYSTEM flag.
#
# ipopt_LINK_LIBRARIES: (out)
# if cppad_has_ipopt is 1,
# is a list of absolute paths to libraries necessary to use ipopt.
#
# This macro may use variables with the name ipopt_*
MACRO(ipopt_info)
    #
    # check for pkg-config
    IF( NOT PKG_CONFIG_FOUND )
        FIND_PACKAGE(PkgConfig REQUIRED)
    ENDIF( )
    #
    # ipopt_*
    pkg_check_modules(ipopt QUIET ipopt )
    #
    IF( ipopt_FOUND )
        MESSAGE(STATUS "Found ipopt.pc")
        SET(cppad_has_ipopt 1)
        INCLUDE_DIRECTORIES( SYSTEM ${ipopt_INCLUDE_DIRS} )
    ELSE( )
        MESSAGE(STATUS "Did not find ipopt.pc" )
        MESSAGE(STATUS "PKG_CONFIG_PATH=$ENV{PKG_CONFIG_PATH}" )
        SET(cppad_has_ipopt 0)
    ENDIF( )
    #
    print_variable( cppad_has_ipopt )
ENDMACRO(ipopt_info)
